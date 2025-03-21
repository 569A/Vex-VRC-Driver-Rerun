
/**
 * @brief Class for storing controller inputs.
 * 
 * This class provides functionality to write / read controller inputs to/from a file.
 * It then stores them as variables.
 */
#include "Recording.h"
#include "AutonSelector.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

Recording::Recording(AutonSelector& autonSelector) : autonSelector(autonSelector) {
    
}

void Recording::serializeToFile(std::string filename, bool invert) {
    // this->filename = filename;
    // Serialize the recording to a file
    // /usd/ is needed for PROS to write to the SD card
    FILE* file = fopen(("/usd/" + filename).c_str(), "w");

    if (file != nullptr) {
        for (int i = 0; i < analogLeftYValues.size(); i++) {
            std::string data = std::to_string(analogLeftYValues[i]) + "," + std::to_string(analogRightYValues[i]) + "," + std::to_string(digitalL1Values[i]) + "," + std::to_string(digitalL2Values[i]) + "," + std::to_string(digitalR1Values[i]) + "," + std::to_string(digitalR2Values[i]) + "\n";
            if (invert) {
                fprintf(file, "%d,", analogRightYValues[i]);
                fprintf(file, "%d,", analogLeftYValues[i]);
            } else {
                fprintf(file, "%d,", analogLeftYValues[i]);
                fprintf(file, "%d,", analogRightYValues[i]);
            }
            fprintf(file, "%d,", digitalL1Values[i]);
            fprintf(file, "%d,", digitalL2Values[i]);
            fprintf(file, "%d,", digitalR1Values[i]);
            fprintf(file, "%d,\n", digitalR2Values[i]);
        }
        fclose(file);
    } else {
        std::cout << "Unable to open file" << std::endl;
    }
}

void Recording::deserializeFromFile(const std::string filename) {
    // this->filename = filename;
    FILE* file = fopen(("/usd/" + filename).c_str(), "r");
    if (file == NULL) {
        recordingValid = false;
        return;
    }

    char dataChunk[256];
    int i = 0;

    analogLeftYValues.clear();
    analogRightYValues.clear();
    digitalL1Values.clear();
    digitalL2Values.clear();
    digitalR1Values.clear();
    digitalR2Values.clear();


    while (fgets(dataChunk, 256, file) != NULL) {
        std::string dataString = "";

       int inputIndex = 0;
        while (dataChunk[i] != '\0') {
            if (dataChunk[i] == ',') {
                if (inputIndex == 0) {
                    addAnalogLeftYValue(std::stoi(dataString));
                } else if (inputIndex == 1) {
                    addAnalogRightYValue(std::stoi(dataString));
                } else if (inputIndex == 2) {
                    addDigitalL1Value(std::stoi(dataString));
                } else if (inputIndex == 3) {
                    addDigitalL2Value(std::stoi(dataString));
                } else if (inputIndex == 4) {
                    addDigitalR1Value(std::stoi(dataString));
                } else if (inputIndex == 5) {
                    addDigitalR2Value(std::stoi(dataString));
                    inputIndex = -1;
                }
                inputIndex++;
                dataString.clear();
            } else {
                dataString += dataChunk[i];
                
            }
            i++;
        }
        i = 0; 
    }

    fclose(file);
}

std::string Recording::getFileName() {
    return "Rerun.569A" + autonSelector.getAuton();
}
