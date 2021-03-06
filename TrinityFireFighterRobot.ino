/*
 * Author: Justin Marple
 * Contact: jmarple@umass.edu
 * Started: 9/27/13
 *
 * Trinity College Firefighter Code V0.01
 */

#include "tcMotor.h"

DriveMotor leftDrive;
DriveMotor rightDrive; 

void setup()
{
  /* Motor Setup */
  setMotorDefaults(&leftDrive);
  leftDrive.stepPin = 22;
  setMotorDefaults(&rightDrive);
  rightDrive.stepPin = 23;
  
  pinMode(leftDrive.stepPin, OUTPUT);
  pinMode(rightDrive.stepPin, OUTPUT);  
}

void loop()
{
   runMotor(&leftDrive);
   runMotor(&rightDrive);
}





 
