/* Imports */
#include "waitlib.h"
#include <ArduinoBLE.h>

/*  Button & LED pin definition */
const int button = 2;
const int red_led = 3;
const int green_led = 5;
const int blue_led = 6;
const int red_led2 = 9;
const int green_led2 = 10;
const int blue_led2 = 11;

/* PWM setup */ 
const int MAX_ANALOG = 255;
const int MIN_ANALOG = 0;

/* Variable setup */
bool enabled = true;
bool isBrightnessIncreasing = true;
int current_analog = MIN_ANALOG;
const unsigned long LED_WAIT = 50;
const unsigned long INTERRUPT_WAIT = 250;

WaitLib ledWait; 
WaitLib interruptWait;

/* BLE setup */
BLEService bleService("018f4f57-a145-7e01-b0c8-1f8c51211152");
BLEBoolCharacteristic switchCharacteristic("018f4f57-a145-7e01-b0c8-1f8c51211152", BLERead | BLEWrite);

void setup() {
  Serial.begin(9600);

  /* Pin setup */
  pinMode(red_led, OUTPUT);
  pinMode(green_led, OUTPUT);
  pinMode(blue_led, OUTPUT);
  pinMode(button, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(button), buttonPressed, FALLING);
  writeAll(MIN_ANALOG);

  /* BLE Setup */
  if (!BLE.begin()) {
    Serial.println("starting Bluetooth® Low Energy module failed!");
    while (1);
  }

  /* BLE Service init */
  BLE.setLocalName("Rowan's Bed");
  BLE.setAdvertisedService(bleService);
  bleService.addCharacteristic(switchCharacteristic);
  BLE.addService(bleService);
  switchCharacteristic.writeValue(enabled);
  BLE.advertise();
  Serial.println("BLE Service Init complete.");
}

void loop() {
  BLEDevice controller = BLE.central();
  if (controller)
  {
    Serial.println("Connected to a central device.");
    if (controller.connected())
    {
      if (switchCharacteristic.written())
      {
        enabled = switchCharacteristic.value();
      }
    }
  }
  if (ledWait.isWaitOver(LED_WAIT))
  {
    if (isBrightnessIncreasing)
    {
      current_analog += 5;
      writeAll(current_analog);
      isBrightnessIncreasing = current_analog < MAX_ANALOG;
    }
    else
    {
      current_analog -= 5;
      writeAll(current_analog);
      isBrightnessIncreasing = current_analog <= MIN_ANALOG;
    }
  }
}

void buttonPressed()
{
  if(interruptWait.isWaitOver(INTERRUPT_WAIT))
  {
    changeEnabled();
  }
}

void changeEnabled()
{
  enabled = !enabled;
  /* Write the ble characteristic to match the current state */
  switchCharacteristic.writeValue(enabled);
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


