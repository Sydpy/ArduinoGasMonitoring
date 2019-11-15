#ifndef __LORA_H__
#define __LORA_H__

#include <rn2xx3.h>
#include <SoftwareSerial.h>

#define ABP_ACTIVATION  0
#define OTAA_ACTIVATION 1

void lora_init(rn2xx3* lora, SoftwareSerial* loraSerial, int rst_pin);

#endif
