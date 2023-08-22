#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>
#include <list>
#include <espnow.h>

//Networking Array work
#include "temp.h"
std::list<String> macAddresses;

//esp-Now
typedef struct struct_message {
  String a;
  String b;
  int c;
  int d;
} struct_message;


struct_message myData;

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


//#define OTA_Host_Name "HHClient-Temperature"
#define OTA_Host_Name "THREE"

const char* ssid = OTA_Host_Name;

ESP8266WebServer server(80);

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
  server.send(200, "text/javascript", jquery_min_js);
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
  server.send(200, "application/json", R"=====([
    {"Name": "Bed1", "Catagory": "Temperature", "Data": "32"},
    {"Name": "Bed2", "Catagory": "Temperature", "Data": "48"}
  ])=====");
}

void OnDataRecv(uint8_t* mac, uint8_t* incomingData, uint8_t len) {
  memcpy(&myData, incomingData, sizeof(myData));
  Serial.print("Bytes received: ");
  Serial.println(len);
  Serial.print("Name: ");
  Serial.println(myData.a);
  Serial.print("Type: ");
  Serial.println(myData.b);
  Serial.print("Temp: ");
  Serial.println(myData.c);
  Serial.println();
  Serial.print("Humidity: ");
  Serial.println(myData.d);
  Serial.println();
}

void setup() {
  Serial.begin(115200);
  //createPoint(ssid);
  Serial.println("");
  //Serial.print("Connected to WiFi. IP address: ");
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


 Serial.println("[*] Initializing ESP-NOW");
  if (esp_now_init() != 0) {
    Serial.println("[!] Error initializing ESP-NOW");
    return;
  }
  else{
    esp_now_set_self_role(ESP_NOW_ROLE_SLAVE);
    Serial.println("[+] ESP-NOW Ready");
  }
 esp_now_register_recv_cb(OnDataRecv);

  server.begin();
  Serial.println("HTTP server started.");
  ArduinoOTA.setHostname(OTA_Host_Name);
  ArduinoOTA.begin();
  Serial.println("OTA server started.");

}

void loop() {
  server.handleClient();
  ArduinoOTA.handle();
  //Serial.println("Free heap: " + String(ESP.getFreeHeap()) + " bytes");
  client_status();
  esp_now_register_recv_cb(OnDataRecv);
}