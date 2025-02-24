#ifndef ALIYUN_MQTT_H
#define ALIYUN_MQTT_H

#include <WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>

// WiFi and MQTT Configuration
extern const char* ssid;
extern const char* password;
extern const char* mqttHostUrl;
extern const int mqttPort;
extern const char* mqttUser;
extern const char* mqttPassword;
extern const char* clientId;

// Aliyun IoT Configuration
#define PRODUCT_KEY       "a1Zs299lJym"
#define DEVICE_NAME       "ABCD"
#define ALINK_BODY_FORMAT         "{\"id\":\"level\",\"version\":\"1.0\",\"method\":\"thing.event.property.post\",\"params\":%s}"
#define ALINK_TOPIC_PROP_POST     "/sys/" PRODUCT_KEY "/" DEVICE_NAME "/thing/event/property/post"
#define ALINK_TOPIC_PROP_SET      "/sys/" PRODUCT_KEY "/" DEVICE_NAME "/thing/service/property/set"

// Global variables
extern unsigned long lastMs;
extern WiFiClient espClient;
extern PubSubClient client;
extern float target_temperature, target_humidity, target_objectTemp;
extern unsigned char relay_light, relay_fogger, relay_heat, auto_adjust_flag;

// Function declarations
void wifiInit();
void mqttCheckConnect();
void callback(char* topic, byte* payload, unsigned int length);
void mqttIntervalPost(float temperature, float humidity, int co2, int lightValueAnalog);
void aliyun_MQTT_setup();
void aliyun_MQTT_loop();

#endif // ALIYUN_MQTT_H
