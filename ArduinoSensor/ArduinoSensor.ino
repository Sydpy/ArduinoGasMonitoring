#include <rn2xx3.h>
#include <SoftwareSerial.h>
#include "lora.h"
#include "gas.h"

//#define DO_GAS_CALIBRATION
//#define USE_OTAA

#define LORA_TX   10
#define LORA_RX   11
#define LORA_RST  12

#define GAS_PIN   A0

SoftwareSerial lora_serial(LORA_TX, LORA_RX);
rn2xx3 lora(lora_serial);

void setup()
{
  Serial.begin(57600);

  Serial.println("Startup");

#ifdef DO_GAS_CALIBRATION
  Serial.println("-- Gas calibration mode --");
#else
  Serial.println("-- Gas watcher mode -- ");
  lora_serial.begin(9600);

#ifdef USE_OTAA
  lora_init_OTAA(&lora, &lora_serial, LORA_RST);
#else
  lora_init_ABP(&lora, &lora_serial, LORA_RST);
#endif

  gas_setup_interrupt(6, 2, on_gas_interrupt);
#endif
}

void loop()
{
#ifdef DO_GAS_CALIBRATION
  gas_calibration(GAS_PIN);
  delay(1000);
#endif
}

void on_gas_interrupt() {
  
  int CH4, CO, LPG, AIR;
  float gas_ratio = gas_measurement(GAS_PIN, &CH4, &CO, &LPG);

  String to_send = "";
  to_send += String(gas_ratio);
  to_send += ";";
  to_send += String(CH4);
  to_send += ";";
  to_send += String(CO);
  to_send += ";";
  to_send += String(LPG);

  Serial.print("TXing : ");
  Serial.println(to_send);

  lora.tx(to_send);
}
