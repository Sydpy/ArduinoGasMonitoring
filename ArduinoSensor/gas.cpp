#include <Arduino.h>

#define CALIBRATION_MILLIS 60000

float gas_calibration(int analogPin) {

  Serial.print("Gas sensor calibration, please wait ");
  Serial.print(CALIBRATION_MILLIS / 1000);
  Serial.print("s ... ");

  unsigned long time_start = millis();

  float sensor_volt;
  float RS_air; //  Get the value of RS via in a clear air
  float R0;  // Get the value of R0 via in LPG
  float sensorValue;

  while ((millis() - time_start) < CALIBRATION_MILLIS) {

    sensorValue = 0;

    /*--- Get a average data by testing 100 times ---*/
    for(int x = 0 ; x < 100 ; x++)
    {
        sensorValue = sensorValue + analogRead(A0);
    }
    sensorValue = sensorValue/100.0;

    sensor_volt = sensorValue/1024*5.0;
    RS_air = (5.0-sensor_volt)/sensor_volt; // omit *RL
    R0 = RS_air/9.9; // The ratio of RS/R0 is 9.9 in LPG gas from Graph (Found using WebPlotDigitizer)

    delay(1000);
  }

  Serial.println("Done");

  return R0;
}
