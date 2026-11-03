#include <Servo.h>
#include <NewPing.h>
#include <SoftwareSerial.h>

// =========================
// Modes
// =========================
#define OBSTACLE_AVOIDING_MODE                    1
#define LINE_FOLLOWER_MODE                        2
#define LINE_FOLLOWER_WITH_OBSTACLE_AVOIDING_MODE 3
#define OBJECT_FOLLOWING_MODE                     4
#define BLUETOOTH_CONTROL_MODE                    5   // Manual RC mode

// =========================
// Pins
// =========================
#define SERVO_PIN 3
#define ULTRASONIC_SENSOR_TRIG 11
#define ULTRASONIC_SENSOR_ECHO 12

#define BOTTOM_IR_SENSOR_RIGHT A0
#define BOTTOM_IR_SENSOR_LEFT A1
#define TOP_IR_SENSOR_RIGHT A3
#define TOP_IR_SENSOR_LEFT A2

// Right motor
int enableRightMotor = 5;
int rightMotorPin1 = 7;
int rightMotorPin2 = 8;

// Left motor
int enableLeftMotor = 6;
int leftMotorPin1 = 9;
int leftMotorPin2 = 10;

// =========================
// Objects
// =========================
NewPing mySensor(ULTRASONIC_SENSOR_TRIG, ULTRASONIC_SENSOR_ECHO, 400);
Servo myServo;

// HC-05 Bluetooth on pins 2 (RX), 4 (TX)
SoftwareSerial BTSerial(2, 4);

// Current robot control mode (start manual)
int currentMode = BLUETOOTH_CONTROL_MODE;

void setup() {
  pinMode(enableRightMotor, OUTPUT);
  pinMode(rightMotorPin1, OUTPUT);
  pinMode(rightMotorPin2, OUTPUT);

  pinMode(enableLeftMotor, OUTPUT);
  pinMode(leftMotorPin1, OUTPUT);
  pinMode(leftMotorPin2, OUTPUT);

  myServo.attach(SERVO_PIN);
  myServo.write(90);

  pinMode(BOTTOM_IR_SENSOR_RIGHT, INPUT);
  pinMode(BOTTOM_IR_SENSOR_LEFT, INPUT);
  pinMode(TOP_IR_SENSOR_RIGHT, INPUT);
  pinMode(TOP_IR_SENSOR_LEFT, INPUT);

  rotateMotor(0, 0);

  Serial.begin(9600);   // USB serial monitor
  BTSerial.begin(9600); // Bluetooth serial
  Serial.println("Bluetooth Robot Ready");
}

void loop() {
  switch (currentMode) {
    case OBSTACLE_AVOIDING_MODE:
      obstacleAvoidingControl();
      break;

    case LINE_FOLLOWER_MODE:
      lineFollowerControl();
      break;

    case LINE_FOLLOWER_WITH_OBSTACLE_AVOIDING_MODE:
      lineFollowerWithObstacleAvoidingControl();
      break;

    case OBJECT_FOLLOWING_MODE:
      objectFollowingControl();
      break;

    case BLUETOOTH_CONTROL_MODE:
      bluetoothControl();
      break;

    default:
      rotateMotor(0, 0);
      break;
  }
}

