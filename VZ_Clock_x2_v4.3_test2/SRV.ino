void serverInit(void) {
  server.on("/", auth_auth);
  server.on("/index.vz", auth_index);  //server.on("/index.vz", [](){server.send_P(200, "text/html", P_index);});
  server.on("/time.vz", auth_time);  //server.on("/time.vz", [](){server.send_P(200, "text/html", P_time);}); 
  server.on("/weather.vz", auth_weather);  //server.on("/weather.vz", [](){server.send_P(200, "text/html", P_weath);});
  server.on("/mqtt.vz", auth_mqtt);  //server.on("/mqtt.vz", [](){server.send_P(200, "text/html", P_mqtt);});
  server.on("/thing.vz", auth_thing);
  server.on("/setup.vz", auth_setup);  //server.on("/setup.vz", [](){server.send_P(200, "text/html", P_setup);});
  server.on("/sgp.vz", auth_sgp);  //server.on("/sgp.vz", [](){server.send_P(200, "text/html", P_sgp);});
  server.on("/help.vz", auth_help);  //server.on("/help.vz", [](){server.send_P(200, "text/html", P_help);});
  server.on("/style.css", [](){server.send_P ( 200, "text/css", P_css);});
  server.on("/function.js", [](){server.send_P ( 200, "text/plain", P_js);});
  server.on("/favicon.ico", [](){server.send(200, "text/html", "");});
  server.onNotFound([]() {(404, "text/plain", "FileNotFound");});
  server.on("/configs.json", handle_ConfigJSON);    // формування configs.json сторінки для передачі данних в web інтерфейс
  server.on("/configs_wifi.json", handle_ConfigWifiJson);
  server.on("/configs_time.json", handle_ConfigTimeJson);
  server.on("/configs_mqtt.json", handle_ConfigMqttJson);
  server.on("/configs_weath.json", handle_ConfigWeathJson);
  server.on("/configs_setup.json", handle_ConfigSetupJson);
  server.on("/configs_sgp.json", handle_ConfigSgpJson);
  server.on("/configs_thing.json", handle_ConfigThingJson);
  server.on("/ssid", handle_Set_Ssid);
  server.on("/ntp", handle_ntp);         // Установка часової зони по запиту типа http://192.168.2.100/timeZone?timeZone=3
  server.on("/set_time", handle_set_time);
  server.on("/timepc", handle_timepc);
  server.on("/weatherUpdate", handle_weather_update);
  server.on("/weather", handle_weather);    // Установка сервера погоди по запиту типа http://192.168.2.100/weatherHost?weatherHost=api.openweathermap.org
  server.on("/mqttUst", handle_mqtt_ust);
  server.on("/mqttOn", handle_mqtt_on);
  server.on("/thing_on", handle_thing_on);
  server.on("/set_thing", handle_set_thing);
  server.on("/setup", handle_setup);
  server.on("/sgp", handle_sgp);
  server.on("/mess", handle_message);
  server.on("/restart", handle_Restart);            // перезавантаження можуля по запиту типу http://192.168.1.11/restart?device=ok
  server.on("/stopalarm", handle_stopAlarm);
  server.on("/resetConfig", handle_resetConfig);
  server.on("/printCom", handle_set_printCom);
  httpUpdater.setup(&server);
  server.begin();
}
//======================================================================================================
void auth_auth(){
  if(!authOn) server.send_P(200, "text/html", P_index);
  else server.send_P(200, "text/html", P_auth);
}
//======================================================================================================
void auth_index(){
  if(server.arg("auth")==auth || !authOn) server.send_P(200, "text/html", P_index);
  else server.send_P(200, "text/html", P_auth);
}
//======================================================================================================
void auth_time(){
  if(server.arg("auth")==auth || !authOn) server.send_P(200, "text/html", P_time);
  else server.send_P(200, "text/html", P_auth);
}
//======================================================================================================
void auth_weather(){
  if(server.arg("auth")==auth || !authOn) server.send_P(200, "text/html", P_weath);
  else server.send_P(200, "text/html", P_auth);
}
//======================================================================================================
void auth_mqtt(){
  if(server.arg("auth")==auth || !authOn) server.send_P(200, "text/html", P_mqtt);
  else server.send_P(200, "text/html", P_auth);
}
//======================================================================================================
void auth_thing(){
  if(server.arg("auth")==auth || !authOn) server.send_P(200, "text/html", P_thing);
  else server.send_P(200, "text/html", P_auth);
}
//======================================================================================================
void auth_setup(){
  if(server.arg("auth")==auth || !authOn) server.send_P(200, "text/html", P_setup);
  else server.send_P(200, "text/html", P_auth);
}
//======================================================================================================
void auth_sgp(){
  if(server.arg("auth")==auth || !authOn) server.send_P(200, "text/html", P_sgp);
  else server.send_P(200, "text/html", P_auth);
}
//======================================================================================================
void auth_help(){
  if(server.arg("auth")==auth || !authOn) server.send_P(200, "text/html", P_help);
  else server.send_P(200, "text/html", P_auth);
}
//======================================================================================================
void handle_ConfigJSON() {
  if(verific()) return;
  String json = "{";
  json += "\"ver\":\"";
  json += ver;
  json += "\",\"time\":\"";
  json += (String(hour) + ":" + (minute < 10 ? "0" : "") + String(minute) + ":" + (second < 10 ? "0" : "") + String(second));
  json += "\",\"ip\":\"";
  json += WiFi.localIP().toString();
  json += "\",\"printCom\":\"";
  json += (printCom==1?"checked":"");
  json += "\"}";
  server.send(200, "text/json", json);
}
//======================================================================================================
void handle_ConfigWifiJson() {
  if(verific()) return;
  String json = "{";
  json += "\"ver\":\"";
  json += ver;
  json += "\",\"auth\":\"";
  json += auth;
  json += "\",\"time\":\"";
  json += (String(hour) + ":" + (minute < 10 ? "0" : "") + String(minute) + ":" + (second < 10 ? "0" : "") + String(second));
  json += "\",\"ssid\":\"";
  json += ssid;
  json += "\",\"password\":\"";
  json += password;
  json += "\",\"ssidAP\":\"";
  json += ssidAP;
  json += "\",\"passwordAP\":\"";
  json += passwordAP;
  json += "\",\"authOn\":\"";
  json += (authOn==1?"checked":"");
  json += "\"}";
  server.send(200, "text/json", json);
}
//======================================================================================================
void handle_ConfigTimeJson() {
  if(verific()) return;
  String json = "{";
  json += "\"ver\":\"";
  json += ver;
  json += "\",\"time\":\"";
  json += (String(hour) + ":" + (minute < 10 ? "0" : "") + String(minute) + ":" + (second < 10 ? "0" : "") + String(second));
  json += "\",\"ntpServerName\":\"";
  json += ntpServerName;
  json += "\",\"timeZone\":\"";
  json += timeZone;
  json += "\",\"isDayLightSaving\":\"";
  json += (isDayLightSaving==1?"checked":"");
  json += "\",\"rtcStat\":\"";
  json += (rtcStat==1?"checked":"");
  json += "\",\"al_0_0\":\"";
  json += alarme[0][0];
  json += "\",\"al_0_1\":\"";
  json += alarme[0][1];
  json += "\",\"al_0_2\":\"";
  json += alarme[0][2];
  json += "\",\"al_1_0\":\"";
  json += alarme[1][0];
  json += "\",\"al_1_1\":\"";
  json += alarme[1][1];
  json += "\",\"al_1_2\":\"";
  json += alarme[1][2];
  json += "\",\"al_2_0\":\"";
  json += alarme[2][0];
  json += "\",\"al_2_1\":\"";
  json += alarme[2][1];
  json += "\",\"al_2_2\":\"";
  json += alarme[2][2];
  json += "\",\"al_3_0\":\"";
  json += alarme[3][0];
  json += "\",\"al_3_1\":\"";
  json += alarme[3][1];
  json += "\",\"al_3_2\":\"";
  json += alarme[3][2];
  json += "\",\"al_4_0\":\"";
  json += alarme[4][0];
  json += "\",\"al_4_1\":\"";
  json += alarme[4][1];
  json += "\",\"al_4_2\":\"";
  json += alarme[4][2];
  json += "\",\"md_0_0\":\"";
  json += memory_date[0][0];
  json += "\",\"md_0_1\":\"";
  json += memory_date[0][1];
  json += "\",\"md_0_2\":\"";
  json += memory_date_mes0;
  json += "\",\"md_1_0\":\"";
  json += memory_date[1][0];
  json += "\",\"md_1_1\":\"";
  json += memory_date[1][1];
  json += "\",\"md_1_2\":\"";
  json += memory_date_mes1;
  json += "\",\"md_2_0\":\"";
  json += memory_date[2][0];
  json += "\",\"md_2_1\":\"";
  json += memory_date[2][1];
  json += "\",\"md_2_2\":\"";
  json += memory_date_mes2;
  json += "\",\"md_3_0\":\"";
  json += memory_date[3][0];
  json += "\",\"md_3_1\":\"";
  json += memory_date[3][1];
  json += "\",\"md_3_2\":\"";
  json += memory_date_mes3;
  json += "\",\"md_4_0\":\"";
  json += memory_date[4][0];
  json += "\",\"md_4_1\":\"";
  json += memory_date[4][1];
  json += "\",\"md_4_2\":\"";
  json += memory_date_mes4;
  json += "\",\"md_5_0\":\"";
  json += memory_date[5][0];
  json += "\",\"md_5_1\":\"";
  json += memory_date[5][1];
  json += "\",\"md_5_2\":\"";
  json += memory_date_mes5;
  json += "\",\"md_6_0\":\"";
  json += memory_date[6][0];
  json += "\",\"md_6_1\":\"";
  json += memory_date[6][1];
  json += "\",\"md_6_2\":\"";
  json += memory_date_mes6;
  json += "\",\"md_7_0\":\"";
  json += memory_date[7][0];
  json += "\",\"md_7_1\":\"";
  json += memory_date[7][1];
  json += "\",\"md_7_2\":\"";
  json += memory_date_mes7;
  json += "\",\"md_8_0\":\"";
  json += memory_date[8][0];
  json += "\",\"md_8_1\":\"";
  json += memory_date[8][1];
  json += "\",\"md_8_2\":\"";
  json += memory_date_mes8;
  json += "\",\"md_start\":\"";
  json += memory_hour_start;
  json += "\",\"md_stop\":\"";
  json += memory_hour_end;
  json += "\",\"t0\":\"";
  json += hour;
  json += "\",\"t1\":\"";
  json += minute;
  json += "\",\"d0\":\"";
  json += day;
  json += "\",\"d1\":\"";
  json += month;
  json += "\",\"d2\":\"";
  json += year;
  json += "\"}";
  server.send(200, "text/json", json);
}
//======================================================================================================
void handle_ConfigMqttJson() {
  if(verific()) return;
  String json = "{";
  json += "\"ver\":\"";
  json += ver;
  json += "\",\"time\":\"";
  json += (String(hour) + ":" + (minute < 10 ? "0" : "") + String(minute) + ":" + (second < 10 ? "0" : "") + String(second));
  json += "\",\"mqttOn\":\"";
  json += (mqttOn==1?"checked":"");
  json += "\",\"mqtt_server\":\"";
  json += mqtt_server;
  json += "\",\"mqtt_port\":\"";
  json += mqtt_port;
  json += "\",\"mqtt_user\":\"";
  json += mqtt_user;
  json += "\",\"mqtt_pass\":\"";
  json += mqtt_pass;
  json += "\",\"mqtt_name\":\"";
  json += mqtt_name;
  json += "\",\"mqtt_sub1\":\"";
  json += mqtt_sub1;
  json += "\",\"mqtt_sub2\":\"";
  json += mqtt_sub2;
  json += "\",\"mqtt_sub3\":\"";
  json += mqtt_sub3;
  json += "\",\"mqtt_sub_inform\":\"";
  json += mqtt_sub_inform;
  json += "\",\"mqtt_pub_temp\":\"";
  json += mqtt_pub_temp;
  json += "\",\"mqtt_pub_tempUl\":\"";
  json += mqtt_pub_tempUl;
  json += "\",\"mqtt_pub_tempH\":\"";
  json += mqtt_pub_tempH;
  json += "\",\"mqtt_pub_hum\":\"";
  json += mqtt_pub_hum;
  json += "\",\"mqtt_pub_press\":\"";
  json += mqtt_pub_press;
  json += "\",\"mqtt_pub_alt\":\"";
  json += mqtt_pub_alt;
  json += "\",\"timeOutMqtt\":\"";
  json += timeOutMqtt;
  json += "\"}";
  server.send(200, "text/json", json);
}
//======================================================================================================  
void handle_ConfigWeathJson() {
  if(verific()) return;
  int sr = location_sunrise.substring(0, 2).toInt() + (int)hourCorr;
  if(sr>23) sr -= 24;
  if(sr<0) sr += 24;
  String sunrise = String(sr) + location_sunrise.substring(2, 5);
  int ss = location_sunset.substring(0, 2).toInt() + (int)hourCorr;
  if(ss>23) ss -= 24;
  if(ss<0) ss += 24;
  String sunset = String(ss) + location_sunset.substring(2, 5);
  int st = location_localtime.substring(11, 13).toInt() + (int)hourCorr;
  int ly = location_localtime.substring(0, 4).toInt();
  byte lm = location_localtime.substring(5, 7).toInt();
  byte ld = location_localtime.substring(8, 10).toInt(); 
  if(st>23) {
    st -= 24;
    ld++;
    if(ld==32 || (ld==31 && (lm==4 || lm==6 || lm==9 || lm==11)) || (lm==2 && ((ld==29 && ly%4!=0) || (ld==30 && ly%4==0)))) {
      ld=1;
      lm++;
      if(lm>12){
        lm=1;
        ly++;
      }
    }
  }
  if(st<0) {
    st += 24;
    ld--;
    if(ld<1) {
      ld = 0 + ((lm==5 || lm==7 || lm==10 || lm==12 || (lm==3 && ly%4==0))?30:(lm==3 && ly%4!=0)?29:31);
      lm--;
      if(lm<1){
        lm=12;
        ly--;
      }
    }
  }
  String lt = String(ly) + "-" + (lm<10?"0":"") + String(lm) + "-" + (ld<10?"0":"") + String(ld) + " " + (st<10?"0":"") + String(st) + location_localtime.substring(13, 16);
  String json = "{";
  json += "\"ver\":\"";
  json += ver;
  json += "\",\"time\":\"";
  json += (String(hour) + ":" + (minute < 10 ? "0" : "") + String(minute) + ":" + (second < 10 ? "0" : "") + String(second));
  json += "\",\"weatherKey0\":\"";
  json += weatherKey0;
  json += "\",\"weatherKey1\":\"";
  json += weatherKey1;
  json += "\",\"weatherHost\":\"";
  json += weatherHost;
  json += "\",\"cityID0\":\"";
  json += cityID0;
  json += "\",\"cityID1\":\"";
  json += cityID1;
  json += "\",\"weatherLang\":\"";
  json += weatherLang;
  json += "\",\"displayForecast\":\"";
  json += (displayForecast==1?"checked":"");
  json += "\",\"displayCityName\":\"";
  json += (displayCityName==1?"checked":"");
  json += "\",\"displayForecastNow\":\"";
  json += (displayForecastNow==1?"checked":"");
  json += "\",\"displayForecastToday\":\"";
  json += (displayForecastToday==1?"checked":"");
  json += "\",\"displayForecastTomorrow\":\"";
  json += (displayForecastTomorrow==1?"checked":"");
  json += "\",\"animNotWeather\":\"";
  json += (animNotWeather==1?"checked":"");
  json += "\",\"timeScrollStart\":\"";
  json += timeScrollStart;
  json += "\",\"timeScrollStop\":\"";
  json += timeScrollStop;
  json += "\",\"location_name\":\"";
  json += location_name;
  json += "\",\"location_region\":\"";
  json += location_region;
  json += "\",\"location_country\":\"";
  json += location_country;
  json += "\",\"location_localtime\":\"";
  json += lt;
  json += "\",\"location_temp\":\"";
  json += location_temp;
  json += "\",\"location_app_temp\":\"";
  json += location_app_temp;
  json += "\",\"location_rh\":\"";
  json += location_rh;
  json += "\",\"location_pres\":\"";
  json += location_pres;
  json += "\",\"location_wind_spd\":\"";
  json += location_wind_spd;
  json += "\",\"location_wind_cdir_full\":\"";
  json += location_wind_cdir_full;
  json += "\",\"location_sunrise\":\"";
  json += sunrise;
  json += "\",\"location_sunset\":\"";
  json += sunset;
  json += "\",\"location_clouds\":\"";
  json += location_clouds;
  json += "\",\"location_vis\":\"";
  json += location_vis;
  json += "\",\"location_uv\":\"";
  json += location_uv;
  json += "\",\"location_weather_description\":\"";
  json += location_weather_description;
  json += "\",\"uuid\":\"";
  json += uuid;
  json += "\",\"api_key\":\"";
  json += api_key;
  json += "\",\"sensors_ID0\":\"";
  json += sensors_ID0;
  json += "\",\"sensors_ID1\":\"";
  json += sensors_ID1;
  json += "\",\"sensors_ID2\":\"";
  json += sensors_ID2;
  json += "\",\"personalCityName\":\"";
  json += personalCityName;
  json += "\"}";
  server.send(200, "text/json", json);
}
//======================================================================================================
void handle_ConfigSetupJson() {
  if(verific()) return;
  String json = "{";
  json += "\"ver\":\"";
  json += ver;
  json += "\",\"time\":\"";
  json += (String(hour) + ":" + (minute < 10 ? "0" : "") + String(minute) + ":" + (second < 10 ? "0" : "") + String(second));
  json += "\",\"kuOn\":\"";
  json += kuOn;
  json += "\",\"kuOff\":\"";
  json += kuOff;
  json += "\",\"tbd\":\"";
  json += timeDay;
  json += "\",\"tbn\":\"";
  json += timeNight;
  json += "\",\"vbd\":\"";
  json += volBrightnessD;
  json += "\",\"vbn\":\"";
  json += volBrightnessN;
  json += "\",\"clockNight\":\"";
  json += clockNight;
  json += "\",\"secLine\":\"";
  json += secLine;
  json += "\",\"timeScrollSpeed\":\"";
  json += 100 - timeScrollSpeed;
  json += "\",\"vba\":\"";
  json += (volBrightnessAuto==1?"checked":"");
  json += "\",\"lowLivelBrightness\":\""; 
  json += lowLivelBrightness;
  json += "\",\"upLivelBrightness\":\"";
  json += upLivelBrightness;
  json += "\",\"lba\":\"";
  json += levelBridhtness;
  json += "\",\"buzzerSet\":\"";
  json += (buzzerSet==1?"checked":"");
  json += "\",\"sensorDom\":\"";
  json += sensorDom;
  json += "\",\"corrTempD\":\"";
  json += corrTempD;
  json += "\",\"Td\":\"";
  json += t0;
  json += "\",\"sensorUl\":\"";
  json += sensorUl;
  json += "\",\"corrTempU\":\"";
  json += corrTempU;
  json += "\",\"Tu\":\"";
  json += t3;
  json += "\",\"sensorHome\":\"";
  json += sensorHome;
  json += "\",\"corrTempH\":\"";
  json += corrTempH;
  json += "\",\"Th\":\"";
  json += t6;
  json += "\",\"sensorHumi\":\"";
  json += sensorHumi;
  json += "\",\"corrHumi\":\"";
  json += corrHumi;
  json += "\",\"Hd\":\"";
  json += h0;
  json += "\",\"sensorPrAl\":\"";
  json += sensorPrAl;
  json += "\",\"corrPress\":\"";
  json += corrPress;
  json += "\",\"Pu\":\"";
  json += p0;
  json += "\",\"pressSys\":\"";
  json += pressSys;
  json += "\",\"rotate0\":\"";
  json += rotate0;
  json += "\",\"rotate1\":\"";
  json += rotate1;
  json += "\",\"NUM_MAX0\":\"";
  json += NUM_MAX0;
  json += "\",\"NUM_MAX1\":\"";
  json += NUM_MAX1;
  json += "\",\"fontCLOCK\":\"";
  json += fontCLOCK;
  json += "\",\"fontSizeCLOCK\":\"";
  json += fontSizeCLOCK;
  json += "\",\"fontSizeData\":\"";
  json += fontSizeData;
  json += "\",\"dataDown\":\"";
  json += dataDown;
  json += "\",\"timeStopBigCklock\":\"";
  json += timeStopBigCklock;
  json += "\",\"textDown\":\"";
  json += textDown;
  json += "\",\"butStat\":\"";
  json += butStat;
  json += "\",\"displayData\":\"";
  json += displayData;
  json += "\",\"sgp\":\"";
  if(sgpFound){
    json += "SGP30";
  } else json += "";
  json += "\"}";
  server.send(200, "text/json", json);
}
//======================================================================================================
void handle_ConfigSgpJson() {
  if(verific()) return;
  String json = "{";
  json += "\"ver\":\"";
  json += ver;
  json += "\",\"time\":\"";
  json += (String(hour) + ":" + (minute < 10 ? "0" : "") + String(minute) + ":" + (second < 10 ? "0" : "") + String(second));
  json += "\",\"sgpCo2\":\"";
  json += sgpCo2;
  json += "\",\"textCo2\":\"";
  json += sgpCo2Message[sgpCo2Livel];
  json += "\",\"sgpCo2LivelAlarm\":\"";
  json += sgpCo2LivelAlarm;
  json += "\",\"eCo2AlarmEsp\":\"";
  json += (eCo2AlarmEsp==1?"checked":"");
  json += "\",\"eCo2AlarmMqtt\":\"";
  json += (eCo2AlarmMqtt==1?"checked":"");
  json += "\",\"eCo2Led\":\"";
  json += (eCo2Led==1?"checked":"");
  json += "\",\"mqtt_pub_eco2\":\"";
  json += mqtt_pub_eco2;
  json += "\",\"sgpTvoc\":\"";
  json += sgpTvoc;
  json += "\",\"textTvoc\":\"";
  json += sgpTvocMessage[sgpTvocLivel];
  json += "\",\"sgpTvocLivelAlarm\":\"";
  json += sgpTvocLivelAlarm;
  json += "\",\"tvocAlarmEsp\":\"";
  json += (tvocAlarmEsp==1?"checked":"");
  json += "\",\"tvocAlarmMqtt\":\"";
  json += (tvocAlarmMqtt==1?"checked":"");
  json += "\",\"tvocLed\":\"";
  json += (tvocLed==1?"checked":"");
  json += "\",\"mqtt_pub_tvoc\":\"";
  json += mqtt_pub_tvoc;
  json += "\",\"sgpCorr\":\"";
  json += (sgpCorr==1?"checked":"");
  json += "\",\"sgpCorrMan\":\"";
  json += (sgpCorrMan==1?"checked":"");
  json += "\",\"sgpCorrTemp\":\"";
  json += sgpCorrTemp;
  json += "\",\"sgpCorrHumi\":\"";
  json += sgpCorrHumi;
  json += "\"}";
  server.send(200, "text/json", json);
}
//======================================================================================================
void handle_ConfigThingJson() {
  if(verific()) return;
  String json = "{";
  json += "\"ver\":\"";
  json += ver;
  json += "\",\"time\":\"";
  json += (String(hour) + ":" + (minute < 10 ? "0" : "") + String(minute) + ":" + (second < 10 ? "0" : "") + String(second));
  json += "\",\"sendThing\":\"";
  json += (sendThing==1?"checked":"");
  json += "\",\"channelIdWrite\":\"";
  json += channelIdWrite;
  json += "\",\"writeApiKey\":\"";
  json += writeApiKey;
  json += "\",\"wrApiKey\":\"";
  json += wrApiKey;
  json += "\",\"nameWriteChannel\":\"";
  json += nameWriteChannel;
  json += "\",\"nameField1\":\"";
  json += (nameField1!=""?nameField1:"Отключен");
  json += "\",\"nameField2\":\"";
  json += (nameField2!=""?nameField2:"Отключен");
  json += "\",\"nameField3\":\"";
  json += (nameField3!=""?nameField3:"Отключен");
  json += "\",\"nameField4\":\"";
  json += (nameField3!=""?nameField4:"Отключен");
  json += "\",\"nameField5\":\"";
  json += (nameField5!=""?nameField5:"Отключен");
  json += "\",\"nameField6\":\"";
  json += (nameField6!=""?nameField6:"Отключен");
  json += "\",\"nameField7\":\"";
  json += (nameField7!=""?nameField7:"Отключен");
  json += "\",\"nameField8\":\"";
  json += (nameField8!=""?nameField8:"Отключен");
  json += "\",\"writeFild1\":\"";
  json += writeFild1;
  json += "\",\"writeFild2\":\"";
  json += writeFild2;
  json += "\",\"writeFild3\":\"";
  json += writeFild3;
  json += "\",\"writeFild4\":\"";
  json += writeFild4;
  json += "\",\"writeFild5\":\"";
  json += writeFild5;
  json += "\",\"writeFild6\":\"";
  json += writeFild6;
  json += "\",\"writeFild7\":\"";
  json += writeFild7;
  json += "\",\"writeFild8\":\"";
  json += writeFild8;
  json += "\",\"getThing\":\"";
  json += (getThing==1?"checked":"");
  json += "\",\"channelIdRead\":\"";
  json += channelIdRead;
  json += "\",\"readApiKey\":\"";
  json += readApiKey;
  json += "\",\"nameReadChannel\":\"";
  json += nameReadChannel;
  json += "\",\"nameReadField1\":\"";
  json += (nameReadField1!=""?nameReadField1:"Откл.");
  json += "\",\"nameReadField2\":\"";
  json += (nameReadField2!=""?nameReadField2:"Откл.");
  json += "\",\"nameReadField3\":\"";
  json += (nameReadField3!=""?nameReadField3:"Откл.");
  json += "\",\"nameReadField4\":\"";
  json += (nameReadField4!=""?nameReadField4:"Откл.");
  json += "\",\"nameReadField5\":\"";
  json += (nameReadField5!=""?nameReadField5:"Откл.");
  json += "\",\"nameReadField6\":\"";
  json += (nameReadField6!=""?nameReadField6:"Откл.");
  json += "\",\"nameReadField7\":\"";
  json += (nameReadField7!=""?nameReadField7:"Откл.");
  json += "\",\"nameReadField8\":\"";
  json += (nameReadField8!=""?nameReadField8:"Откл.");
  json += "\",\"thing1\":\"";
  json += thing1;
  json += "\",\"thing2\":\"";
  json += thing2;
  json += "\",\"thing3\":\"";
  json += thing3;
  json += "\",\"thing4\":\"";
  json += thing4;
  json += "\",\"thing5\":\"";
  json += thing5;
  json += "\"}";
  server.send(200, "text/json", json);
}
//======================================================================================================
void handle_Set_Ssid() {
  if(verific()) return;
  if(server.arg("ssid")!="") ssid = server.arg("ssid").c_str();
  if(server.arg("password")!="") password = server.arg("password").c_str();
  if(server.arg("ssidAP")!="") ssidAP = server.arg("ssidAP").c_str();
  if(server.arg("passwordAP")!="") passwordAP = server.arg("passwordAP").c_str();
  if(server.arg("auth")!="") auth = server.arg("auth").c_str();
  if(server.arg("authOn")!="") authOn = server.arg("authOn").toInt();
  saveConfig();
  if(printCom) {
    printTime();
    Serial.println("Set SSID: " + ssid + ",  Set password: " + password + ",  Set SSID AP: " + ssidAP + ",  Set AP password: " + passwordAP + ",  Set auth: " + auth + ",  Set authOn: " + authOn);
  }
  server.send(200, "text/plain", "OK");
  ESP.reset();
}
//======================================================================================================
void handle_ntp(){
  if(verific()) return;
  if(server.arg("ntpServerName")!="") ntpServerName = server.arg("ntpServerName").c_str();
  if(server.arg("timeZone")!="") timeZone = server.arg("timeZone").toFloat();
  if(server.arg("isDayLightSaving")!="") isDayLightSaving = server.arg("isDayLightSaving").toInt();
  if(server.arg("rtcStat")!="") rtcStat = server.arg("rtcStat").toInt();
  if(server.arg("al_0_0")!="") alarme[0][0]=server.arg("al_0_0").toInt();
  if(server.arg("al_0_1")!="") alarme[0][1]=server.arg("al_0_1").toInt();
  if(server.arg("al_0_2")!="") alarme[0][2]=server.arg("al_0_2").toInt();
  if(server.arg("al_1_0")!="") alarme[1][0]=server.arg("al_1_0").toInt();
  if(server.arg("al_1_1")!="") alarme[1][1]=server.arg("al_1_1").toInt();
  if(server.arg("al_1_2")!="") alarme[1][2]=server.arg("al_1_2").toInt();
  if(server.arg("al_2_0")!="") alarme[2][0]=server.arg("al_2_0").toInt();
  if(server.arg("al_2_1")!="") alarme[2][1]=server.arg("al_2_1").toInt();
  if(server.arg("al_2_2")!="") alarme[2][2]=server.arg("al_2_2").toInt();
  if(server.arg("al_3_0")!="") alarme[3][0]=server.arg("al_3_0").toInt();
  if(server.arg("al_3_1")!="") alarme[3][1]=server.arg("al_3_1").toInt();
  if(server.arg("al_3_2")!="") alarme[3][2]=server.arg("al_3_2").toInt();
  if(server.arg("al_4_0")!="") alarme[4][0]=server.arg("al_4_0").toInt();
  if(server.arg("al_4_1")!="") alarme[4][1]=server.arg("al_4_1").toInt();
  if(server.arg("al_4_2")!="") alarme[4][2]=server.arg("al_4_2").toInt();
  if(server.arg("md_0_0")!="") memory_date[0][0]=server.arg("md_0_0").toInt();
  if(server.arg("md_0_1")!="") memory_date[0][1]=server.arg("md_0_1").toInt();
  if(server.arg("md_1_0")!="") memory_date[1][0]=server.arg("md_1_0").toInt();
  if(server.arg("md_1_1")!="") memory_date[1][1]=server.arg("md_1_1").toInt();
  if(server.arg("md_2_0")!="") memory_date[2][0]=server.arg("md_2_0").toInt();
  if(server.arg("md_2_1")!="") memory_date[2][1]=server.arg("md_2_1").toInt();
  if(server.arg("md_3_0")!="") memory_date[3][0]=server.arg("md_3_0").toInt();
  if(server.arg("md_3_1")!="") memory_date[3][1]=server.arg("md_3_1").toInt();
  if(server.arg("md_4_0")!="") memory_date[4][0]=server.arg("md_4_0").toInt();
  if(server.arg("md_4_1")!="") memory_date[4][1]=server.arg("md_4_1").toInt();
  if(server.arg("md_5_0")!="") memory_date[5][0]=server.arg("md_5_0").toInt();
  if(server.arg("md_5_1")!="") memory_date[5][1]=server.arg("md_5_1").toInt();
  if(server.arg("md_6_0")!="") memory_date[6][0]=server.arg("md_6_0").toInt();
  if(server.arg("md_6_1")!="") memory_date[6][1]=server.arg("md_6_1").toInt();
  if(server.arg("md_7_0")!="") memory_date[7][0]=server.arg("md_7_0").toInt();
  if(server.arg("md_7_1")!="") memory_date[7][1]=server.arg("md_7_1").toInt();
  if(server.arg("md_8_0")!="") memory_date[8][0]=server.arg("md_8_0").toInt();
  if(server.arg("md_8_1")!="") memory_date[8][1]=server.arg("md_8_1").toInt();
  if(server.arg("md_0_2")!="") snprintf(memory_date_mes0, 51, "%s", server.arg("md_0_2").c_str());
  if(server.arg("md_1_2")!="") snprintf(memory_date_mes1, 51, "%s", server.arg("md_1_2").c_str());
  if(server.arg("md_2_2")!="") snprintf(memory_date_mes2, 51, "%s", server.arg("md_2_2").c_str());
  if(server.arg("md_3_2")!="") snprintf(memory_date_mes3, 51, "%s", server.arg("md_3_2").c_str());
  if(server.arg("md_4_2")!="") snprintf(memory_date_mes4, 51, "%s", server.arg("md_4_2").c_str());
  if(server.arg("md_5_2")!="") snprintf(memory_date_mes5, 51, "%s", server.arg("md_5_2").c_str());
  if(server.arg("md_6_2")!="") snprintf(memory_date_mes6, 51, "%s", server.arg("md_6_2").c_str());
  if(server.arg("md_7_2")!="") snprintf(memory_date_mes7, 51, "%s", server.arg("md_7_2").c_str());
  if(server.arg("md_8_2")!="") snprintf(memory_date_mes8, 51, "%s", server.arg("md_8_2").c_str());
  if(server.arg("md_start")!="") memory_hour_start = server.arg("md_start").toInt();
  if(server.arg("md_stop")!="") memory_hour_end = server.arg("md_stop").toInt();
  if(printCom) {
    printTime();
    Serial.println("Set NTP Server Name: " + ntpServerName + ",  NTP Time Zone: " + String(timeZone) + ",  isDayLightSaving: " + String(isDayLightSaving));
  }
  alarm_hold=0;
  saveAlarm();
  timeUpdateNTP();
  server.send(200, "text/plain", "OK"); 
}
//======================================================================================================
void handle_set_time(){
  if(verific()) return;
  if(server.arg("t0")!="") hour = server.arg("t0").toInt();
  if(server.arg("t1")!="") minute = server.arg("t1").toInt();
  if(server.arg("d0")!="") day = server.arg("d0").toInt();
  if(server.arg("d1")!="") month = server.arg("d1").toInt();
  if(server.arg("d2")!="") year = server.arg("d2").toInt();
  if(printCom) {
    printTime();
    Serial.println("Set manual time: " + String(hour) + ":" + String(minute) + "      " + String(day) + "-" + String(month) + "-" + String(year));
  }
  server.send(200, "text/plain", "OK");
  localEpoc = (hour * 60 * 60 + minute * 60);
  saveTime();
  showSimpleDate();
  hour_rtc = hour;
  minute_rtc = minute;
  second_rtc = 0;
  month_rtc = month;
  day_rtc = day;
  year_rtc = year;
  setRTCDateTime();
}
//======================================================================================================
void handle_timepc(){
  if(verific()) return;
  if(server.arg("hours")!="") hour = server.arg("hours").toInt();
  if(server.arg("minute")!="") minute = server.arg("minute").toInt();
  if(server.arg("sec")!="") second = server.arg("sec").toInt();
  if(server.arg("tz")!="") timeZone = server.arg("tz").toFloat();
  if(server.arg("day")!="") day = server.arg("day").toInt();
  if(server.arg("month")!="") month = server.arg("month").toInt();
  if(server.arg("year")!="") year = server.arg("year").toInt();
  if(printCom) {
    printTime();
    Serial.println("Set Date/Time from PC - "+String(day)+"."+String(month)+"."+String(year)+"     "+String(hour)+":"+String(minute)+":"+String(second)+"    timeZone="+String(timeZone));
  }
  localMillisAtUpdate = millis();
  localEpoc = (hour * 60 * 60 + minute * 60 + second);
  saveConfig();
  server.send(200, "text/plain", "OK"); 
}
//======================================================================================================
void handle_weather_update(){
  if(verific()) return;
  if(server.arg("update") == "ok") {
    if(printCom) {
      printTime();
      Serial.println("Update weather!");
    }
    if(displayForecast) {
      if(!weatherHost){
        getWeatherData0();
        getWeatherDataz0();
      } else {
        getWeatherData1();
        getWeatherDataz1();
      }
    }
    if(sensorUl==7)getNarodmon();
    bip();
    server.send(200, "text/plain", "OK");
  }
}
//======================================================================================================
void handle_weather(){
  if(verific()) return;
  if(server.arg("weatherHost")!="") weatherHost = server.arg("weatherHost").toInt();
  if(server.arg("weatherKey0")!="") weatherKey0 = server.arg("weatherKey0").c_str();
  if(server.arg("weatherKey1")!="") weatherKey1 = server.arg("weatherKey1").c_str();
  if(server.arg("cityID0")!="") cityID0 = server.arg("cityID0").c_str();
  if(server.arg("cityID1")!="") cityID1 = server.arg("cityID1").c_str();
  if(server.arg("weatherLang")!="") weatherLang = server.arg("weatherLang").c_str();
  if(server.arg("displayForecast")!="") displayForecast = server.arg("displayForecast").toInt();
  if(server.arg("displayCityName")!="") displayCityName = server.arg("displayCityName").toInt();
  if(server.arg("displayForecastNow")!="") displayForecastNow = server.arg("displayForecastNow").toInt();
  if(server.arg("displayForecastToday")!="") displayForecastToday = server.arg("displayForecastToday").toInt();
  if(server.arg("displayForecastTomorrow")!="") displayForecastTomorrow = server.arg("displayForecastTomorrow").toInt();
  if(server.arg("animNotWeather")!="") animNotWeather = server.arg("animNotWeather").toInt();
  if(server.arg("timeScrollStart")!="") timeScrollStart = server.arg("timeScrollStart").toInt();
  if(server.arg("timeScrollStop")!="") timeScrollStop = server.arg("timeScrollStop").toInt();
  if(server.arg("uuid")!="") uuid = server.arg("uuid").c_str();
  if(server.arg("api_key")!="") api_key = server.arg("api_key").c_str();
  if(server.arg("sensors_ID0")!="") sensors_ID0 = server.arg("sensors_ID0").toInt();
  if(server.arg("sensors_ID1")!="") sensors_ID1 = server.arg("sensors_ID1").toInt();
  if(server.arg("sensors_ID2")!="") sensors_ID2 = server.arg("sensors_ID2").toInt();
  if(server.arg("personalCityName")!="") snprintf(personalCityName, 51, "%s", server.arg("personalCityName").c_str());
  if(printCom) {
    printTime();
    Serial.print("Set Weather Server: ");
    if(!weatherHost) {
      if(printCom) Serial.print(weatherHost0);
    } else if(printCom) Serial.print(weatherHost1);
    if(printCom) Serial.println(", Key0: " + weatherKey0 + ", Key1: " + weatherKey1 + ", City ID0: " + cityID0 + ", City ID1: " + cityID1 + ", weatherLang: " + weatherLang);
    if(printCom) Serial.println("displayForecast: " + String(displayForecast) + ", displayCityName: " + String(displayCityName) + ", displayForecastNow: " + String(displayForecastNow) + ", displayForecastTomorrow: " + String(displayForecastTomorrow));
    if(printCom) Serial.println("outtimeScrollStart: " + String(timeScrollStart) + ", timeScrollStop: " + String(timeScrollStop) + ", timeScrollSpeed: " + String(timeScrollSpeed) + ", uuid: " + String(uuid) + ", api_key: " + String(api_key) + ", sensors_ID0: " + String(sensors_ID0) + ", sensors_ID1: " + String(sensors_ID1) + ", sensors_ID2: " + String(sensors_ID2));
  }
  saveConfig();
  lang();
  if(displayForecast && WiFi.status() == WL_CONNECTED){ 
    if(!weatherHost){
      getWeatherData0();
      getWeatherDataz0();
    } else {
      getWeatherData1();
      getWeatherDataz1();
    }
    if(printCom) {
      printTime();
      Serial.println("Update weather!");
    }
  }
  if(sensorUl==7) getNarodmon();
  server.send(200, "text/plain", "OK");
}
//======================================================================================================
void handle_setup(){
  if(verific()) return;
  if(server.arg("tbd")!="") timeDay = server.arg("tbd").toInt();
  if(server.arg("vbd")!="") volBrightnessD = server.arg("vbd").toInt();
  if(server.arg("tbn")!="") timeNight = server.arg("tbn").toInt();
  if(server.arg("vbn")!="") volBrightnessN = server.arg("vbn").toInt();
  if(server.arg("vba")!="") volBrightnessAuto = server.arg("vba").toInt();
  if(server.arg("lowLivelBrightness")!="") lowLivelBrightness = server.arg("lowLivelBrightness").toInt();
  if(server.arg("upLivelBrightness")!="") upLivelBrightness = server.arg("upLivelBrightness").toInt();
  if(server.arg("clockNight")!="") clockNight = server.arg("clockNight").toInt();
  if(server.arg("secLine")!="") secLine = server.arg("secLine").toInt();
  if(server.arg("buzzerSet")!="") buzzerSet = server.arg("buzzerSet").toInt();
  if(server.arg("rotate0")!="") rotate0 = server.arg("rotate0").toInt();
  if(server.arg("rotate1")!="") rotate1 = server.arg("rotate1").toInt();
  if(server.arg("NUM_MAX0")!="") NUM_MAX0 = server.arg("NUM_MAX0").toInt();
  if(server.arg("NUM_MAX1")!="") NUM_MAX1 = server.arg("NUM_MAX1").toInt();
  if(server.arg("kuOn")!="") kuOn = server.arg("kuOn").toInt();
  if(server.arg("kuOff")!="") kuOff = server.arg("kuOff").toInt();
  if(server.arg("sensorDom")!="") sensorDom = server.arg("sensorDom").toInt();
  if(server.arg("sensorUl")!="") sensorUl = server.arg("sensorUl").toInt();
  if(server.arg("sensorHome")!="") sensorHome = server.arg("sensorHome").toInt();
  if(server.arg("sensorHumi")!="") sensorHumi = server.arg("sensorHumi").toInt();
  if(server.arg("sensorPrAl")!="") sensorPrAl = server.arg("sensorPrAl").toInt();
  if(server.arg("pressSys")!="") pressSys = server.arg("pressSys").toInt();
  if(server.arg("fontCLOCK")!="") fontCLOCK = server.arg("fontCLOCK").toInt();
  if(server.arg("fontSizeCLOCK")!="") fontSizeCLOCK = server.arg("fontSizeCLOCK").toInt();
  if(server.arg("fontSizeData")!="") fontSizeData = server.arg("fontSizeData").toInt();
  if(server.arg("dataDown")!="") dataDown = server.arg("dataDown").toInt();
  if(server.arg("timeStopBigCklock")!="") timeStopBigCklock = server.arg("timeStopBigCklock").toInt();
  if(server.arg("textDown")!="") textDown = server.arg("textDown").toInt();
  if(server.arg("timeScrollSpeed")!="") timeScrollSpeed = 100 - server.arg("timeScrollSpeed").toInt();
  if(server.arg("corrTempD")!="") corrTempD = server.arg("corrTempD").toFloat();
  if(server.arg("corrTempU")!="") corrTempU = server.arg("corrTempU").toFloat();
  if(server.arg("corrTempH")!="") corrTempH = server.arg("corrTempH").toFloat();
  if(server.arg("corrHumi")!="") corrHumi = server.arg("corrHumi").toFloat();
  if(server.arg("corrPress")!="") corrPress = server.arg("corrPress").toInt();
  if(server.arg("displayData")!="") displayData = server.arg("displayData").toInt();
  if(server.arg("butStat")!="") butStat = server.arg("butStat").toInt();
  saveConfig();
  if(printCom) {
    printTime();
    Serial.println("TBD: "+String(timeDay)+", VBD: "+String(volBrightnessD)+", TBN: "+String(timeNight)+", VBN: "+String(volBrightnessN)+",  kuOn: "+String(kuOn)+",  kuOff: "+String(kuOff)+",  rotate0: "+String(rotate0)+", rotate1: "+String(rotate1)+", clockNight: "+String(clockNight)+", buzzerSet: "+String(buzzerSet));
    Serial.println("sensorDom: "+String(sensorDom)+", sensorUl: "+String(sensorUl)+", sensorHome: "+String(sensorHome)+", sensorHumi: "+String(sensorHumi)+",  sensorPrAl: "+String(sensorPrAl));
  }
  sensorsAll();
  server.send(200, "text/plain", "OK");
}
//======================================================================================================
void handle_sgp(){
  if(verific()) return;
  if(server.arg("eCo2Led")!="") sgpCo2 = server.arg("eCo2Led").toInt();
  if(server.arg("sgpCo2LivelAlarm")!="") sgpCo2LivelAlarm = server.arg("sgpCo2LivelAlarm").toInt();
  if(server.arg("eCo2AlarmEsp")!="") eCo2AlarmEsp = server.arg("eCo2AlarmEsp").toInt();
  if(server.arg("eCo2AlarmMqtt")!="") eCo2AlarmMqtt = server.arg("eCo2AlarmMqtt").toInt();
  if(server.arg("mqtt_pub_eco2")!="") snprintf(mqtt_pub_eco2, 24, "%s", server.arg("mqtt_pub_eco2").c_str());
  
  if(server.arg("tvocLed")!="") tvocLed = server.arg("tvocLed").toInt();
  if(server.arg("sgpTvocLivelAlarm")!="") sgpTvocLivelAlarm = server.arg("sgpTvocLivelAlarm").toInt();
  if(server.arg("tvocAlarmEsp")!="") tvocAlarmEsp = server.arg("tvocAlarmEsp").toInt();
  if(server.arg("tvocAlarmMqtt")!="") tvocAlarmMqtt = server.arg("tvocAlarmMqtt").toInt();
  if(server.arg("mqtt_pub_tvoc")!="") snprintf(mqtt_pub_tvoc, 24, "%s", server.arg("mqtt_pub_tvoc").c_str());
  
  if(server.arg("sgpCorr")!="") sgpCorr = server.arg("sgpCorr").toInt();
  if(server.arg("sgpCorrMan")!="") sgpCorrMan = server.arg("sgpCorrMan").toInt();
  if(sgpCorr && sgpCorrMan){
    if(server.arg("sgpCorrTemp")!="") sgpCorrTemp = server.arg("sgpCorrTemp").toFloat();
    if(server.arg("sgpCorrHumi")!="") sgpCorrHumi = server.arg("sgpCorrHumi").toFloat();
  }


  if(printCom) {
    printTime();
    Serial.println("Set eCo2Led: " + String(eCo2Led) + ",  sgpCo2LivelAlarm: " + String(sgpCo2LivelAlarm) + ",  eCo2AlarmEsp: " + String(eCo2AlarmEsp) + ",  eCO2AlatmMqtt: " + String(eCo2AlarmMqtt) + ", mqtt_pub_eco2: " + String(mqtt_pub_eco2));
    Serial.print("          Set tvocLed: " + String(tvocLed) + ",  sgpTvocLivelAlarm: " + String(sgpTvocLivelAlarm) + ",  tvocAlarmEsp: " + String(tvocAlarmEsp)+ ",  tvocAlarmMqtt: " + String(tvocAlarmMqtt) + ",  mqtt_pub_tvoc: " + String(mqtt_pub_tvoc) + ",  sgpCorr: " + String(sgpCorr) + ",  sgpCorrMan: " + String(sgpCorrMan));
    if(sgpCorr && sgpCorrMan){
      if(printCom) Serial.println(",  sgpCorrTemp: " + String(sgpCorrTemp) + ",  sgpCorrMan: " + String(sgpCorrHumi));
    } else if(printCom) Serial.println("");
  }
  saveConfig(); 
  server.send(200, "text/plain", "OK");
  if(mqttOn) reconnect();
}
//======================================================================================================
void handle_mqtt_ust() {
  if(verific()) return;
  if(server.arg("mqtt_server")!="") snprintf(mqtt_server, 24, "%s", server.arg("mqtt_server").c_str());
  if(server.arg("mqtt_port")!="") mqtt_port = server.arg("mqtt_port").toInt();
  if(server.arg("mqtt_user")!="") snprintf(mqtt_user, 24, "%s", server.arg("mqtt_user").c_str());
  if(server.arg("mqtt_pass")!="") snprintf(mqtt_pass, 24, "%s", server.arg("mqtt_pass").c_str());
  if(server.arg("mqtt_name")!="") snprintf(mqtt_name, 24, "%s", server.arg("mqtt_name").c_str());
  if(server.arg("mqtt_sub1")!="") snprintf(mqtt_sub1, 24, "%s", server.arg("mqtt_sub1").c_str());
  if(server.arg("mqtt_sub2")!="") snprintf(mqtt_sub2, 24, "%s", server.arg("mqtt_sub2").c_str());
  if(server.arg("mqtt_sub3")!="") snprintf(mqtt_sub3, 24, "%s", server.arg("mqtt_sub3").c_str());
  if(server.arg("mqtt_sub_inform")!="") snprintf(mqtt_sub_inform, 24, "%s", server.arg("mqtt_sub_inform").c_str());
  if(server.arg("mqtt_pub_temp")!="") snprintf(mqtt_pub_temp, 24, "%s", server.arg("mqtt_pub_temp").c_str());
  if(server.arg("mqtt_pub_tempUl")!="") snprintf(mqtt_pub_tempUl, 24, "%s", server.arg("mqtt_pub_tempUl").c_str());
  if(server.arg("mqtt_pub_tempH")!="") snprintf(mqtt_pub_tempH, 24, "%s", server.arg("mqtt_pub_tempH").c_str());
  if(server.arg("mqtt_pub_hum")!="") snprintf(mqtt_pub_hum, 24, "%s", server.arg("mqtt_pub_hum").c_str());
  if(server.arg("mqtt_pub_press")!="") snprintf(mqtt_pub_press, 24, "%s", server.arg("mqtt_pub_press").c_str());
  if(server.arg("mqtt_pub_alt")!="") snprintf(mqtt_pub_alt, 24, "%s", server.arg("mqtt_pub_alt").c_str());
  if(server.arg("timeOutMqtt")!="") timeOutMqtt = server.arg("timeOutMqtt").toInt();
  if(printCom) {
    printTime();
    Serial.println("Set mqtt_server: " + String(mqtt_server) + ",  mqtt_port: " + String(mqtt_port) + ",  mqtt_user: " + String(mqtt_user) + ",  mqtt_pass: " + String(mqtt_pass) + ", mqtt_name: " + String(mqtt_name));
    Serial.println("          Set mqtt_sub1: " + String(mqtt_sub1) + ",  mqtt_sub2: " + String(mqtt_sub2) + ",  mqtt_sub3: " + String(mqtt_sub3)+ ",  mqtt_sub_inform: " + String(mqtt_sub_inform) + ",  mqtt_pub_temp: " + String(mqtt_pub_temp) + ",  mqtt_pub_tempUl: " + String(mqtt_pub_tempUl) + ",  mqtt_pub_hum: " + String(mqtt_pub_hum));
  }
  saveConfig(); 
  server.send(200, "text/plain", "OK");
  if(mqttOn) reconnect();
}
//======================================================================================================
void handle_mqtt_on() {
  if(verific()) return;
  if(server.arg("mqttOn")!="") mqttOn = server.arg("mqttOn").toInt(); 
  if(printCom) {
    printTime();
    Serial.println(server.arg("mqttOn"));
  }
  if(mqttOn) reconnect();
  saveConfig(); 
  server.send(200, "text/plain", "OK");
}
//======================================================================================================
void handle_thing_on() {
  if(verific()) return;
  if(server.arg("sendThing")!=""){
    sendThing = server.arg("sendThing").toInt();
    if(sendThing) sendThings();
  }
  if(server.arg("getThing")!=""){
    getThing = server.arg("getThing").toInt();
    if(getThing) getThings();
  }
  if(printCom) {
    printTime();
    Serial.println("sendThing = " + String(sendThing) + ",  getThing = " + String(getThing));
  }
  saveConfig(); 
  server.send(200, "text/plain", "OK");
}
//======================================================================================================
void handle_set_thing() {
  if(verific()) return;
  if(server.arg("channelIdWrite")!="") channelIdWrite = server.arg("channelIdWrite").c_str();
  if(server.arg("writeApiKey")!="") writeApiKey = server.arg("writeApiKey").c_str();
  if(server.arg("wrApiKey")!="") weatherKey1 = server.arg("wrApiKey").c_str();
  if(server.arg("writeFild1")!="") writeFild1 = server.arg("writeFild1").toInt();
  if(server.arg("writeFild2")!="") writeFild2 = server.arg("writeFild2").toInt();
  if(server.arg("writeFild3")!="") writeFild3 = server.arg("writeFild3").toInt();
  if(server.arg("writeFild4")!="") writeFild4 = server.arg("writeFild4").toInt();
  if(server.arg("writeFild5")!="") writeFild5 = server.arg("writeFild5").toInt();
  if(server.arg("writeFild6")!="") writeFild6 = server.arg("writeFild6").toInt();
  if(server.arg("writeFild7")!="") writeFild7 = server.arg("writeFild7").toInt();
  if(server.arg("writeFild8")!="") writeFild8 = server.arg("writeFild8").toInt();
  if(server.arg("channelIdRead")!="") channelIdRead = server.arg("channelIdRead").c_str();
  if(server.arg("readApiKey")!="") readApiKey = server.arg("readApiKey").c_str();
  if(server.arg("thing1")!="") thing1 = server.arg("thing1").toInt();
  if(server.arg("thing2")!="") thing2 = server.arg("thing2").toInt();
  if(server.arg("thing3")!="") thing3 = server.arg("thing3").toInt();
  if(server.arg("thing4")!="") thing4 = server.arg("thing4").toInt();
  if(server.arg("thing5")!="") thing5 = server.arg("thing5").toInt();
  if(printCom) {
    printTime();
    Serial.println("channelIdWrite= " + channelIdWrite + ", writeApiKey= " + writeApiKey + ", wrApiKey= " + wrApiKey);
    Serial.println("             writeFild1 = " + String(writeFild1) + ", writeFild2 = " + String(writeFild2) + ", writeFild3 = " + String(writeFild3) + ", writeFild4 = " + String(writeFild4) + ", writeFild5 = " + String(writeFild5)  + ", writeFild6 = " + String(writeFild6)  + ", writeFild7 = " + String(writeFild7) + ", writeFild8 = " + String(writeFild8));
    Serial.println("             channelIdRead= " + channelIdRead + ", readApiKey= " + readApiKey + ", thing1 = " + String(thing1) + ", thing2 = " + String(thing2) + ", thing3 = " + String(thing3) + ", thing4 = " + String(thing4) + ", thing5 = " + String(thing5));

  }
  saveConfig();
  server.send(200, "text/plain", "OK");
}
//======================================================================================================
void handle_message() {
  if(verific()) return;
  if(server.arg("text")!=""){
    server.send(200, "text/plain", "OK");
    String text = server.arg("text").c_str();
    for(int i = 0; i < 3; i++) {
      bip();
    }
    printStringWithShift(("        " + String(text) + "           ").c_str(), timeScrollSpeed, 1);
    if(printCom) {
      printTime();
      Serial.println(text);
    }
  }
}
//======================================================================================================
void handle_stopAlarm() {
  if(verific()) return;
  if(server.arg("stopAlarm")=="ok"){ 
    if(alarm_stat){
      stopAlarm = true;
      if(printCom) {
        printTime();
        Serial.println("STOP ALARM");
      }
      bip();
    }
  }
  server.send(200, "text/plain", "OK");
}
//======================================================================================================
void handle_resetConfig() {
  if(verific()) return;
  if(server.arg("device") == "ok"){
    //SPIFFS.format();
    SPIFFS.remove("/config.json");
    if(printCom) {
      printTime();
      Serial.println("ESP erase Config file");
    }
    delay(3000);
    server.send(200, "text/plain", "OK");
    delay(3000);
    bip();
    ESP.reset();
  }
}
//======================================================================================================
void handle_set_printCom(){
  if(verific()) return;
  if(server.arg("printCom")!=""){
    printCom = server.arg("printCom").toInt();
    if(printCom) {
      printTime();
      Serial.println("Set printCom: " + String(printCom));
    }
    saveConfig();
  }
  server.send(200, "text/plain", "OK"); 
}
//======================================================================================================
void handle_Restart() {
  if(verific()) return;
  if(server.arg("device") == "ok"){
    server.send(200, "text/plain", "OK"); 
    ESP.reset();
  }
}
//====================================== Тут функції для роботи з файловою системою
String getContentType(String filename){
  if (server.hasArg("download")) return "application/octet-stream";
  else if (filename.endsWith(".htm")) return "text/html";
  else if (filename.endsWith(".html")) return "text/html";
  else if (filename.endsWith(".vz")) return "text/html";
  else if (filename.endsWith(".json")) return "application/json";
  else if (filename.endsWith(".css")) return "text/css";
  else if (filename.endsWith(".js")) return "application/javascript";
  else if (filename.endsWith(".png")) return "image/png";
  else if (filename.endsWith(".gif")) return "image/gif";
  else if (filename.endsWith(".jpg")) return "image/jpeg";
  else if (filename.endsWith(".ico")) return "image/x-icon";
  else if (filename.endsWith(".xml")) return "text/xml";
  else if (filename.endsWith(".pdf")) return "application/x-pdf";
  else if (filename.endsWith(".zip")) return "application/x-zip";
  else if (filename.endsWith(".gz")) return "application/x-gzip";
  return "text/plain";
}
//======================================= Читання файлу
bool handleFileRead(String path){
  if (path.endsWith("/")) path += "index.htm";
  String contentType = getContentType(path);
  String pathWithGz = path + ".gz";
  if (SPIFFS.exists(pathWithGz) || SPIFFS.exists(path)) {
    if (SPIFFS.exists(pathWithGz))
      path += ".gz";
    File file = SPIFFS.open(path, "r");
    size_t sent = server.streamFile(file, contentType);
    file.close();
    return true;
  }
  return false;
}
//======================================== Завантаження файлу
void handleFileUpload(){
  if (server.uri() != "/edit") return;
  HTTPUpload& upload = server.upload();
  if (upload.status == UPLOAD_FILE_START) {
    String filename = upload.filename;
    if (!filename.startsWith("/")) filename = "/" + filename;
    fsUploadFile = SPIFFS.open(filename, "w");
    filename = String();
  } else if (upload.status == UPLOAD_FILE_WRITE) {
    if (fsUploadFile)
      fsUploadFile.write(upload.buf, upload.currentSize);
  } else if (upload.status == UPLOAD_FILE_END) {
    if (fsUploadFile)
      fsUploadFile.close();
  }
}
//======================================== Видалення файлу
void handleFileDelete(){
  if (server.args() == 0) return server.send(500, "text/plain", "BAD ARGS");
  String path = server.arg(0);
  if (path == "/")
    return server.send(500, "text/plain", "BAD PATH");
  if (!SPIFFS.exists(path))
    return server.send(404, "text/plain", "FileNotFound");
  SPIFFS.remove(path);
  server.send(200, "text/plain", "");
  path = String();
}
//========================================= Створення файлу
void handleFileCreate(){
  if (server.args() == 0)
    return server.send(500, "text/plain", "BAD ARGS");
  String path = server.arg(0);
  if (path == "/")
    return server.send(500, "text/plain", "BAD PATH");
  if (SPIFFS.exists(path))
    return server.send(500, "text/plain", "FILE EXISTS");
  File file = SPIFFS.open(path, "w");
  if (file)
    file.close();
  else
    return server.send(500, "text/plain", "CREATE FAILED");
  server.send(200, "text/plain", "");
  path = String();
}
//========================================== Список файлів 
void handleFileList(){
  if (!server.hasArg("dir")){
    server.send(500, "text/plain", "BAD ARGS");
    return;
  }
  String path = server.arg("dir");
  Dir dir = SPIFFS.openDir(path);
  path = String();
  String output = "[";
  while (dir.next()) {
    File entry = dir.openFile("r");
    if (output != "[") output += ',';
    bool isDir = false;
    output += "{\"type\":\"";
    output += (isDir) ? "dir" : "file";
    output += "\",\"name\":\"";
    output += String(entry.name()).substring(1);
    output += "\"}";
    entry.close();
  }
  output += "]";
  server.send(200, "text/json", output);
}
//==========================================
bool verific(){
  if(server.arg("auth")!=auth && authOn){
    server.send_P(404, "text/html", "404, you are not authorized!");
    return true;
  }
  return false;
}
