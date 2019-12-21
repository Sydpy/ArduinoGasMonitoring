#include <SoftwareSerial.h>
#include <EEPROM.h>
#include <rn2xx3.h>
#include <avr/sleep.h>

#include "lora-parameters.h"

#define GAS_R0_ADDR         0 * sizeof(float)
#define GAS_VTHRESH_ADDR    1 * sizeof(float)
volatile float GAS_R0;
volatile float GAS_VTHRESH;

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


void lora_init(rn2xx3* lora, SoftwareSerial* lora_serial, int rst_pin) {
  //reset rn2483
  pinMode(rst_pin, OUTPUT);
  digitalWrite(rst_pin, LOW);
  delay(500);
  digitalWrite(rst_pin, HIGH);

  delay(100); //wait for the RN2xx3's startup message
  lora_serial->flush();

  //Autobaud the rn2483 module to 9600. The default would otherwise be 57600.
  lora->autobaud();

  //check communication with radio
  String hweui = lora->hweui();
  while (hweui.length() != 16)
  {
    Serial.println("Communication with RN2xx3 unsuccessful. Power cycle the board.");
    Serial.println(hweui);
    delay(10000);
    hweui = lora->hweui();
  }

  Serial.print("RN2xx3 firmware version: ");
  Serial.println(lora->sysver());

  //print out the HWEUI so that we can register it via ttnctl
  Serial.println("When using OTAA, register this DevEUI: ");
  Serial.println(lora->hweui());

  //configure your keys and join the network
  Serial.println("Trying to join TTN");
  bool join_result = lora->initABP(ABP_DEV_ADDR, ABP_APP_SKEY, ABP_NWK_SKEY);

  while (!join_result)
  {
    Serial.println("Unable to join. Are your keys correct, and do you have TTN coverage?");
    delay(60000); //delay a minute before retry
    join_result = lora->init();
  }
  Serial.println("Successfully joined TTN");
}

float gas_measurement() {

  float sensor_volt;
  float RS_gas;
  float ratio;

  int sensorValue = analogRead(GAS_PIN);

  sensor_volt = ((float)sensorValue / 1024) * 5.0;
  RS_gas = (5.0 - sensor_volt) / sensor_volt;
  ratio = RS_gas / GAS_R0;

  CH4 = (int) exp(double(-2.985 * log(double(ratio / 15.374))));
  CO = (int) exp(double(-1.984 * log(double(ratio / 25.982))));
  LPG = (int) exp(double(-2.087 * log(double(ratio / 27.633))));

  return ratio;
}

void on_gas_threshold() {
  gas_ratio = gas_measurement();
  should_tx = true;
}

void setup()
{
  Serial.begin(57600);

  // Retrieve calibrated parameters
  EEPROM.get(GAS_R0_ADDR, GAS_R0);
  EEPROM.get(GAS_VTHRESH_ADDR, GAS_VTHRESH);

  // Init lora modem
  lora_serial.begin(9600);
  lora_init(&lora, &lora_serial, LORA_RST);

  // Init gas sensor
  pinMode(GAS_PIN, INPUT);
  pinMode(GAS_REF, OUTPUT);
  analogWrite(GAS_REF, 255.0 * (float) GAS_VTHRESH / 5.);
  attachInterrupt(digitalPinToInterrupt(GAS_INT), on_gas_threshold, HIGH);

  // Enable sleep mode (idle because we need to keep pwm)
  sleep_enable();
  set_sleep_mode(SLEEP_MODE_IDLE);

  Serial.println("-- Gas sensor -- ");
}

void loop()
{

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
}
