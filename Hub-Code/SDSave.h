#include "FS.h"
#include "SD.h"
#include "SPI.h"

void SDStart(){
if (!SD.begin()) {
    Serial.println("Card Mount Failed");
    return;
  }
  
  Serial.println("SD card mounted");
}

void SDWrite(String Name, String Time){
File file = SD.open("/log.txt", FILE_APPEND);
  
  if (!file) {
    Serial.println("Failed to open file for appending");
    return;
  }
  
  file.print(Time);
  file.print(" - ");
  file.println(Name);
  file.close();
  
  Serial.println("String appended to the file successfully");
}
