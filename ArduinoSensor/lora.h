#ifndef __LORA_H__
#define __LORA_H__

#include <rn2xx3.h>
#include <SoftwareSerial.h>

void lora_init(rn2xx3* lora, SoftwareSerial* loraSerial, int rstPin);

#endif
