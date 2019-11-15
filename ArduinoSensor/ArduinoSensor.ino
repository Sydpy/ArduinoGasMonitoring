#include <rn2xx3.h>
#include <SoftwareSerial.h>
#include "lora.h"
#include "gas.h"

#define DO_GAS_CALIBRATION

#define USE_OTAA

#define LORA_TX   10
#define LORA_RX   11
#define LORA_RST  12

#define GAS_PIN  A0



SoftwareSerial loraSerial(LORA_TX, LORA_RX);
rn2xx3 lora(loraSerial);
float gasCalibr = 3.7;

void setup()
{
  Serial.begin(57600);
  loraSerial.begin(9600);

  Serial.println("Startup");

  #ifdef DO_GAS_CALIBRATION
    gasCalibr = gas_calibration(GAS_PIN);
  #endif

  Serial.print("Gas calibration value : ");
  Serial.println(gasCalibr);

  lora_init(&lora, &loraSerial, LORA_RST);
  lora.tx("Starting gas sensor");
}

void loop()
{
    Serial.println("TXing");
    lora.tx("!");
    delay(200);
}
