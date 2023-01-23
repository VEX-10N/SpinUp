#include "vex.h"
#include <string>
#include <iostream>

using namespace vex;

// A global instance of competition
competition Competition;

// define your global instances of motors and other devices here

/*---------------------------------------------------------------------------*/
/*                          Pre-Autonomous Functions                         */
/*                                                                           */
/*  You may want to perform some actions before the competition starts.      */
/*  Do them in the following function.  You must return from this function   */
/*  or the autonomous and usercontrol tasks will not be started.  This       */
/*  function is only called once after the V5 has been powered on and        */
/*  not every time that the robot is disabled.                               */
/*---------------------------------------------------------------------------*/

bool rollerRunning = false;
bool cataRunning = false;
bool selectingAuton = true;
bool calibratingCata = false;
double initialDegree;
int autonType = 1;
directionType rollerDirection = fwd;

//PID Auton Settings
int desiredValue = 0;
int desiredTurnValue = 0;

// PID CONTROLLER
bool enableDrivePID = false;

bool resetDriveSensors = false;

const double kP = 0.0;
const double kI = 0.0;
const double kD = 0.0;

const double turnkP = 0.0;
const double turnkI = 0.0;
const double turnkD = 0.0;

int error;
int prevError = 0;
int derivative;
int totalError = 0;

int turnError;
int turnPrevError = 0;
int turnDerivative;
int turnTotalError = 0;

int drivePID() {
  while (enableDrivePID) {
    if (resetDriveSensors) {
      LeftFront.resetPosition();
      LeftBack.resetPosition();
      RightFront.resetPosition();
      RightBack.resetPosition();
      resetDriveSensors = false;
    }

    //Movement PID
    int averagePosition = (LeftFront.position(degrees) + RightFront.position(degrees) + LeftBack.position(degrees) + LeftFront.position(degrees)) / 4;
    error = desiredValue - averagePosition;
    derivative = error - prevError;
    totalError += error;
    double lateralMotorPower = error * kP + derivative * kD + totalError * kI;

    //Turning PID
    int turnDifference = ((LeftFront.position(degrees) + LeftBack.position(degrees)) / 2) - ((RightFront.position(degrees) + RightBack.position(degrees)) / 2);
    turnError = desiredTurnValue - turnDifference;
    turnDerivative = turnError - turnPrevError;
    turnTotalError += turnError;
    double turnMotorPower = turnError * turnkP + turnDerivative * turnkD + turnTotalError * turnkI;

    LeftFront.spin(forward, lateralMotorPower + turnMotorPower, voltageUnits::volt);
    LeftBack.spin(forward, lateralMotorPower + turnMotorPower, voltageUnits::volt);
    RightFront.spin(forward, lateralMotorPower - turnMotorPower, voltageUnits::volt);
    RightBack.spin(forward, lateralMotorPower - turnMotorPower, voltageUnits::volt);

    prevError = error;
    turnPrevError = turnError;
    vex::task::sleep(20);
  }
  return 1;
}

void resetPID() {
  resetDriveSensors = true;
}

void movePID(int distance) {
  resetPID();
  desiredValue = distance;
  desiredTurnValue = 0;
  waitUntil(error == 0);
}

void rotatePID(int rotation) {
  resetPID();
  desiredTurnValue = rotation;
  desiredValue = 0;
  waitUntil(turnError = 0);
}

void change_roller_direction() {
  if (rollerDirection == fwd) {
    rollerDirection = reverse;
  } else if (rollerDirection == reverse) {
    rollerDirection = fwd;
  }
  RollerMotor.spin(rollerDirection, 150, rpm);
}

void start_stop_roller() {
  rollerRunning = !rollerRunning;
  if (rollerRunning) {
    RollerMotor.spin(rollerDirection, 150, rpm);
  } else {
    RollerMotor.stop();
  }
}

void drawGUI() {
  Brain.Screen.clearScreen();
  Brain.Screen.setFillColor(transparent);
  Brain.Screen.printAt(0, 20, "10 Alpha Auton Selection");
  Brain.Screen.setFillColor(blue);
  Brain.Screen.drawRectangle(300, 50, 100, 50);
  Brain.Screen.printAt(300, 40, "Switch Mode");
  Brain.Screen.setFillColor(red);
  Brain.Screen.drawRectangle(300, 150, 100, 50);
  Brain.Screen.printAt(300, 140, "Lock In Mode");
  if (!selectingAuton) {
    Brain.Screen.setFillColor(green);
  } else {
    Brain.Screen.setFillColor(red);
  }
  Brain.Screen.drawCircle(150, 150, 50);
  Brain.Screen.setFillColor(transparent);
  if (autonType == 1) Brain.Screen.printAt(150, 80, "1");
  if (autonType == 2) Brain.Screen.printAt(150, 80, "2");
  if (autonType == 3) Brain.Screen.printAt(150, 80, "3");
  if (autonType == 4) Brain.Screen.printAt(150, 80, "4");
  if (autonType == 5) Brain.Screen.printAt(150, 80, "5");
}

