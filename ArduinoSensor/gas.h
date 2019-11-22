#ifndef __GAS_H__
#define __GAS_H__

void gas_calibration(int analog_pin);
void gas_setup_interrupt(int pwm_pin, int interrupt_pin, void (*subroutine)(void) );
float gas_measurement(int pin, int* CH4, int* CO, int* LPG);

#endif
