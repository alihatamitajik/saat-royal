// Library forReal Time Clock Module
#include <RTClib.h>
RTC_DS1307 rtc;

bool isAlarm() {
  DateTime noww = rtc.now();
  return noww.hour() == RemoteXY.alarm_h &&
       noww.minute() == RemoteXY.alarm_m &&
       noww.second() <= 5;
}

void initRTC(){
  if (! rtc.begin()) {
    Serial.println("Couldn't find RTC");
    Serial.flush();
    while (1) delay(10);
  }

  if (! rtc.isrunning()) {
    Serial.println("RTC is NOT running, let's set the time!");
    // When time needs to be set on a new device, or after a power loss, the
    // following line sets the RTC to the date & time this sketch was compiled
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
    // This line sets the RTC with an explicit date & time, for example to set
    // January 21, 2014 at 3am you would call:
    // rtc.adjust(DateTime(2014, 1, 21, 3, 0, 0));
  }
}
