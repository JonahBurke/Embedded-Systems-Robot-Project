#include<Arduino.h>
#include"ir.h"
#include"motor.h"
#include"ultrasonic.h"

// These are the codes for the different buttons on the remote
uint8_t pause[] = {0,1,0,1,1,1,1,0,1,0,1,0,0,0,0,1,0,0,0,1,1,0,1,0,1,1,1,0,0,1,0,1};
uint8_t stop[] = {0,1,0,1,1,1,1,0,1,0,1,0,0,0,0,1,1,0,0,1,1,0,1,0,0,1,1,0,0,1,0,1};
uint8_t skipBack[] = {0,1,0,1,1,1,1,0,1,0,1,0,0,0,0,1,1,1,0,0,1,0,1,0,0,0,1,1,0,1,0,1};
uint8_t skipForward[] = {0,1,0,1,1,1,1,0,1,0,1,0,0,0,0,1,0,1,0,0,1,0,1,0,1,0,1,1,0,1,0,1};
uint8_t chUp[] = {0,1,0,1,1,1,1,0,1,0,1,0,0,0,0,1,0,0,0,0,1,0,0,0,1,1,1,1,0,1,1,1};
uint8_t chDown[] = {0,1,0,1,1,1,1,0,1,0,1,0,0,0,0,1,1,0,0,0,1,0,0,0,0,1,1,1,0,1,1,1};
uint8_t empty[] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};

void irSetup() {
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
}

Instruction decodeInstruction(uint8_t *inputSignal, Instruction *lastValidInstruction) {
    if (isButton(inputSignal, pause)) {
        *lastValidInstruction = MOVE_FORWARD;
        return MOVE_FORWARD;
    } else if (isButton(inputSignal, stop)) {
        *lastValidInstruction = MOVE_BACKWARD;
        return MOVE_BACKWARD;
    } else if (isButton(inputSignal, skipBack)) {
        *lastValidInstruction = TURN_LEFT;
        return TURN_LEFT;
    } else if (isButton(inputSignal, skipForward)) {
        *lastValidInstruction = TURN_RIGHT;
        return TURN_RIGHT;
    } else if (isButton(inputSignal, chUp)) {
        *lastValidInstruction = SPEED_UP;
        return SPEED_UP;
    }else if (isButton(inputSignal, chDown)) {
        *lastValidInstruction = SPEED_DOWN;
        return SPEED_DOWN;
    } else if (isButton(inputSignal, empty)) {
        return *lastValidInstruction;
    } else {
        return INVALID;
    }
}

bool isButton(uint8_t *inputSignal, uint8_t *instrArr) {
    // I tried to be cleverer than this, but bitmasking wasn't forking for some reason
    for (int i = 1; i < SAMPLE_SIZE-1; i++) {
        if (inputSignal[i] != instrArr[i-1]) {
            return false;
        }
    }
    return true;
}

void executeinstruction(Instruction instruction, double *percentSpeed) {
    switch(instruction) {
        case MOVE_FORWARD:
            setMotorSpeed(LEFT, *percentSpeed, FORWARD);
            setMotorSpeed(RIGHT, *percentSpeed, FORWARD);
            break;
        case MOVE_BACKWARD:
            setMotorSpeed(LEFT, *percentSpeed, BACKWARD);
            setMotorSpeed(RIGHT, *percentSpeed, BACKWARD);
            break;
        case TURN_RIGHT:
            setMotorSpeed(LEFT, *percentSpeed, FORWARD);
            setMotorSpeed(RIGHT, *percentSpeed, BACKWARD);
            break;
        case TURN_LEFT:
            setMotorSpeed(LEFT, *percentSpeed, BACKWARD);
            setMotorSpeed(RIGHT, *percentSpeed, FORWARD);
            break;
        case SPEED_UP:
            *percentSpeed += (*percentSpeed<=0.9)?0.05:0;
            break;
        case SPEED_DOWN:
            *percentSpeed -= (*percentSpeed>=0.1)?0.05:0;
            break;
        default:
            Serial.println("Not an instruction");
            break;
    }
}