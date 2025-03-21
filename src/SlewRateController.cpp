#include "SlewRateController.h"
/**
 * @file SlewRateController.cpp
 * @brief This file contains the implementation of the SlewRateController class.
 * 
 * Slew rate determines how fast a value can change. This will hopefully aid with consistency
 * of autonomous playback and driver control.
 */
SlewRateController::SlewRateController(int slewRate) {
    this->slewRate = slewRate;
}

int SlewRateController::calculate(int value) {
    if (!initialized) {
        lastValue = value;
        initialized = true;
    }
    // Time interval is 20ms and constant, so no need to account for it.
    int change = value - lastValue;
    if (change < -slewRate) {
        change = -slewRate;
    } else if (change > slewRate) {
        change = slewRate;
    }
    int newValue = lastValue + change;
    lastValue = newValue;
    return newValue;
}