#include <WiFi.h>
#include "esp_wifi.h"
#include <WebServer.h>
#include <mDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>
#include <list>
#include <EEPROM.h>
#include <WiFiUdp.h>
#include <NTPClient.h>

String RadioOutput;
String RadioMAC;
String RadioType;
String RadioC;
String RadioD;
String RadioInput;

//Networking Array work
std::list<String> macAddresses;

//Website Pages
#include "index.h"
#include "hub.h"
#include "actions.h"
#include "settings.h"
#include "test.h"
#include "network.h"
#include "WIFI_AP.h"
#include "Graphs.h"

//JQuery Pages
#include "JQuery.h"
#include "temp.h"

//External Scripts
#include "NetworkSettings.h"
#include "SensorData.h"
#include "Support.h"

//#define OTA_Host_Name "HHClient-Temperature"
#define OTA_Host_Name "WASP-Network"

const char* ssid = OTA_Host_Name;

WebServer server(80);

void handleGetMAC() {
  //server.send(200, "text/plain", String(WiFi.macAddress()));
  server.send(200, "text/plain", String(RadioMAC));
  Serial.print("Someone requested HUB - Radio MAC: ");
  Serial.println(String(RadioMAC));
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
  Serial.println("Jquery Requested");
  server.send(200, "application/javascript", huhu3);
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

void handleGraphs() {
  Serial.println("Client requested Graphs");
  server.send(200, "text/html", graphs_html);
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
    String rawData = server.arg(0);

    Serial.println("[*] Raw Data");
    Serial.println("[√] Received POST data for Wifi:");
    rawData = rawData + "!";
    phraseWifiString(rawData);
    return;
  }

  server.send(200, "application/json", "Ok");
  Serial.println("Client sent wifi data");
}

void handleButtonInput() {
  if (server.method() == HTTP_POST) {
    String rawData = server.arg(0);
    Serial.println("[*] Raw Data");
    Serial.println("[√] Received POST data for Buttonss:");
    rawData = rawData + "!";
    Serial.println(rawData);
    phraseButtonData(rawData);
    return;
  }
  server.send(200, "text/html", "Ok");
  Serial.println("Client sent button data");
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
void handleGraphData() {
  Serial.println("Graph Data Requested");
  String Graph_Data = GetData();
  server.sendHeader("Access-Control-Allow-Origin", "*");
  server.send(200, "application/json", Graph_Data);
}

void setup() {
  Serial.begin(115200);
  Serial2.begin(115200);
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
  server.on("/graphs", handleGraphs);
  server.on("/Network_Add", handleNetworkInput);
  server.on("/Network_Found", handleNetworkDevices);
  server.on("/Sensor_Data", handleSensorData);
  server.on("/Graph_Data", handleGraphData);
  server.on("/Button_Data", handleButtonInput);
  Serial.println("---------------------");
  Serial.println("[*] Setting up WiFi Station");
  createPoint(ssid);
  Serial.println("---------------------");
  Serial.println("[√] WiFi Station online!");
  Serial.println("[*] Starting HTTP Server");
  server.begin();
  Serial.println("---------------------");
  Serial.println("[√] HTTP Server Ready!");
  Serial.println("[*] Starting OTA server");
  ArduinoOTA.setHostname(OTA_Host_Name);
  ArduinoOTA.begin();
  Serial.println("[+] OTA Server Ready!");
  Serial.println("---------------------");
  Serial.println("[*] Starting RTC: ");
  URTCLIB_WIRE.begin();
  Serial.println("[*] Date & Time: " + String(GetTime()));
  Serial.println("---------------------");
  Serial.println("[√] RTC Started: ");
  Serial.println("[*] Starting SD: ");
  SDStart();
  Serial.println("[+] SD Started: ");
  Serial.println("[*] Updating SD Database: ");
  UpdateDB();
  Serial.println("---------------------");
  Serial.println("[√] Updated SD Database: ");
  Serial.print("[*] Ram Free: ");
  Serial.println(ESP.getFreeHeap());
  Serial.println("---------------------");
  Serial.print("[*] Querying ESP8266: ");
  Serial2.println("2");
  unsigned long starttime;
  starttime = millis();
  while (RadioMAC.length() != 17) {
    if (millis() - starttime >= 2000) {
      Serial2.println("2");
      starttime = millis();
      Serial.println("[!] Incorrect or NULL Responce");
      Serial.println("[!] Trying Again");
    }
    if (Serial2.available()) {
      RadioMAC = Serial2.readString();
      Serial.println(RadioMAC);
      RadioMAC = RadioMAC.substring(0, RadioMAC.length() - 17);
    }
  }
  Serial.print("[√] ESP8266 MAC Address: ");
  Serial.println(RadioMAC);
  Serial.println("---------------------");
  Serial.println("[√] Setup Finished!");

  //readfile();

  //changeWifiSettings("TheBlock", "10cartledge", "0", "0");
  //SetTime();
}

void loop() {
  server.handleClient();
  //delay(1);

  if (Serial2.available()) {
    RadioOutput = Serial2.readString();
    Serial.println(RadioOutput);
    parseDataString(RadioOutput);
  }

  //fun();
  //ArduinoOTA.handle();
  //Serial.println("Free heap: " + String(ESP.getFreeHeap()) + " bytes");
  //client_status();
}
