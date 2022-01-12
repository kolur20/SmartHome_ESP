void connectToWifi()
{

  //вывод отладочных сообщений
  wifiManager.setDebugOutput(true);

  //установка минимально приемлемого уровня сигнала WiFi сетей (8% по умолчанию)
  //wifiManager.setMinimumSignalQuality();


  WiFi.mode(WIFI_STA);

  //ожидание подключения к точке доступа до рестарта
  wifiManager.setConfigPortalTimeout(ESP_AP_CONNECT_TIMEOUT);
  //ожидание подключения к роутеру доступа до перехода в STA
  wifiManager.setConnectTimeout(ESP_STA_CONNECT_TIMEOUT);
  wifiManager.setConnectRetries(ESP_AP_CONNECT_RETRIES);


  /* ---------------------------       ПЕРЕМЕННЫЕ ДЛЯ НАСТРОЕК          ------------------------------------------------------------------------------*/

  // callbacks
  wifiManager.setAPCallback(ConfigModeCallback);
  wifiManager.setWebServerCallback(WebServerCallback);
  wifiManager.setSaveConfigCallback(SaveWifiCallback);
  wifiManager.setSaveParamsCallback(SaveParamCallback);
  wifiManager.setBreakAfterConfig(true); // needed to use saveWifiCallback
  //// visual
  WiFiManagerParameter custom_html("<p>Configuration settings ESPControl</p>");

  WiFiManagerParameter custom_mqtt_topic("topic", "Mqtt topic", fsManager.mqtt_topic, sizeof(fsManager.mqtt_topic));
  WiFiManagerParameter custom_mqtt_server("server", "Mqtt server", fsManager.mqtt_server, sizeof(fsManager.mqtt_server));
  WiFiManagerParameter custom_mqtt_port("port", "Mqttqtt port", fsManager.mqtt_port, sizeof(fsManager.mqtt_port));
  WiFiManagerParameter custom_mqtt_login("login", "Mqtt login", fsManager.mqtt_login, sizeof(fsManager.mqtt_login));
  WiFiManagerParameter custom_mqtt_password("password", "Mqtt Password", fsManager.mqtt_password, sizeof(fsManager.mqtt_password));
  WiFiManagerParameter custom_device_name("name", "Name device", fsManager.device_name, sizeof(fsManager.device_name));

#ifdef BTN_SENSOR_1_USE
  WiFiManagerParameter custom_checkbox_1("usesensor_1", "Use sensor 1", "1", 2,
                                         fsManager.UseSensor_1 ? "type=\"checkbox\" checked" : "type=\"checkbox\" ",
                                         WFM_LABEL_AFTER);
#endif

#ifdef BTN_SENSOR_2_USE
  WiFiManagerParameter custom_checkbox_2("usesensor_2", "Use sensor 2", "1", 2,
                                         fsManager.UseSensor_2 ? "type=\"checkbox\" checked" : "type=\"checkbox\" ",
                                         WFM_LABEL_AFTER);
#endif

  WiFiManagerParameter p_lineBreak_notext("<p></p>");
  //add Parametr to manager
  wifiManager.addParameter(&custom_html);
  wifiManager.addParameter(&custom_mqtt_topic);
  wifiManager.addParameter(&custom_mqtt_server);
  wifiManager.addParameter(&custom_mqtt_port);
  wifiManager.addParameter(&custom_mqtt_login);
  wifiManager.addParameter(&custom_mqtt_password);
  wifiManager.addParameter(&p_lineBreak_notext);             //linebreak

  wifiManager.addParameter(&custom_device_name);


#ifdef BTN_SENSOR_1_USE
  wifiManager.addParameter(&custom_checkbox_1);
#endif

#ifdef BTN_SENSOR_2_USE
  wifiManager.addParameter(&custom_checkbox_2);
#endif
  /* ---------------------------   -------------------------      ------------------------------------------------------------------------------*/


  // invert theme, dark
  //wm.setDarkMode(true);

  const char* menu[] = {"wifi", "wifinoscan", "param", "info", "sep", "erase", "update", "restart", "sep", "exit"};
  wifiManager.setMenu(menu, 10);
  wifiManager.setCountry("EN");
  //wifiManager.setHostname("WIFIMANAGER_TESTING");



  if (!wifiManager.autoConnect(strcat(ESP_AP_NAME, fsManager.device_name)/*, ESP_AP_PASSWORD*/))
  {

    if (shouldSaveConfig) {

      //       Log.printf("custom_checkbox: %d\n", custom_checkbox.getValue() == "1" ? 1:0);
      //     EepromManager::SetValueIk(custom_checkbox.getValue() == "1" ? 1:0);

      strcpy(fsManager.mqtt_topic, custom_mqtt_topic.getValue());
      strcpy(fsManager.mqtt_server, custom_mqtt_server.getValue());
      strcpy(fsManager.mqtt_port, custom_mqtt_port.getValue());
      strcpy(fsManager.mqtt_login, custom_mqtt_login.getValue());
      strcpy(fsManager.mqtt_password, custom_mqtt_password.getValue());
      strcpy(fsManager.device_name, custom_device_name.getValue());

#ifdef BTN_SENSOR_1_USE
      strcpy(fsManager.use_sensor_1, strncmp (custom_checkbox_1.getValue(), "1", 1) == 0 ? "1" : "0");
#endif

#ifdef BTN_SENSOR_2_USE
      strcpy(fsManager.use_sensor_2, strncmp (custom_checkbox_2.getValue(), "1", 1) == 0 ? "1" : "0");
#endif

      fsManager.SaveConfig();
      shouldSaveConfig = false;
    }
#ifdef GENERAL_DEBUG
    Log.println("-------------------------- Failed to connect and hit timeout");
#endif
    delay(3000);
    ESP.restart();
    delay(5000);
  }


#ifdef GENERAL_DEBUG
  Log.println("Connecting to WiFi: sucsessfule");
#endif

  if (shouldSaveConfig) {
    strcpy(fsManager.mqtt_topic, custom_mqtt_topic.getValue());
    strcpy(fsManager.mqtt_server, custom_mqtt_server.getValue());
    strcpy(fsManager.mqtt_port, custom_mqtt_port.getValue());
    strcpy(fsManager.mqtt_login, custom_mqtt_login.getValue());
    strcpy(fsManager.mqtt_password, custom_mqtt_password.getValue());
    strcpy(fsManager.device_name, custom_device_name.getValue());

#ifdef BTN_SENSOR_1_USE
    strcpy(fsManager.use_sensor_1, strncmp (custom_checkbox_1.getValue(), "1", 1) == 0 ? "1" : "0");
#endif

#ifdef BTN_SENSOR_2_USE
    strcpy(fsManager.use_sensor_2, strncmp (custom_checkbox_2.getValue(), "1", 1) == 0 ? "1" : "0");
#endif

    fsManager.SaveConfig();
    shouldSaveConfig = false;
  }



  
}


void SaveWifiCallback() {
#ifdef GENERAL_DEBUG
  Log.println("[CALLBACK] saveCallback fired");
#endif
}

//gets called when WiFiManager enters configuration mode
void ConfigModeCallback (WiFiManager *wifiManager) {
#ifdef GENERAL_DEBUG
  Log.println("[CALLBACK] configModeCallback fired");
#endif
}

void SaveParamCallback() {
#ifdef GENERAL_DEBUG
  Log.println("[CALLBACK] saveParamCallback fired");
#endif
  shouldSaveConfig = true;
  wifiManager.server->on("/custom", HandleRoute);
  // wm.stopConfigPortal();
}

void WebServerCallback() {

  wifiManager.server->on("/custom", HandleRoute);
  // wm.server->on("/info",handleRoute); // you can override wm!

}

void HandleRoute() {
#ifdef GENERAL_DEBUG
  Log.println("[HTTP] handle route");
#endif
  wifiManager.server->send(200, "text/plain", "hello from user code");
}
