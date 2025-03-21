#pragma once
#include "main.h"
#include "ControllerBase.h"
#include "Ticker.h"
#include "Recording.h"
#include "Recorder.h"
#include "SlewRateController.h"

class DriverControl
{
private:
    Ticker driveTicker = Ticker(20);
    bool recordingInput = false;
    Recorder &recorder;
    int decreasedSensitivityFactor = 3;
    int velocity = 600;
    SlewRateController leftDriveSlewRateController = SlewRateController(60);
    SlewRateController rightDriveSlewRateController = SlewRateController(60);
    bool manual = false;
    bool intakeAvailable = true;
    bool competitionMode = false;
    int stage = 0;
    int ticksSinceIntakeCandidateDetected = 0;
    int ticksPerIntakeCheck = 4;
    bool intakeCandidateDetected = false;

    ControllerBase &master;
    okapi::MotorGroup &leftMotors;
    okapi::MotorGroup &rightMotors;
    pros::Motor &intake;
    pros::Motor &flexWheelIntake;
    pros::Motor &arm;
    pros::ADIDigitalOut &pistonA;
    pros::ADIDigitalOut &pistonB;
    pros::ADIDigitalOut &doinker;
    pros::Optical &optical;
    pros::Distance &distanceSensor;
public:
    DriverControl(ControllerBase &controller, okapi::MotorGroup &leftMotors, okapi::MotorGroup &rightMotors, pros::Motor &intake, pros::Motor &flexWheelIntake, pros::Motor &arm, pros::ADIDigitalOut &pistonA, pros::ADIDigitalOut &doinker, pros::ADIDigitalOut &pistonB, pros::Optical &optical, pros::Distance &distanceSensor, Recorder &recorder);
    void run();
    void setCompetitonMode(bool mode) {
        competitionMode = mode;
    }
};