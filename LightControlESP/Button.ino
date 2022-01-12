bool firstRun = true;
bool LedStatus_1 = false;
bool LedStatus_2 = false;
uint32_t TimeChangeStatus_1;
uint32_t TimeChangeStatus_2;
#define SENSOR_TIMEOUT_CHANGE_STATUS (2000U)  //время между переключение статуса светильника
#define TIMEOUT_RST_SETTINGS_BTN (4000U)    //время удержания кнопки сброса, до момента сброса настроек
#define SENSOR_TIMEOUT_DEBOUNCE (7000U)     //интервал времени игнорирования сенсора, после его сработывания

void buttonBegin()
{
  //настройка кнопки сброса
  btn_settings.setDebounce(100);
  btn_settings.setTimeout(TIMEOUT_RST_SETTINGS_BTN);

  //настройка переключателей
#ifdef BTN_SWITCH_1_USE
  btn_switch_1.setDebounce(170);
  btn_switch_1.setTickMode(AUTO);
#endif

#ifdef BTN_SWITCH_2_USE
  btn_switch_2.setDebounce(170);
  btn_switch_2.setTickMode(AUTO);
#endif

  //настройка датчиков
#ifdef BTN_SENSOR_1_USE
  if (fsManager.UseSensor_1) {
    btn_sensor_1.setDebounce(SENSOR_TIMEOUT_DEBOUNCE);
  }
#endif

#ifdef BTN_SENSOR_2_USE
  if (fsManager.UseSensor_2) {
    btn_sensor_2.setDebounce(SENSOR_TIMEOUT_DEBOUNCE);
  }
#endif
}


void buttonTick()
{
  // ---- НАСТРОЙКА   -----------------------
  btn_settings.tick();
  if (btn_settings.isHolded())
  {
    Log.printf_P(PSTR("[BUTTON] Settings is press\n"));
    Log.printf_P(PSTR("Settings will be clear...\n"));

    Log.printf_P(PSTR("WiFi settings will be clear\n"));
    wifiManager.resetSettings();
    wifiManager.erase();
  }



  // ---- СЕНСОРЫ   -----------------------
#ifdef BTN_SENSOR_1_USE
  if (fsManager.UseSensor_1) {
    btn_sensor_1.tick();
    if (btn_sensor_1.isPress())
    {
      Log.printf_P(PSTR("[BUTTON] SENSOR_1 is press\n"));
      if (TimeChangeStatus_1 + SENSOR_TIMEOUT_CHANGE_STATUS < millis())
      {

        LedChangeStatus_1(false);
      }
    }
  }
#endif



#ifdef BTN_SENSOR_2_USE
  if (fsManager.UseSensor_2) {
    btn_sensor_2.tick();
    if (btn_sensor_2.isPress())
    {
      Log.printf_P(PSTR("[BUTTON] SENSOR_2 is press\n"));
      if (TimeChangeStatus_2 + SENSOR_TIMEOUT_CHANGE_STATUS < millis())
      {

        LedChangeStatus_2(false);
      }
    }
  }
#endif



  // ---- ВЫКЛЮЧАТЕЛИ ---------------------


#ifdef BTN_SWITCH_1_USE
  if (btn_switch_1.isPress())
  {
    if (firstRun)    {
      return;
    }
    Log.printf_P(PSTR("[BUTTON] Switch_1 button has switched (press)\n"));
    if (TimeChangeStatus_1 + SENSOR_TIMEOUT_CHANGE_STATUS < millis())
    {

      LedChangeStatus_1(false);
    }
  }
  if (btn_switch_1.isRelease())
  {
    Log.printf_P(PSTR("[BUTTON] Switch_1 button has switched (release)\n"));
    if (TimeChangeStatus_1 + SENSOR_TIMEOUT_CHANGE_STATUS < millis())
    {

      LedChangeStatus_1(false);
    }
  }
#endif

#ifdef BTN_SWITCH_2_USE
  if (btn_switch_2.isPress())
  {
    if (firstRun)    {
      return;
    }
    Log.printf_P(PSTR("[BUTTON] Switch_2 button has switched (press)\n"));
    if (TimeChangeStatus_2 + SENSOR_TIMEOUT_CHANGE_STATUS < millis())
    {

      LedChangeStatus_2(false);
    }
  }
  if (btn_switch_2.isRelease())
  {
    Log.printf_P(PSTR("[BUTTON] Switch_1 button has switched (release)\n"));
    if (TimeChangeStatus_2 + SENSOR_TIMEOUT_CHANGE_STATUS < millis())
    {

      LedChangeStatus_2(false);
    }
  }
#endif


  firstRun = false;
}

void LedChangeStatus_1(bool status)
{

#ifdef LED_1_USE
  LedStatus_1 = status ? status : !LedStatus_1;
  digitalWrite(LED_1, LED_1_INVERT ? !LedStatus_1 : LedStatus_1);
  TimeChangeStatus_1 = millis();
  Log.printf_P(PSTR("[LedStatus] LedStatus_1: %s\n"), LedStatus_1 ? "True" : "False");

  //публикация статуса светильнака
  mqttClient.publish(ch1_status, MQTT_QOS, true, LedStatus_1 ? "1" : "0");

  Log.printf_P(PSTR("[MQTT]  topic: %s LedStatus_1: %d\n"), ch1_status, LedStatus_1);
#endif


}

void LedChangeStatus_2(bool status)
{

#ifdef LED_2_USE
  LedStatus_2 = status ? status : !LedStatus_2;

  digitalWrite(LED_2, LED_2_INVERT ? !LedStatus_2 : LedStatus_2);
  TimeChangeStatus_2 = millis();
  Log.printf_P(PSTR("[LedStatus] LedStatus_2: %s\n"), LedStatus_2 ? "True" : "False");

  //публикация статуса светильнака
  mqttClient.publish(ch2_status, MQTT_QOS, true, LedStatus_2 ? "1" : "0");
  Log.printf_P(PSTR("[MQTT]  topic: %s LedStatus_2: %d\n"), ch2_status, LedStatus_2);


#endif



}
