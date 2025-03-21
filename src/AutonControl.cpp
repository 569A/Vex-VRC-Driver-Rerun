#include "AutonControl.h"

AutonControl::AutonControl(MotorGroup &leftMotors, MotorGroup &rightMotors, pros::IMU &inertial)
    : leftMotors(leftMotors), rightMotors(rightMotors), inertial(inertial)
{
}

void AutonControl::turnFor(double degrees)
{
    double scaledDegrees;
    while (degrees > 360) {
        degrees -= 360;
    } 
    while (degrees < -360) {
        degrees += 360;
    }
    
    while (true)
    {
        pros::lcd::print(4, "Heading: %f", inertial.get_heading());
        double error = inertial.get_heading() - degrees;
        leftMotors.moveVelocity(-error * 0.5);
        rightMotors.moveVelocity(error * 0.5);
        pros::delay(20);
    }
}