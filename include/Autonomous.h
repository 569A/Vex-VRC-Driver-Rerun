#pragma once
#include "main.h"
#include "ControllerBase.h" 
#include "DummyController.h"

class Autonomous
{
private:
    DummyController &master;
    okapi::MotorGroup &leftDrive;
    okapi::MotorGroup &rightDrive;
    pros::Motor &intake;
    pros::ADIDigitalOut &pistonA;
    pros::ADIDigitalOut &pistonB;
    pros::ADIDigitalOut &doinker;
    pros::Optical &optical;
    pros::Distance &distanceSensor;
    int stage = 0;
public:
    Autonomous(DummyController &controller, okapi::MotorGroup &leftDrive, okapi::MotorGroup &rightDrive, pros::Motor &intake, pros::ADIDigitalOut &pistonA, pros::ADIDigitalOut &pistonB, pros::ADIDigitalOut &doinker, pros::Optical &optical, pros::Distance &distanceSensor);
    void run();
    void updateStage();
};