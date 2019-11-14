#include "lora.h"
#include "otaa-parameters.h"

void lora_init(rn2xx3* lora, SoftwareSerial* loraSerial, int rstPin) {
  
  //reset rn2483
  pinMode(rstPin, OUTPUT);
  digitalWrite(rstPin, LOW);
  delay(500);
  digitalWrite(rstPin, HIGH);

  delay(100); //wait for the RN2xx3's startup message
  loraSerial->flush();

  //Autobaud the rn2483 module to 9600. The default would otherwise be 57600.
  lora->autobaud();

  //check communication with radio
  String hweui = lora->hweui();
  while(hweui.length() != 16)
  {
    Serial.println("Communication with RN2xx3 unsuccessful. Power cycle the board.");
    Serial.println(hweui);
    delay(10000);
    hweui = lora->hweui();
  }

  //print out the HWEUI so that we can register it via ttnctl
  Serial.println("When using OTAA, register this DevEUI: ");
  Serial.println(lora->hweui());
  Serial.println("RN2xx3 firmware version:");
  Serial.println(lora->sysver());

  //configure your keys and join the network
  Serial.println("Trying to join TTN");
  bool join_result = lora->initOTAA(OTAA_APP_EUI, OTAA_APP_KEY);

  while(!join_result)
  {
    Serial.println("Unable to join. Are your keys correct, and do you have TTN coverage?");
    delay(60000); //delay a minute before retry
    join_result = lora->init();
  }
  Serial.println("Successfully joined TTN");

}