// =========================
// BLUETOOTH CONTROL
// =========================
void bluetoothControl() {
  static int MOTOR_SPEED = 200;
  static int TURNING_SPEED = 150;

  if (BTSerial.available()) {
    char command = BTSerial.read();
    Serial.print("Received: ");
    Serial.println(command);

    switch (command) {
      case 'F': rotateMotor(MOTOR_SPEED, MOTOR_SPEED); break;
      case 'B': rotateMotor(-MOTOR_SPEED, -MOTOR_SPEED); break;
      case 'L': rotateMotor(-TURNING_SPEED, TURNING_SPEED); break;
      case 'R': rotateMotor(TURNING_SPEED, -TURNING_SPEED); break;
      case 'S': rotateMotor(0, 0); break;

      // Mode switching
      case 'C': currentMode = OBSTACLE_AVOIDING_MODE; rotateMotor(0, 0); Serial.println("Autonomous: Obstacle Avoiding"); break;
      case '1': currentMode = LINE_FOLLOWER_MODE; rotateMotor(0, 0); Serial.println("Autonomous: Line Follower"); break;
      case '2': currentMode = LINE_FOLLOWER_WITH_OBSTACLE_AVOIDING_MODE; rotateMotor(0, 0); Serial.println("Autonomous: Line + Obstacle"); break;
      case '3': currentMode = OBJECT_FOLLOWING_MODE; rotateMotor(0, 0); Serial.println("Autonomous: Object Following"); break;
      case 'M': currentMode = BLUETOOTH_CONTROL_MODE; rotateMotor(0, 0); Serial.println("Manual Mode"); break;
    }
  }
}

// =========================
// AUTONOMOUS MODES
// =========================
void obstacleAvoidingControl() {
  static int MAX_REGULAR_MOTOR_SPEED = 150;
  static int MAX_MOTOR_TURN_SPEED = 200;
  static int DISTANCE_TO_CHECK = 30;

  checkBluetoothOverride(); // allow switching anytime

  int distance = mySensor.ping_cm();
  if (distance > 0 && distance < DISTANCE_TO_CHECK) {
    rotateMotor(0, 0);
    delay(200);
    rotateMotor(-MAX_MOTOR_TURN_SPEED, -MAX_MOTOR_TURN_SPEED);
    delay(200);
    rotateMotor(0, 0);
    delay(200);

    myServo.write(180); delay(200); int distanceLeft = mySensor.ping_cm();
    myServo.write(0);   delay(200); int distanceRight = mySensor.ping_cm();
    myServo.write(90);  delay(200);

    if (distanceLeft >= distanceRight)
      rotateMotor(MAX_MOTOR_TURN_SPEED, -MAX_MOTOR_TURN_SPEED);
    else
      rotateMotor(-MAX_MOTOR_TURN_SPEED, MAX_MOTOR_TURN_SPEED);

    delay(300);
    rotateMotor(0, 0);
  } else {
    rotateMotor(MAX_REGULAR_MOTOR_SPEED, MAX_REGULAR_MOTOR_SPEED);
  }
}

void lineFollowerControl() {
  static int MOTOR_SPEED = 100;
  static int TURNING_MOTOR_SPEED = 200;

  checkBluetoothOverride();

  int rightIR = digitalRead(BOTTOM_IR_SENSOR_RIGHT);
  int leftIR = digitalRead(BOTTOM_IR_SENSOR_LEFT);

  if (rightIR == LOW && leftIR == LOW)
    rotateMotor(MOTOR_SPEED, MOTOR_SPEED);
  else if (rightIR == HIGH && leftIR == LOW)
    rotateMotor(-TURNING_MOTOR_SPEED, TURNING_MOTOR_SPEED);
  else if (rightIR == LOW && leftIR == HIGH)
    rotateMotor(TURNING_MOTOR_SPEED, -TURNING_MOTOR_SPEED);
  else
    rotateMotor(0, 0);
}

void lineFollowerWithObstacleAvoidingControl() {
  static int MOTOR_SPEED = 100;
  static int TURNING_MOTOR_SPEED = 200;
  static int DISTANCE_TO_CHECK = 20;

  checkBluetoothOverride();

  int rightIR = digitalRead(BOTTOM_IR_SENSOR_RIGHT);
  int leftIR = digitalRead(BOTTOM_IR_SENSOR_LEFT);

  if (rightIR == LOW && leftIR == LOW) {
    int distance = mySensor.ping_cm();
    if (distance > 0 && distance < DISTANCE_TO_CHECK)
      adjustRobotToAvoidObjectWhileFollowingLine();
    else
      rotateMotor(MOTOR_SPEED, MOTOR_SPEED);
  } else if (rightIR == HIGH && leftIR == LOW)
    rotateMotor(-TURNING_MOTOR_SPEED, TURNING_MOTOR_SPEED);
  else if (rightIR == LOW && leftIR == HIGH)
    rotateMotor(TURNING_MOTOR_SPEED, -TURNING_MOTOR_SPEED);
  else
    rotateMotor(0, 0);
}

