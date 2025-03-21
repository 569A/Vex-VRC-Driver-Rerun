
/**
 * @brief The Autonomous class controls the autonomous behavior of the robot.
 * 
 * This is pretty much an exact copy of the driver control code, but
 * the controller is not a real controller, but a virtual one that gives
 * pre-recorded values at preset times.
 */
#include "Autonomous.h"
#include "DummyController.h"
#include "Ticker.h"
#include "SlewRateController.h"

Ticker autonTicker(20);
int decreasedSensitivityFactor = 3;
bool autonManual = false;

int autonTicksSinceIntakeCandidateDetected = 0;
bool autonIntakeCandidateDetected = false;
bool autonIntakeAvailable = true;

SlewRateController autonLeftDriveSlewRateController(60);
SlewRateController autonRightDriveSlewRateController(60);

bool copyOpposite = false;

Autonomous::Autonomous(DummyController& controller, okapi::MotorGroup& leftDrive, okapi::MotorGroup& rightDrive, pros::Motor& intake, pros::ADIDigitalOut& pistonA, pros::ADIDigitalOut& pistonB, pros::ADIDigitalOut& doinker, pros::Optical& optical, pros::Distance& distanceSensor) 
: master(controller), leftDrive(leftDrive), rightDrive(rightDrive), intake(intake), pistonA(pistonA), pistonB(pistonB), doinker(doinker), optical(optical), distanceSensor(distanceSensor) {
}

// Readies the next stage of the autonomous program
// This is in case we want to run a bit of manual code in between driver records. (Manual intake movement in this instance)
void Autonomous::updateStage() {
	stage++;
	master.player->index = 0;
	master.player->recording.deserializeFromFile(master.player->recording.getFileName() + "-stage-" + std::to_string(stage));
	if (copyOpposite) {			
		string autonName = (master.player->recording.getFileName() + "-stage-" + std::to_string(stage));
		size_t red = autonName.find("Red");
		size_t blue = autonName.find("Blue");

		if (red != string::npos) {
			autonName.replace(red, 3, "Blue");
		}
		if (blue != string::npos) {
			autonName.replace(blue, 4, "Red");
		}
		master.player->recording.serializeToFile(autonName, true);
	}
}

void Autonomous::run() {
	if (stage == 0) {
		master.player->recording.deserializeFromFile(master.player->recording.getFileName() + "-stage-" + std::to_string(stage));
		if (copyOpposite) {			
			string autonName = (master.player->recording.getFileName() + "-stage-" + std::to_string(stage));
			size_t red = autonName.find("Red");
			size_t blue = autonName.find("Blue");

			if (red != string::npos) {
				autonName.replace(red, 3, "Blue");
			}
			if (blue != string::npos) {
				autonName.replace(blue, 4, "Red");
			}
			master.player->recording.serializeToFile(autonName, true);
		}
	}
	while (true)
	{
		autonTicker.startTick();
        master.runUpdate();
		
		// Drive
		int left = master.get_analog(ANALOG_LEFT_Y);
		int right = master.get_analog(ANALOG_RIGHT_Y);

		// Prevent drift when idle
		if (abs(left) < 10)
		{
			left = 0;
		}
		if (abs(right) < 10)
		{
			right = 0;
		}
		
		double rawVoltageOutputLeft = pow(left, decreasedSensitivityFactor) / pow(127, (decreasedSensitivityFactor - 1));
		double rawVoltageOutputRight = pow(right, decreasedSensitivityFactor) / pow(127, (decreasedSensitivityFactor - 1));

		int leftVelocity = rawVoltageOutputLeft / 127 * 600;
		int rightVelocity = rawVoltageOutputRight / 127 * 600;

		leftDrive.moveVelocity(autonLeftDriveSlewRateController.calculate(leftVelocity));
		rightDrive.moveVelocity(autonRightDriveSlewRateController.calculate(rightVelocity));

		// Intake
		if (autonManual)
		{
			if (master.get_digital(DIGITAL_L1))
			{
				// Velocity depends on gearset 
				intake.move_velocity(600);
			}
			else if (master.get_digital(DIGITAL_L2))
			{
				intake.move_velocity(-600);
			}
			else
			{
				intake.move_velocity(0);
			}
		}

		// MOGO Mechanism
		if (master.get_digital(DIGITAL_R1))
		{
			pistonA.set_value(true);
			pistonB.set_value(true);
		}
		else if (master.get_digital(DIGITAL_R2))
		{
			pistonA.set_value(false);
			pistonB.set_value(false);
		}

		if (master.get_digital(DIGITAL_UP))
		{
			break;
		}

		if (!autonManual && distanceSensor.get() < 100)
		{
			// intakeAvailable stops interference with intake after it has detected a ring and is starting to score
			if (autonTicksSinceIntakeCandidateDetected > 3) {
				if (autonIntakeAvailable)
				{
					intake.move_relative(2242, 600);
					autonIntakeAvailable = false;
				} else {
					if (intake.get_position() > intake.get_target_position() - 20 && intake.get_position() < intake.get_target_position() + 20)
					{
						autonIntakeCandidateDetected = false;
						autonIntakeAvailable = true;
					}
				}
			} else {
				if (!autonIntakeCandidateDetected) {
					autonIntakeCandidateDetected = true;
					autonTicksSinceIntakeCandidateDetected = 0;
				}
				autonTicksSinceIntakeCandidateDetected++;
			}	
		}
		if (master.player->recording.isFinished(master.player->index)) {
			break;
		}
		autonTicker.waitTick();
	}
	updateStage();
}




