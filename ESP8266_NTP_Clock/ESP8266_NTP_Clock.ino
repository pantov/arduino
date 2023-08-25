#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>

#include <SPI.h>
#include <bitBangedSPI.h>
#include <MAX7219_Dot_Matrix.h>

#include "Arduino.h"
#include <ArduinoJson.h>

#include <String.h>
#include <NTPClient.h>
#include <Time.h>
#include <TimeLib.h>
#include <Timezone.h>

const byte chips = 4; //number of matrix in the display

// for NodeMCU 1.0
#define DIN_PIN 13  // 
#define CS_PIN  15  // 
#define CLK_PIN 14  // 

MAX7219_Dot_Matrix display (chips, CS_PIN, DIN_PIN, CLK_PIN);  // 2 chips, then specify the LOAD, DIN, CLK pins

// Define NTP properties
#define NTP_OFFSET   60 * 60      // In seconds
#define NTP_INTERVAL 60 * 1000    // In miliseconds
#define NTP_ADDRESS  "ca.pool.ntp.org"  // change this to whatever pool is closest (see ntp.org)

WiFiClient client;
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, NTP_ADDRESS, NTP_OFFSET, NTP_INTERVAL);


String date;
String t;
const char * days[] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"} ;
const char * months[] = {"Jan", "Feb", "Mar", "Apr", "May", "June", "July", "Aug", "Sep", "Oct", "Nov", "Dec"} ;
const char * ampm[] = {"AM", "PM"} ;

String weatherMain = "";
String weatherDescription = "";
String weatherLocation = "";
String country;
int humidity;
int pressure;
float temp;
float tempMin, tempMax;
int clouds;
float windSpeed;

int sunrise, sunset;

String currencyRates;
String weatherString;

const char* ssid = "sky";
const char* password = "1234paradize$";

String weatherKey = "eaf19aa8fc1d30a7f01e734f3e2382cf";
String weatherLang = "&lang=en";
String cityID = "lipetsk"; //Dhaka
// read OpenWeather api description for more info
// =======================================================================

char message1 [] = "This is a testing display. Designed by Md. Khairul Alam";
char weather_message[300];
char time_msg[50];
char date_msg[50];

int LED = 2;

void setup() {
  Serial.begin(115200);
  timeClient.begin();   // Start the NTP UDP client
  display.begin ();
  display.setIntensity (1);
  Serial.print("Connecting WiFi ");
  Serial.println("Booting");
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while (WiFi.waitForConnectResult() != WL_CONNECTED) {
    Serial.println("Connection Failed! Rebooting...");
    delay(5000);
    ESP.restart();
  }

  // Port defaults to 8266
  // ArduinoOTA.setPort(8266);

  // Hostname defaults to esp8266-[ChipID]
  // ArduinoOTA.setHostname("myesp8266");

  // No authentication by default
  // ArduinoOTA.setPassword("admin");

  // Password can be set with it's md5 value as well
  // MD5(admin) = 21232f297a57a5a743894a0e4a801fc3
  // ArduinoOTA.setPasswordHash("21232f297a57a5a743894a0e4a801fc3");

  ArduinoOTA.onStart([]() {
    String type;
    if (ArduinoOTA.getCommand() == U_FLASH)
      type = "sketch";
    else // U_SPIFFS
      type = "filesystem";

    // NOTE: if updating SPIFFS this would be the place to unmount SPIFFS using SPIFFS.end()
    Serial.println("Start updating " + type);
  });
  ArduinoOTA.onEnd([]() {
    Serial.println("\nEnd");
  });
  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
    Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
  });
  ArduinoOTA.onError([](ota_error_t error) {
    Serial.printf("Error[%u]: ", error);
    if (error == OTA_AUTH_ERROR) Serial.println("Auth Failed");
    else if (error == OTA_BEGIN_ERROR) Serial.println("Begin Failed");
    else if (error == OTA_CONNECT_ERROR) Serial.println("Connect Failed");
    else if (error == OTA_RECEIVE_ERROR) Serial.println("Receive Failed");
    else if (error == OTA_END_ERROR) Serial.println("End Failed");
  });
  ArduinoOTA.begin();
  Serial.println("Ready");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  pinMode(LED, OUTPUT);     // Initialize the LED_BUILTIN pin as an output

  getWeatherData();
  String str = weatherString;
  int str_len = str.length() + 1; 
  str.toCharArray(weather_message, str_len) ;   
}

