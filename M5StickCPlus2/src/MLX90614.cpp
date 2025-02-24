#include <Adafruit_MLX90614.h>
#include <Wire.h>

Adafruit_MLX90614 mlx;    // = Adafruit_MLX90614();
int retryCount = 0;
const int maxRetries = 4;

void MLX90614_setup() {

  Serial.println("Adafruit MLX90614 test");

  // Wire.setClock(100000);   

  //MLX90614_I2CADDR, &Wire
  while (retryCount < maxRetries) {
    if (mlx.begin()) {
      Serial.println("MLX90614 initialized successfully.");
      break;
    } else {
      retryCount++;
      Serial.println("MLX90614 initialization failed, retrying...");
      delay(1000);   
    }
  }
 
  mlx.writeEmissivity(1);

  Serial.print("Emissivity = "); Serial.println(mlx.readEmissivity());
  Serial.println("================================================");
}

double ambientTemp,objectTemp;

void MLX90614_loop() {
  objectTemp = mlx.readObjectTempC();
  ambientTemp = mlx.readAmbientTempC();

  Serial.print("Ambient = "); Serial.print(ambientTemp);
  Serial.print("*C\tObject = "); Serial.print(objectTemp); Serial.println("*C");
  Serial.println();
  delay(500);
}