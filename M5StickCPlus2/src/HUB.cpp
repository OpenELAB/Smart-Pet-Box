/*
 * SPDX-FileCopyrightText: 2024 M5Stack Technology CO LTD
 *
 * SPDX-License-Identifier: MIT
 */
/*
    Example: Example of using PaHub with SCD40 CO2 Sensor
    Device => PaHub2(hub0) ch:0 => SCD40 CO2 Sensor
*/
/*
 * SPDX-FileCopyrightText: 2024 M5Stack Technology CO LTD
 *
 * SPDX-License-Identifier: MIT
 */
#include <M5Unified.h>
#include <M5UnitUnified.h>
#include <M5UnitUnifiedHUB.h> // PaHub library
#include "M5UnitPbHub.h"      // PbHub library
#include "SCD40_CO2.h"
#include "aliyun_MQTT.h"
#include "MLX90614.h"
#include "HUB.h"


namespace
{
    auto& lcd = M5.Display;        // Make lcd an alias for M5.Display
    m5::unit::UnitUnified Units;
    m5::unit::UnitPaHub2 pahub;  
    M5UnitPbHub pbhub;             // Instance of PbHub (I2C address managed by PbHub library)
}


void HUB_init()
{
    auto pin_num_sda = 32;
    auto pin_num_scl = 33;
    Serial.printf("getPin: SDA:%u SCL:%u\n", pin_num_sda, pin_num_scl);

    // Using 100kHz I2C frequency may help solve communication issues, 100000U
    Wire.begin(pin_num_sda, pin_num_scl);
    Wire.setClock(100000);  // Reduce I2C bus speed to 100kHz

    /********************* The following is PaHub content ********************/
    // Initialize PaHub
    if (!Units.add(pahub, Wire) || !Units.begin()) {
        M5_LOGE("Failed to initialize HUB");
        lcd.clear(TFT_RED);
        while (true) {
            m5::utility::delay(1000);
        }
    }
    delay(1000); // Ensure initialization is complete

    /****************************************************************/
    // Switch to channel 0 to connect SCD40
    if (!pahub.selectChannel(0)) {
        M5_LOGE("Failed to select PaHub channel 0  ");
        lcd.clear(TFT_RED);
        while (true) {
            m5::utility::delay(1000);
        }
    }else {
        Serial.println("PaHub: Successfully selected channel 0  \r\n");
        Wire.begin(pin_num_sda, pin_num_scl);
        Wire.setClock(100000);  // Reduce I2C bus speed to 100kHz
        // Initialize SCD40
        SCD40_init();
    }
    delay(1000); // Ensure initialization is complete

    /*****************************************************************/
    // Switch to channel 1 to connect PbHub
    if (!pahub.selectChannel(2)) {
        M5_LOGE("Failed to select PaHub channel 2  ");
        lcd.clear(TFT_RED);
        while (true) {
            m5::utility::delay(1000);
        }
    } else {
        Serial.println("PaHub: Successfully selected channel 2   \r\n");

        // Initialize PbHub 100000U
        if (!pbhub.begin(&Wire, UNIT_PBHUB_I2C_ADDR, pin_num_sda, pin_num_scl, 100000U)) {
            M5_LOGE("Couldn't find PbHub on channel 2   ");
            while (1) delay(1);
        } else {
            Serial.println("PbHub: Successfully initialized    \r\n");
        }

        M5.Lcd.fillRect(0, 0, M5.Lcd.width(), 70, BLACK);  // Clear only the first 70 pixels height area
        
    }
    delay(1000); // Ensure initialization is complete
    
    /*****************************************************************/
    // Switch to channel 1 to connect MLX90614
    if (!pahub.selectChannel(1)) {
        M5_LOGE("Failed to select PaHub channel 1  ");
        lcd.clear(TFT_RED);
        while (true) {
            m5::utility::delay(1000);
        }
    }else {
        Serial.println("PaHub: Successfully selected channel 1   \r\n");
        Wire.begin(pin_num_sda, pin_num_scl);
        Wire.setClock(100000);  
        MLX90614_setup();
    }
    delay(1000); 
    
}

