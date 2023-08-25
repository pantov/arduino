void interface() {
  crm.app_name(F("CRM-RELE"));

  //Разделы меню
  crm.menu(F("Главная"));
  crm.menu(F("Опции"));
  crm.menu(F("Настройки"));
  crm.menu(F("WiFi"));
  crm.page();


  //Главная
  crm.alive_tabl(F("cT"), F("Время"));
  crm.alive_tabl(F("cD"), F("Дата"));
  crm.hr();
  String dtch = crm.param(F("Sens"));
  if (dtch == F("3") || dtch == F("2") || dtch == F("1")) {
    crm.alive_tabl(F("Temp"), F("Температура"));
    if (dtch == F("2") || dtch == F("1")) crm.alive_tabl(F("Hum"), F("Влажность"));
    crm.br();
  }
  crm.alive_tabl(F("Rele"), F("Реле"));
  crm.button(F("bt_sw"), F("ПЕРЕКЛЮЧИТЬ"));
  crm.hr();
  crm.alive_tabl(F("tWork"), F("Время работы"));
  crm.alive_tabl(F("WFP"), F("Мощность WiFi"));
  crm.alive_tabl(F("RAM"), F("Свободно ОЗУ"));
  crm.alive_tabl(F("ver"), F("Версия FW"), String(VER));
  crm.page();


  //Опции
  crm.alive_tabl(F("lstcd"), F("Код пульта"));
  crm.number(F("rfCode"), F("Код кнопки ДУ"));
  crm.checkbox(F("useBThw"), F("Кнопка на модуле"));
  crm.hr();
  crm.option(F("Не использовать"), F("0"));
  crm.option(F("DS18B20"), F("3"));
  crm.option(F("DHT22"), F("2"));
  crm.option(F("DHT11 (высокая погрешность)"), F("1"));
  crm.select(F("Sens"), F("Внешний датчик"));
  crm.range(F("TRSens"), 1, 120, 1, F("Частота опроса датчика"), F("сек."));
  crm.range(F("CorTS"), -5, 5, 0.1, F("Подстройка температуры"), F("°C"));
  crm.range(F("CorHS"), -10, 10, 1, F("Подстройка влажности"), F("%"));
  crm.option(F("Не использовать"), F("0"));
  crm.option(F("Температуры"), F("1"));
  crm.option(F("Влажности"), F("2"));
  crm.select(F("Ctrl"), F("Контроль"));
  crm.range(F("POn"), -30, 110, 0.1, F("ВКЛ при значении &#11015;"));
  crm.range(F("PDn"), -30, 110, 0.1, F("ВЫКЛ при значении &#11014;"));
  crm.hr();
  crm.checkbox(F("useT1"), F("Расписание 1"));
  crm.time("T1On", "ВКЛ в");
  crm.time("T1Off", "ВЫКЛ в");
  crm.br();
  crm.checkbox(F("useT2"), F("Расписание 2"));
  crm.time("T2On", "ВКЛ в");
  crm.time("T2Off", "ВЫКЛ в");
  crm.br();
  crm.checkbox(F("useT3"), F("Расписание 3"));
  crm.time("T3On", "ВКЛ в");
  crm.time("T3Off", "ВЫКЛ в");
  crm.hr();
  crm.checkbox(F("useTmr"), F("Выключать по таймеру"));
  crm.number(F("Tmr"), F("Таймер в сек."));
  crm.page();


  //Настройки
  crm.range(F("TZ"), -12, 12, 1, F("Часовой пояс"), F("ч."));
  crm.range(F("TSinc"), 6, 48, 1, F("Частота синхронизации с сервером"), F("ч."));
  crm.datetime(F("DaTi"), F("Дата и время"));
  crm.button(F("bt_Set"), F("Задать"));
  crm.hr();
  crm.button(F("bt_SettRst"), F("Сбросить все настроки"), false);
  crm.button(F("bt_rbt"), F("&#8634; Перезагрузить"));
  crm.page();


  //WiFi
  crm.wifi_page();
  crm.hr();
  crm.button(F("bt_rbt"), F("&#8634; Перезагрузить"));
  crm.page();
}


//Выполняется по запросу обрадного вызова.
void adata() {
  crm.aliveData_tabl (F("cT"), curTime());
  crm.aliveData_tabl (F("cD"), curDate());
  crm.aliveData_tabl (F("tWork"), crm.time_work());
  int wfrssi = WiFi.RSSI();
  if (wfrssi < 0) crm.aliveData_tabl (F("WFP"), String(wfrssi) + F(" dBm   (") + String(map(wfrssi, -100, -40, 0, 100)) + F(" %)"));
  else crm.aliveData_tabl (F("WFP"), String(wfrssi) + F(" dBm"));
  crm.aliveData_tabl (F("RAM"), String(ESP.getFreeHeap()) + F(" Бт"));
  digitalRead(PN_RELE) ? crm.aliveData_tabl (F("Rele"), F("ВКЛ"), F("f00")) : crm.aliveData_tabl (F("Rele"), F("ВЫКЛ"), F("0f0"));
  crm.aliveData_tabl (F("lstcd"), lstcd ? String(lstcd) : F("x"));

  String dtch = crm.param(F("Sens"));
  if (dtch == F("3")) {
    crm.aliveData_tabl (F("Temp"), String(T, 1) + (crm.param(F("Ctrl")) == F("1") ? String(F(" °C   (")) + crm.param(F("POn")) + F(" - ") + crm.param(F("PDn")) + F(")") : String(F(" °C"))), F("ff0"));
  } else if (dtch == F("2") || dtch == F("1")) {
    crm.aliveData_tabl (F("Temp"), String(T, 1) + (crm.param(F("Ctrl")) == F("1") ? String(F(" °C   (")) + crm.param(F("POn")) + F(" - ") + crm.param(F("PDn")) + F(")") : String(F(" °C"))), F("ff0"));
    crm.aliveData_tabl (F("Hum"), String(H, 1) + (crm.param(F("Ctrl")) == F("2") ? String(F(" %   (")) + crm.param(F("POn")) + F(" - ") + crm.param(F("PDn")) + F(")") : String(F(" %"))), F("ff0"));
  }
}
