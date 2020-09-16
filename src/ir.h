#define SAMPLE_SIZE    34
#define IR_DECODER_PIN    (PIND & (1 << PIND2)) // (i.e. Digital Pin 2)
#define IR_PIN    (PIND2)
#define   TRUE        1
#define   FALSE       0

typedef enum {
    INVALID,
    MOVE_FORWARD,
    MOVE_BACKWARD,
    TURN_RIGHT,
    TURN_LEFT,
    SPEED_UP,
    SPEED_DOWN
} Instruction;

// Setup Timer 1 and the IR input receiver pin
void irSetup();

// Determines the isntructions the input signal correstponds to
Instruction decodeInstruction(uint8_t *inputSignal, Instruction *lastValidInstruction);

// Check to see if the input signal matches the signal for one of the supported remote buttons
bool isButton(uint8_t *inputSignal, uint8_t *instrArr);

// Set or change motor speeds/directions based on the instruction
void executeinstruction(Instruction instruction, double *percentSpeed);
