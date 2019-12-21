#include <EEPROM.h>

#define GAS_PIN   A0

#define R0_ADDR         0 * sizeof(float)
#define VTHRESH_ADDR    1 * sizeof(float)
#define VTHRESH_FACTOR  4

void gas_calibration() {

  float sensor_volt, sensor_res, sensor_value = 0;

  /*--- Get a average data by testing 100 times ---*/
  for (int x = 0 ; x < 100 ; x++)
  {
    sensor_value += analogRead(GAS_PIN);
  }
  sensor_value = sensor_value / 100.0;

  sensor_volt = sensor_value / 1024 * 5.0;
  sensor_res = (5 - sensor_volt) / sensor_volt;

  Serial.print("Vs : ");
  Serial.print(sensor_volt);
  Serial.println(" V");
  Serial.print("Rs : ");
  Serial.print(sensor_res);
  Serial.println(" ohms");

  // R0 = R_air / 10
  EEPROM.put(R0_ADDR, sensor_res / 10);
  EEPROM.put(VTHRESH_ADDR, sensor_volt * VTHRESH_FACTOR);
}

void setup()
{
  Serial.begin(57600);
  pinMode(GAS_PIN, INPUT);
  Serial.println("-- Gas sensor calibration --");
}

void loop()
{
  gas_calibration();
  delay(1000);
}
