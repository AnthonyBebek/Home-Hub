#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <espnow.h>
#include <Arduino.h>
String response;

//My Scripts
#include "register.h"

#include <DHT.h>

#define DHTPIN 2
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

int Time_Delay = 5000; //In Milliseconds 900000 = 15 min AND 10000 = 10 seconds AND 5000 = 5 seconds

int type = 1;

typedef struct struct_message {
  int type;
  float c;
  float d;
} struct_message;

struct_message myData;

unsigned long lastTime = 0;  
unsigned long timerDelay = 2000;

void OnDataSent(uint8_t *mac_addr, uint8_t sendStatus) {
  Serial.print("Last Packet Send Status: ");
  if (sendStatus == 0){
    Serial.println("Delivery success");
  }
  else{
    Serial.println("Delivery fail");
  }
}

 void StartESPNOW(uint8_t* broadcastAddress){
  WiFi.mode(WIFI_STA);

  if (esp_now_init() != 0) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  esp_now_set_self_role(ESP_NOW_ROLE_CONTROLLER);
  esp_now_register_send_cb(OnDataSent);
  
  esp_now_add_peer(broadcastAddress, ESP_NOW_ROLE_SLAVE, 1, NULL, 0);
}
 

void SendESPData(uint8_t* Data, float Temp, float Humid){
  uint8_t broadcastAddress[6];
  macAddressToByteArray(response, broadcastAddress);
  if ((millis() - lastTime) > timerDelay) {
    myData.type = type;

    // Serialize Temp and Humid as little-endian bytes
    myData.c = Temp;
    myData.d = Humid;
    /*
    // Convert integers to little-endian byte order
    uint8_t tempBytes[sizeof(int)];
    uint8_t humidBytes[sizeof(int)];

    for (int i = 0; i < sizeof(int); i++) {
      tempBytes[i] = (Temp >> (i * 8)) & 0xFF;
      humidBytes[i] = (Humid >> (i * 8)) & 0xFF;
    }

    // Copy the bytes into the data structure
    memcpy(&myData.c, tempBytes, sizeof(int));
    memcpy(&myData.d, humidBytes, sizeof(int));
    */

    esp_now_send(broadcastAddress, (uint8_t *) &myData, sizeof(myData));

    lastTime = millis();
    Serial.println("Sent ESP-NOW Data");
  }
}

void setup() {
  Serial.begin(115200);
  connect_SSID();
  StartESPNOW(getMAC());
  dht.begin();
}

float get_temp()
{
 float t = dht.readTemperature();

 if (isnan(t)) {
 Serial.println("Failed to read from DHT sensor!");
 }
  //delay(1000);
  return t;
}

float get_humid()
{
 float h = dht.readHumidity();

 if (isnan(h)) {
 Serial.println("Failed to read from DHT sensor!");
 }
  //delay(1000);
  return h;
}


void loop() {
  float temp = get_temp();
  float humid = get_humid();
  Serial.println("----------");
  Serial.print("Temp: ");
  Serial.println(temp);
  Serial.print("Humidity: ");
  Serial.println(humid);
  Serial.println("----------");
  if (!isnan(temp) && !isnan(humid)){
    Serial.println("Found Temp");
    SendESPData(getMAC(), temp, humid);
  }
  else{
    Serial.println("Found NULL");
    SendESPData(getMAC(),0.00,0.00);
  }
  delay(Time_Delay);
}
