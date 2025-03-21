#pragma once
#include "main.h"
class AutonControl
{
private:
    MotorGroup &leftMotors;
    MotorGroup &rightMotors;
    pros::IMU &inertial;
public:
    AutonControl(MotorGroup &leftMotors, MotorGroup &rightMotors, pros::IMU &inertial);
    void turnFor(double degrees);
    void forwardFor(double distance);
};
