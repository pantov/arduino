void var() {
  //Системные
  crm.led_conf(PN_LED, LOW, true);                //Информационный индикатор, Порт, Начальное значение, Инверсия значения
#ifdef WORK
  crm.var(F("SSID"), F("___"));
  crm.var(F("Pass"), F("___"));
  crm.var(F("WiFi_Mode"), F("STA"));              //Режим по умолчанию, (STA, AP).
#else
  crm.var(F("SSID"), F(""));
  crm.var(F("Pass"), F(""));
  crm.var(F("WiFi_Mode"), F("AP"));               //Режим по умолчанию, (STA, AP).
#endif
  crm.var(F("WTCon"), F("120"));                  //Время ожидания подключения к WiFi, сек.
  crm.var(F("AP_SSID"), F("CRM-Rele"));           //Имя точки доступа, она же идентификатор для OTA.
  crm.var(F("AP_Pass"), F("0000-0000"));          //Пароль точки доступа.
  //Пользовательские
  crm.var(F("TZ"), F("3"));
  crm.var(F("TSinc"), F("6"));
  crm.var(F("DaTi"), F(""));
  crm.var(F("rfCode"), F("0"));
  crm.var(F("useBThw"), F("true"));
  crm.var(F("Sens"), F("1"));
  crm.var(F("TRSens"), F("5"));
  crm.var(F("useTmr"), F("false"));
  crm.var(F("Tmr"), F("3600"));
  crm.var(F("CorTS"), F("0"));
  crm.var(F("CorHS"), F("0"));
  crm.var(F("Ctrl"), F("0"));
  crm.var(F("POn"), F("-30"));
  crm.var(F("PDn"), F("110"));
  crm.var(F("useT1"), F("false"));
  crm.var(F("T1On"), F(""));
  crm.var(F("T1Off"), F(""));
  crm.var(F("useT2"), F("false"));
  crm.var(F("T2On"), F(""));
  crm.var(F("T2Off"), F(""));
  crm.var(F("useT3"), F("false"));
  crm.var(F("T3On"), F(""));
  crm.var(F("T3Off"), F(""));

  crm.ui(interface);                      //Интерфейс UI. Обязательно
  crm.aliveArray(adata);                  //Динамические данные на странице. Обязательно
  crm.begin();                            //Старт Фреймворка
}
