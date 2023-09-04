const char* ssid = "THREE";
const char* password = "password";
const char* hubIP = "192.168.4.1";

WiFiClient client;



int hexDigitToValue(char hexDigit) {
  if (hexDigit >= '0' && hexDigit <= '9') {
    return hexDigit - '0';
  } else if (hexDigit >= 'a' && hexDigit <= 'f') {
    return 10 + (hexDigit - 'a');
  } else if (hexDigit >= 'A' && hexDigit <= 'F') {
    return 10 + (hexDigit - 'A');
  }
  return 0;
}

void macAddressToByteArray(const String& macStr, uint8_t* byteArray) {
  int byteIndex = 0;
  int num = 0;
  for (size_t i = 0; i < macStr.length(); i++) {
    char c = macStr.charAt(i);
    if (isHexadecimalDigit(c)) {
      num = num * 16 + hexDigitToValue(c);
    } else if (c == ':') {
      byteArray[byteIndex] = static_cast<uint8_t>(num);
      byteIndex++;
      num = 0;
    }
  }
  byteArray[byteIndex] = static_cast<uint8_t>(num);
}

uint8_t* getMAC(){
  uint8_t broadcastAddress[6];
  while (response == ""){
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    String url = "http://" + String(hubIP) + "/getmac";
    
    http.begin(client, url);
    
    int httpCode = http.GET();
    if (httpCode > 0) {
      if (httpCode == HTTP_CODE_OK) {
        response = http.getString();
        Serial.println("Received MAC address from hub: " + response);
      } else {
        Serial.println("HTTP request failed with error code: " + String(httpCode));
      }
    } else {
      Serial.println("Connection to server failed");
    }
    
    http.end();
  }
  
  delay(5000); 
  }
  Serial.println(response);
  macAddressToByteArray(response, broadcastAddress);

  return broadcastAddress;
}

void connect_SSID(){
    WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Searching for HUB...");
  }
  Serial.println("Found HUB Wifi!");
  getMAC();
  delay(500);
  WiFi.disconnect();
}
