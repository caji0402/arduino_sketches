/* Imports */
#include "waitlib.h"

/*  Button & LED pin definition */
const int button = 2;
const int red_led = 3;
const int green_led = 5;
const int blue_led = 6;
const int red_led2 = 9;
const int green_led2 = 10;
const int blue_led2 = 11;

/* PWM setup */ 
const int max_analog = 255;
const int min_analog = 0;

/* Variable setup */
bool enabled = false;
int current_analog = min_analog;
const unsigned long LED_WAIT = 50;
const unsigned long INTERRUPT_WAIT = 250;

WaitLib ledWait; 
WaitLib interruptWait;

void setup() {
  // put your setup code here, to run once:
  pinMode(red_led, OUTPUT);
  pinMode(green_led, OUTPUT);
  pinMode(blue_led, OUTPUT);
  pinMode(button, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(button), buttonPressed, FALLING);

  writeAll(min_analog);

  Serial.begin(9600);
}

void loop() {
  while (current_analog < max_analog)
  {
    current_analog += 5;
    writeAll(current_analog);
    while(!ledWait.isWaitOver(LED_WAIT)) {}
  }
  while(current_analog > min_analog)
  {
    current_analog -= 5;
    writeAll(current_analog);
    while(!ledWait.isWaitOver(LED_WAIT)) {}
  }
}

void writeAll(int val)
{
  writeRed(val);
  writeGreen(val);
  writeBlue(val);
}

void writeRed(int val)
{
  analogWrite(red_led, enabled ? val : 0);
}

void writeGreen(int val)
{
  analogWrite(green_led, enabled ? val : 0);
}

void writeBlue(int val)
{
  analogWrite(blue_led, enabled ? val : 0);
}

void buttonPressed()
{
  if(interruptWait.isWaitOver(INTERRUPT_WAIT))
  {
    Serial.println("Changing enabled state.");
    changeEnabled();
  }
}

void changeEnabled()
{
  enabled = !enabled;
}
