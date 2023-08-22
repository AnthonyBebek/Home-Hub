void changeWifiSettings(const char* newSSID, const char* newPassword, const char* newIP, const char* newSubnet) {
  WiFi.begin(newSSID, newPassword);

  if (strlen(newIP) > 0 && strlen(newSubnet) > 0) {
    IPAddress newIPAddress, newSubnetMask;

    if (newIPAddress.fromString(newIP) && newSubnetMask.fromString(newSubnet)) {
      WiFi.disconnect();
      delay(100);
      WiFi.softAPdisconnect(true);

      WiFi.mode(WIFI_STA);
      WiFi.config(newIPAddress, IPAddress(0, 0, 0, 0), newSubnetMask);

      WiFi.begin(newSSID, newPassword);
    } else {
      Serial.println("Invalid IP address or subnet mask format. Keeping DHCP settings.");
    }
  }

  Serial.println("Applying new WiFi settings...");
  delay(100);
  Serial.println("Done");
}
