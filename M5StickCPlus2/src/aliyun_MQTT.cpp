#include <WiFi.h>
#include <PubSubClient.h>
#include <aliyun_MQTT.h>
#include <ArduinoJson.h>  
#include <HUB.h>
#include <SCD40_CO2.h>
#include <MLX90614.h>
 
const char* ssid ="OpenELAB"; 
const char* password ="OEL20240801"; 


/*  Germany debug  */ 
const char* mqttHostUrl = "a1Zs299lJym.iot-as-mqtt.cn-shanghai.aliyuncs.com"; 
const int mqttPort =1883; 
const char* mqttUser = "Carl&a1Zs299lJym";//username
const char* mqttPassword = "01d580fa7484ff42b483e634e06e4526d07aa6f88774c0db73c768dd5b8eafe5"; 
const char* clientId = "a1Zs299lJym.Carl|securemode=2,signmethod=hmacsha256,timestamp=1739332867443|" ;  

#define PRODUCT_KEY       "a1Zs299lJym"
#define DEVICE_NAME       "Carl"

/*  China debug  */ 
// const char* mqttHostUrl = "a1Zs299lJym.iot-as-mqtt.cn-shanghai.aliyuncs.com";
// const int mqttPort =1883; 
// const char* mqttUser = "ABCD&a1Zs299lJym";//username
// const char* mqttPassword = "2c7b3123a18fe8cb111f74024ebe67862c3ef24f26e3bade727dde5d048da4ca";
// const char* clientId = "a1Zs299lJym.ABCD|securemode=2,signmethod=hmacsha256,timestamp=1738806840309|" ;

// #define PRODUCT_KEY       "a1Zs299lJym"
// #define DEVICE_NAME       "ABCD"


#define ALINK_BODY_FORMAT         "{\"id\":\"level\",\"version\":\"1.0\",\"method\":\"thing.event.property.post\",\"params\":%s}"
#define ALINK_TOPIC_PROP_POST     "/sys/" PRODUCT_KEY "/" DEVICE_NAME "/thing/event/property/post"

#define ALINK_TOPIC_PROP_SET      "/sys/" PRODUCT_KEY "/" DEVICE_NAME "/thing/service/property/set"
 
unsigned long lastMs = 0;

WiFiClient espClient;
PubSubClient client(espClient);



void wifiInit()
{
  WiFi.begin(ssid,password);
  while (WiFi.status()!= WL_CONNECTED) {
    delay(500);
    Serial.println("Connectingto WiFi..");
  }
    Serial.println("Connectedto the WiFi network"); 
}



void mqttCheckConnect()
{
  client.setServer(mqttHostUrl, mqttPort);
  client.setCallback(callback); 
  while (!client.connected())
  {
    Serial.println("Connecting to MQTT...");
    if (client.connect(clientId, mqttUser, mqttPassword))
    {
        Serial.println("connected");

        //   ALINK_TOPIC_PROP_SET
        if (client.subscribe(ALINK_TOPIC_PROP_SET)) {
          Serial.println("Successfully subscribed to topic. ");
        } else {
          Serial.println("Failed to subscribe to topic.  ");
        }
        client.publish(ALINK_TOPIC_PROP_SET, "Hi, I'm ESP32 ^^");  

    }
    else
    {
      Serial.print("failed with state ");
      Serial.print(client.state());
      delay(2000);
    }
  }
}

