#include "main.h"
#include "lemlib/api.hpp" // IWYU pragma: keep
#include "DummyController.h"
#include "Autonomous.h"
#include "Recording.h"
#include "DriverControl.h"
#include "AutonSelector.h"
#include "AutonControl.h"
#include <cmath>

using namespace std;

string colorMode = "blue";

	
// Controller
ControllerBase master(pros::E_CONTROLLER_MASTER);
ControllerBase& masterRef = master;


// Motor groups for drive
okapi::MotorGroup leftMotors({-11, 12, -13});
okapi::MotorGroup& leftMotorsRef = leftMotors;

okapi::MotorGroup rightMotors({18, -19, 20});
okapi::MotorGroup& rightMotorsRef = rightMotors;

// Intake
pros::Motor intake(9);
pros::Motor& intakeRef = intake;

// Intake 2a
pros::Motor flexWheelIntake(10);
pros::Motor& flexWheelIntakeRef = flexWheelIntake;

// Arm

pros::Motor arm(3);
pros::Motor& armRef = arm;

// Two way piston - MOGO
pros::ADIDigitalOut pistonA('A');
pros::ADIDigitalOut& pistonARef = pistonA;
pros::ADIDigitalOut pistonB('B');
pros::ADIDigitalOut& pistonBRef = pistonB;

// Doinker
pros::ADIDigitalOut doinker('C');
pros::ADIDigitalOut& doinkerRef = doinker;

// Optical (Port TBD)
pros::Optical optical(15);
pros::Optical& opticalRef = optical;

pros::Vision vision(1);
pros::Vision& visionRef = vision;

// Distance
pros::Distance distanceSensor(9);
pros::Distance& distanceRef = distanceSensor;

// Inertial
pros::IMU inertial(8);
pros::IMU& inertialRef = inertial;

// Auton Selector
AutonSelector autonSelector;
AutonSelector& autonSelectorRef = autonSelector;

// Recording
Recording recording(autonSelectorRef);
Recording& recordingRef = recording;

// Recorder
Recorder recorder(masterRef, recordingRef);
Recorder &recorderRef = recorder;

// Player
Player player(recordingRef);

// Dummy Controller for playing autons
DummyController dummy(pros::E_CONTROLLER_MASTER, true, &player);
DummyController& dummyRef = dummy;

// Autonomous
Autonomous autonomousManager(dummyRef, leftMotorsRef, rightMotorsRef, intakeRef, pistonARef, pistonBRef, doinkerRef, opticalRef, distanceRef);

// Driver Control
DriverControl driverControl(masterRef, leftMotorsRef, rightMotorsRef, intakeRef, flexWheelIntakeRef, armRef, pistonARef, pistonBRef, doinkerRef, opticalRef, distanceRef, recorderRef);

// Position tracking (EXPERIMENTAL)
double x = 0;
double y = 0;
double z = 0;

double lastXVelocity = 0;
double lastYVelocity = 0;
double lastZVelocity = 0;

double theta = 0;

/**
 * A callback function for LLEMU's center button.
 *
 * Toggles auton (Auton Selector)
 */
void on_center_button()
{
	autonSelector.toggleAuton();
	pros::lcd::set_text(4, autonSelector.getAuton());
}

/**
 * Runs initialization code. This occurs as soon as the program is started.
 *
 * All other competition modes are blocked by initialize; it is recommended
 * to keep execution time for this mode under a few seconds.
 */
void initialize()
{	
	pros::lcd::initialize();
	pros::lcd::set_background_color(LV_COLOR_RED);
	pros::lcd::set_text(4, autonSelector.getAuton());
	// Mogo Mechanism open by default
	pistonA.set_value(true);
	pistonB.set_value(true);
	
	pros::lcd::set_text(1, "Initialized!");
	// master.print(0, 0, "Manual: %d", manual);
	// master.print(0, 1, "Velocity: %d", velocity);
	arm.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);

	pros::lcd::register_btn1_cb(on_center_button);
}

/**
 * Runs while the robot is in the disabled state of Field Management System or
 * the VEX Competition Switch, following either autonomous or opcontrol. When
 * the robot is enabled, this task will exit.
 */
void disabled() {}

/**
 * Runs after initialize(), and before autonomous when connected to the Field
 * Management System or the VEX Competition Switch. This is intended for
 * competition-specific initialization routines, such as an autonomous selector
 * on the LCD.
 *
 * This task will exit when the robot is enabled and autonomous or opcontrol
 * starts.
 */
void competition_initialize() {
	// driverControl.setCompetitonMode(true);
}
/**
 * Runs the user autonomous code. This function will be started in its own task
 * with the default priority and stack size whenever the robot is enabled via
 * the Field Management System or the VEX Competition Switch in the autonomous
 * mode. Alternatively, this function may be called in initialize or opcontrol
 * for non-competition testing purposes.
 *
 * If the robot is disabled or communications is lost, the autonomous task
 * will be stopped. Re-enabling the robot will restart the task, not re-start it
 * from where it left off._absolute(1000, 600);
 */
