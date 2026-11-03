# Arduino Autonomous RC Car

A four-wheel multifunctional robotic car documented as an academic **All-in-One Robot** project. The recovered Arduino sketch describes Bluetooth-based manual driving and four sensor-based autonomous operating modes.

> **Reconstruction note:** This repository was assembled from the source file, photographs, demonstration video and project report that remain available. The original physical prototype is no longer available for testing. The recovered sketch has not been independently compiled or tested as part of this repository reconstruction. It is preserved as recovered source, not represented as verified final firmware.

## Project at a Glance

- Four-wheel mobile robot chassis with DC geared motors.
- Ultrasonic distance sensing mounted on a servo for directional scanning.
- Infrared sensing for line and object detection.
- Bluetooth serial control for manual driving and selecting modes.
- Recovered firmware written for an Arduino-style board. Its pin definitions and `SoftwareSerial` usage indicate an **Arduino Uno-class setup**.

## Photographs and Demonstration

### Outdoor Prototype

![Four-wheel robot prototype outdoors](images/robot-outdoor.jpg)

### Front View

![Front view of robot with ultrasonic sensor and servo](images/robot-front.jpg)

### Demonstration Video

[▶ Watch the RC Car Demonstration](videos/robot-demo.mp4)

## Modes Described in the Recovered Sketch

| Bluetooth Command | Mode | Behaviour Described by the Code |
|---|---|---|
| `M` | Manual Bluetooth Control | Reads movement commands from the Bluetooth serial connection. |
| `C` | Obstacle Avoidance | Uses ultrasonic distance readings and a servo scan to choose a direction. |
| `1` | Line Following | Uses the two bottom IR sensors to steer along a line. |
| `2` | Line Following with Obstacle Avoidance | Combines bottom IR sensing with an ultrasonic distance check. |
| `3` | Object Following | Uses the ultrasonic sensor and the two top IR sensor inputs. |

### Manual Movement Commands

| Command | Action |
|---|---|
| `F` | Forward |
| `B` | Backward |
| `L` | Left |
| `R` | Right |
| `S` | Stop |

These are features implemented in the **recovered sketch**. They should not be interpreted as a claim that every mode has been retested recently.

## Hardware Identified from Available Evidence

The photographs show a four-wheel chassis, yellow DC geared motors, a front-mounted ultrasonic module on a servo, and infrared sensor boards.

| Component | Description |
|---|---|
| Four-wheel chassis | Mobile robotic platform with four wheels. |
| DC Geared Motors | Provide movement to the robotic car. |
| Ultrasonic Distance Sensor | Measures distance to nearby obstacles. |
| Servo Motor | Positions the ultrasonic sensor for directional scanning. |
| Infrared Sensors | Used for line and object detection according to the recovered sketch. |
| Bluetooth Module | The recovered sketch specifies an HC-05-style serial interface. |
| Motor Driver | Controls the motors using direction and enable signals. The exact fitted board is not confirmed. |
| Microcontroller | The recovered sketch is consistent with an Arduino Uno-class setup. |

**Hardware identification note:** The exact controller-board revision, motor-driver model and complete wiring arrangement cannot be confirmed from the available photographs.

## Pin Assignments in the Recovered Sketch

| Function | Arduino Pin |
|---|---|
| Servo Signal | D3 |
| Ultrasonic Trigger | D11 |
| Ultrasonic Echo | D12 |
| Bottom Right IR Sensor | A0 |
| Bottom Left IR Sensor | A1 |
| Top Left IR Sensor | A2 |
| Top Right IR Sensor | A3 |
| Right Motor Enable | D5 |
| Right Motor Direction | D7, D8 |
| Left Motor Enable | D6 |
| Left Motor Direction | D9, D10 |
| Bluetooth Receive (Arduino RX) | D2 |
| Bluetooth Transmit (Arduino TX) | D4 |

These are **code-defined pin assignments**, not a verified wiring diagram. Check the actual circuit before reusing them on hardware.

## Source Code and Dependencies

The recovered Arduino sketch is available here:

[**RC_CAR_recovered.ino**](src/RC_CAR_recovered.ino)

The sketch includes the following libraries:

- `Servo.h`
- `NewPing.h`
- `SoftwareSerial.h`

The source file has been preserved without silently rewriting its behaviour.

It should be reviewed before attempting to compile or run it. In particular:

- Autonomous routines use blocking delays.
- Bluetooth override handling is limited.
- Mode changes other than the manual override may not be handled as expected while an autonomous mode is running.

## Project Report

The available three-page report names the academic project **All-in-one robot**, prepared for **Basic Mechanical Engineering (ME135)**.

The report describes the concept of combining sensors, processing, actuators and Bluetooth control into a multifunctional robotic system.

It discusses:

- Obstacle-detecting IR sensors.
- Bluetooth communication.
- Motor driver.
- Ultrasonic distance sensor.
- SMD board.
- Servo motor.

A summary of the original report is available here:

[**Project Report Summary**](docs/project-report-summary.md)

For additional information about the recovered materials and verification status, see [**Project Status**](PROJECT_STATUS.md).

## Repository Structure

```text
Arduino-Autonomous-RC-Car/
│
├── README.md
├── PROJECT_STATUS.md
│
├── src/
│   └── RC_CAR_recovered.ino
│
├── images/
│   ├── robot-front.jpg
│   └── robot-outdoor.jpg
│
├── docs/
│   └── project-report-summary.md
│
└── videos/
    └── robot-demo.mp4
```

## Known Limitations

- The original physical prototype is no longer available for further testing.
- The recovered sketch has not been independently compiled or tested during this repository reconstruction.
- The precise motor-driver model, controller-board revision, complete wiring and power arrangement are not confirmed.
- The exact smartphone application name is not confirmed.
- The report contains broad descriptions of possible robot applications. Those descriptions are not evidence that all such applications were implemented in this prototype.

## Future Improvements

- Recover another copy of the final sketch or original wiring diagram, if available.
- Confirm the exact controller and motor-driver boards using additional photographs.
- Improve Bluetooth handling so commands and mode changes are processed consistently in every mode.
- Replace long blocking delays with non-blocking timing where appropriate.
- Re-test each operating mode on compatible hardware.
- Add a verified wiring diagram and complete bill of materials.

## Credits

This was originally documented as a team academic project.

Team member names and student identifiers from the original report have intentionally been omitted from this public repository copy.
