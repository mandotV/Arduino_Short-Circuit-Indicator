//
//    FILE: ACS712_20_DC.ino
//  AUTHOR: Rob Tillaart
// PURPOSE: demo to measure mA DC
//     URL: https://github.com/RobTillaart/ACS712
//
//  use with Arduino Serial Plotter
//Tuburan Project

#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include "ACS712.h"
#include "INA219.h"

LiquidCrystal_I2C thisLCD(39, 16, 2);
ACS712 ACS(A0, 5.0, 1023, 100);
INA219 INA(0x40);

const float SHORT_CIRCUIT_CURRENT_MIN = 5.0;
const float SHORT_CIRCUIT_CURRENT_MAX = 15.0;

int redLed = 9;
int greenLed = 8;
int buzzer = 7;

void setup() {
  Wire.begin();
  Serial.begin(115200);

  thisLCD.init();
  thisLCD.backlight();
  
  pinMode(redLed, OUTPUT);
  pinMode(greenLed, OUTPUT);
  pinMode(buzzer, OUTPUT);

  Serial.println();
  Serial.println(__FILE__);
  Serial.print("ACS712_LIB_VERSION: ");
  Serial.println(ACS712_LIB_VERSION);
  Serial.print("INA219_LIB_VERSION: ");
  Serial.println(INA219_LIB_VERSION);
  Serial.println();

  Serial.println(INA.getMaxShuntVoltage());

  if (!INA.begin() )
  {
    Serial.println("Could not connect. Fix and Reboot");
  }

  INA.setMaxCurrentShunt(5, 0.002);
  INA.setBusVoltageRange(16);
  INA.setGain(8);
  delay(1000);

  ACS.autoMidPoint();
}

void displayLCD(float current, float voltage) {
  thisLCD.clear();
  thisLCD.setCursor(0, 0);
  thisLCD.print("CURRENT");
  thisLCD.setCursor(0, 1);
  thisLCD.print(current, 2);
  thisLCD.print(" A");
  
  thisLCD.setCursor(8, 0);
  thisLCD.print("VOLTAGE");
  thisLCD.setCursor(8, 1);
  thisLCD.print(voltage, 2);
  thisLCD.print(" V");

  delay(1000);
}

void loop() {
  int mA = ACS.mA_DC();
  mA = abs(mA);
  float currentA = mA / 1000.0;
  float shuntVoltage = INA.getShuntVoltage_mV();

  Serial.println("Voltage: ");
  Serial.println(shuntVoltage, 2);
  Serial.println("Current: ");
  Serial.println(currentA, 2);
  displayLCD(currentA, shuntVoltage);

  if (currentA >= SHORT_CIRCUIT_CURRENT_MIN)
  {
    Serial.println("SHORT CIRCUIT / OVERCURRENT DETECTED!");
    digitalWrite(redLed, HIGH);
    digitalWrite(greenLed, LOW);
  }
  else
  {
    digitalWrite(redLed, LOW);
    digitalWrite(greenLed, HIGH);
  }
  delay(500);
}


//  -- END OF FILE --
