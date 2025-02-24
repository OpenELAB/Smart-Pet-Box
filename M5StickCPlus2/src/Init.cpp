#include <Init.h>


void All_Init()
{
    
    M5.begin();                 //This will initialize the M5StickC+2
    M5.Lcd.setRotation(1);      //This will rotate the screen 90 degrees
    M5.Lcd.fillScreen(BLACK);   //This will fill the screen with black color
    M5.Lcd.setTextColor(WHITE); //This will set the text color to white

    Serial.begin(115200);
    while (!Serial) {
        delay(100);
    }

    //Web_init();
    aliyun_MQTT_setup();
    Scanning_I2C();
    HUB_init(); // Initialize I2C devices (including SCD40 and light sensor)
    // MLX90614_setup();
    //SCD40_init();
    //Light_init();

    Scanning_I2C();

}

void ALL_loop()
{
    button_RST();
    HUB_loop();          
    // MLX90614_loop();
    // SCD40_loop();
    // Light_loop();

    aliyun_MQTT_loop();


    
}




void Scanning_I2C()
{
    Wire.begin();   
    
    Serial.println("Scanning I2C bus...");

    for (byte i = 8; i < 120; i++) {
        Wire.beginTransmission(i);
        byte error = Wire.endTransmission();
        if (error == 0) {
            Serial.print("Found I2C device at address 0x");
            if (i < 16) {
                Serial.print("0");
            }
            Serial.print(i, HEX);
            Serial.println();
        }
    }  
}
