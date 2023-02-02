#include "vex.h"
#include <iostream>

using namespace vex;

double calculateError(double initial, double desired) {
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
    return error1;
  } else {
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

void move_for(int distance, int speed,bool waitForCompletion) {
  int gear = 84/36;
  LeftFront.spinFor(forward, gear * M_PI * 4 * distance, degrees, speed, rpm,  false);
  LeftBack.spinFor(forward, gear * M_PI * 4 * distance, degrees, speed, rpm, false);
  RightFront.spinFor(forward, gear * M_PI * 4 * distance, degrees, speed, rpm, false);
  RightBack.spinFor(forward, gear * M_PI * 4 * distance, degrees, speed, rpm, waitForCompletion);
}

void turn_to(double desired) {
  double error = calculateError(Inertial.heading(degrees), desired);
  while (error > 2) {
    error = calculateError(Inertial.heading(degrees), desired);
    double speed = error * 0.4;
    if (speed < 10) {
      speed = 10;
    } else if (speed > -10) {
      speed = -10;
    }
    LeftFront.spin(forward, speed, rpm);
    LeftBack.spin(forward, speed, rpm);
    RightFront.spin(reverse, speed , rpm);
    RightBack.spin(reverse, speed, rpm);
    task::sleep(10);
  }
  waitUntil(error < 2);
  LeftFront.stop();
  LeftBack.stop();
  RightFront.stop();
  RightBack.stop();
}