void autonomous()
{
	// pros::vision_signature_s_t blueRing = vision.signature_from_utility(1, -3901, -2775, -3338, 3293, 5509, 4401, 3.000, 0);
	// pros::vision_signature_s_t redRing = vision.signature_from_utility(2, 8043, 9741, 8892, -2259, -929, -1594, 3.000, 0);
	// vision.set_signature(1, &blueRing);
	// vision.set_signature(2, &redRing);
	// pistonA.set_value(false);
	// pistonB.set_value(false);
	// bool intakeAvailable = true;
	// while (true)
	// {

	// 	pros:: vision_object_s_t object = vision.get_by_sig(0, 2);
	// 	pros::lcd::print(0, "X: %d", object.x_middle_coord);
	// 	pros::lcd::print(1, "Y: %d", object.y_middle_coord);
	// 	pros::lcd::print(3, "Angle: %d", object.angle);

	// 	int screenCenterX = 158;
	// 	int screenCenterY = 120;

	// 	double rotationError = object.x_middle_coord - screenCenterX;
	// 	pros::lcd::print(2, "Width: %d", object.width);

	// 	double ratio = 100.0 / object.width;

	// 	if (!object.width == 0)
	// 	{

	// 	if (distanceSensor.get() > 100)
	// 	{
	// 			if (intake.get_position() > intake.get_target_position() - 20 && intake.get_position() < intake.get_target_position() + 20)
	// 			{
	// 				intakeAvailable = true;
	// 			}
	// 			leftMotors.moveVelocity(rotationError * .15);
	// 		rightMotors.moveVelocity(-rotationError * .15);
	// 	} else {
	// 		if (intakeAvailable)
	// 		{
	// 			intake.move_relative(2242, 600);
	// 			intakeAvailable = false;
	// 		} else {
	// 			if (intake.get_position() > intake.get_target_position() - 20 && intake.get_position() < intake.get_target_position() + 20)
	// 			{
	// 				intakeAvailable = true;
	// 			}
	// 		}
	// 		leftMotors.moveVelocity(0);
	// 		rightMotors.moveVelocity(0);
	// 	}
	// 	}

	// 	pros::delay(20);
	// }
	
	// We will be using my rerun implementation this tournament.
	/**
	 * Stage System:
	 * 	Driver runs are stored in files on the sd card. Each file is a stage.
	 * 	Each stage is a different part of the auton.
	 *  Running autonomousManager.run() plays the next stage. Stages are determined by their
	 *  file name on the sd card, and are read and saved as such.
	 */
	autonomousManager.run(); // Runs the first stage of the auton - should clamp MOGO
	intake.move_relative(1200, 600); // Scores preload
	autonomousManager.run(); // Runs the next stage of the auton
	autonomousManager.run();
	// autonomousManager.run();
	// autonomousManager.run();

	// AutonControl autonControl = AutonControl(leftMotorsRef, rightMotorsRef, inertialRef);
	// autonControl.turnFor(90);

	// Incomplete
	// Preferably with actual odom, but if it is accurate enough, we can use this
	// auto drive = ChassisControllerBuilder()
	// 				 .withMotors(leftMotors, rightMotors)
	// 				 .withDimensions({AbstractMotor::gearset::blue, 48.0/36.0}, {{3.25_in, 13_in}, imev5BlueTPR})
	// 				 .withMaxVelocity(200)
	// 				 .withOdometry()
	// 				 .buildOdometry();
	// drive->getOdometry()->setState({0_ft, 0_ft, 0_deg});
	// drive->moveDistanceAsync(1_ft);
	// drive->waitUntilSettled();
	// drive->turnAngle(90_deg);
	// while (true) {
	// 	pros::lcd::print(4, "X: %f", drive->getState().x);
	// 	pros::lcd::print(5, "Y: %f", drive->getState().y);
	// 	pros::lcd::print(6, "Theta: %f", drive->getState().theta);
	// 	pros::delay(20);
	// }
	// std::shared_ptr<AsyncMotionProfileController> profileController = 
	// AsyncMotionProfileControllerBuilder()
	// 	.withLimits({
	// 	1.0, // Maximum linear velocity of the Chassis in m/s
	// 	2.0, // Maximum linear acceleration of the Chassis in m/s/s
	// 	7.0 // Maximum linear jerk of the Chassis in m/s/s/s
	// 	})
	// 	.withOutput(drive)
	// 	.buildMotionProfileController();
	// profileController->generatePath({
	// 	{1_ft, 0_ft, 0_deg}
	// }, "A");
	// profileController->setTarget("A");
	// profileController->waitUntilSettled();

}

/**
 * Runs the operator control code. This function will be started in its own task
 * with the default priority and stack size whenever the robot is enabled via
 * the Field Management System or the VEX Competition Switch in the operator
 * control mode.
 *
 * If no competition control is connected, this function will run immediately
 * following initialize().
 *
 * If the robot is disabled or communications is lost, the
 * operator control task will be stopped. Re-enabling the robot will restart the
 * task, not resume it from where it left off.
 */
void opcontrol()
{
	driverControl.run();
}

