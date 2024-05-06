#ifndef _WAITLIB_H
#define _WAITLIB_H
#include "Arduino.h"

class WaitLib
{
public:
  WaitLib();
  bool isWaitOver(unsigned long waitTime);
  void printValues();
private:
  unsigned long lastMillis;
  unsigned long currentMillis;
};

#endif
