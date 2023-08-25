#include "CRMui.h"
#include <ESP8266HTTPClient.h>
#include <WiFiClientSecureBearSSL.h>
//#include <UniversalTelegramBot.h>
#define LED 2     //Светодиод на плате, муляж устройства.
#include <OneWire.h>
#include <DallasTemperature.h>

// линия данных DS18B20
#define ONE_WIRE_BUS 2
// линия данных Геркон
#define DOOR_BUS 4
// настройка объекта oneWire для связи с любым устройством OneWire
OneWire oneWire(ONE_WIRE_BUS);  
// передать ссылку на oneWire библиотеке DallasTemperature
DallasTemperature sensors(&oneWire);

CRMui crm;
bool DBG = true;
float Temperature;
long cnt;         //тестовая переменная
bool checkbox_1;
bool Sendm = false;
int Hcontrol  = 0; // Охрана/   вкл 1 выкл 0 тревога 2
bool SendDvij = false;
bool SendDoor = false;
bool SendTmprost = false;
bool SendTmpspad = false;

void setup() {
  Serial.begin(115200);
  sensors.begin();  // запустить библиотеку
 
  crm.led_conf(LED, LOW, false);               //LED индикация (Порт, Начальное значение, Инверсия значения)
  pinMode(DOOR_BUS, INPUT_PULLUP);
  //СИСТЕМНЫЕ ПЕРЕМЕННЫЕ, ID - CONST. При отсутствии создаются автоматом с дефолтными параметрами.
  crm.var(F("mWiFi"), F("STA"));             //Режим по умолчанию, (STA, AP, AP_STA).
  crm.var(F("SSID"), F("sky"));              //Имя сети к которой подключаемся. Если не задан запускается режим AP.
  crm.var(F("Pass"), F("1234paradize$"));         //Пароль от подключаемой сети.
  crm.var(F("WTCon"), F("120"));             //Время ожидания подключения к WiFi, сек.
  crm.var(F("AP_SSID"), F("CRMui"));         //Имя точки доступа, она же идентификатор для OTA.
  crm.var(F("AP_Pass"), F(""));              //Пароль точки доступа.

  //ПОЛЬЗОВАТЕЛЬСКИЕ ПЕРЕМЕННЫЕ
  crm.var(F("HAU"), F("admin"));
  crm.var(F("HAP"), F("admin"));
  crm.var(F("TEXT"), F("Текст текст текст"));
  crm.var(F("password_input"), F("Pass-1234"));
  crm.var(F("number_input"), F("5645454"));
  crm.var(F("time_input"), F("13:45"));
  crm.var(F("dt"), F("2019-09-20"));
  crm.var(F("dtat"), F("2019-09-20T00:01"));
  crm.var(F("email_input"), F("my-email@bk.ru"));
  crm.var(F("range_input"), F("45"));
  crm.var(F("select_input"), F("1"));
  crm.var(F("checkbox_1"), F("true"));
  crm.var(F("color_input"), F("#ff00ff"));
  crm.var(F("textarea_tlg"), F("Просто не учи физику в школе и вся твоя жизнь будет наполнена чудесами, и волшебством."));


  //ОБЯЗАТЕЛЬНЫЕ МЕТОДЫ
  crm.ui(interface);          //Интерфейс UI. Обязательно
  crm.getRequest(dataReq);    //GET запросы. Вида:   http://IP/data?ПЕРЕМЕННАЯ=ЗНАЧЕНИЕ&П2=З2 и т.д.
  crm.aliveArray(adata);      //Динамические данные на странице. Обязательно
  crm.update(update_vol);      //Выполнение функции, при обновлении переменных через Web
  crm.begin();                //Старт Фреймворка

  update_vol();
}


//Выполнение при обновлении переменных через Web
void update_vol() {
  checkbox_1 = crm.param(F("checkbox_1")) == F("true") ? true : false;
  crm.setWebAut(crm.param(F("HAU")), crm.param(F("HAP")));    //Авторизация в Web интерфейсе
  Serial.println(String(F("Использовать подсветку: ")) + (checkbox_1 ? F("ДА") : F("НЕТ")));
  Serial.println("Обновление переменных");
}


void loop() {
  
  crm.handle();
 // crm.btnCallback(F("btn_print"), test_print);
 // crm.btnCallback(F("btn_invert"), test_invert);
 // crm.btnCallback(F("btn_reboot"), rbt);
 // crm.btnCallback(F("btn_reset"), rst);
  crm.btnCallback(F("btn_send_message"), send_message);
  crm.btnCallback(F("btn_ohrana0"), ohrana0);
  crm.btnCallback(F("btn_ohrana1"), ohrana1);
 // if (Sendm) { send_message2();}
 
 sensors.requestTemperatures(); 
 Temperature=sensors.getTempCByIndex(0);
 Obzor();
}


//Обработка GET запроса
void dataReq() {
  Serial.println(crm.param_get(F("text")));  //Вывод в терминал значения переменной "text", для запроса: http://IP/data?text=Тестовое сообщение

  //Возврат ответа в Web
  if (crm.param_get(F("stat")) != F("null")) crm.getResponse(crm.time_work());
  else crm.getResponse(String(millis()));
}


