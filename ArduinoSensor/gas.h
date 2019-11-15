#ifndef __GAS_H__
#define __GAS_H__

#include <rn2xx3.h>
#include <SoftwareSerial.h>
#include <math.h>


const int LED = 2; 
const int DO = 8; 
const int calibr = 0;



float measurement(int PIN_READ_GAS, double* CH4,double* CO, double* LPG, double* AIR);

#endif
