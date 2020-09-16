/** Jonah Burke
  * 4/29/2020
  * CSCE 236, Prof. Falkinburg
  * 
  * This file contains the logic for the obstacle course portion of Project 2
  */

#include <Arduino.h>
#include "ultrasonic.h"
#include "motor.h"

const double originalSpeed = 0.3;
const double distanceThreshold = 12;
double percentSpeed = originalSpeed;
int idealDirection = CENTER;
unsigned long t = 0;
bool firstWall = false;
bool secondWall = false;

void setup() {
    Serial.begin(9600);
    Serial.println("Setup Starting");
    ledSetup();
    setupServo();
    setupMotors();
    Serial.println("Setup Done");
    swivelRead(RIGHT);
    t = millis();
}

void loop() {
    if (idealDirection == RIGHT) {
        setMotorSpeed(LEFT, percentSpeed, FORWARD);
        setMotorSpeed(RIGHT, percentSpeed, FORWARD);
        double d = swivelRead(RIGHT);
        // bring the speed up to try to counteract the natural motor speed decrease
        percentSpeed += 0.0004;
        if (d > 3 * distanceThreshold && d < 60) {
            // reset the speed for the next leg
            percentSpeed = originalSpeed;
            setMotorSpeed(LEFT, 0, FORWARD);
            setMotorSpeed(RIGHT, 0, FORWARD);
            delay(50);
            // get a bit further past the obstacle before turning
            setMotorSpeed(LEFT, 0.35, FORWARD);
            setMotorSpeed(RIGHT, 0.4, FORWARD);
            delay(1000);

            setMotorSpeed(LEFT, 0, FORWARD);
            setMotorSpeed(RIGHT, 0, FORWARD);
            delay(50);
            // turn Right
            setMotorSpeed(LEFT, percentSpeed + 0.4, FORWARD);
            setMotorSpeed(RIGHT, percentSpeed + 0.4, BACKWARD);
            delay(400);
            setMotorSpeed(LEFT, 0, FORWARD);
            setMotorSpeed(RIGHT, 0, FORWARD);
            delay(50);
            idealDirection = CENTER;
            swivelRead(RIGHT);
            firstWall = true;
        }
    } else if (idealDirection == LEFT) {
        setMotorSpeed(LEFT, percentSpeed, FORWARD);
        setMotorSpeed(RIGHT, percentSpeed, FORWARD);
        double d = swivelRead(CENTER);
        // bring the speed up to try to counteract the natural motor speed decrease
        percentSpeed += 0.0004;
        if (d < distanceThreshold) {
            // reset the speed for the next leg
            percentSpeed = originalSpeed;
            setMotorSpeed(LEFT, 0, FORWARD);
            setMotorSpeed(RIGHT, 0, FORWARD);
            delay(50);
            // turn Left
            setMotorSpeed(LEFT, percentSpeed + 0.4, BACKWARD);
            setMotorSpeed(RIGHT, percentSpeed + 0.4, FORWARD);
            delay(400);
            setMotorSpeed(LEFT, 0, FORWARD);
            setMotorSpeed(RIGHT, 0, FORWARD);
            delay(50);
            idealDirection = CENTER;
            swivelRead(RIGHT);
            secondWall = true;
        }
    } else { // CENTER
        if ((!firstWall && !secondWall)) { // If we are before the first wall or passed the second, we just follow the wall
            if (millis() - t < 3000) {
                followWall(percentSpeed, distanceThreshold, RIGHT);
                // bring the speed up to try to counteract the natural motor speed decrease
                percentSpeed += 0.0004;
            } else {
                setMotorSpeed(LEFT, 0, FORWARD);
                setMotorSpeed(RIGHT, 0, FORWARD);
                if (swivelRead(CENTER) < distanceThreshold) {
                    // reset the speed for the next leg
                    percentSpeed = originalSpeed;
                    if (!firstWall) {
                        // Turn Left
                        setMotorSpeed(LEFT, percentSpeed + 0.4, BACKWARD);
                        setMotorSpeed(RIGHT, percentSpeed + 0.4, FORWARD);
                        delay(500);
                        setMotorSpeed(LEFT, 0, FORWARD);
                        setMotorSpeed(RIGHT, 0, FORWARD);
                        swivelRead(RIGHT);
                        idealDirection = RIGHT;
                        firstWall = true;
                    }
                }
                turnServo(RIGHT);
                t = millis();
            }
        } else if (firstWall && secondWall) { // If we're past all the obstacles, we don't need to check ahead
            followWall(percentSpeed, distanceThreshold, RIGHT);
            // bring the speed up to try to counteract the natural motor speed decrease
            percentSpeed += 0.0004;
        } else { // We know this will be between the two obstacles
            setMotorSpeed(LEFT, percentSpeed, FORWARD);
            setMotorSpeed(RIGHT, percentSpeed, FORWARD);
            // bring the speed up to try to counteract the natural motor speed decrease
            percentSpeed += 0.0004;
            if (swivelRead(CENTER) < distanceThreshold) {
                // reset the speed for the next leg
                percentSpeed = originalSpeed;
                setMotorSpeed(LEFT, 0, FORWARD);
                setMotorSpeed(RIGHT, 0, FORWARD);
                delay(50);
                // turn right
                setMotorSpeed(LEFT, percentSpeed + 0.4, FORWARD);
                setMotorSpeed(RIGHT, percentSpeed + 0.4, BACKWARD);
                delay(500);
                setMotorSpeed(LEFT, 0, FORWARD);
                setMotorSpeed(RIGHT, 0, FORWARD);
                delay(50);
                idealDirection = LEFT;
            }
        }
        
    }
}