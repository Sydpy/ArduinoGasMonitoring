#include "gas.h"

float sensor_volt; 
float RS_gas; 
float ratio; 
float R0 = 1.50; 

float measurement(int PIN_READ_GAS, double*CH4,double* CO, double* LPG, double* AIR){

 pinMode(PIN_READ_GAS, INPUT);
int sensorValue = analogRead(PIN_READ_GAS); 
sensor_volt = ((float)sensorValue / 1024) * 5.0; 
RS_gas = (5.0 - sensor_volt) / sensor_volt; 
ratio = RS_gas / R0; 

*CH4= exp(double(-2.985*log(double(ratio/15.374))));
*CO=exp(double(-1.984*log(double(ratio/25.982))));
*LPG=exp(double(-2.087*log(double(ratio/27.633))));



////------------------------------------------------------------/ 
return ratio;
}
