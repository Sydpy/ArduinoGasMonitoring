#include <rn2xx3.h>
#include <SoftwareSerial.h>
#include "lora.h"
#include "gas.h"

SoftwareSerial loraSerial(10, 11); // RX, TX
rn2xx3 lora(loraSerial);

void setup()
{
  Serial.begin(57600); //serial port to computer
  loraSerial.begin(9600); //serial port to radio
  Serial.println("Startup");
  
  lora_init(&lora, &loraSerial, 12);
  lora.tx("Starting gas sensor");
}
// the loop routine runs over and over again forever:
void loop()
{
    Serial.println("TXing");
    lora.tx("!"); //one byte, blocking function
    delay(200);
}
