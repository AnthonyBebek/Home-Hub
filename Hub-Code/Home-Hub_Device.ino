#include <WiFi.h>
#include "esp_wifi.h"
#include <WebServer.h>
#include <mDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>
#include <list>
#include <EEPROM.h>
#include <esp_now.h>

//Networking Array work
#include "temp.h"
std::list<String> macAddresses;


struct MyData {
  int type; // Maximum length for Type
  float c;         // Temp
  float d;         // Humid
};

MyData myData;

//Website Pages
#include "index.h"
#include "hub.h"
#include "actions.h"
#include "settings.h"
#include "test.h"
#include "network.h"
#include "WIFI_AP.h"

//JQuery Pages
#include "JQuery.h"

//External Scripts
#include "NetworkSettings.h"
#include "SensorData.h"
#include "Support.h"
#include "time.h"
#include "SDSave.h"

//#define OTA_Host_Name "HHClient-Temperature"
#define OTA_Host_Name "WASP-Network"

const char* ssid = OTA_Host_Name;

WebServer server(80);

void handleGetMAC() {
  Serial.println(WiFi.macAddress());
  server.send(200, "text/plain", String(WiFi.macAddress()));
  Serial.println("Someone requested HUB MAC!");
}

void setupHub() {

  const char* hubIP = "192.168.1.60";
  const char* hubPassword = "thritr";
  Serial.println("Hub Set");
}

void readHub() {
  Serial.print("Hub IP: ");
  Serial.print("Hub Password: ");
}

void handleRoot() {
  Serial.println("Client requested Home");
  server.send(200, "text/html", index_html);
}

void handleJQuery() {
  Serial.println("Free heap: " + String(ESP.getFreeHeap()) + " bytes");
  server.send(200, "text/html", jquery_min_js);
  Serial.println(jquery_min_js);
  Serial.println("Jquery Requested");
}

void handleHub() {
  Serial.println("Client requested Hub");
  server.send(200, "text/html", hub_html);
  readHub();
}

void handleActions() {
  Serial.println("Client requested Actions");
  server.send(200, "text/html", actions_html);
  setupHub();
}

void handleSettings() {
  Serial.println("Client requested Settings");
  server.send(200, "text/html", settings_html);
}

void handleNetwork() {
  Serial.println("Client requested Network");
  server.send(200, "text/html", network_html);
}

void handleInfoInput() {
  if (server.method() == HTTP_POST) {
    String name = server.arg("Name");
    String password = server.arg("Pass");

    Serial.println("Received POST data for Data:");
    Serial.print("Name: ");
    Serial.println(name);
    Serial.print("Password: ");
    Serial.println(password);
  }

  server.send(200, "text/html", "Info page");
  Serial.println("Client sent info data");
}

void handleWifiInput() {
  if (server.method() == HTTP_POST) {
    String SSID = server.arg("SSID_Name");
    String password = server.arg("Pass");
    String IP = server.arg("IPAddr");
    String Subnet = server.arg("Subent_Mask");

    Serial.println("Received POST data for Wifi:");
    Serial.print("SSID: ");
    Serial.println(SSID);
    Serial.print("Password: ");
    Serial.println(password);
    Serial.print("IP: ");
    Serial.println(IP);
    Serial.print("Subnet: ");
    Serial.println(Subnet);
    changeWifiSettings(SSID.c_str(), password.c_str(), IP.c_str(), Subnet.c_str());
    return;
  }

  server.send(200, "text/html", "Ok");
  Serial.println("Client sent wifi data");
}

void handleNetworkInput() {
  if (server.method() == HTTP_POST) {
    String MAC = server.arg("MAC_Address");

    Serial.println("Received POST data for Network Input");
    Serial.print("MAC address to add: ");
    Serial.println(MAC);

    server.send(200, "text/plain", "Ok");
    return;
  }

  server.send(200, "text/html", "Ok");
  Serial.println("Client sent Network Join Data");
}

void handleNetworkDevices() {
  Serial.println("Finding Network Devices Requested");
  String MAC_temp = generateJsonWithMACs();
  server.send(200, "application/json", MAC_temp);
}

