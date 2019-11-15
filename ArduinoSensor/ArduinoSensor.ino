#include <rn2xx3.h>
#include <SoftwareSerial.h>
#include "lora.h"
#include "gas.h"

#define LORA_TX   10
#define LORA_RX   11
#define LORA_RST  12
#define PIN_READ_GAS A0


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
float gas_ratio;
int a;
void loop()
{
  double CH4,CO,LPG,AIR;

    gas_ratio=measurement(PIN_READ_GAS,&CH4,&CO,&LPG,&AIR);
    Serial.print("TXing : ");
    Serial.println(gas_ratio); 
    Serial.print(CH4);
    Serial.println(" ppm");
    Serial.print(CO );
    Serial.println(" ppm");
    Serial.print(LPG );
    Serial.println(" ppm");
    lora.tx(String(gas_ratio));
    delay(200);
}
