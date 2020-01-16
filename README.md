# Arduino Gas Monitoring

## Wiring

The MQ-9 gas sensor is connected to the A0 input of the Arduino.

The LoRA module is connected in serial with TX on the pin 10 and Rx on the pin 11.

## Sketches

### Calibration

A Calibration phase is needed for the Monitoring sketch to know what is the expected sensor value in clean air. We compute the reference sensor resistance *R0* according to the computation done [here](http://wiki.seeedstudio.com/Grove-Gas_Sensor-MQ9/#measurement-approximation). We also compute a voltage threshold for the sensor value that, if exceeded, will trigger an interrupt.

These values are both stored in the EEPROM to make them persistent.

### Monitoring

To prevent continuously reading the sensor value, the monitoring works on an interruption basis. To do so, we use a non inverting comparator with a reference voltage being the voltage threshold computed during calibration. As the Arduino produces an analog output in the form of a PWM signal, we need to flatten it using a low-pass filter to obtain our reference voltage. The second input of the comparator is the sensor's output. The output of the comparator is connected to an interrupt capable pin of the Arduino.

Once an interrupt is fired, the Arduino will read the sensor value and ask the LoRA module to send it. The LoRA message is handled by TheThingsNetwork and can later be retrieved by a client in order to better interpret it : display gas concentration, turn on an alarm, inform the building owner...

The rest of the time, the Arduino is put asleep to not waste energy.
