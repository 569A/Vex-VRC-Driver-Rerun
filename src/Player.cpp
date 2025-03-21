/**
 * @file Player.cpp
 * @brief This file contains the implementation of the Player class.
 * 
 * This player prepares controller inputs for dummy controllers to then send to the
 * autonomous program.
 */
#include "main.h"
#include "Player.h"

Player::Player(Recording& recording) :
recording(recording) {
};


void Player::runUpdate() {
    if (index == 0) {
        pros::lcd::set_text(1, std::to_string(recording.recordingValid));
    }
    // Run the player update loop
    // Get the next values from the recording
    nextAnalogLeftY = recording.getAnalogLeftYValue(index);
    nextAnalogRightY = recording.getAnalogRightYValue(index);
    nextDigitalL1 = recording.getDigitalL1Value(index);
    nextDigitalL2 = recording.getDigitalL2Value(index);
    nextDigitalR1 = recording.getDigitalR1Value(index);
    nextDigitalR2 = recording.getDigitalR2Value(index);
    index++;

    
}
int32_t Player::get_analog(pros::controller_analog_e_t channel) {
    switch (channel)
    {
    case ANALOG_LEFT_Y:
        return nextAnalogLeftY;
    case ANALOG_RIGHT_Y:
        return nextAnalogRightY;
    case DIGITAL_L1:
        return nextDigitalL1;
    case DIGITAL_L2:
        return nextDigitalL2;
    case DIGITAL_R1:
        return nextDigitalR1;
    case DIGITAL_R2:
        return nextDigitalR2;
    default:
        return 0;
    }
}

int32_t Player::get_digital(pros::controller_digital_e_t button) {
    switch (button)
    {
    case DIGITAL_L1:
        return nextDigitalL1;
    case DIGITAL_L2:
        return nextDigitalL2;
    case DIGITAL_R1:
        return nextDigitalR1;
    case DIGITAL_R2:
        return nextDigitalR2;
    default:
        return 0;
    }
}
