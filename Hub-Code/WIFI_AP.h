bool isMacAddressStored(const String& macStr) {
  for (const String& storedMac : macAddresses) {
    if (storedMac == macStr) {
      return true; 
    }
  }
  return false;
}

void addMacAddress(const String& macStr) {
  if (!isMacAddressStored(macStr)) {
    macAddresses.push_back(macStr);
  } else {
  }
}


void createPoint(const char* ssid) {
  const char* password = "password";
  Serial.print("[*] Creating New Access Point ... ");
  WiFi.mode(WIFI_AP_STA);
  Serial.println(WiFi.softAP(ssid, NULL, 1) ? "Ready" : "Failed!");
  Serial.println("[+] AccessPoint Created");
  Serial.print("[*] AccessPoint SSID: ");
  Serial.println(ssid);
  Serial.print("[*] AccessPoint Password: ");
  Serial.println(password);
  Serial.print("[*] Gateway: ");
  Serial.println(WiFi.softAPIP());

}

void printIPAddress(const ip4_addr_t* ipAddr) {
  Serial.print(ip4_addr1_16(ipAddr));
  Serial.print(".");
  Serial.print(ip4_addr2_16(ipAddr));
  Serial.print(".");
  Serial.print(ip4_addr3_16(ipAddr));
  Serial.print(".");
  Serial.print(ip4_addr4_16(ipAddr));
}

String replaceSpacesWithHyphens(const String& input) {
  String output = "";
  for (size_t i = 0; i < input.length(); i++) {
    if (input.charAt(i) == ' ') {
      output += '-';
    } else {
      output += input.charAt(i);
    }
  }
  output.remove(output.length() - 1);
  return output;
}

String generateJsonWithMACs() {
  String jsonResult = "[";

  int deviceNumber = 0;
  for (auto it = macAddresses.begin(); it != macAddresses.end(); ++it) {
    String modifiedMac = replaceSpacesWithHyphens(*it);
    String deviceName = "Device " + String(deviceNumber);
    String newDeviceEntry = "{\"Device\": \"" + deviceName + "\", \"MAC\": \"" + modifiedMac + "\"},";\
    jsonResult = jsonResult + "\n" + newDeviceEntry;
    deviceNumber++;
  }
  jsonResult.remove(jsonResult.length() - 1);
  jsonResult = jsonResult + "]";

  Serial.println(jsonResult);

  return jsonResult;
}

void client_status() {
/*

unsigned char number_client;
struct station_info* stat_info;

struct ip4_addr* IPaddress;
int i = 1;

number_client = WiFi.softAPgetStationNum();
stat_info = WiFi.softAPgetStation();

while (stat_info != NULL) {
  IPaddress = &stat_info->ip;
  String MAC;
  for (int i = 0; i < 6; i++) {
    MAC += (i > 0 ? " " : "") + String(stat_info->bssid[i], HEX);
  }
  MAC.toUpperCase();
  addMacAddress(MAC);
  stat_info = STAILQ_NEXT(stat_info, next);
  i++;
}
wifi_softap_free_station_info();

*/
wifi_sta_list_t stationList;    /*Number of connected stations*/
  esp_wifi_ap_get_sta_list(&stationList);  
  Serial.print("N of connected stations: ");
  Serial.println(stationList.num);
  for(int i = 0; i < stationList.num; i++) {
    wifi_sta_info_t station = stationList.sta[i];
    for(int j = 0; j< 6; j++){
      char str[3];
      sprintf(str, "%02x", (int)station.mac[j]);  /*prints MAC address of connected station*/
      Serial.print(str);
      if(j<5){
        Serial.print(":");
      }
    }
    Serial.println();
  }
}
