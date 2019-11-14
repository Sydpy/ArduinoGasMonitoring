#include <rn2xx3.h>
#include <SoftwareSerial.h>
#include "lora.h"
#include "gas.h"

#define LORA_TX   10
#define LORA_RX   11
#define LORA_RST  12


SoftwareSerial loraSerial(LORA_TX, LORA_RX);
rn2xx3 lora(loraSerial);

void setup()
{
  Serial.begin(57600);
  loraSerial.begin(9600);

  Serial.println("Startup");

  lora_init(&lora, &loraSerial, LORA_RST);
  lora.tx("Starting gas sensor");
}

void loop()
{
    Serial.println("TXing");
    lora.tx("!");
    delay(200);
}
