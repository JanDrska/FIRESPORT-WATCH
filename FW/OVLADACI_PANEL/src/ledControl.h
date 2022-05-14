#ifndef LEDCONTROL
#define LEDCONTROL

class LED
{
private:

public:

  unsigned long int LedActTime = 0;
  unsigned long int LedPrevTime = 0;
  bool ledState = false;

  bool blink(int time);
};

#endif // LEDCONTROL