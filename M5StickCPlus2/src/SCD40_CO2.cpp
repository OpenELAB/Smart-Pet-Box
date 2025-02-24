#include "SCD40_CO2.h"
#include "M5StickCPlus2.h"

SensirionI2CScd4x scd4x;

uint16_t co2 = 0;
float temperature = 0.0f;
float humidity = 0.0f;

void printUint16Hex(uint16_t value) {
    Serial.print(value < 4096 ? "0" : "");
    Serial.print(value < 256 ? "0" : "");
    Serial.print(value < 16 ? "0" : "");
    Serial.print(value, HEX);
}

void printSerialNumber(uint16_t serial0, uint16_t serial1, uint16_t serial2) {
    Serial.print("Serial: 0x");
    printUint16Hex(serial0);
    printUint16Hex(serial1);
    printUint16Hex(serial2);
    Serial.println();
}

void SCD40_init()
{
    //Wire.begin();    
    //Wire.setClock(100000);   

    uint16_t error;
    char errorMessage[256];

    scd4x.begin(Wire);
    // int retryCount = 3;
    // while (retryCount-- > 0) {
    //     if (scd4x.begin(Wire)) {
    //         Serial.println("SCD40 initialized successfully.");
    //         break;
    //     } else {
    //         Serial.println("SCD40 initialization failed. Retrying...");
    //         delay(1000);   
    //     }
    // }
    // if (retryCount <= 0) {
    //     Serial.println("SCD40 initialization failed after multiple attempts.");
    //     return;
    // }

    // stop potentially previously started measurement   
    error = scd4x.stopPeriodicMeasurement();
    if (error) {
        Serial.print("Error trying to execute stopPeriodicMeasurement():    \r\n");
        errorToString(error, errorMessage, 256);
        Serial.println(errorMessage);
    }

    uint16_t serial0;
    uint16_t serial1;
    uint16_t serial2;
    error = scd4x.getSerialNumber(serial0, serial1, serial2);
    if (error) {
        Serial.print("Error trying to execute getSerialNumber():    \r\n");
        errorToString(error, errorMessage, 256);
        Serial.println(errorMessage);
    } else {
        printSerialNumber(serial0, serial1, serial2);
    }

    // Start Measurement 
    error = scd4x.startPeriodicMeasurement();
    if (error) {
        Serial.print("Error trying to execute startPeriodicMeasurement():    \r\n");
        errorToString(error, errorMessage, 256);
        Serial.println(errorMessage);
    }

    Serial.println("Waiting for first measurement... (6 sec)  \r\n");

    bool isDataReady = false;
    unsigned long startTime = millis();
    while (millis() - startTime < 6000) {  
        error = scd4x.getDataReadyFlag(isDataReady);
        if (error) {
            Serial.print("Error trying to execute getDataReadyFlag(): \r\n");
            errorToString(error, errorMessage, 256);
            Serial.println(errorMessage);
            return;
        }
        if (isDataReady) {
            Serial.println("Data is ready!");
            break;
        } else {
            Serial.println("Waiting for data...");
            delay(500); 
        }
    }

    if (!isDataReady) {
        Serial.println("Data not ready after 6 seconds.");
    }
}

void SCD40_loop()
{
    uint16_t error;
    char errorMessage[256];

    delay(200);

    // Read Measurement
    bool isDataReady = false;
    error = scd4x.getDataReadyFlag(isDataReady);
    if (error) {
        Serial.print("Error trying to execute getDataReadyFlag():   \r\n");
        errorToString(error, errorMessage, 256);
        Serial.println(errorMessage);
        return;
    }
    if (!isDataReady) {
        return;
    }
    error = scd4x.readMeasurement(co2, temperature, humidity);
    if (error) {
        Serial.print("Error trying to execute readMeasurement():  \r\n");
        errorToString(error, errorMessage, 256);
        Serial.println(errorMessage);
    } else if (co2 == 0) {
        Serial.println("Invalid sample detected, skipping. \r\n");
    } else {
        Serial.print("Co2:");
        Serial.print(co2);
        Serial.print("\t");
        Serial.print("Temperature:");
        Serial.print(temperature);
        Serial.print("\t");
        Serial.print("Humidity:");
        Serial.println(humidity);

        M5.Lcd.setTextSize(2);       // Set text size to 2
        M5.Lcd.setTextColor(GREEN);  // Set text color to green
        M5.Lcd.fillRect(0, 75, M5.Lcd.width(), 61, BLACK);  // Clear the area
        M5.Lcd.setCursor(0, 75);
        M5.Lcd.print("CO2: ");
        M5.Lcd.print(co2);
        M5.Lcd.print(" ppm");
        M5.Lcd.setCursor(0, 95);
        M5.Lcd.print("Temp: ");
        M5.Lcd.print(temperature);
        M5.Lcd.print(" C");
        M5.Lcd.setCursor(0, 115);
        M5.Lcd.print("Humidity: ");
        M5.Lcd.print(humidity);
        M5.Lcd.print(" %");
    }
}