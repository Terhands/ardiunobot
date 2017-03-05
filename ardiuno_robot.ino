#include <NewPing.h>

// NOTE: These pins might be backwards
#define TRIGGER_PIN  13  // Arduino pin tied to trigger pin on the ultrasonic sensor.
#define ECHO_PIN     12  // Arduino pin tied to echo pin on the ultrasonic sensor.
#define MAX_DISTANCE 100 // Maximum distance we want to ping for (in centimeters). Maximum sensor distance is rated at 400-500cm.

NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE); // NewPing setup of pins and maximum distance to register.

// ********************************* //
// MOTION CONTROLS BELOW
// ********************************* //

enum Direction { FORWARD, BACKWARD };
enum Wheel { LEFT, RIGHT };

// MotorControl :: 
typedef struct MOTOR_CONTROL {
  int motor_pin;
  int dir_pin_a;
  int dir_pin_b;
} MotorControl;

// List of pin tuples for each wheel ordered (motor, directionalA, directionalB). Tuples are indexed by Wheel.
MotorControl motor_controls[2] = {{.motor_pin=5, .dir_pin_a=6, .dir_pin_b=7}, {.motor_pin=10, .dir_pin_a=8, .dir_pin_b=9}};

//
// spin :: Wheel * Direction * int -> void
// Spins the specified wheel forwards or backwards depending on the specified direction at the specified speed
//    wheel (Wheel): The wheel to spin.
//    dir (Direction): The direction to spin the wheel.
//    speed (int): The speed to spin the wheel from 0 to 255.
// 
void spin(Wheel wheel, Direction dir, int speed);

//
// move :: Direction * int -> void
// Moves the robot FOWARD or BACKWARD at the specified speed depending on the passed in direction.
//    dir (Direction): The direction that the robot will move.
//    speed (int): The speed to move in direction between 0 and 255.
//
void move(Direction dir, int speed);

// 
// turn_right :: void -> void
// Turns the roboto right 90 degrees.
// 
void turn_right();

// 
// turn_left :: void -> void
// Turns the roboto left 90 degrees.
//  
void turn_left();

//
// stop :: void -> void
// Stops the robot's wheel from spinning.
//
void stop(); 

void setup() {
  // Setting up the motion sensor input receiver
  Serial.begin(115200); // Open serial monitor at 115200 baud to see ping results.
  
  // Setting up the motor controls
  for(int i=0; i<2; i++) {
    pinMode(motor_controls[i].motor_pin, OUTPUT);
    pinMode(motor_controls[i].dir_pin_a, OUTPUT);
    pinMode(motor_controls[i].dir_pin_b, OUTPUT);
  }
}

void loop() {
  delay(50);  // give some time between pings - this is to help compensate for the delay in input
  unsigned int sonar_sensor_input = sonar.ping();
  if(sonar_sensor_input / US_ROUNDTRIP_CM <= 15) {  // if the robot is closer than 15 CM to an object
    stop();
  } else {
    move(FORWARD, 150);
  }
}

void move(Direction dir, int speed) {
  spin(LEFT, dir, speed);
  spin(RIGHT, dir, speed);
}

void stop() {
  spin(LEFT, FORWARD, 0);
  spin(RIGHT, FORWARD, 0);
}

void turn_right() {
  spin(LEFT, FORWARD, 150);
  spin(RIGHT, BACKWARD, 150);
  delay(900);
  stop();
}

void turn_left() {
  spin(LEFT, BACKWARD, 150);
  spin(RIGHT, FORWARD, 150);
  delay(900);
  stop();
}

void spin(Wheel wheel, Direction dir, int speed) {
  MotorControl control = motor_controls[wheel];
  
  if(dir == FORWARD) {
    digitalWrite(control.dir_pin_a, HIGH);
    digitalWrite(control.dir_pin_b, LOW);

  } else if(dir == BACKWARD) {
    digitalWrite(control.dir_pin_a, LOW);
    digitalWrite(control.dir_pin_b, HIGH);
  }
  analogWrite(control.motor_pin, speed);
}

