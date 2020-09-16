/** Jonah Burke
  * 3/31/2020
  * CSCE 236, Prof. Falkinburg
  * 
  * This is the motor control library
  */

// The motors don't run as fast as they can with this max value
// But I figured that was safer and more power-concious
#define MOTOR_TOP_SPEED 200

// Helps keep what the code in the source file easier to folow
typedef enum {
    BACKWARD,
    FORWARD
} Direction;

// Disable both motors because you don't often disable just one
void disableMotors();

// Disable both motors because you don't often enable just one
void enableMotors();

// Set up the PWM on the motors and initialize them to off
void setupMotors();

// Set the direction and speed of one of the motors
void setMotorSpeed(int side, double percentPower, Direction d);

// This is for the required functionality
void motorTest();

// this function will correct the motor speeds based on distance away from the wall
void followWall(double percentPower, double maintainDistance, int side);