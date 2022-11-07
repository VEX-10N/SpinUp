#include "vex.h"

using namespace vex;
using signature = vision::signature;
using code = vision::code;

// A global instance of brain used for printing to the V5 Brain screen
brain  Brain;

// VEXcode device constructors
controller Controller1 = controller(primary);
motor RightWheelsMotorA = motor(PORT1, ratio18_1, false);
motor RightWheelsMotorB = motor(PORT11, ratio18_1, false);
motor_group RightWheels = motor_group(RightWheelsMotorA, RightWheelsMotorB);
motor LeftWheelsMotorA = motor(PORT10, ratio18_1, false);
motor LeftWheelsMotorB = motor(PORT20, ratio18_1, false);
motor_group LeftWheels = motor_group(LeftWheelsMotorA, LeftWheelsMotorB);

// VEXcode generated functions
// define variable for remote controller enable/disable
bool RemoteControlCodeEnabled = true;

/**
 * Used to initialize code/tasks/devices added using tools in VEXcode Pro.
 * 
 * This should be called at the start of your int main function.
 */
void vexcodeInit( void ) {
  // nothing to initialize
}