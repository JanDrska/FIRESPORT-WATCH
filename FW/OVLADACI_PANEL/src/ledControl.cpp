#include "ledControl.h"
#include "Arduino.h"

bool LED::blink(int time)
{
    LedActTime = millis();

    if(LedActTime - LedPrevTime > time)
    {
      ledState = not ledState;
      LedPrevTime = LedActTime;
    }
    return ledState;
}