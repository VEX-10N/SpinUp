#include "vex.h"
#include <iostream>

using namespace vex;

double calculateError(double initial, double desired) {
  std::cout << initial << std::endl;
  double error1 = desired - initial;
  double error2 = 0;
  if (initial > desired) {
    error2 = (360 - initial) + desired;
  } else if (initial < desired) {
    error2 = (desired - 360) - initial;
  } else {
    return 0;
  }
  if (fabs(error1) < fabs(error2)) {
    //std::cout << "error1" << std::endl;
    return error1;
  } else {
    //std::cout << "error2" << std::endl; 
    return error2;
  }
}

void resetMotors() {
  LeftFront.resetRotation();
  LeftBack.resetRotation();
  RightFront.resetRotation();
  RightBack.resetRotation();
}

double getTotalDistance() {
  return (LeftFront.rotation(deg) + RightFront.rotation(deg)) / 2;
}

void stopMotors() {
  LeftFront.stop(brake);
  LeftBack.stop(brake);
  RightFront.stop(brake);
  RightBack.stop(brake);
}

void spinLeftMotors(directionType direction, double speed) {
  LeftFront.spin(direction, speed, pct);
  LeftBack.spin(direction, speed, pct);
}
void spinRightMotors(directionType direction, double speed) {
  RightFront.spin(direction, speed, pct);
  RightBack.spin(direction, speed, pct);
}

void move_for(double distance, int speed,bool waitForCompletion) {
  int gear = 84/36;
  LeftFront.spinFor(forward, gear * M_PI * 4 * distance, degrees, speed, rpm,  false);
  LeftBack.spinFor(forward, gear * M_PI * 4 * distance, degrees, speed, rpm, false);
  RightFront.spinFor(forward, gear * M_PI * 4 * distance, degrees, speed, rpm, false);
  RightBack.spinFor(forward, gear * M_PI * 4 * distance, degrees, speed, rpm, waitForCompletion);
}

void move_for_sensor(double distance) {
  
}

void turn_to(double desired, double errorCap) {
  double error = calculateError(Inertial.heading(degrees), desired);
  while (fabs(error) > 8) {
    error = calculateError(Inertial.heading(degrees), desired);
    double speed = error * 1.3;
    if (speed < 10) {
      speed = 10;
    } else if (speed > -10) {
      speed = -10;
    }
    LeftFront.spin(reverse, speed, rpm);
    LeftBack.spin(reverse, speed, rpm);
    RightFront.spin(forward, speed , rpm);
    RightBack.spin(forward, speed, rpm);
    task::sleep(10);
  }
  LeftFront.stop();
  LeftBack.stop();
  RightFront.stop();
  RightBack.stop();
}