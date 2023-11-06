#include "FS.h"
#include <SD.h>
#include "SPI.h"

const char* logfile = "/log.txt";
const int bufferSize = 128;

String GraphDataOutput;

void SDStart() {
  if (!SD.begin()) {
    Serial.println("[!] Card Mount Failed");
    return;
  }
  Serial.println("[*] SD card mounted");
}

void SDWrite(String Name, String Time, String Type, String Val1, String Val2) {
  File file = SD.open("/log.txt", FILE_APPEND);

  if (!file) {
    Serial.println("Failed to open file for appending");
    return;
  }
  if (Type == "DHT") {
    Val1 = Val1 + "°C";
    Val2 = Val2 + "%";
  }

  file.print(Time);
  file.print(" - ");
  file.print(Val1);
  file.print(" - ");
  file.print(Val2);
  file.print(" - ");
  file.println(Name);
  file.close();

  Serial.println("String appended to the file successfully");
}


void WriteDB(String File_Name) {
  String contents;
  String temp;
  int LineCount = 0;
  File file;
  if (File_Name == "\log.txt" || File_Name == "\DB.txt") {
    return;
  }
  //Serial.println("/" + File_Name);
  file = SD.open("/" + File_Name);

  if (!file) {
    Serial.println("Failed to open file");
    return;
  }
  while (file.available()) {
    temp = char(file.read());
    contents = contents + temp;
    if (temp == "\n") {
      LineCount = LineCount + 1;
    }
  }
  LineCount = LineCount + 1;
  file.close();
  file = SD.open("/DB.txt", FILE_APPEND);
  file.print(File_Name);
  file.print(" - ");
  file.println(LineCount);
  file.close();
}

void printDirectory(File dir, int numTabs) {
  while (true) {
    File entry = dir.openNextFile();
    if (!entry) {
      break;
    }
    for (uint8_t i = 0; i < numTabs; i++) {
      //Serial.print('\t');
    }
    if (!String(entry.name()).startsWith(".")) {
      if (String(entry.name()).endsWith(".txt")) {
        WriteDB(String(entry.name()));
      }
    }
    entry.close();
  }
}



void SDWriteData(String Name, String Time, String Type, String Val1, String Val2) {
  File file = SD.open("/" + Name + ".txt", FILE_WRITE);
  if (!file) {
    Serial.println("Failed to open file for appending");
    return;
  }
}

void readfile() {
  int data;
  File file = SD.open("/log.txt", FILE_READ);
  file.seek(file.size());
  int i = 0;
  String Output;
  Serial.println("Starting");
  for (int x = 0; x < 10; x++) {
    Serial.println("Loop");
    data = 0;
    while (data != 10) {
      Serial.print(String(data) + " - ");
      Serial.print(char(data));
      Serial.println(" " + String(file.size() - i));
      Output = char(file.peek()) + Output;
      file.seek(file.size() - i);
      i++;
    }
  }
  Serial.println(Output);
}
void UpdateDB() {
  SD.remove("/DB.txt");
  File file = SD.open("/");
  printDirectory(file, 0);
  file.close();
}

void WriteFile(String Device, String Content) {
  String Output;
  int LineCount = 0;
  File file;
  file = SD.open(Device, FILE_READ);
  while (file.available()) {
    Output = Output + char(file.read());
    if (Output.endsWith("\n")) {
      LineCount++;
    }
  }
  file.close();
  while (LineCount > 10) {
    int pos = Output.indexOf('\n');
    if (pos != -1) {
      Output = Output.substring(pos + 1);
      LineCount--;
    }
  }
  file = SD.open(Device, FILE_WRITE);
  if (file) {
    file.print(Output);
    file.println(Content);
    file.close();
  } else {
    Serial.println("Error opening the file for writing");
  }
}

String GetData(String Name) {
  File file;
  String Output;
  file = SD.open("/" + Name, FILE_READ);

  while (file.available()) {
    Output = Output + char(file.read());
  }
  return Output;
}
void GetDirectory(File dir, int numTabs) {
  while (true) {
    File entry = dir.openNextFile();
    if (!entry) {
      break;
    }
    for (uint8_t i = 0; i < numTabs; i++) {
      //Serial.print('\t');
    }
    if (!String(entry.name()).startsWith(".")) {
      if (String(entry.name()).endsWith(".txt")) {
        if (!String(entry.name()).startsWith("log")) {
          if (!String(entry.name()).startsWith("DB")){
            GraphDataOutput = GraphDataOutput + "!" + String(entry.name()) + "\n" + GetData(String(entry.name())) + "\n";
          }
        }
      }
    }
    entry.close();
  }
}

String GetData() {
  GraphDataOutput = "";
  File file = SD.open("/");
  GetDirectory(file, 0);
  file.close();
  return GraphDataOutput;
}
