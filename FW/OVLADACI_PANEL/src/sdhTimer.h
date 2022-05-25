#ifndef SDHTIMER
#define SDHTIMER

class TimerData
{
private:

public:

  long casSTART = 0;          //startovní čas
  long casODPOCET = 0;
  long casTERC = 0;           //průběžný čas 1. terče
  long casTERC_end = 0;       //konečný čas 1. terče
  long casTERC_M = 0;         //konečný čas 1. terče - minuty
  long casTERC_S = 0;         //konečný čas 1. terče - sekundy
  long casTERC_ms = 0;        //konečný čas 1. terče - ms

  void init(); 
  void startTimming();
  void stopTimming();
  void Time();
  void sendDataSerial(char name);
};

#endif // SDH TIMER
