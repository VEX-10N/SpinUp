#include "vex.h"

using namespace vex;
using signature = vision::signature;
using code = vision::code;

// A global instance of brain used for printing to the V5 Brain screen
brain  Brain;

// VEXcode device constructors
controller Controller1 = controller(primary);
motor CataMotor = motor(PORT10, ratio18_1, false);
motor RollerMotor = motor(PORT5, ratio18_1, false);
motor RightFront = motor(PORT17, ratio18_1, false);
motor RightBack = motor(PORT19, ratio18_1, false);
motor LeftFront = motor(PORT18, ratio18_1, true);
motor LeftBack = motor(PORT20, ratio18_1, true);
digital_in DigitalInB = digital_in(Brain.ThreeWirePort.B);
inertial Inertial = inertial(PORT3);

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