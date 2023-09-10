#include "Arduino.h"
#include "uRTCLib.h"

uRTCLib rtc(0x68);

String formatTwoDigits(int value) {
  if (value < 10) {
    return "0" + String(value);
  } else {
    return String(value);
  }
}



String GetTime(){
  String time;
  rtc.refresh();
  time = formatTwoDigits(rtc.hour()) + ":" + formatTwoDigits(rtc.minute()) + ":" + formatTwoDigits(rtc.second()) + " " + formatTwoDigits(rtc.day()) + "/" + formatTwoDigits(rtc.month()) + "/" + String(rtc.year());
  return time;
}
