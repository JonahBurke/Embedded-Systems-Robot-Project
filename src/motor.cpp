/** Jonah Burke
  * 3/31/2020
  * CSCE 236, Prof. Falkinburg
  * 
  * This is the motor control library
  */

#include <Arduino.h>
#include "motor.h"
#include "ultrasonic.h"

void disableMotors() {
    // Right
    DDRD &= ~(1<<3);
    DDRD &= ~(1<<4);
    DDRD &= ~(1<<5);
    // Left
    DDRB &= ~(1<<3);
    DDRD &= ~(1<<6);
    DDRD &= ~(1<<7);
}

void enableMotors() {
    // Right
    DDRD |= (1<<3);
    DDRD |= (1<<4);
    DDRD |= (1<<5);
    // Left
    DDRB |= (1<<3);
    DDRD |= (1<<6);
    DDRD |= (1<<7);
}

void setupMotors() {
    // Enable motors
    enableMotors();
    // set up PWM
    TCCR2A |= (1<<WGM21) | (1<<WGM20);
    TCCR2A |= (1<<COM2A1) | (1<<COM2B1);
    TCCR2B |= (1<<CS22) | (1<<CS21) | (1<<CS20);
    OCR2A = 0;
    OCR2B = 0;
}

void setMotorSpeed(int side, double percentPower, Direction d) {
    if (side == RIGHT) {
        OCR2B = percentPower * MOTOR_TOP_SPEED;
        if (d == FORWARD) {
            PORTD |= (1<<4);
            PORTD &= ~(1<<5);
        } else { // Backward
            PORTD |= (1<<5);
            PORTD &= ~(1<<4);
        }
    } else if (side == LEFT) {
        OCR2A = percentPower * MOTOR_TOP_SPEED;
        if (d == FORWARD) {
            PORTD |= (1<<6);
            PORTD &= ~(1<<7);
        } else { // Backward
            PORTD |= (1<<7);
            PORTD &= ~(1<<6);
        }
    }
}

void motorTest() {
  Serial.println("Begin Robot Motion");
  /// Forward
  setMotorSpeed(RIGHT, 0.4, FORWARD);
  setMotorSpeed(LEFT, 0.4, FORWARD);
  delay(500);
  setMotorSpeed(RIGHT, 0, BACKWARD);
  setMotorSpeed(LEFT, 0, BACKWARD);
  delay(500);
  // Backward
  setMotorSpeed(RIGHT, 0.4, BACKWARD);
  setMotorSpeed(LEFT, 0.4, BACKWARD);
  delay(500);
  setMotorSpeed(RIGHT, 0, BACKWARD);
  setMotorSpeed(LEFT, 0, BACKWARD);
  delay(500);
  // Small Left
  setMotorSpeed(RIGHT, 0.4, FORWARD);
  setMotorSpeed(LEFT, 0.4, BACKWARD);
  delay(350);
  setMotorSpeed(RIGHT, 0, BACKWARD);
  setMotorSpeed(LEFT, 0, BACKWARD);
  delay(500);
  // Small Right
  setMotorSpeed(RIGHT, 0.4, BACKWARD);
  setMotorSpeed(RIGHT, 0.4, FORWARD);
  delay(425);
  setMotorSpeed(RIGHT, 0, BACKWARD);
  setMotorSpeed(LEFT, 0, BACKWARD);
  delay(500);
  // Large Left
  setMotorSpeed(RIGHT, 0.4, FORWARD);
  setMotorSpeed(LEFT, 0.4, BACKWARD);
  delay(700);
  setMotorSpeed(RIGHT, 0, BACKWARD);
  setMotorSpeed(LEFT, 0, BACKWARD);
  delay(500);
  // Large Right
  setMotorSpeed(RIGHT, 0.4, BACKWARD);
  setMotorSpeed(LEFT, 0.4, FORWARD);
  delay(900);
  setMotorSpeed(RIGHT, 0, BACKWARD);
  setMotorSpeed(LEFT, 0, BACKWARD);

  Serial.println("End Robot Motion");
}

void followWall(double percentPower, double maintainDistance, int side) {
    double distance = ultraSonicPulse();
    if (distance > 60) { // For our purposes, we can filter out really long distances when following walls
        distance = maintainDistance;
    }
    if (side == LEFT) {
        setMotorSpeed(RIGHT, percentPower * (distance / maintainDistance), FORWARD);
        setMotorSpeed(LEFT, percentPower * (maintainDistance / distance), FORWARD);
    } else if (side == RIGHT) {
        setMotorSpeed(LEFT, percentPower * (distance / maintainDistance), FORWARD);
        setMotorSpeed(RIGHT, percentPower * (maintainDistance / distance), FORWARD);
    }
    delay(60);
    Serial.println(distance);
}