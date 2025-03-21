#pragma once
#include "main.h"
#include <string>

class AutonSelector {
    private:
        std::string autonNames[5] = {"Red-Negative", "Red-Positive", "Blue-Negative", "Blue-Positive", "Skills"};
        int autonIndex;
        std::string currentAuton;
    public:
        AutonSelector();
        void toggleAuton();
        std::string getAuton();
};