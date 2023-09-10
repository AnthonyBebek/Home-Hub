String types[3] = {"", "DHT", "Light"};

int Names = 3;

String MACs[3] = {"68:C6:3A:D3:BF:7D", "B4:8A:A:F0:E3:BF", "BC:DD:C2:78:12:13"};

String FriendlyNames[3] = {"Bed 1", "Kitchen", "Lounge Room"};

String FindFriendlyName(String MAC){
  String Name;
  for (int i = 0; i < Names; i++){
    if (MACs[i] == MAC){
      Name = FriendlyNames[i];
    }
  }
  return Name;
}

String convertType(int type){
  return types[type];
}


String convertDecimalToHex(int n) {
  String hexNum;
  int remainder;
  while (n > 0) {
    remainder = n % 16;
    n = n / 16;
    if (remainder < 10) {
      hexNum = String(remainder) + hexNum;
    }
    else {
      switch (remainder) {
        case 10:
          hexNum = "A" + hexNum;
          break;
        case 11:
          hexNum = "B" + hexNum;
          break;
        case 12:
          hexNum = "C" + hexNum;
          break;
        case 13:
          hexNum = "D" + hexNum;
          break;
        case 14:
          hexNum = "E" + hexNum;
          break;
        case 15:
          hexNum = "F" + hexNum;
          break;
      }
    }
  }
  return hexNum;
}
