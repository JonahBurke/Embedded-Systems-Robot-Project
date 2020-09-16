/*--------------------------------------------------------------------
Name:   Jonah Burke
Date:   9 April 2020  
Course: CSCE 236 Embedded Systems (Spring 2020) 
File:   test.cpp
HW/Lab: Lab 5/Project 2, Measure IR Pulses

Purp: Uses counters and interrupts to measure IR Pulses for a 
    remote. 

Doc:  <list the names of the people who you helped>
    <list the names of the people who assisted you>

Academic Integrity Statement: I certify that, while others may have 
assisted me in brain storming, debugging and validating this program, 
the program itself is my own work. I understand that submitting code 
which is the work of other individuals is a violation of the honor   
code.  I also understand that if I knowingly give my original work to 
another individual is also a violation of the honor code.
--------------------------------------------------------------------*/
#include<Arduino.h>
//#include <avr/io.h>

#define SAMPLE_SIZE    34
#define IR_DECODER_PIN    (PIND & (1 << PIND2)) // (i.e. Digital Pin 2)
#define IR_PIN    (PIND2)
#define   TRUE        1
#define   FALSE       0

volatile uint8_t  newIrPacket = FALSE;
uint16_t time1[SAMPLE_SIZE], time0[SAMPLE_SIZE];
uint8_t  i;

/*
 * main loop
 */
void loop() {
  
    Serial.println("Waiting for IR input");

    while(IR_DECODER_PIN != 0); // IR input is nominally logic 1
    TCNT1 = 0;                  // reset timer  
    newIrPacket = FALSE;
	  TIFR1 |= (1 << OCF1A);		// Clear Timer/Counter1, Output Compare A Match Flag by writing 1 
	  TIMSK1 = (1 << OCIE1A);		// Enable interrupt on match with OCR1A

    for(i=0; i<SAMPLE_SIZE; i++) {

      TCNT1 = 0;                  // reset timer and 
      while(IR_DECODER_PIN==0 && newIrPacket == FALSE);   // wait while IR is logic 0
      time0[i] = TCNT1;           // and store timer 1 count 

      TCNT1 = 0;                  // reset timer and
      while(IR_DECODER_PIN != 0 && newIrPacket == FALSE); // wait while IR is logic 1
      time1[i] = TCNT1;           // and store timer 1 count 

    } // end for

    Serial.println("Time Logic 1");
    for(i=0; i<SAMPLE_SIZE; i++) {
        Serial.print("time1["); Serial.print(i,DEC); Serial.print("] = ");Serial.println(time1[i],DEC);
    }        
    Serial.println("Time Logic 0");
    for(i=0; i<SAMPLE_SIZE; i++) {
        Serial.print("time0["); Serial.print(i,DEC); Serial.print("] = ");Serial.println(time0[i],DEC);
    }        
} // end main loop


// -----------------------------------------------------------------------
// -----------------------------------------------------------------------
void setup() {
  /* Turn off WDT */ 
  WDTCSR = 0x00; 
  
  Serial.begin(9600);
  Serial.println("Starting up.");

  //Init Timer 1
  TCCR1A = 0;       /* Reset TCCR1A & TCCR1B to defaults */
  TCCR1B = 0;
  TCCR1B |= (1 << WGM12);	   	/* Set Timer 1 to CTC with OCR1A as top */
  TCCR1B |= (1 << CS11) | (1 << CS10);	    /* Set clock prescaler to clk/64 */
  /* Configure OCR1A (the counter top) to generate an interrupt every 65ms. */
  OCR1A = 0x3F7A; // 16250 counts, which is roughly 65 ms
  TCNT1H = 0;     	/* Clear timer 1 counter register high byte first*/
  TCNT1L = 0; 

  /* Configure IR input on Port D, Pin 2 using registers and (1 << IR_PIN) value */
  DDRD &= ~(1 << IR_PIN);			/* Input IR on Port D, Pin 2 */
  PORTD |= (1 << IR_PIN);			/* Enable Pullup on Port D, Pin 2 */

} // end setup

/**
 * This function is called whenever the timer 1 output compare match OCR1A
 * is generated.
 **/
ISR(TIMER1_COMPA_vect){
  TIMSK1 &= ~(1 << OCIE1A);	// Disable interrupt on match with OCR1A
  newIrPacket = TRUE;
}