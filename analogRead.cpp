#include "analogRead.h"

#include <Arduino.h>

float getSupplyVoltage(void)
{
  const int analog_in_pin = A0;
  const int adc_resolution = 1024;
  //internal 220k resistor + external 1M resistor - R1 is placed between Vin and ADCin
  const int R1_value = 1220000; 
   // internal 100k resistor - R2 is placed between ADCin and gnd
  const int R2_value = 100000; 
  
  int adc_value = analogRead(analog_in_pin);

  float voltage = float(adc_value) / adc_resolution * (R1_value + R2_value) / R2_value;
  
  return voltage;
}
