#define SECS_PER_HOUR 60*60
#define daylightOffset_sec 60*60
String sNtpServerName2 = "pool.ntp.org";
String sNtpServerName3 = "time.nist.gov";

time_t getNtpTime() {
  time_t t;  
  uint8_t count = 0;
  struct tm *timeinfo;
  unsigned long summerOffset = 0;
  if (WiFi.status() == WL_CONNECTED) {
      summerOffset = isDayLightSaving ? daylightOffset_sec/60 : 0;
      //configTime(timezone*SECS_PER_HOUR, summerOffset, sNtpServerName.c_str(), sNtpServerName2.c_str(), sNtpServerName3.c_str()); // enable NTP
      configTime(0, 0, sNtpServerName.c_str(), sNtpServerName2.c_str(), sNtpServerName3.c_str()); // enable NTP
      Serial.println("\nWaiting for time");
      t = time(NULL);
      timeinfo = localtime(&t);
      while (timeinfo->tm_year < (2017 - 1900) && count < 10) {
          t = time(NULL);
          timeinfo = localtime(&t);
          Serial.print(".");
          Serial.print("t "); Serial.println(t);
          Serial.print("timeinfo->tm_year "); Serial.println(timeinfo->tm_year);
          count ++;
          delay(500);
      }
      if (timeinfo->tm_year > (2017 - 1900)) {
        Serial.println("Time NTP ready.");
        Serial.print("timeinfo UTC: "); Serial.printf("%02d:%02d", timeinfo->tm_hour, timeinfo->tm_min); Serial.println();
        TimeChangeRule localCEST = {"CEST", Last, Sun, Mar, 2, 60*timezone+summerOffset};     // LOCAL Summer Time
        TimeChangeRule localCET = {"CET ", Last, Sun, Oct, 3, 60*timezone};       // LOCAL Standard Time
        Timezone localCE(localCEST, localCET);
        //TimeChangeRule *tcr; //need only for diff rules
        //return t; 
        //return localCE.toLocal(t, &tcr); //Serial.println(tcr -> abbrev);
        return localCE.toLocal(t); //Serial.println(tcr -> abbrev);           
      }
      else return 0;
  }
  else return 0;
}


#if USE_RTC == true
time_t getRtcTime() {
    DateTime now = rtc.now();
    return now.unixtime();// + timezone * SECS_PER_HOUR; 
}
#endif