void objectFollowingControl() {
  static int MAX_FORWARD_MOTOR_SPEED = 150;
  static int MAX_MOTOR_TURN_SPEED_ADJUSTMENT = 100;
  static int MIN_DISTANCE = 10;
  static int MAX_DISTANCE = 30;

  checkBluetoothOverride();

  int distance = mySensor.ping_cm();
  int rightIR = digitalRead(TOP_IR_SENSOR_RIGHT);
  int leftIR = digitalRead(TOP_IR_SENSOR_LEFT);

  if (rightIR == LOW && leftIR == HIGH)
    rotateMotor(MAX_FORWARD_MOTOR_SPEED - MAX_MOTOR_TURN_SPEED_ADJUSTMENT,
                MAX_FORWARD_MOTOR_SPEED + MAX_MOTOR_TURN_SPEED_ADJUSTMENT);
  else if (rightIR == HIGH && leftIR == LOW)
    rotateMotor(MAX_FORWARD_MOTOR_SPEED + MAX_MOTOR_TURN_SPEED_ADJUSTMENT,
                MAX_FORWARD_MOTOR_SPEED - MAX_MOTOR_TURN_SPEED_ADJUSTMENT);
  else if (distance >= MIN_DISTANCE && distance <= MAX_DISTANCE)
    rotateMotor(MAX_FORWARD_MOTOR_SPEED, MAX_FORWARD_MOTOR_SPEED);
  else
    rotateMotor(0, 0);
}

// =========================
// HELPER FUNCTIONS
// =========================
void adjustRobotToAvoidObjectWhileFollowingLine() {
  static int MAX_MOTOR_TURN_SPEED = 200;

  rotateMotor(0, 0);
  delay(200);

  myServo.write(180); delay(200); int distanceLeft = mySensor.ping_cm();
  myServo.write(0);   delay(200); int distanceRight = mySensor.ping_cm();
  myServo.write(90);  delay(200);

  if (distanceLeft >= distanceRight)
    rotateMotor(MAX_MOTOR_TURN_SPEED, -MAX_MOTOR_TURN_SPEED);
  else
    rotateMotor(-MAX_MOTOR_TURN_SPEED, MAX_MOTOR_TURN_SPEED);

  delay(400);
  rotateMotor(0, 0);
}

void rotateMotor(int rightMotorSpeed, int leftMotorSpeed) {
  if (rightMotorSpeed < 0) {
    digitalWrite(rightMotorPin1, LOW);
    digitalWrite(rightMotorPin2, HIGH);
  } else {
    digitalWrite(rightMotorPin1, HIGH);
    digitalWrite(rightMotorPin2, LOW);
  }

  if (leftMotorSpeed < 0) {
    digitalWrite(leftMotorPin1, LOW);
    digitalWrite(leftMotorPin2, HIGH);
  } else {
    digitalWrite(leftMotorPin1, HIGH);
    digitalWrite(leftMotorPin2, LOW);
  }

  analogWrite(enableRightMotor, abs(rightMotorSpeed));
  analogWrite(enableLeftMotor, abs(leftMotorSpeed));
}

void checkBluetoothOverride() {
  if (BTSerial.available()) {
    char cmd = BTSerial.read();
    if (cmd == 'M') {
      currentMode = BLUETOOTH_CONTROL_MODE;
      rotateMotor(0, 0);
      Serial.println("Manual Mode");
    }
  }
}