unsigned long lastMoved = 0;
unsigned long MOVE_INTERVAL = 2;  // mS
int  messageOffset;

void updateDisplay (char *msg)
  {
  //String message = msg;
  display.sendSmooth (msg, messageOffset);
  
  // next time show one pixel onwards
  if (messageOffset++ >= (int) (strlen (msg) * 8))
    messageOffset = - chips * 8;
  }  // end of updateDisplay
  
long last = millis();
long lastTime = millis();
int count_sec = 0;

void loop() {
  ArduinoOTA.handle();

  if(millis()-lastTime>=1000){
    count_sec++;
    if(count_sec>70)
      count_sec = 0;
    lastTime = millis();
    }
  
  if((millis()- last)>1000*60*15){  //update weather every 15 minutes 
    getWeatherData();
    String str = weatherString;
    int str_len = str.length() + 1;
    str.toCharArray(weather_message, str_len) ;
    last = millis();
   }
   
  // update display if time is up
  if(count_sec>0 && count_sec<50){
  if (millis () - lastMoved >= MOVE_INTERVAL)
    {
      updateDisplay(weather_message);
      lastMoved = millis ();
    }  
  }

  if(count_sec>50 && count_sec<60){
  read_time_date();
  String s = date;
  int str_len = s.length() + 1; 
  s.toCharArray(date_msg, str_len) ; 
  if (millis () - lastMoved >= MOVE_INTERVAL)
    {
      updateDisplay(date_msg);
      lastMoved = millis ();
    }  
  }
  if(count_sec>60){
      read_time_date();
      String ti = t;
      int str_len = ti.length() + 1; 
      ti.toCharArray(time_msg, str_len) ; 
      if (millis () - lastMoved >= MOVE_INTERVAL)
      {
      updateDisplay(time_msg);
      lastMoved = millis ();
      } 
    }   
}

// =======================================================================
// retrive weather data

const char *weatherHost = "api.openweathermap.org";

void getWeatherData()
{
  Serial.print("connecting to "); Serial.println(weatherHost);
  if (client.connect(weatherHost, 80)) {
    client.println(String("GET /data/2.5/weather?q=") + cityID + "&appid=" + weatherKey + "\r\n" +
                "Host: " + weatherHost + "\r\nUser-Agent: ArduinoWiFi/1.1\r\n" +
                "Connection: close\r\n\r\n");
  } else {
    Serial.println("connection failed");
    return;
  }
  String line;
  int repeatCounter = 0;
  while (!client.available() && repeatCounter < 10) {
    delay(500);
    Serial.println("w.");
    repeatCounter++;
  }
  while (client.connected() && client.available()) {
    char c = client.read(); 
    if (c == '[' || c == ']') c = ' ';
    line += c;
  }

  client.stop();

  DynamicJsonBuffer jsonBuf;
  JsonObject &root = jsonBuf.parseObject(line);
  if (!root.success())
  {
    Serial.println("parseObject() failed");
    return;
  }
  //weatherMain = root["weather"]["main"].as<String>();
  weatherDescription = root["weather"]["description"].as<String>();
  weatherDescription.toLowerCase();
  Serial.println(weatherDescription);
  //  weatherLocation = root["name"].as<String>();
  //  country = root["sys"]["country"].as<String>();
  sunrise = root["sys"]["sunrise"];
  sunset = root["sys"]["sunset"];
  temp = root["main"]["temp"];
  humidity = root["main"]["humidity"];
  pressure = root["main"]["pressure"];
  tempMin = root["main"]["temp_min"];
  tempMax = root["main"]["temp_max"];
  windSpeed = root["wind"]["speed"];
  clouds = root["clouds"]["all"];
  //String deg = String(char('~')+131);
  //weatherString = "  Temp: " + String(temp,1) /*+ deg*/ + "C (" + String(tempMin,1) /*+ deg*/ + "-" + String(tempMax,1) /*+ deg*/ + ")  ";
  weatherString = "  Temp: " + String(temp,1) /*+ deg*/ + "C";
  //weatherString += weatherDescription;
  weatherString += "  Humidity: " + String(humidity) + "% ";
  weatherString += "  Pressure: " + String(pressure) + "hPa ";
  weatherString += "  Clouds: " + String(clouds) + "%  ";
  weatherString += "  Wind: " + String(windSpeed,1) + "m/s   ";
  Serial.println(weatherString);
}