void interface() {
  crm.app_name("Мониторинг");         //Название устройства/проекта, отображается в меню.

  //Разделы меню
  crm.menu(F("Главная"));             //Пункт 1
  crm.menu(F("События"));             //Пункт 1
  crm.menu(F("Настройки"));           //Пункт 2
  crm.menu(F("Сообщения"));           //Пункт 3 
  crm.menu(F("WiFi"));      //Пункт 4
  crm.page();

  //Соответствует 1 пунку меню
  crm.alive_tabl(F("AHcontrol"), F("Режим охраны"));
  crm.button(F("btn_ohrana0"), F("Снять с охраны"), false);
  crm.button(F("btn_ohrana1"), F("Поставить на охрану"));
  crm.hr();
  crm.alive_tabl(F("Ddvij"), F("Датчик движения"));
  crm.alive_tabl(F("Ddoor"), F("Датчик двери"));
  crm.alive_tabl(F("Aread"), F("Температура"));
  crm.hr();
 // crm.button(F("btn_print"), F("Печать в терминал"), false);
 // crm.button(F("btn_invert"), F("Переключить LED"));
 // crm.button(F("btn_reboot"), F("&#8634; Перезагрузить"));
  crm.page();
  crm.alive_tabl(F("Aread"), F("Температура сейчас"));
  crm.hr();
  crm.checkbox(F("checkbox_tmprost"), F("Контроль повышения температуры"));
  crm.range(F("range_tmprost"), 0, 100, 0.1, F("Ползунок с шагом"), F("°C"));
  crm.hr();
  crm.checkbox(F("checkbox_tmpspad"), F("Контроль понижения температуры"));
  crm.range(F("range_tmpspad"), 0, 100, 0.1, F("Ползунок с шагом"), F("°C"));  
  crm.page();
  //Соответствует 2 пунку меню
 // crm.text(F("TEXT"), F("Какой то текст"));
 // crm.password(F("password_input"), F("Пароль от всего"));
 // crm.number(F("number_input"), F("Только числа"));
 // crm.time(F("time_input"), F("Будильник"));
 // crm.date(F("dt"), F("Дата мероприятия"));
 // crm.datetime(F("dtat"), F("Дата и время включения"));
 // crm.email(F("email_input"), F("Адрес электронки"));
 // crm.range(F("range_input"), 0, 100, 0.1, F("Ползунок с шагом"), F("°C"));
 // crm.option(F("Комедии"), F("0"));
 // crm.option(F("Боевики"), F("1"));
 // crm.option(F("Ужасы"), F("2"));
 // crm.select(F("select_input"), F("Что любишь?"));
//  crm.color(F("color_input"), F("Цвет подсветки ОЗУ"));
 // crm.checkbox(F("checkbox_1"), F("Использовать подсветку"));
 // crm.checkbox(F("checkbox_2"), F("Авто режим"));
 // crm.checkbox(F("checkbox_3"), F("Использовать LED индикацию"));
 // crm.br();     //Строчный интервал, размер в пикселях. По умолчанию = 10
 // crm.textarea(F("textarea_input"), F("Текстовое поле для большого ввода текста прямо в него"));
  crm.alive_tabl(F("TimeWork"), F("Время работы"));
  crm.alive_tabl(F("Dev_IP"), F("IP адрес"), WiFi.localIP().toString());
  crm.alive_tabl(F("Dev_MAC"), F("MAC адрес"), String(WiFi.macAddress()));
  crm.alive_tabl(F("WFP"), F("Мощность WiFi"));
  crm.alive_tabl(F("RAM"), F("Свободно ОЗУ"));
 // crm.alive_tabl(F("ledst"), F("Состояние LED"));
 // crm.alive_tabl(F("PrintTerm"), F("Количество нажатий"));
  crm.hr();
  crm.text(F("HAU"), F("WEB LOGIN"));
  crm.password(F("HAP"), F("WEB PASSWORD"));
 // crm.button(F("btn_reset"), F("Reset settings"));
  crm.page();
  //Соответствует 3 пунку меню
  crm.checkbox(F("checkbox_tlgsend"), F("Отправлять сообщения в Телеграмм"));
  crm.text(F("Bot_id"), F("ID телеграмм бота"));
  crm.text(F("Chat_id"), F("ID получателя сообщений"));
  crm.hr();
  crm.textarea(F("textarea_tlg"), F("Тестовое сообщение для проверки:"));
  crm.button(F("btn_send_message"), F("Отправить тестовое сообщение"));
  crm.page();
  
  //Соответствует 4 пунку меню
  crm.wifi_page();
  crm.hr();
//  crm.button(F("btn_reboot"), F("&#128153; Перезагрузить"));
  crm.page();

  //И т.д.
}