float target_temperature=25.00, target_humidity=50.00,target_objectTemp=38.00;
unsigned char relay_light=2, relay_fogger=0, relay_heat=0, auto_adjust_flag=0;

 
void callback(char* topic, byte* payload, unsigned int length)
{
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");

   
  char msg[length + 1];
  for (unsigned int i = 0; i < length; i++) {
    msg[i] = (char)payload[i];
  }
  msg[length] = '\0';  

   
  Serial.println(msg);

   
  JsonDocument doc;   
  DeserializationError error = deserializeJson(doc, msg);   

   
  if (error) {
    Serial.printf("deserializeJson() failed: ");
    Serial.println(error.f_str());
    return;  
  }

   
  if (doc["params"]["target_temperature"].is<float>()) {
    target_temperature = doc["params"]["target_temperature"];
    Serial.print("Received target_temperature: ");
    Serial.println(target_temperature);
  }
  
  if (doc["params"]["target_humidity"].is<float>()) {
    target_humidity = doc["params"]["target_humidity"];
    Serial.print("Received target_humidity: ");
    Serial.println(target_humidity);
  }

   
  if (doc["params"]["relay_light"].is<unsigned char>()) {
    relay_light = doc["params"]["relay_light"].as<unsigned char>();
    Serial.print("Received relay_light: ");
    Serial.println((unsigned int)relay_light);
  }
   
  if (doc["params"]["relay_fogger"].is<unsigned char>()) {
    relay_fogger = doc["params"]["relay_fogger"].as<unsigned char>();
    Serial.print("Received relay_fogger: ");
    Serial.println((unsigned int)relay_fogger);
  }
 
  if (doc["params"]["relay_heat"].is<unsigned char>()) {
    relay_heat = doc["params"]["relay_heat"].as<unsigned char>();
    Serial.print("Received relay_heat: ");
    Serial.println((unsigned int)relay_heat);
  }

 
  if (doc["params"]["auto_adjust_flag"].is<unsigned char>()) {
    auto_adjust_flag = doc["params"]["auto_adjust_flag"].as<unsigned char>();
    Serial.print("Received auto_adjust_flag: ");
    Serial.println((unsigned int)auto_adjust_flag);
  }

 
  if (doc["params"]["target_objectTemp"].is<float>()) {
    target_objectTemp = doc["params"]["target_objectTemp"];
    Serial.print("Received target_objectTemp: ");
    Serial.println(target_objectTemp);
  }


}

void testCallback() {
  const char* testTopic = "test/topic";
  const char* testPayload = "{\"target_temperature\":25.5}";
  
  byte testPayloadBytes[strlen(testPayload)];
  for (int i = 0; i < strlen(testPayload); i++) {
    testPayloadBytes[i] = testPayload[i];
  }

  callback((char*)testTopic, (byte*)testPayload, strlen(testPayload));
}


void mqttIntervalPost(float temperature, float humidity, int co2, int lightValueAnalog)
{
    char param[256];
    char jsonBuf[512];
  
    float current_target_temp = target_temperature;
    float current_target_hum = target_humidity;
    float current_target_objectTemp = target_objectTemp;

    sprintf(param, "{\"temperature\":%0.2f, \"humidity\":%0.2f, \"co2\":%d, \"lightValueAnalog\":%d, \"target_temperature\":%0.2f, \"target_humidity\":%0.2f, \"objectTemp\":%0.2f, \"target_objectTemp\":%0.2f}", temperature, humidity, co2, lightValueAnalog, current_target_temp, current_target_hum, objectTemp, current_target_objectTemp);
    

    sprintf(jsonBuf, ALINK_BODY_FORMAT, param);
    
    Serial.println(jsonBuf);
    Serial.printf("\r\n");
    

    client.publish(ALINK_TOPIC_PROP_POST, jsonBuf);
}


void aliyun_MQTT_setup() {
  wifiInit();
  mqttCheckConnect();
}



void aliyun_MQTT_loop() {
  client.loop();

  if (millis() - lastMs >= 5000)
  {
    lastMs = millis();

    //testCallback();

    mqttCheckConnect();
    mqttIntervalPost(temperature, humidity, co2, lightValueAnalog);

    // char testTopic[] = "/sys/a1Zs299lJym/ABCD/thing/service/property/set";
    // byte testPayload[] = "{\"target_temperature\":25.5}";
    // callback(testTopic, testPayload, sizeof(testPayload) - 1);
  }
}
