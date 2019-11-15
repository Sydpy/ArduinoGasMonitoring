#include "gas.h"
#include <Arduino.h>
#include <rn2xx3.h>
#include <SoftwareSerial.h>
#include <math.h>

#define CALIBRATION_MILLIS 900000

float gas_calibration_value = 3.0;

float gas_calibration(int analogPin) {

  Serial.print("Gas sensor calibration, please wait ");
  Serial.print(CALIBRATION_MILLIS / 1000);
  Serial.print("s ... ");

  unsigned long time_start = millis();

  float sensor_volt;
  float RS_air; //  Get the value of RS via in a clear air
  float sensorValue;

  while ((millis() - time_start) < CALIBRATION_MILLIS) {

    sensorValue = 0;

    /*--- Get a average data by testing 100 times ---*/
    for (int x = 0 ; x < 100 ; x++)
    {
      sensorValue = sensorValue + analogRead(A0);
    }
    sensorValue = sensorValue / 100.0;

    sensor_volt = sensorValue / 1024 * 5.0;
    RS_air = (5.0 - sensor_volt) / sensor_volt; // omit *RL
    gas_calibration_value = RS_air / 9.9; // The ratio of RS/R0 is 9.9 in LPG gas from Graph (Found using WebPlotDigitizer)

    delay(1000);
  }

  Serial.println("Done");
  Serial.print("Calibration value : ");
  Serial.println(gas_calibration_value);

  return gas_calibration_value;
}

float gas_measurement(int pin, int* CH4, int* CO, int* LPG, int* AIR) {

  float sensor_volt;
  float RS_gas;
  float ratio;

  pinMode(pin, INPUT);
  int sensorValue = analogRead(pin);
  
  sensor_volt = ((float)sensorValue / 1024) * 5.0;
  RS_gas = (5.0 - sensor_volt) / sensor_volt;
  ratio = RS_gas / gas_calibration_value;

  *CH4 = (int) exp(double(-2.985 * log(double(ratio / 15.374))));
  *CO = (int) exp(double(-1.984 * log(double(ratio / 25.982))));
  *LPG = (int) exp(double(-2.087 * log(double(ratio / 27.633))));

  return ratio;
}
