# Project report summary — All-in-one robot

## Source

This summary is based on the available three-page CIA-3 report for **Basic Mechanical Engineering (ME135)**. It preserves the report's broad framing and does not add claims that are unsupported by that document.

## Project description

The report describes an **All-in-One Robot** as a multifunctional machine intended to combine multiple capabilities in one system, rather than being dedicated to only one task.

## Working concept described in the report

1. **Input and sensing:** the robot collects information from its surroundings using sensors.
2. **Processing and decision making:** the sensor data is sent to a central processing unit or AI chip.
3. **Action and execution:** actuators and motors carry out actions, while the mobility system moves the robot.
4. **Bluetooth control:** a person can control the robot using a smartphone app through Bluetooth.

The report gives a high-level explanation; it does not provide a detailed circuit diagram or a complete software architecture.

## Components listed in the report

1. **Obstacle detecting IR sensor:** described as using an IR LED and photodiode to detect reflected infrared light.
2. **Bluetooth module:** described as providing short-range wireless serial communication between a microcontroller and a device such as a smartphone.
3. **Motor driver:** described as an interface that allows a microcontroller to control motors requiring higher current and voltage.
4. **Ultrasonic distance sensor:** described as measuring distance using transmitted ultrasonic pulses and the echo return time.
5. **SMD board:** described in terms of surface-mounted components on a PCB.
6. **Servo motor:** described as an actuator that can rotate to a commanded position using PWM control.

## Advantages and applications listed in the report

The report lists time and effort savings, reduced cost, convenience, safety and productivity as potential advantages. It mentions domestic assistance, industrial use, healthcare support, entertainment and exploration as possible application areas for all-in-one robots generally. These are broad application examples in the report, not verified functions of this particular RC car prototype.

## Relationship to the recovered source code and photos

The recovered sketch and photographs provide more specific evidence of a four-wheel mobile robot with an ultrasonic sensor, servo and infrared sensors, plus Bluetooth commands and autonomous modes. The report does not specify the exact controller model, pin assignments, motor driver model or exact autonomous-mode implementation.
