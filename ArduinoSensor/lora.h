#ifndef __LORA_H__
#define __LORA_H__

#include <rn2xx3.h>
#include <SoftwareSerial.h>

void lora_init_ABP(rn2xx3* lora, SoftwareSerial* loraSerial, int rst_pin);
void lora_init_OTAA(rn2xx3* lora, SoftwareSerial* loraSerial, int rst_pin);

#endif
