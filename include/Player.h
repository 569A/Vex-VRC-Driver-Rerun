#pragma once
#include "main.h"
#include "Recorder.h"
#include "Recording.h"

class Player
{
private:
    int32_t nextAnalogLeftY = 0;
    int32_t nextAnalogRightY = 0;
    int32_t nextDigitalL1 = 0;
    int32_t nextDigitalL2 = 0;
    int32_t nextDigitalR1 = 0;
    int32_t nextDigitalR2 = 0;

public:
    int index = 0;
    Recording& recording;

    Player(Recording& recording);


    void runUpdate();

    int32_t get_analog(pros::controller_analog_e_t channel);
    
    int32_t get_digital(pros::controller_digital_e_t button);
};
