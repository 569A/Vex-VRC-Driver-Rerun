/**
 * @brief The Recorder class is responsible for recording the state of the robot
 * at a given time.
 * 
 * Recording the individual motorgroups and pneumatics would be extremely complex.
 * Instead, we will only record the controller inputs.
 * By running code for driving the robot, then spoofing the controller inputs, we can
 * play back an autonomous program that does everything a driver can.
 * 
 * @author Thomas Hao
 */
#include "main.h"
#include "Recorder.h"
#include "ControllerBase.h"
#include <vector>
#include <fstream>
#include "Recording.h"

using namespace std;



// Recorder(ControllerBase& controller, pros::MotorGroup& leftDrive, pros::MotorGroup& rightDrive, pros::MotorGroup& intake, pros::ADIDigitalOut& pistonA, pros::ADIDigitalOut& pistonB, pros::Optical& optical) :
// controller(controller), leftDrive(leftDrive), rightDrive(rightDrive), intake(intake), pistonA(pistonA), pistonB(pistonB), optical(optical)
// {
//     // Initialize the recorder
//     pistonAState = false;
//     pistonBState = false;

// }
Recorder::Recorder(ControllerBase& controller, Recording& recording) : controller(controller), recording(recording) {
    // Initialize the recorder
    pistonAState = false;
    pistonBState = false;
}

void Recorder::recordUpdate() {
    int time = 0;
    timestamps.push_back(time);

    recording.addAnalogLeftYValue(controller.get_analog(ANALOG_LEFT_Y));
    recording.addAnalogRightYValue(controller.get_analog(ANALOG_RIGHT_Y));

    recording.addDigitalL1Value(controller.get_digital(DIGITAL_L1));
    recording.addDigitalL2Value(controller.get_digital(DIGITAL_L2));

    recording.addDigitalR1Value(controller.get_digital(DIGITAL_R1));
    recording.addDigitalR2Value(controller.get_digital(DIGITAL_R2));
    
    time += 20;
}

void Recorder::stop() {
    // Stop the recording
}

Recording& Recorder::getRecording() {
    // Return the recording
    return recording;
}

void Recorder::saveRecording(int stage) {
    // Save the recording to a file
    recording.serializeToFile(recording.getFileName() + "-stage-" + to_string(stage), false);
}

void Recorder::resetRecording() {
    recording.reset();
}