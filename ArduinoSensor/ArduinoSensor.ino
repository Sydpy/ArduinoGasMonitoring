#include <rn2xx3.h>
#include <SoftwareSerial.h>
#include <avr/sleep.h>

#include "lora.h"
#include "gas.h"

//#define DO_GAS_CALIBRATION
//#define USE_OTAA

#define LORA_TX   10
#define LORA_RX   11
#define LORA_RST  12

#define GAS_REF   6
#define GAS_INT   2

#define GAS_PIN   A0

SoftwareSerial lora_serial(LORA_TX, LORA_RX);
rn2xx3 lora(lora_serial);

volatile float gas_ratio;
volatile int CH4, CO, LPG, AIR;
volatile bool should_tx = false;

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

  gas_setup_interrupt(GAS_REF, GAS_INT, on_gas_interrupt);
  
  sleep_enable();
  set_sleep_mode(SLEEP_MODE_IDLE);
#endif
}

void loop()
{
#ifdef DO_GAS_CALIBRATION
  gas_calibration(GAS_PIN);
  delay(1000);
#else

  if (should_tx) {
    String to_send = "";
    to_send += String(gas_ratio);
    to_send += ";";
    to_send += String(CH4);
    to_send += ";";
    to_send += String(CO);
    to_send += ";";
    to_send += String(LPG);

    Serial.print("TXing : ");
    Serial.print(to_send);
    Serial.print("...");
    lora.tx(to_send);
    Serial.println("Done");
    should_tx = false;
  }

  sleep_cpu();
#endif
}

void on_gas_interrupt() {
  gas_ratio = gas_measurement(GAS_PIN, &CH4, &CO, &LPG);
  should_tx = true;
}