void handleSensorData() {
  Serial.println("Sensor Data Requested");
  String Sensor_Data = GetSensorValues();
  server.send(200, "application/json", Sensor_Data);
}

void OnDataRecv(const uint8_t* mac, const uint8_t* incomingData, int len) {
  String Data1;
  String MAC;
  memcpy(&myData, incomingData, sizeof(myData));

 
  for (int i = 0; i < 6; i++) {
    MAC = MAC + (convertDecimalToHex(mac[i]));
    if (i < 5){
      MAC = MAC + ":";
    }
  }
  Serial.print("Received RAW data: ");
  for (int i = 0; i < len; i++) {
    Serial.print(incomingData[i], HEX);
    Serial.print(' ');
  }
  Serial.println();
  Serial.println("----------------------------------------------------");
  Serial.print("Received data from MAC: ");
  Serial.println(MAC);
  Serial.println("----------------------------------------------------");
  Serial.print("Type: ");
  Serial.println(convertType(myData.type));
  Serial.println();
  Serial.print("Value 1: ");
  Serial.println(myData.c);
  Serial.print("Value 2: ");
  Serial.println(myData.d);
  Serial.println("----------------------------------------------------");
  Serial.print("Friendly Name: ");
  Serial.println(FindFriendlyName(MAC));
  Serial.println("----------------------------------------------------");
  Serial.print("Recorded At: ");
  Serial.println(String(GetTime()));
  Serial.println("----------------------------------------------------");
  printSensorData();
  Serial.println("----------------------------------------------------");
  updateSensorData(FindFriendlyName(MAC), String(convertType(myData.type)), myData.c, myData.d);
  SDWrite(String(FindFriendlyName(MAC)), String(GetTime()));
}


void setup() {
  Serial.begin(115200);
  EEPROM.begin(512);
  server.on("/getmac", HTTP_GET, handleGetMAC);

  //JQUERY
  server.on("/jquery.min.js", HTTP_GET, handleJQuery);

  //WEBSERVER
  server.on("/", handleRoot);
  server.on("/hub", handleHub);
  server.on("/actions", handleActions);
  server.on("/settings", handleSettings);
  server.on("/network", handleNetwork);
  server.on("/info", handleInfoInput);
  server.on("/wifi", handleWifiInput);
  server.on("/Network_Add", handleNetworkInput);
  server.on("/Network_Found", handleNetworkDevices);
  server.on("/Sensor_Data", handleSensorData);
  Serial.println("---------------------");
  Serial.println("[*] Setting up WiFi Station");
  createPoint(ssid);
  Serial.println("[+] WiFi Station online!");
  Serial.println("---------------------");
  Serial.println("[*] Initializing ESP-NOW");
  if (esp_now_init() != ESP_OK) {
    Serial.println("[!] Error initalizing ESP-NOW");
    return;
  } else {
    Serial.println("[+] ESP-NOW Ready");
    esp_now_register_recv_cb(OnDataRecv);
  }
  Serial.println("---------------------");
  Serial.println("[*] Starting HTTP Server");
  server.begin();
  Serial.println("[+] HTTP Server Ready!");
  Serial.println("---------------------");
  Serial.println("[*] Starting OTA server");
  ArduinoOTA.setHostname(OTA_Host_Name);
  ArduinoOTA.begin();
  Serial.println("[+] OTA Server Ready!");
  Serial.println("---------------------");
  Serial.print("[*] Starting RTC: ");
  URTCLIB_WIRE.begin();
  Serial.print("[+] RTC Started: ");
  Serial.println("---------------------");
  Serial.println("[*] Starting RTC: ");
  SDStart();
  Serial.println("[+] RTC Started: ");
  Serial.println("---------------------");
  Serial.print("[*] Ram Free: ");
  Serial.println(ESP.getFreeHeap());
  Serial.println("---------------------");

  
  //changeWifiSettings("TheBlock", "10cartledge", "0", "0");
}

void loop() {
  server.handleClient();
  delay(1);
  //ArduinoOTA.handle();
  //Serial.println("Free heap: " + String(ESP.getFreeHeap()) + " bytes");
  //client_status();
}
