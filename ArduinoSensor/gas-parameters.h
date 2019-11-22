#ifndef __GAS_PARAMS_H__
#define __GAS_PARAMS_H__

// To compute these values, run ArduinoSensor using DO_GAS_CALIBRATION

// Put the sensor in fresh air, retrieve Rs from the serial port after several minutes
// R0 = Rs / 10
#define GAS_SENSOR_R0           1.65
// Put the sensor in fresh air for several minutes, then in a dangerous gas (CH4, CO2 or LPG)
// V threshold = Vs 
#define GAS_SENSOR_V_THRESHOLD  1.60

#endif
