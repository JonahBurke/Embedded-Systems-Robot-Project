/*--------------------------------------------------------------------
Name:   Jonah Burke
Date:   19 April 2020  
Course: CSCE 236 Embedded Systems (Spring 2020) 
File:   remoteControl.cpp
HW/Lab: Project 2

Purp: This program drives the robot using IR signals 

Doc:  Did not receive help, did not help anyone else

Academic Integrity Statement: I certify that, while others may have 
assisted me in brain storming, debugging and validating this program, 
the program itself is my own work. I understand that submitting code 
which is the work of other individuals is a violation of the honor   
code.  I also understand that if I knowingly give my original work to 
another individual is also a violation of the honor code.
--------------------------------------------------------------------*/
#include <Arduino.h>
#include "ultrasonic.h"
#include "motor.h"
#include "ir.h"

volatile uint8_t  newIrPacket = FALSE;
uint8_t irCode[SAMPLE_SIZE];
double percentSpeed = 0.5; // arbitrary default percent speed
Instruction lastValid = INVALID;
unsigned long time = 0;

void setup() {
  /* Turn off WDT */ 
  WDTCSR = 0x00; 
  
  Serial.begin(9600);
  Serial.println("Setup Starting");
  ledSetup();
  setupMotors();
  irSetup();
  Serial.println("Setup Done");
}

void loop() {
    time = millis();
    while(IR_DECODER_PIN != 0) { // When not receiving an IR signal, do nothing.
      if (millis() - time > 50) {
        // The program was entering this loop even when the button was held down,
        // so we check to see if we've been in the loop long enough before stopping the motors
        setMotorSpeed(LEFT, 0, FORWARD);
        setMotorSpeed(RIGHT, 0, FORWARD);
      }
    } 

    TCNT1 = 0;                  // reset timer  
    newIrPacket = FALSE;
	  TIFR1 |= (1 << OCF1A);		// Clear Timer/Counter1, Output Compare A Match Flag by writing 1 
	  TIMSK1 = (1 << OCIE1A);	    // Enable interrupt on match with OCR1A

    for(int i=0; i<SAMPLE_SIZE; i++) {
      while(IR_DECODER_PIN==0 && newIrPacket == FALSE);   // wait while IR is logic 0

      TCNT1 = 0;                  // reset timer
      while(IR_DECODER_PIN != 0 && newIrPacket == FALSE); // wait while IR is logic 1
      irCode[i] = (TCNT1>200)?1:0;           // store logic 1 or 0 depending on the duration of the signal 

    }

    executeinstruction(decodeInstruction(irCode, &lastValid), &percentSpeed);
}

/**
 * This function is called whenever the timer 1 output compare match OCR1A
 * is generated.
 **/
ISR(TIMER1_COMPA_vect){
  TIMSK1 &= ~(1 << OCIE1A);	// Disable interrupt on match with OCR1A
  newIrPacket = TRUE;
}
