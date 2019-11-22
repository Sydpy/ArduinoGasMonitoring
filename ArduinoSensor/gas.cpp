#include "gas.h"
#include "gas-parameters.h"

#include <Arduino.h>
#include <rn2xx3.h>
#include <SoftwareSerial.h>
#include <math.h>

void gas_calibration(int analogPin) {

  float sensor_volt, sensor_res;
  float sensor_value = 0;

  /*--- Get a average data by testing 100 times ---*/
  for (int x = 0 ; x < 100 ; x++)
  {
    sensor_value += analogRead(A0);
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
}

float gas_measurement(int pin, int* CH4, int* CO, int* LPG) {

  float sensor_volt;
  float RS_gas;
  float ratio;

  pinMode(pin, INPUT);
  int sensorValue = analogRead(pin);

  sensor_volt = ((float)sensorValue / 1024) * 5.0;
  RS_gas = (5.0 - sensor_volt) / sensor_volt;
  ratio = RS_gas / GAS_SENSOR_R0;

  *CH4 = (int) exp(double(-2.985 * log(double(ratio / 15.374))));
  *CO = (int) exp(double(-1.984 * log(double(ratio / 25.982))));
  *LPG = (int) exp(double(-2.087 * log(double(ratio / 27.633))));

  Serial.print("Resistance ratio: ");
  Serial.println(ratio);

  Serial.print("CH4: ");
  Serial.print(*CH4);
  Serial.println(" ppm");

  Serial.print("CO : ");
  Serial.print(*CO);
  Serial.println(" ppm");

  Serial.print("LPG: ");
  Serial.print(*LPG);
  Serial.println(" ppm");

  return ratio;
}

void gas_setup_interrupt(int pwm_pin, int interrupt_pin, void (*subroutine)(void) ) {
  pinMode(pwm_pin, OUTPUT);
  analogWrite(pwm_pin, 255.0 * (float) GAS_SENSOR_V_THRESHOLD / 5.);
  attachInterrupt(digitalPinToInterrupt(interrupt_pin), subroutine, RISING);
}
