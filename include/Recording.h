#pragma once
#include <string>
#include <vector>
#include "main.h"
#include "AutonSelector.h"

class Recording {
private:
    std::vector<int> analogLeftYValues;
    std::vector<int> analogRightYValues;

    std::vector<int> digitalL1Values;
    std::vector<int> digitalL2Values;

    std::vector<int> digitalR1Values;
    std::vector<int> digitalR2Values;

    AutonSelector& autonSelector;
public:
    Recording(AutonSelector& autonSelector);
    bool recordingValid = true;

    void serializeToFile(std::string filename, bool invert);
    void deserializeFromFile(std::string filename);

    void addAnalogLeftYValue(int value) {
        analogLeftYValues.push_back(value);
    }

    void addAnalogRightYValue(int value) {
        analogRightYValues.push_back(value);
    }

    void addDigitalL1Value(int value) {
        digitalL1Values.push_back(value);
    }

    void addDigitalL2Value(int value) {
        digitalL2Values.push_back(value);
    }

    void addDigitalR1Value(int value) {
        digitalR1Values.push_back(value);
    }

    void addDigitalR2Value(int value) {
        digitalR2Values.push_back(value);
    }


    //Getters
    int getAnalogLeftYValue(int index) {
        if (index >= analogLeftYValues.size()) {
            return 0;
        }
        return analogLeftYValues[index];
    }

    int getAnalogRightYValue(int index) {
        if (index >= analogRightYValues.size()) {
            return 0;
        }
        return analogRightYValues[index];
    }

    int getDigitalL1Value(int index) {
        if (index >= digitalL1Values.size()) {
            return 0;
        }
        return digitalL1Values[index];
    }

    int getDigitalL2Value(int index) {
        if (index >= digitalL2Values.size()) {
            return 0;
        }
        return digitalL2Values[index];
    }

    int getDigitalR1Value(int index) {
        if (index >= digitalR1Values.size()) {
            return 0;
        }
        return digitalR1Values[index];
    }

    int getDigitalR2Value(int index) {
        if (index >= digitalR2Values.size()) {
            return 0;
        }
        return digitalR2Values[index];
    }

    bool isFinished(int index) {
        return index >= analogLeftYValues.size();
    }

    void reset() {
        analogLeftYValues.clear();
        analogRightYValues.clear();
        digitalL1Values.clear();
        digitalL2Values.clear();
        digitalR1Values.clear();
        digitalR2Values.clear();
    }

    std::string getFileName(); 
};