//Выполняется по запросу обрадного вызова.
void adata() {
  crm.aliveData_tabl (F("TimeWork"), crm.time_work(), F("0f0"));    //Вставка данных по ID для crm.alive_tabl (ID, ДАННЫЕ, ЦВЕТ)
  int wfrssi = WiFi.RSSI();
  crm.aliveData_tabl (F("WFP"), String(wfrssi) + F(" dBm   (") + String(map(wfrssi, -100, -40, 0, 100)) + F(" %)"));
  crm.aliveData_tabl (F("RAM"), String(ESP.getFreeHeap()) + F(" Бт"), crm.param(F("color_input")));
  !digitalRead(LED) ? crm.aliveData_tabl (F("ledst"), F("ВКЛ"), F("f00")) : crm.aliveData_tabl (F("ledst"), F("ВЫКЛ"), F("0f0"));
  crm.aliveData_tabl (F("PrintTerm"), String(cnt));
  crm.alive_tabl(F("TimeWork"), F("Время работы"));
 // crm.aliveData_tabl (F("Aread"), String(analogRead(A0))+" = "+ crm.param(F("range_input"))+" = "+ Temperature  );
  crm.aliveData_tabl (F("Aread"),  String(Temperature)+F(" °C")  );
  Hcontrol==0 ? crm.aliveData_tabl (F("AHcontrol"), F("Отключено"), F("f00")) : crm.aliveData_tabl (F("AHcontrol"), F("ОХРАНА"), F("0f0"));
  analogRead(A0)<500 ? crm.aliveData_tabl (F("Ddvij"), F("Норма"), F("0f0")) : crm.aliveData_tabl (F("Ddvij"), F("Тревога"), F("f00"));
  digitalRead(DOOR_BUS)==1 ?  crm.aliveData_tabl (F("Ddoor"), F("Тревога"), F("f00")) : crm.aliveData_tabl (F("Ddoor"), F("Норма"), F("0f0"));
  
}


void rbt() {
  crm.reboot();       // Перезагрузка ESP
}

void rst() {
  crm.reset_cfg();       // Перезагрузка ESP
}


//************************************ Т Е С Т   Ф У Н К Ц И И ************************************
void ohrana0() {
send_all("Снято с охраны!");
Hcontrol  = 0;
SendDvij = true; SendDoor = true;
//SendTmprost = true;
//SendTmpspad = true;
Serial.println("Снято с охраны!");
}
void ohrana1() {
send_all("Взято под охрану!");
Hcontrol  = 1;
SendDvij = false; SendDoor = false;
SendTmprost = false;
SendTmpspad = false;
Serial.println("Взято под охрану!");
}
void Obzor() {
    // Serial.println(String(crm.param(F("checkbox_tmprost"))));
   
if (Hcontrol==1) {
    if ((analogRead(A0)>500) and (SendDvij == false))  {
      send_all("Тревога - сработал датчик движения!!!");
      SendDvij = true;
    }
    if ((digitalRead(DOOR_BUS)==1)and(SendDoor == false)) {

      send_all("Тревога - сработал датчик открытия двери!!!");
      SendDoor = true;
    }

    //if  (Temperature  crm.param(F("range_tmprost")).toFloat()) and {
   // Serial.println("tmp");
   // Serial.println(crm.param(F("range_tmprost")).toFloat());
    //}
      if ((String(crm.param(F("checkbox_tmprost")))=="true") and (SendTmprost == false) and (Temperature > crm.param(F("range_tmprost")).toFloat()) ) {
      send_all("Температура превысила пороговое значение");
      SendTmprost = true;
      SendTmpspad = false;
       }
      if ((String(crm.param(F("checkbox_tmpspad")))=="true") and (SendTmpspad == false) and (Temperature < crm.param(F("range_tmpspad")).toFloat())) {
      send_all("Температура стала ниже порогового значения");
      SendTmprost = false;
      SendTmpspad = true;
      }
     }
}
void test_print() {
  Serial.println(String(F("Количество нажатий: ")) + String(++cnt));
 //Hcontrol=1;
 //Serial.println(crm.param(F("checkbox_tlgsend")));
// gercon = digitalRead(DOOR_BUS);
// Serial.println(gercon); 
}
void send_message2() {

}
void send_message() {
//
send_all(crm.param(F("textarea_tlg")));
// 
}
void send_all(String msg) {
//
    if (String(crm.param(F("checkbox_tlgsend")))=="true") {
    HTTPClient http;  //Declare an object of class HTTPClient
    String Urls2=String("http://pantov.cf/tlg2.php?bot_id="+crm.param(F("Bot_id"))+"&chat_id="+crm.param(F("Chat_id"))+"&msg="+msg); 
    String Urls=String("http://pantov.cf/tlg.php?msg="+msg); 
    Urls2.replace(" ","%20");
    Serial.println(Urls2);
    http.begin(Urls2);  //Specify request destination
    int httpCode = http.GET();                                  //Send the request
    if (httpCode > 0) { //Check the returning code
       String payload = http.getString();   //Get the request response payload
       Serial.println(payload);             //Print the response payload
    }
    http.end();   //Close connection
    }
// 
}


void test_invert() {
  crm.led_use((crm.param(F("checkbox_3")) == F("true") ? true : false));
  digitalWrite(LED, !digitalRead(LED));
  Serial.println(String(F("LED: ")) + String(digitalRead(LED) ? F("ВЫКЛ") : F("ВКЛ")));
  //Hcontrol=0;
}
