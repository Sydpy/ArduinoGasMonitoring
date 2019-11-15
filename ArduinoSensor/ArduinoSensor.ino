#include <rn2xx3.h>
#include <SoftwareSerial.h>
#include "lora.h"
#include "gas.h"

#define DO_GAS_CALIBRATION
#define USE_OTAA

#define LORA_TX   10
#define LORA_RX   11
#define LORA_RST  12

#define GAS_PIN   A0

SoftwareSerial lora_serial(LORA_TX, LORA_RX);
rn2xx3 lora(lora_serial);

void setup()
{
  Serial.begin(57600);
  lora_serial.begin(9600);

  Serial.println("Startup");

#ifdef DO_GAS_CALIBRATION
  gas_calibration(GAS_PIN);
#endif

#ifdef USE_OTAA
  lora_init_OTAA(&lora, &lora_serial, LORA_RST);
#else
  lora_init_ABP(&lora, &lora_serial, LORA_RST);
#endif

  lora.tx("Starting gas sensor");
}

void loop()
{
  int CH4, CO, LPG, AIR;
  float gas_ratio = gas_measurement(GAS_PIN, &CH4, &CO, &LPG, &AIR);


  Serial.print(CH4);
  Serial.println(" ppm");
  Serial.print(CO );
  Serial.println(" ppm");
  Serial.print(LPG );
  Serial.println(" ppm");

  Serial.print("TXing : ");
  Serial.println(gas_ratio);
  lora.tx(String(gas_ratio));

  delay(6000);
}