// =======================================================================
// retrive curency rate

const char* currencyHost = "cinkciarz.pl";

void getCurrencyRates()
{
  WiFiClientSecure client;
  Serial.print("connecting to "); Serial.println(currencyHost);
  if (!client.connect(currencyHost, 443)) {
    Serial.println("connection failed");
    return;
  }
  client.print(String("GET / HTTP/1.1\r\n") +
               "Host: " + currencyHost + "\r\nConnection: close\r\n\r\n");

  //Serial.print("request sent");
  int repeatCounter = 0;
  while (!client.available() && repeatCounter < 10) {
    delay(500);
    Serial.println("c.");
    repeatCounter++;
  }
  Serial.println("connected");
  while (client.connected() && client.available()) {
    String line = client.readStringUntil('\n');
    //      Serial.println(line);
    int currIdx = line.indexOf("/kantor/kursy-walut-cinkciarz-pl/usd");
    if (currIdx > 0) {
      String curr = line.substring(currIdx + 33, currIdx + 33 + 3);
      curr.toUpperCase();
      line = client.readStringUntil('\n');
      int rateIdx = line.indexOf("\">");
      if (rateIdx <= 0) {
        Serial.println("Found rate but wrong structure!");
        return;
      }
      currencyRates = "        PLN/" + curr + ": ";
      if (line[rateIdx - 1] == 'n') currencyRates += char('~'+24); else currencyRates += char('~'+23); // down/up
      currencyRates += line.substring(rateIdx + 2, rateIdx + 8) + " ";

      line = client.readStringUntil('\n');
      rateIdx = line.indexOf("\">");
      if (rateIdx <= 0) {
        Serial.println("Found rate but wrong structure!");
        return;
      }
      if (line[rateIdx - 1] == 'n') currencyRates += char('~'+24); else currencyRates += char('~'+23); // down/up
      currencyRates += line.substring(rateIdx + 2, rateIdx + 8);
      currencyRates.replace(',', '.');
      break;
    }
  }
  client.stop();
}

// =======================================================================

void read_time_date(){
    date = "";  // clear the variables
    t = "";
    
    // update the NTP client and get the UNIX UTC timestamp 
    timeClient.update();
    unsigned long epochTime =  timeClient.getEpochTime();

    // convert received time stamp to time_t object
    time_t local, utc;
    utc = epochTime;

    // Then convert the UTC UNIX timestamp to local time
    TimeChangeRule usEDT = {"EDT", Second, Sun, Mar, 2, +180};  //UTC - 5 hours - change this as needed
    TimeChangeRule usEST = {"EST", First, Sun, Nov, 2, +180};   //UTC - 6 hours - change this as needed
    Timezone usEastern(usEDT, usEST);
    local = usEastern.toLocal(utc);

    // now format the Time variables into strings with proper names for month, day etc
    date += days[weekday(local)-1];
    date += ", ";
    date += months[month(local)-1];
    date += " ";
    date += day(local);
    date += ", ";
    date += year(local);

    // format the time to 12-hour format with AM/PM and no seconds
    t += hourFormat12(local);
    t += ":";
    if(minute(local) < 10)  // add a zero if minute is under 10
      t += "0";
    t += minute(local);
    t += " ";
    t += ampm[isPM(local)];
    Serial.println(date);
    Serial.println(t);
  }
