#pragma once
#include "main.h"
#include "ControllerBase.h"
#include "Recording.h"
#include <vector>
#include <fstream>

using namespace std;

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
class Recorder {
private:
    ControllerBase &controller;
    Recording& recording;

    // pros::MotorGroup &leftDrive;
    // pros::MotorGroup &rightDrive;
    // pros::MotorGroup &intake;
    // pros::ADIDigitalOut &pistonA;
    // pros::ADIDigitalOut &pistonB;

    bool pistonAState;
    bool pistonBState;

    // pros::Optical &optical;

    vector<int> timestamps;
    
    vector<int> analogLeftYValues;
    vector<int> analogRightYValues;

    vector<int> digitalL1Values;
    vector<int> digitalL2Values;

    vector<int> digitalR1Values;
    vector<int> digitalR2Values;

public:
    
    // Recorder(ControllerBase& controller, pros::MotorGroup& leftDrive, pros::MotorGroup& rightDrive, pros::MotorGroup& intake, pros::ADIDigitalOut& pistonA, pros::ADIDigitalOut& pistonB, pros::Optical& optical) :
    // controller(controller), leftDrive(leftDrive), rightDrive(rightDrive), intake(intake), pistonA(pistonA), pistonB(pistonB), optical(optical)
    // {
    //     // Initialize the recorder
    //     pistonAState = false;
    //     pistonBState = false;

    // }
    Recorder(ControllerBase& controller, Recording& recording);

    void recordUpdate();

    void stop();

    Recording& getRecording();

    void saveRecording(int stage);

    void resetRecording();
};
