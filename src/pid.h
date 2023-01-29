#include <vex.h>
#include "movement.h"

using namespace vex;

const double kP = 0;
const double kD = 0;

const double turnkP = 0;
const double turnkD = 0;

const double errorThreshold = 0.25;

double error = 0;
double prevError = 0;
double derivative = 0;

double lateralPIDCycle(double targetDistance, double maxSpeed) {
  double speed = 0;
  error = targetDistance;
  if (fabs(error) > errorThreshold) {
    derivative = error - prevError;
    speed = (kP * error) + (kD * derivative);
    if (speed < -maxSpeed) speed = -maxSpeed;
    if (speed > maxSpeed) speed = maxSpeed;
    if (speed > 0 && speed < 2) speed = 2;
    if (speed < 0 && speed > -2) speed = -2;
  } else {
    derivative = 0;
    speed = 0;
  }
  prevError = error;
  return speed;
}

void forwardPID(double distance, double maxSpeed) {
  double speed = 1;
  resetMotors();
  while (speed != 0) {
    double error = distance - getTotalDistance();
    speed = lateralPIDCycle(error, maxSpeed);
    spinLeftMotors(forward, speed);
    spinRightMotors(forward, speed);
    task::sleep(10);
  }
  stopMotors();
}