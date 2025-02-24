#ifndef INIT_H
#define INIT_H

#include "M5StickCPlus2.h"
#include "WiFi.h"
#include "HTTPClient.h"
#include "RST.h"
#include "SCD40_CO2.h"
#include "HUB.h"
#include "Light.h"
#include <TimerOne.h>   
#include <aliyun_MQTT.h>
#include <MLX90614.h>

// Your header file content will go here
void All_Init();
void ALL_loop();
void Scanning_I2C();

#endif // INIT_H