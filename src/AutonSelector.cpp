#include "AutonSelector.h"

AutonSelector::AutonSelector() {
    autonIndex = 0;
    currentAuton = autonNames[autonIndex];
}

void AutonSelector::toggleAuton() {
    autonIndex++;
    if (autonIndex > 4) {
        autonIndex = 0;
    }
    currentAuton = autonNames[autonIndex];
}

std::string AutonSelector::getAuton() {
    return currentAuton;
}