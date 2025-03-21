#include "DriverControl.h"

DriverControl::DriverControl(ControllerBase &controller, okapi::MotorGroup &leftMotors, okapi::MotorGroup &rightMotors, pros::Motor &intake, pros::Motor &flexWheelIntake, pros::Motor &arm, pros::ADIDigitalOut &pistonA, pros::ADIDigitalOut &pistonB, pros::ADIDigitalOut &doinker, pros::Optical &optical, pros::Distance &distanceSensor, Recorder &recorder) :
    master(controller), leftMotors(leftMotors), rightMotors(rightMotors), intake(intake), flexWheelIntake(flexWheelIntake), arm(arm), pistonA(pistonA), pistonB(pistonB), doinker(doinker), optical(optical), distanceSensor(distanceSensor), recorder(recorder) {


}

void DriverControl::run() {
    bool digitalUpButtonHeld = false;
	bool digitalDownButtonHeld = false;

	bool digitalLeftButtonHeld = false;
	bool digitalRightButtonHeld = false;

	bool digitalBButtonHeld = false;
	bool digitalAButtonHeld = false;
	bool digitalXButtonHeld = false;

	bool doinkerValue = false;

	while (true)
	{
		driveTicker.startTick();
		master.runUpdate();

		if (recordingInput) {
			recorder.recordUpdate();
		}
		
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

		// Cubic function (as of now), makes small joystick movements less sensitive, for easier maneuverability
		double rawVoltageOutputLeft = pow(left, decreasedSensitivityFactor) / pow(127, (decreasedSensitivityFactor - 1));
		double rawVoltageOutputRight = pow(right, decreasedSensitivityFactor) / pow(127, (decreasedSensitivityFactor - 1));
		/**
		 * The above code is the raw voltage to aphhply to the motors.
		 * However, I choose to use velocity instead of voltage to control the motors.
		 * 	1 - This is because velocity is more consistent than voltage,
		 * 		which is important when playing the auton
		 * 	2 - Both sides will hopefully spin at the same speed for the same input,
		 * 		something that is not guaranteed with voltage. This will make it
		 * 		easier for the driver.
		 */
		
		int leftVelocity = rawVoltageOutputLeft / 127 * velocity;
		int rightVelocity = rawVoltageOutputRight / 127 * velocity;
		if (recordingInput) {	
			leftMotors.moveVelocity(leftDriveSlewRateController.calculate(leftVelocity));
			rightMotors.moveVelocity(rightDriveSlewRateController.calculate(rightVelocity));
		} else {
			leftMotors.moveVelocity(leftVelocity);
			rightMotors.moveVelocity(rightVelocity);
		}

		// Intake - manual control
		if (manual) {
			intakeAvailable = true;
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
			// recorder.listenerMOGO(true);
			pistonA.set_value(true);
			pistonB.set_value(true);
		}
		else if (master.get_digital(DIGITAL_R2))
		{
			// recorder.listenerMOGO(false);
			pistonA.set_value(false);
			pistonB.set_value(false);
		}

		if (master.get_digital(DIGITAL_Y))
		{
			arm.move_velocity(200);
		}
		else if (master.get_digital(DIGITAL_A))
		{
			arm.move_velocity(-200);
		}
		else
		{
			arm.move_velocity(0);
 		}
		if (master.get_digital(DIGITAL_LEFT))
		{
			flexWheelIntake.move_velocity(200);
		}
		else if (master.get_digital(DIGITAL_RIGHT))
		{
			flexWheelIntake.move_velocity(-200);
		}
		else
		{
			flexWheelIntake.move_velocity(0);
		}

		// Saves recording
		if (master.get_digital(DIGITAL_UP) && !competitionMode)
		{
			if (!digitalUpButtonHeld) {
				digitalUpButtonHeld = true;
				if (recordingInput) {
					recorder.saveRecording(stage);
					pros::lcd::set_text(6, "Recording saved!");
				} else {
					recordingInput = true;
				}
			} 
		} else {
			digitalUpButtonHeld = false;
		}

		// Starts recording
		if (master.get_digital(DIGITAL_DOWN) && !competitionMode)
		{
			if (!digitalDownButtonHeld) {
				digitalDownButtonHeld = true;
				recordingInput = true;
				recorder.resetRecording();
				pros::lcd::set_text(6, "Recording started!");
			}
		} else {
			digitalDownButtonHeld = false;
		}

		// Selects which stage of auton to record (begins at 0, can record as many as you want)
		if (master.get_digital(DIGITAL_LEFT) && !competitionMode)
		{
			if (!digitalLeftButtonHeld) {
				digitalLeftButtonHeld = true;
				// Stage cannot be less than 0
				stage = (stage - 1 < 0) ? 0 : stage - 1;
				master.print(2, 0, "Stage: %d", stage);
			}
			
		} else {
			digitalLeftButtonHeld = false;

		}
		if (master.get_digital(DIGITAL_RIGHT) && !competitionMode)
		{
			if (!digitalRightButtonHeld) {
				digitalRightButtonHeld = true;
				
				stage = (stage + 1 > 5) ? 5 : stage + 1;
				master.print(2, 0, "Stage: %d", stage);
			}
		} else {
			digitalRightButtonHeld = false;
		}

		// Automatic intake scoring - currently works, but delay code not working yet
		if (!manual && distanceSensor.get() < 100) // If distance sensor detects a ring
		{
			// Only start scoring if ring has been detected for a few ticks, to prevent knocking it away
			if (ticksSinceIntakeCandidateDetected > ticksPerIntakeCheck) {

				// This is to prevent the intake from initiating the scoring process multiple times for the same ring
				if (intakeAvailable)
				{
					intake.move_relative(2242, 600);
					intakeAvailable = false;
				} else {
					if (intake.get_position() > intake.get_target_position() - 20 && intake.get_position() < intake.get_target_position() + 20)
					{
						intakeCandidateDetected = false;
						intakeAvailable = true;
					}
				}
			} else {
				if (!intakeCandidateDetected) {
					intakeCandidateDetected = true;
					ticksSinceIntakeCandidateDetected = 0;
				}
				ticksSinceIntakeCandidateDetected++;
			}
		}

		if (master.get_digital(DIGITAL_B))
		{
			if (!digitalBButtonHeld) {
				digitalBButtonHeld = true;
				manual = !manual;
				master.print(0, 0, "Manual: %d", manual);
			}
		} else {
			digitalBButtonHeld = false;
		}

		if (master.get_digital(DIGITAL_A) && !competitionMode)
		{
			if (!digitalAButtonHeld) {
				digitalAButtonHeld = true;
				velocity = (velocity == 600) ? 200 : 600;
				master.print(1, 1, "Velocity: %d", velocity);
			}		
		} else {
			digitalAButtonHeld = false;
		}

		if (master.get_digital(DIGITAL_X)) {
			if (!digitalXButtonHeld) {
				digitalXButtonHeld = true;
				doinkerValue = !doinkerValue;
				doinker.set_value(doinkerValue);
			}
		} else {
			digitalXButtonHeld = false;
		}

		// Get data for distance sensor for testing purposes
		pros::lcd::set_text(1, to_string(distanceSensor.get()));
		pros::lcd::set_text(2, to_string(intake.get_position()));
		// pros::lcd::set_text(3, to_string(inertial.get_heading()));
		driveTicker.waitTick();
	} 
}