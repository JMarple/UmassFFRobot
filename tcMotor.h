#include "Arduino.h"

/* Structure to hold information about our drive motors */
struct DriveMotor{ 
  int stepState;
  int steps; 
  int stepPin;
  int stepDelay;
  int stepsPerRotation;
  float wheelDiam; //In mm
  float velocity; //In mm/sec (wheelDiam must be set!)
  float targetVelocity; //In mm/sec (wheelDiam must be set!)
  float acceleration; //In mm/sec/sec
  unsigned long previousStep; //In uS
  unsigned long previousAcc; //In uS
};

/* Function Prototypes */
int getStepDelay(DriveMotor motor);
void setMotorDefaults(DriveMotor *motor);
void runMotor(DriveMotor *motor);

/* Set default values to the motors */
void setMotorDefaults(DriveMotor *motor)
{
  motor->stepPin = 22;
  motor->velocity = 0;
  motor->targetVelocity = 500;  
  motor->wheelDiam = 31;
  motor->stepsPerRotation = 200;
  motor->acceleration = 10;
  motor->stepDelay = getStepDelay(*motor);
}

/* Figure out the step delay of the current velocity */
int getStepDelay(DriveMotor motor)
{
  //Figure out the number of steps per second our robot needs to travel
  float revolutionsPerSecond = motor.velocity / (motor.wheelDiam * 3.14);
  float stepsPerSecond = revolutionsPerSecond * motor.stepsPerRotation;
  
  //Determine how many micro (not millis!) seconds to wait between pulses 
  float delayStepMicroseconds = 1000000/stepsPerSecond; 
 
  return (int)(delayStepMicroseconds/2); 
}

//Run motor at given speed "stepDelay"
void runMotor(DriveMotor *motor)
{
  unsigned long currentTime = micros();
  
  /* Is our motor in the right time frame to "step" again? */
  if((currentTime - motor->previousStep > motor->stepDelay ) && motor->velocity != 0)
  {
    //Reverse pinState to step
    motor->stepState = !motor->stepState;    
    digitalWrite(motor->stepPin, motor->stepState);
    
    //To maintain accuracy, ensure we base this step off the previous step
    motor->previousStep = motor->previousStep + motor->stepDelay;
    
    //Maintain count of the number of steps
    motor->steps++;
  }
  
  /* Update Acceleration */
  unsigned long elaspedTime = currentTime - motor->previousAcc;
 
  //If the velocity and targetVelocity isn't close, lets update
  if(abs(motor->velocity-motor->targetVelocity) > 0.001)
  {
    //Figure out how much to add to velocity based off of how many microseconds have ellasped
    float addToVelocity = (elaspedTime * motor->acceleration)/1000000;    
    
    //Add to velocity if the target is higher
    if(motor->velocity < motor->targetVelocity)
    {
      motor->velocity += addToVelocity;
    }
    //Subtract from velocity if the target is lower
    else
    {
      motor->velocity -= addToVelocity;
    }
    
     //Update how long each step should be in microseconds
     motor->stepDelay = getStepDelay(*motor);
  }   
  
  //Update the last time we checked for acceleration values
  motor->previousAcc = currentTime;  
}



