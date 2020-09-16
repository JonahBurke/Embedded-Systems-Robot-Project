/** Jonah Burke
  * 3/31/2020
  * CSCE 236, Prof. Falkinburg
  * 
  * This is the ultrasonic (and servo) control library
  */

#define LEFT 615
#define RIGHT 120
#define CENTER 380
#define RED_LED (1<<5)
#define GREEN_LED (1<<4)
#define US_TRIGGER (1<<1)
#define US_ECHO (1<<0)
#define SERVO_PIN (1<<3)

// Initialize Fast PWM for the servo
void setupServo();

// Move the servo to one of the 3 key positions
void turnServo(int position);

// Turn an LED on
void ledOn(char led);

// Turn an LED off
void ledOff(char led);

// Initialize the pins for the LEDs
void ledSetup();

// Initialize the ultrasonic sensor pins
void ultraSonicSetup();

// Send out a pulse on the ultrasonic sensor and return the distance to an object
double ultraSonicPulse();

// Turn the servo to a direction and light up an LED if an 
// object in that direction is closer than some threshold
double swivelReadLED(int direction, double threshold);

// Turn the servo to a direction and measure the distance
double swivelRead(int direction);