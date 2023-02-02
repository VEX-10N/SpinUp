// ---- START VEXCODE CONFIGURED DEVICES ----
// Robot Configuration:
// [Name]               [Type]        [Port(s)]
// Controller1          controller                    
// CataMotor            motor         10              
// RollerMotor          motor         5               
// RightFront           motor         17              
// RightBack            motor         19              
// LeftFront            motor         18              
// LeftBack             motor         20              
// DigitalInB           digital_in    B               
// Inertial             inertial      3               
// ---- END VEXCODE CONFIGURED DEVICES ----
// ---- START VEXCODE CONFIGURED DEVICES ----
// Robot Configuration:
// [Name]               [Type]        [Port(s)]
// Controller1          controller                    
// CataMotor            motor         10              
// RollerMotor          motor         5               
// RightFront           motor         17              
// RightBack            motor         19              
// LeftFront            motor         18              
// LeftBack             motor         20              
// DigitalInB           digital_in    B               
// Inertial             inertial      3               
// ---- END VEXCODE CONFIGURED DEVICES ----
// ---- START VEXCODE CONFIGURED DEVICES ----
// Robot Configuration:
// [Name]               [Type]        [Port(s)]
// Controller1          controller                    
// CataMotor            motor         10              
// RollerMotor          motor         5               
// RightFront           motor         17              
// RightBack            motor         19              
// LeftFront            motor         18              
// LeftBack             motor         20              
// DigitalInB           digital_in    B               
// Inertial             inertial      3               
// ---- END VEXCODE CONFIGURED DEVICES ----
// ---- START VEXCODE CONFIGURED DEVICES ----
// Robot Configuration:
// [Name]               [Type]        [Port(s)]
// Controller1          controller                    
// CataMotor            motor         10              
// RollerMotor          motor         5               
// RightFront           motor         17              
// RightBack            motor         19              
// LeftFront            motor         18              
// LeftBack             motor         20              
// DigitalInB           digital_in    B               
// Inertial             inertial      3               
// ---- END VEXCODE CONFIGURED DEVICES ----
#include "vex.h"
#include <string>
#include <sstream>
#include <iostream>
#include "pid.h"

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
bool firingCata = false;
double initialDegree;
int autonType = 5;
directionType rollerDirection = fwd;

void fire_cata() {
  firingCata = true;
  CataMotor.spin(forward, 100, rpm);
  task::sleep(500);
  std::cout << DigitalInB.value() << std::endl;
  waitUntil(DigitalInB.value() == 0);
  CataMotor.stop();
  firingCata = false;
}

void change_roller_direction() {
  if (rollerDirection == fwd) {
    rollerDirection = reverse;
  } else if (rollerDirection == reverse) {
    rollerDirection = fwd;
  }
  RollerMotor.spin(rollerDirection, 200, rpm);
}