void screenPressed() {
  int x = Brain.Screen.xPosition();
  int y = Brain.Screen.yPosition();
  if (!selectingAuton) return;
  if (x >= 300 && x <= 400) {
    if (y >= 50 && y <= 100) {
      if (autonType >= 5) {
        autonType = 1;
      } else {
        autonType++;
      }
    } 
    if (y >= 150 && y <= 200) {
      selectingAuton = false;
    }
  }
  drawGUI();
}

void calibrateCata() {
  if (!calibratingCata) {
    initialDegree = CataMotor.position(degrees);
  } else {
    double target = initialDegree - CataMotor.position(degrees);
    std::cout << target << std::endl;
  }
}

void reset_to_intake() {
  CataMotor.spinFor(forward, 1578, degrees);
}

void reset_from_intake() {
  CataMotor.spinFor(forward, 1856, degrees);
}

void pre_auton(void) {
  // Initializing Robot Configuration. DO NOT REMOVE!
  vexcodeInit();
  RollerMotor.setVelocity(200, rpm);
  Controller1.ButtonR1.pressed(start_stop_roller);
  Controller1.ButtonR2.pressed(change_roller_direction);
  Controller1.ButtonL1.pressed(calibrateCata);
  Brain.Screen.pressed(screenPressed);
  LeftFront.setStopping(brake);
  LeftBack.setStopping(brake);
  RightFront.setStopping(brake);
  RightBack.setStopping(brake);
  drawGUI();
  // All activities that occur before the competition starts
  // Example: clearing encoders, setting servo positions, ...
}

/*---------------------------------------------------------------------------*/
/*                                                                           */
/*                              Autonomous Task                              */
/*                                                                           */
/*  This task is used to control your robot during the autonomous phase of   */
/*  a VEX Competition.                                                       */
/*                                                                           */
/*  You must modify the code to add your own robot specific commands here.   */
/*---------------------------------------------------------------------------*/

// This runs in autonomous
void autonomous(void) {
    task drivePIDTask(drivePID);
    movePID(200);
}


/*---------------------------------------------------------------------------*/
/*                                                                           */
/*                              User Control Task                            */
/*                                                                           */
/*  This task is used to control your robot during the user control phase of */
/*  a VEX Competition.                                                       */
/*                                                                           */
/*  You must modify the code to add your own robot specific commands here.   */
/*---------------------------------------------------------------------------*/

void usercontrol(void) {
  drawGUI();
  int threshold = 15;
  while (1) {
    int x = -Controller1.Axis3.position(percent);
    int y = -Controller1.Axis1.position(percent);
    
    if (x >= -threshold && x <= threshold) {
      x = 0;
    } else if (y >= -threshold && y <= threshold) {
      y = 0;
    }

    if (Controller1.ButtonL2.pressing()) {
      CataMotor.spin(forward, 100, rpm);
    } else {
      CataMotor.stop(brake);
    }

    int leftSpeed = y + x;
    int rightSpeed = y - x;

    LeftFront.setVelocity(leftSpeed * .65, rpm);
    LeftBack.setVelocity(leftSpeed * .65, rpm);
    RightFront.setVelocity(rightSpeed * .65, rpm);
    RightBack.setVelocity(rightSpeed * .65, rpm);

    LeftFront.spin(forward);
    LeftBack.spin(forward);
    RightFront.spin(forward);
    RightBack.spin(forward);

    wait(20, msec); // Sleep the task for a short amount of time to
                    // prevent wasted resources.
  }
}

//
// Main will set up the competition functions and callbacks.
//

int main() {
  // Set up callbacks for autonomous and driver control periods.
  Competition.autonomous(autonomous);
  Competition.drivercontrol(usercontrol);

  // Run the pre-autonomous function.
  pre_auton();

  // Prevent main from exiting with an infinite loop.
  while (true) {
    wait(100, msec);
  }
}
