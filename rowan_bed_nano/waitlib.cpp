#include "waitlib.h"
#include "Arduino.h"

unsigned long lastMillis = 0;
unsigned long currentMillis = 0;

WaitLib::WaitLib() {}

bool WaitLib::isWaitOver(unsigned long waitTime)
{
  currentMillis = millis();
  /* millis() will roll over every 50 days or so.  Check for that edge case.
  If this happens reset the last millis reading to 0 and catch this on the next cycle.
  There's probably a more accurate way to do this, but it doesn't matter for my purposes :)
  https://www.arduino.cc/reference/en/language/functions/time/millis/ */
  if (currentMillis < lastMillis)
  {
    lastMillis = 0;
  }
  if (currentMillis - lastMillis > waitTime)
  {
    lastMillis = currentMillis;
    return true;
  }
  return false;
}

void WaitLib::printValues()
{
  Serial.print("Current mills = ");
  Serial.print(currentMillis);
  Serial.print ("     lastMillis = ");
  Serial.println(lastMillis);
}