void start_stop_roller() {
  rollerRunning = !rollerRunning;
  if (rollerRunning) {
    RollerMotor.spin(rollerDirection, 200, rpm);
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
  std::ostringstream ss;
  ss << autonType;
  if (autonType == 1) Brain.Screen.printAt(150, 80, "1");
  if (autonType == 2) Brain.Screen.printAt(150, 80, "2");
  if (autonType == 3) Brain.Screen.printAt(150, 80, "3");
  if (autonType == 4) Brain.Screen.printAt(150, 80, "4");
  if (autonType == 5) Brain.Screen.printAt(150, 80, "5");
  if (autonType == 6) Brain.Screen.printAt(150, 80, "6");
}

void drawGUIController() {
  Controller1.Screen.clearScreen();
  Controller1.Screen.setCursor(1, 1);
  if (autonType == 1) Controller1.Screen.print("Auton Mode 1");
  if (autonType == 2) Controller1.Screen.print("Auton Mode 2");
  if (autonType == 3) Controller1.Screen.print("Auton Mode 3");
  if (autonType == 4) Controller1.Screen.print("Auton Mode 4");
  if (autonType == 5) Controller1.Screen.print("Auton Mode 5");
  if (autonType == 6) Controller1.Screen.print("Auton Mode 6");
  Controller1.Screen.setCursor(2, 1);
  Controller1.Screen.print("A: Change X: Lock");
  Controller1.Screen.setCursor(3, 1);
  if (!selectingAuton) {
    Controller1.Screen.print("Locked In"); 
  } else {
    Controller1.Screen.print("Not Locked In");
  }
}

void increaseAutonMode() {
  if (!selectingAuton) return;
  if (autonType >= 6) {
    autonType = 1;
  } else {
    autonType++;
  }
  drawGUI();
  drawGUIController();
}

void lockInAutonMode() {
  selectingAuton = false;
  drawGUI();
  drawGUIController();
}

void screenPressed() {
  int x = Brain.Screen.xPosition();
  int y = Brain.Screen.yPosition();
  if (!selectingAuton) return;
  if (x >= 300 && x <= 400) {
    if (y >= 50 && y <= 100) {
      increaseAutonMode();
    } 
    if (y >= 150 && y <= 200) {
      lockInAutonMode();
    }
  }
}

void pre_auton(void) {
  // Initializing Robot Configuration. DO NOT REMOVE!
  vexcodeInit();
  RollerMotor.setVelocity(200, rpm);
  Controller1.ButtonR1.pressed(start_stop_roller);
  Controller1.ButtonR2.pressed(change_roller_direction);
  Controller1.ButtonL2.pressed(fire_cata);
  Controller1.ButtonA.pressed(increaseAutonMode);
  Controller1.ButtonX.pressed(lockInAutonMode);
  Brain.Screen.pressed(screenPressed);
  LeftFront.setStopping(brake);
  LeftBack.setStopping(brake);
  RightFront.setStopping(brake);
  RightBack.setStopping(brake);
  Inertial.calibrate();
  drawGUI();
  drawGUIController();
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
  waitUntil(!Inertial.isCalibrating());
  forwardPID(500, 25);
  if (autonType == 1 || autonType == 2) {
    move_for(13, 40,  true);
    task::sleep(2000);
    std::cout << "mank" << std::endl;
    turn_to(90);
    task::sleep(2000);
    move_for(4, 10, false);
    task::sleep(2000);
    if (autonType == 1) {
      RollerMotor.spinFor(forward, 160, degrees);
    } else {
      RollerMotor.spinFor(reverse, 160, degrees);
    }
  } if (autonType == 3 || autonType == 4) {
    task::sleep(2000);
    if (autonType == 3) {
      RollerMotor.spinFor(forward, 160, degrees);
    } else {
      RollerMotor.spinFor(reverse, 160, degrees);
    }
  } else if (autonType == 5) {
    Inertial.setHeading(90, degrees);
    move_for(13, 40, true);
    turn_to(180);
    move_for(6, 40, false);
    task::sleep(2000);
    RollerMotor.spinFor(forward, 270, degrees, true);
    move_for(-10, 40, true);
    turn_to(270);
    move_for(15, 40, false);
    task::sleep(2500);
    RollerMotor.spinFor(forward, 270, degrees, true);
  }
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
    int y = -Controller1.Axis1.position(percent) * .75;
    
    if (x >= -threshold && x <= threshold) {
      x = 0;
    } else if (y >= -threshold && y <= threshold) {
      y = 0;
    }

    if (Controller1.ButtonL1.pressing()) {
      CataMotor.spin(forward, 100, rpm);
    } else if (!firingCata) {
      CataMotor.stop();
    }

    int leftSpeed = y + x;
    int rightSpeed = y - x;

    LeftFront.setVelocity(leftSpeed * .65, rpm);
    LeftBack.setVelocity(leftSpeed * .65, rpm);
    RightFront.setVelocity(rightSpeed * .65, rpm);
    RightBack.setVelocity(rightSpeed * .65, rpm);

    LeftFront.spin(reverse);
    LeftBack.spin(reverse);
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
