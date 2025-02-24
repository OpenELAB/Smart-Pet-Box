#ifndef SCD40_CO2_H
#define SCD40_CO2_H

#include <Arduino.h>
#include <SensirionI2CScd4x.h>
#include <Wire.h>

extern uint16_t co2;
extern float temperature;
extern float humidity;

void SCD40_init();
void SCD40_loop();

#endif // SCD40_CO2_H
