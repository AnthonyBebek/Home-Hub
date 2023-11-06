#include "time.h"
#include "SDSave.h"

String data[24] = { "1", "1", "1", "1", "1", "1", "1", "1", "1", "1", "1", "1", "1", "1", "1", "1", "1", "1", "1", "1", "1", "1", "1", "1" };

void parseDataString(String data) {
  int firstComma = data.indexOf(',');
  int secondComma = data.indexOf(',', firstComma + 1);
  int thirdComma = data.indexOf(',', secondComma + 1);
  int fourthComma = data.indexOf(',', thirdComma + 1);

  String MAC = data.substring(0, firstComma);
  String TYPE = data.substring(firstComma + 1, secondComma);
  String DATA1 = data.substring(secondComma + 1, thirdComma);
  String DATA2 = data.substring(thirdComma + 1, fourthComma);
  String NAME = data.substring(fourthComma + 1, data.indexOf('!'));
  Serial.println("----------------------------------------------");
  Serial.println("MAC: " + MAC);
  Serial.println("TYPE: " + TYPE);
  Serial.println("DATA1: " + DATA1);
  Serial.println("DATA2: " + DATA2);
  Serial.println("NAME: " + NAME);
  updateSensorData(NAME, TYPE, DATA1.toFloat(), DATA2.toFloat());
  SDWrite(String(NAME), String(GetTime()), String(TYPE), String(DATA1), String(DATA2));
  WriteFile("/" + NAME + ".txt", String(GetJustTime()) + "," + String(DATA1) + "," + String(DATA2));
}

void phraseWifiString(String data) {
  int firstComma = data.indexOf(',');
  int secondComma = data.indexOf(',', firstComma + 1);
  int thirdComma = data.indexOf(',', secondComma + 1);

  String SSID = data.substring(0, firstComma);
  String PASS = data.substring(firstComma + 1, secondComma);
  String IP = data.substring(secondComma + 1, thirdComma);
  String MASK = data.substring(thirdComma + 1, data.indexOf('!'));
  SSID.remove(0, 14);
  PASS.remove(0, 8);
  IP.remove(0, 10);
  MASK.remove(0, 15);

  SSID.remove(SSID.length() - 1, 1);
  PASS.remove(PASS.length() - 1, 1);
  IP.remove(IP.length() - 1, 1);
  MASK.remove(MASK.length() - 2, 2);
  changeWifiSettings(SSID.c_str(), PASS.c_str(), IP.c_str(), MASK.c_str());
}



void runOutput(int Button, int State) {
  String OutputStuff;
  data[Button] = State;
  for (int i = 0; i < 24; i++){
    OutputStuff = OutputStuff + data[i];
  }
  Serial2.print("3");
  Serial2.print(OutputStuff);
  Serial2.print("X");
  Serial.print("3");
  Serial.print(OutputStuff);
  Serial.print("X");
}

void fun() {
  String OutputStuff;
  for (int i = 0; i < 24; i++) {
    for (int x = 0; x < 24; x++) {
      data[x] = "1";
    }
    data[i] = "2";
    for (int i = 0; i < 24; i++) {
      OutputStuff = OutputStuff + data[i];
    }
    Serial2.print("3");
    Serial2.print(OutputStuff);
    Serial2.print("X");
    Serial.print("3");
    Serial.print(OutputStuff);
    Serial.print("X");
    delay(500);
  }
  return;
}

void phraseButtonData(String data) {
  int firstComma = data.indexOf(',');
  int secondComma = data.indexOf(',', firstComma + 1);

  String Button = data.substring(0, firstComma);
  String State = data.substring(firstComma + 1, data.indexOf('!'));
  Button.remove(0, 11);
  State.remove(0, 8);

  Button.remove(Button.length() - 1, 1);
  State.remove(State.length() - 1, 2);
  int butt = Button.toInt();
  int stat;
  if (State == "true"){
    stat = 2;
  }
  else if (State == "false"){
    stat = 1;
  }
  else{
    stat = 0;
  }

  //Serial.println(Button);
  //Serial.println(State);
  runOutput(butt, stat);
}