#include "Ticker.h"
#include "main.h"

Ticker::Ticker(int interval)
{
    this->interval = interval;
}

void Ticker::waitTick()
{
    long currentTime = pros::millis();
    long timeSinceLastTick = currentTime - lastTick;
    if (timeSinceLastTick < interval)
    {
        pros::delay(interval - timeSinceLastTick);
    }
}

void Ticker::startTick()
{
    lastTick = pros::millis();
}
