#include "CRMui.h"
//#include "CRM_Time.h"
#include <GyverButton.h>
#include <RCSwitch.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include "SDHT.h"



//#define WORK
#define DEBUG
#define VER "2.0.0225a.263"


#ifdef DEBUG
#define PN_LED 16
#define PIN_SENSOR 4
#else
#define PN_LED 13
#define PIN_SENSOR 3
#endif
#define PN_RELE 12
#define PN_RECIVER 14
#define PN_BUTTON 0


CRMui crm;
GButton BT(PN_BUTTON);
RCSwitch mySwitch = RCSwitch();
OneWire oneWire(PIN_SENSOR);
DallasTemperature sensors(&oneWire);
SDHT dht;




uint32_t lstcd;
uint32_t SecTimer, SensTimer, POffTimer;
uint16_t TimeRefreshSens;
float T;
float H;
bool PowerStTimer, PinStat;
bool dsInit, dht11Init, dht22Init;


void setup() {
#ifdef DEBUG
  Serial.begin(115200);
#endif
  var();
  pinMode(PN_RELE, OUTPUT);
  digitalWrite(PN_RELE, LOW);
  BT.setType(HIGH_PULL);
  BT.setDirection(NORM_OPEN);
  BT.setTimeout(5000);
  BT.setTickMode(AUTO);
  mySwitch.enableReceive(PN_RECIVER);

  time_conf(crm.param(F("TZ")).toInt(), crm.param(F("TSinc")).toInt());
}


void loop() {
  crm.handle();
  crm.btnCallback(F("bt_rbt"), Rbt);
  crm.btnCallback(F("bt_sw"), RSw);
  crm.btnCallback(F("bt_Set"), SetDT);
  crm.btnCallback(F("bt_SettRst"), settings_reset);
  if (BT.isClick()) if (crm.param(F("useBThw")) == F("true")) RSw();
  if (BT.isHolded()) settings_reset();
  RF();

  if (millis() - SecTimer >= 1000) {
    SecTimer = millis();

    if (digitalRead(PN_RELE) != PinStat) {
      PinStat = digitalRead(PN_RELE);
      crm.led(PinStat);
    }

    //Опрос датчиков
    if (millis() - SensTimer >= TimeRefreshSens) {
      SensTimer = millis();
      TimeRefreshSens = crm.param(F("TRSens")).toInt() * 1000UL;
      byte dtchl = crm.param(F("Sens")).toInt();
      if (dtchl == 3) {
        DS18B20();                                                        //6 мсек
        if (crm.param(F("Ctrl")) == F("1")) {
          if (crm.param(F("POn")).toFloat() > T && !rST()) RPUp();
          else if (crm.param(F("PDn")).toFloat() < T && rST()) RPDown();
        }
      } else if (dtchl == 2 || dtchl == 1) {
        DHTsens(dtchl);                                                  //23 мсек
        if (crm.param(F("Ctrl")) == F("1")) {
          if (crm.param(F("POn")).toFloat() > T && !rST()) RPUp();
          else if (crm.param(F("PDn")).toFloat() < T && rST()) RPDown();
        } else if (crm.param(F("Ctrl")) == F("2")) {
          if (crm.param(F("POn")).toFloat() > H && !rST()) RPUp();
          else if (crm.param(F("PDn")).toFloat() < H && rST()) RPDown();
        }
      }
    }

    //Таймер выключения
    if (millis() - POffTimer >= 1000UL * crm.param(F("Tmr")).toInt() && PowerStTimer) {
      PowerStTimer = false;
      RPDown();
    }

    //Расписание
    if (crm.param(F("Ctrl")) == F("0") && (crm.param(F("useT1")) == F("true") || crm.param(F("useT2")) == F("true") || crm.param(F("useT3")) == F("true"))) {
      static bool PowerSt;
      String CurrentTime = String(hour()) + F(":") + String(minute());
      if (!PowerSt && (crm.param(F("T1On")) == CurrentTime || crm.param(F("T2On")) == CurrentTime || crm.param(F("T3On")) == CurrentTime)) {
        PowerSt = true;
        RPUp();
      }
      if (PowerSt && (crm.param(F("T1Off")) == CurrentTime || crm.param(F("T2Off")) == CurrentTime || crm.param(F("T3Off")) == CurrentTime)) {
        PowerSt = false;
        RPDown();
      }
    }

  }

}


void RF() {
  if (mySwitch.available()) {
    lstcd = mySwitch.getReceivedValue();
#ifdef DEBUG
    Serial.println(lstcd);
#endif
    static uint32_t rf_timer;
    if (millis() - rf_timer >= 400)
      if (lstcd == crm.param(F("rfCode")).toInt() && lstcd > 0) RSw();
    rf_timer = millis();
    mySwitch.resetAvailable();
  }
}


void DS18B20() {
  if (!dsInit) {
    dsInit = true;
    dht22Init = dht11Init = false;
    pinMode(PIN_SENSOR, INPUT_PULLUP);                //Работает на любом пине
    sensors.setWaitForConversion(false);              //Параметр без задержки. Обязательно должна быть реализована таймером.
    sensors.begin();
    sensors.requestTemperatures();
    return;
  }
  T = sensors.getTempCByIndex(0);
  if (T != 404) T += crm.param(F("CorTS")).toFloat();
  sensors.requestTemperatures();
}


void DHTsens(byte dtchl) {
  if (dsInit) dsInit = false;

  if (dtchl == 1) {
    if (dht.read(DHT11, PIN_SENSOR)) {
      T = double(dht.celsius) * 0.1;
      H = double(dht.humidity) * 0.1;
    } else {
      T = 404;
      return;
    }
  } else if (dtchl == 2) {
    if (dht.read(DHT22, PIN_SENSOR)) {
      T = double(dht.celsius) * 0.1;
      H = double(dht.humidity) * 0.1;
    } else {
      T = 404;
      return;
    }
  }

  T += crm.param(F("CorTS")).toFloat();
  H += crm.param(F("CorHS")).toFloat();
}


void Rbt() {
  crm.reboot();
}


void RSw() {
  bool r_st = !digitalRead(PN_RELE);
  digitalWrite(PN_RELE, r_st);
  crm.led(r_st);
  if (r_st && crm.param(F("useTmr")) == F("true") && crm.param(F("Ctrl")) == F("0") && crm.param(F("useT1")) == F("false") && crm.param(F("useT2")) == F("false") && crm.param(F("useT3")) == F("false")) {
    POffTimer = millis();
    PowerStTimer = true;
  }
#ifdef DEBUG
  Serial.println(String(F("Реле ")) + String(r_st ? F("ВКЛ") : F("ВЫКЛ")));
#endif
}


void RPUp() {
  digitalWrite(PN_RELE, HIGH);
  crm.led(1);
#ifdef DEBUG
  Serial.println(F("Реле ВКЛ"));
#endif
}


void RPDown() {
  digitalWrite(PN_RELE, LOW);
  crm.led(0);
#ifdef DEBUG
  Serial.println(F("Реле ВЫКЛ"));
#endif
}

bool rST() {
  return digitalRead(PN_RELE);
}

void SetDT() {
  setTm(crm.param(F("DaTi")));
}

void settings_reset() {
  crm.reset_cfg();
}
