#pragma once
class SlewRateController {
    private:
        // Per 20ms
        int slewRate = 100;
        int lastValue = 0;
        bool initialized = false;
    public:
        SlewRateController(int slewRate);
        int calculate(int value);
};