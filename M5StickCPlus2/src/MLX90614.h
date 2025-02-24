#ifndef MLX90614_H
#define MLX90614_H

#include <Wire.h>

extern double ambientTemp,objectTemp;

void MLX90614_setup();
void MLX90614_loop();

#endif // MLX90614_H