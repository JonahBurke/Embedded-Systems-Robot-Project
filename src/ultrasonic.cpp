/** Jonah Burke
  * 3/31/2020
  * CSCE 236, Prof. Falkinburg
  * 
  * This is the ultrasonic (and servo) control library
  */

#include <Arduino.h>
#include "ultrasonic.h"

void setupServo() {
  // Set up pin as an output
  DDRB |= (1<<2);
  // Enable Fast PWM
  TCCR1A |= (1<<WGM11) | (1<<WGM10);
  TCCR1B |= (1<<WGM13) | (1<<WGM12);
  // Set up COM bits
  TCCR1A |= (1<<COM1B1) | (1<<COM1A1);
  // Set up Clock Divider
  TCCR1B |= (1<<CS11) | (1<<CS10);
  TCCR1B &= ~(1<<CS12);
  // Set up the period for the timer
  OCR1A = 4992;
  // set up the duty cycle for the timer
  OCR1B = CENTER;
}
 
void turnServo(int position) {
  // Changing the duty cycle with the same period changes the position of the servo
  OCR1B = position;
}

void ledOn(char led) {
  if (led == 'r') {
    PORTB |= RED_LED;
  } else if (led == 'g')  {
    PORTB |= GREEN_LED;
  }
}

void ledOff(char led) {
  if (led == 'r') {
    PORTB &= ~RED_LED;
  } else if (led == 'g') {
    PORTB &= ~GREEN_LED;
  }
}

void ledSetup() {
  // Set up Pins 12 and 13 as outputs
  DDRB |= RED_LED | GREEN_LED;
  // Go through a little sequence to visually show setup
  ledOn('r');
  delay(500);
  ledOff('r');
  ledOn('g');
  delay(500);
  ledOff('g');
  delay(500);
  ledOn('g');
  ledOn('r');
  delay(500);
  ledOff('r');
  ledOff('g');
}

void ultraSonicSetup() {
  // Set trigger as output, echo as input
  DDRB |= US_TRIGGER;
  PORTB &= ~US_TRIGGER;
  DDRB &= ~US_ECHO;
}

double ultraSonicPulse() {
  // Send 10 microsec pulse
  PORTB |= US_TRIGGER;
  delayMicroseconds(10);
  PORTB &= ~US_TRIGGER;

  // Wait for the return signal
  while(!(PINB & US_ECHO));
  // Time the return signal
  TCNT1 = 0;
  while((TIFR1&(1<<5)) != (1<<5));
  uint16_t time = ICR1;
  // This modifier comes from multiplying the speed of sound by how long it takes to 
  // increment Timer1 with a 64 prescaler (which comes from the servo setup)
  return (double) time * 0.02700792;
}

double swivelReadLED(int direction, double threshold) {
  turnServo(direction);
  // Wait for servo to be in position before sending out a pulse
  delay(550);
  // Not sure why, but the Ultrasonic sensor is giving delayed values 
  // So I have to "flush out" the value here so that I get an accurate read
  ultraSonicPulse();
  delay(60);
  double distance = ultraSonicPulse();
  if (distance < threshold) {
    switch (direction) {
      case LEFT:
        ledOn('g');
        ledOff('r');
        break;
      case RIGHT:
        ledOn('r');
        ledOff('g');
        break;
      case CENTER:
        ledOn('g');
        ledOn('r');
        break;
    }
  } else {
    ledOff('g');
    ledOff('r');
  }
  return distance;
}

double swivelRead(int direction) {
  turnServo(direction);
  // Wait for servo to be in position before sending out a pulse
  delay(400);
  // Not sure why, but the Ultrasonic sensor is giving delayed values 
  // So I have to "flush out" the value here so that I get an accurate read
  ultraSonicPulse();
  delay(60);
  return ultraSonicPulse();
}
