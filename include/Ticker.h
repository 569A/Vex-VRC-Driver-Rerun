/**
 * @file Ticker.h
 * 
 * @brief A class for managing consistent intervals of time for driver control.
 * 
 * Due to us using a self-made replay system that relies on controller inputs, the
 * intervals at which they are noted / played back must be consistent. This class
 * allows for variable delay like a game tick loop.
 */
#pragma once

class Ticker
{
private:
    long interval;
    long lastTick = 0;
    
public:
    Ticker(int interval);

    void waitTick();
    void startTick();
};