int lightValueAnalog;  
bool lightValueDigital;

unsigned long HUB_Ms = 0;

unsigned long lastMLXTime = 0;  
unsigned long hubTimeout = 500; 

void HUB_loop()
{
    // M5.update();
    Units.update();
    
    if (millis() - HUB_Ms >= 1000) 
    {
        HUB_Ms = millis(); 


        /*********************** pa channel 0: SCD40 *********************************/
       
        if (pahub.selectChannel(0)) {
            Wire.begin();
            SCD40_loop();                   
        }
        else {
            Serial.println("PaHub_loop:Failed to switch to channel 0   \t");
        }

        delay(100); 

        /*********************** pa channel 2: PbHub *********************************/
        
        if (pahub.selectChannel(2)) {
            
            M5.Lcd.fillRect(0, 0, 180, 40, BLACK);  
            // M5.Lcd.fillRect(0, 40, 200, 40, BLACK);
            lcd.setTextSize(2);
            lcd.setTextColor(TFT_WHITE);
            lcd.setCursor(0, 10);

            PbHub_loop();                   
        } else {
            Serial.println("PaHub_loop:Failed to switch to channel 2    \t");
        }

        delay(100); 

        /*********************** pa channel 1: MLX90614 *********************************/
        
        if (pahub.selectChannel(1)) {
            Wire.begin();
            Wire.setClock(100000);  
            
            
            if (millis() - lastMLXTime > hubTimeout) {
                MLX90614_loop();                   
                lastMLXTime = millis();
            }
        }
        else {
            Serial.println("PaHub_loop:Failed to switch to channel 1     \t");
        }

        delay(100); 

    }

}

