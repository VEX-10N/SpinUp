using namespace vex;

extern brain Brain;

// VEXcode devices
extern controller Controller1;
extern motor CataMotor;
extern motor RollerMotor;
extern motor RightFront;
extern motor RightBack;
extern motor LeftFront;
extern motor LeftBack;

/**
 * Used to initialize code/tasks/devices added using tools in VEXcode Pro.
 * 
 * This should be called at the start of your int main function.
 */
void  vexcodeInit( void );