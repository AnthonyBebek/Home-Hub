const int maxSensors = 32;

int SensorNumber;

bool NewSensor;

struct SensorData {
  String name;
  String type;
  float value1;
  float value2;
};

SensorData sensorArray[maxSensors];
int numSensors = 0;

void updateSensorData(String name, String type, float value1, float value2) {
  if (name != NULL && type != NULL) {
    NewSensor = true;
    for (int i = 0; i < numSensors; i++) {
      //Serial.println(String(sensorArray[i].name) + " - " + String(name) + " , " + String(sensorArray[i].type) + " - " + type);
      if (sensorArray[i].name == name) {
        //Serial.println("Updating exsisting sensor");
        sensorArray[i].value1 = value1;
        sensorArray[i].value2 = value2;
        NewSensor = false;
      }
    }
    if (NewSensor == true) {
      if (numSensors < maxSensors) {
        //Serial.println("Creating new sensor entry: " + String(numSensors));
        sensorArray[numSensors].name = name;
        sensorArray[numSensors].type = type;
        sensorArray[numSensors].value1 = value1;
        sensorArray[numSensors].value2 = value2;
        numSensors++;
      } else {
        Serial.println("Sensor Limit Met!");
      }
    } else {
      //Serial.println("Old Sensor Exception Met");
    }
  }
  //Serial.println("");
}

void printSensorData() {
  Serial.println("Printing Sensor Data");
  Serial.println();
  for (int i = 0; i < numSensors; i++) {
    Serial.print("Sensor Name: ");
    Serial.println(sensorArray[i].name);

    Serial.print("Sensor Type: ");
    Serial.println(sensorArray[i].type);

    Serial.print("Value 1: ");
    Serial.println(sensorArray[i].value1);

    Serial.print("Value 2: ");
    Serial.println(sensorArray[i].value2);

    Serial.println();
  }
}


String GetSensorValues() {
  String newSensor;
  String result = "[";
  String sensor[4];
  for (int i = 0; i < numSensors; i++) {
    String name = sensorArray[i].name;
    String type = sensorArray[i].type;
    int value1 = sensorArray[i].value1;
    int value2 = sensorArray[i].value2;

    //type.remove(result.length() - 1);


    Serial.println("----------------------------------------------------");
    Serial.println("Updating Website...");
    Serial.print("Name: ");
    Serial.println(sensorArray[i].name);
    Serial.print("Type: ");
    Serial.println(sensorArray[i].type);
    Serial.println("----------------------------------------------------");
    if (String(sensorArray[i].type) == "DHT") {
      newSensor = "{\"Name\": \"" + String(sensorArray[i].name) + "\", \"Category\": \"" + String(sensorArray[i].type) + "\", \"Temperature\": \"" + String(sensorArray[i].value1) + "°C" + "\", \"Humidity\": \"" + String(sensorArray[i].value2) + "%" + "\"},";
    } else {
      newSensor = "{\"Name\": \"" + String(sensorArray[i].name) + "\", \"Category\": \"" + String(sensorArray[i].type) + "\", \"Value1\": \"" + String(sensorArray[i].value1) + "\", \"Value2\": \"" + String(sensorArray[i].value2) + "\"},";
    }
    result = result + newSensor;
  }
  Serial.println(result);
  result.remove(result.length() - 1);
  Serial.println(result);
  result = result + "]";
  return result;
}