void PbHub_loop()
{
    /********************* pb channel 0: photoresistor ****************************/
    
    lightValueAnalog = pbhub.analogRead(0);  
    lightValueDigital = pbhub.digitalRead(0, 0);   
    
    Serial.printf("PbHub ch 0 - Analog Value: %d, Digital Value: %d  -光敏数据 \r\n",lightValueAnalog, lightValueDigital);
    
    lcd.printf("Analog: %d\n", lightValueAnalog);
    lcd.printf("Digital: %d\n", lightValueDigital ? 1 : 0); 
    /********************** pb channel 1: fan control **********************/
    //outputState = pbhub.digitalRead(1, 0); 

    /* High/Low level switching */
   
    // static bool outputState = false; 
    // outputState = !outputState;      
    // pbhub.digitalWrite(1, 0, outputState); 
    // Serial.printf("PbHub ch 1 - Set O pin to: %s   \r\n", outputState ? "HIGH" : "LOW");

    /* High and Low level control for TB6612 */
    // pbhub.digitalWrite(1, 1, false);
    // pbhub.digitalWrite(1, 0, true);
    // Serial.printf("PbHub ch 1 - Set O pin to: %s  \r\n", "HIGH");

    /* Relay control */
    if(humidity > target_humidity+3)   
    {
        pbhub.digitalWrite(1, 0, true); 
        Serial.printf("PbHub ch 1 - Set O pin to: %s  \r\n", "HIGH");
    }
    else
    {
        pbhub.digitalWrite(1, 0, false); 
        Serial.printf("PbHub ch 1 - Set O pin to: %s   \r\n", "LOW");
    }
    
    /********************* pb channel 2: PWM ***********************/
    // pbhub.setPWM(2, 1, 120); //MAX: 255
    // Serial.printf("PbHub ch 2 - PWM: 120   \r\n");
    
        /* 
        Serial.printf("PbHub ch 2 - PWM: 0 -> 100 -> 0   \r\n");
        for(int i = 0; i < 100; i++)
        {
            pbhub.setPWM(2, 1, i); 
            delay(20);
        }
        for(int i=100; i>=0; i--)
        {
            pbhub.setPWM(2, 1, i);
            delay(20);
        }
        */

    /********************* pb channel 3: fogger relay ***********************/
    static unsigned long lasthumidityTime = 0;  
    const unsigned long humidity_INTERVAL = 10000;  
    static bool foggerRunning = false;
    static unsigned long foggerStartTime = 0;
    const unsigned long FOGGER_RUN_TIME = 2000; 

    switch (auto_adjust_flag)   
    {
    case 0: 
        if(relay_fogger == 1)   
        {
            pbhub.digitalWrite(3, 0, true); 
            Serial.printf("PbHub ch 3 - Relay:1    \r\n");
        }
        else
        {
            pbhub.digitalWrite(3, 0, false); 
            Serial.printf("PbHub ch 3 - Relay:0   \r\n");
        }
        break;
    case 1: 
        if (millis() - lasthumidityTime > humidity_INTERVAL)  
        {
            if(humidity < target_humidity-3)   
            {
                pbhub.digitalWrite(3, 0, true); 
                Serial.printf("PbHub ch 3 - Relay:1    \r\n");
                foggerStartTime = millis();  
            }
            else if(humidity > target_humidity+3) 
            {
                pbhub.digitalWrite(3, 0, false);  
                Serial.printf("PbHub ch 3 - Relay:0    \r\n");
            }
            lasthumidityTime = millis();   
        }
         
        if (millis() - foggerStartTime >= FOGGER_RUN_TIME) {
            pbhub.digitalWrite(3, 0, false);
            Serial.println("PbHub ch 3 - Fogger auto shutdown after 1s");
            foggerStartTime = millis();   
        }
        break;
    default:
        break;
    }
    
    /********************* pb channel 4: heating pad relay ***********************/
    static unsigned long lastHeatTime = 0;   
    const unsigned long HEAT_INTERVAL = 10000;   

    switch (auto_adjust_flag)  
    {
    case 0: 
        if (millis() - lastHeatTime > HEAT_INTERVAL)   
        {
            if(objectTemp > target_objectTemp)
            {
                pbhub.digitalWrite(4, 0, false);  
                Serial.printf("PbHub ch 4 - Relay:0   \r\n");
            }
            lastHeatTime = millis();  
        }
        else if(relay_heat == 1)   
        {
            if(objectTemp < target_objectTemp-3)
            {
                pbhub.digitalWrite(4, 0, true);  
                Serial.printf("PbHub ch 4 - Relay:1    \r\n");
            }
        }
        else
        {
            pbhub.digitalWrite(4, 0, false);  
            Serial.printf("PbHub ch 4 - Relay:0    \r\n");
        }
        break;
    case 1:  
        if (millis() - lastHeatTime > HEAT_INTERVAL)  
        {
            if(objectTemp>target_objectTemp)
            {
                pbhub.digitalWrite(4, 0, false); 
                Serial.printf("PbHub ch 4 - Relay:0    \r\n");
            }
            else if(temperature < target_temperature-3)   
            {
                pbhub.digitalWrite(4, 0, true);  
                Serial.printf("PbHub ch 4 - Relay:1     \r\n");
            }
            else if(temperature > target_temperature+3) 
            {
                pbhub.digitalWrite(4, 0, false);  
                Serial.printf("PbHub ch 4 - Relay:0    \r\n");
            }
            lastHeatTime = millis();   
        }
        break;
    default:
        break;
    }

    /********************* pb channel 5: UVA light relay ***********************/
    if(relay_light == 1)   
    {
        relay_light = 2;    
        pbhub.digitalWrite(5, 0, true);
        delay(200);
        pbhub.digitalWrite(5, 0, false);    
        Serial.printf("PbHub ch 5 - Relay:    \r\n");
    }
    else if(relay_light == 0)
    {
        relay_light = 2;    
        pbhub.digitalWrite(5, 0, true);
        delay(200);
        pbhub.digitalWrite(5, 0, false);    
        Serial.printf("PbHub ch 5 - Relay:    \r\n");
    }

}