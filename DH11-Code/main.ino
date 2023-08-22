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


String Name = "Master_Bed";

String Type = "Temperature_Sensor";

typedef struct struct_message {
  String a;
  String b;
  int c;
  int d;
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
 

void SendESPData(uint8_t* Data, int Temp, int Humid){
  uint8_t broadcastAddress[6];
  macAddressToByteArray(response, broadcastAddress);
  if ((millis() - lastTime) > timerDelay) {
    myData.a = Name;
    myData.b = Type;
    myData.c = Temp;
    myData.d = Humid;

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
  delay(1000);
  return t;
}

float get_humid()
{
 float h = dht.readHumidity();

 if (isnan(h)) {
 Serial.println("Failed to read from DHT sensor!");
 }
  delay(1000);
  return h;
}


void loop() {
  if (!isnan(get_temp()) && !isnan(get_humid())){
    SendESPData(getMAC(), get_temp(), get_humid());
  }
}
