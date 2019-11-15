#ifndef __GAS_H__
#define __GAS_H__

float gas_calibration(int analogPin);
float gas_measurement(int PIN_READ_GAS, int* CH4,int* CO, int* LPG);

#endif
