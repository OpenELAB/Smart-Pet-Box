#include <M5Unified.h>

const int analogPin = 33; // GPIO36: Analog signal pin
const int digitalPin = 32; // GPIO26: Digital signal pin

void Light_init()
{
    pinMode(analogPin, INPUT);   
    pinMode(digitalPin, INPUT);  

     
    M5.Lcd.setRotation(3);
    M5.Lcd.setTextSize(2);
    M5.Lcd.setCursor(10, 10);
    M5.Lcd.println("Light Unit Test");
}

void Light_loop()
{
     
    int lightValue = analogRead(analogPin);

     
    int digitalValue = digitalRead(digitalPin);

     
    Serial.printf("Light Value (Analog): %d\n", lightValue);
    Serial.printf("Light Status (Digital): %d\n", digitalValue);

    
    M5.Lcd.fillScreen(TFT_BLACK);   
    M5.Lcd.setTextColor(TFT_WHITE);   
    M5.Lcd.setCursor(10, 40);
    M5.Lcd.printf("Analog: %d", lightValue);
    M5.Lcd.setCursor(10, 80);
    M5.Lcd.printf("Digital: %d", digitalValue);

    delay(1000);  
}