Ticker mqttReconnectTimer;

char mqtt_status[60];
char mqtt_temp[60];
char mqtt_ppm[60];
char mqtt_hum[60];

void SendMqtt(const char* topic, float i)
{
  char text[6];
  sprintf(text, "%.2f", i);
  mqttClient.publish(topic, MQTT_QOS, false, text);
}
void SendMqtt(const char* topic, byte i)
{
  char text[6];
  sprintf(text, "%d", i);
  mqttClient.publish(topic, MQTT_QOS, false, text);
}
void SendMqtt(const char* topic, int i)
{
  char text[6];
  sprintf(text, "%d", i);
  mqttClient.publish(topic, MQTT_QOS, false, text);
}
void SendMqtt(const char* topic, const char* i)
{
  mqttClient.publish(topic, MQTT_QOS, false, i);
}

void connectToMqtt() {
#ifdef GENERAL_DEBUG
  Log.println("[MQTT] Connecting to MQTT...");
#endif


  //  mqttClient.onDisconnect(onMqttDisconnect);
  //
  //  mqttClient.onUnsubscribe(onMqttUnsubscribe);
  //
  //


  ///подключение к брокеру
  mqttClient.setServer(
    Convert::str2IP(fsManager.mqtt_server),
    atoi(fsManager.mqtt_port));
  mqttClient.setCredentials(
    fsManager.mqtt_login,
    fsManager.mqtt_password);


  mqttClient.onPublish(onMqttPublish);
  mqttClient.onSubscribe(onMqttSubscribe);
  mqttClient.onMessage(onMqttMessage);
  mqttClient.onConnect(onMqttConnect);

  ///   -------------------------------------  подписка на топики для прослушивания   -------------------------------------


  ///   -------------------------------------  реакция на обновление в прослушиваемых топиках   -------------------------------------

  ///   -------------------------------------  реакция на публикацию   -------------------------------------

  mqttClient.connect();
}

/// выполняется после подключения к брокеру, подписка на необходимые топики
void onMqttConnect(bool sessionPresent) {

#ifdef GENERAL_DEBUG
  Log.println("[MQTT] Connected to MQTT.");
  Log.print("[MQTT] Session present: ");
  Log.println(sessionPresent);
#endif
  /// инициализируем необходимые переменные, если подключились
  char str[30];
  strcpy(str, "ESP_Climate/"); strcat(str, fsManager.mqtt_topic);
  
  strcpy(mqtt_status, str); strcat(mqtt_status, "/status");
  strcpy(mqtt_temp, str);   strcat(mqtt_temp, "/temp");
  strcpy(mqtt_ppm, str);    strcat(mqtt_ppm, "/ppm");
  strcpy(mqtt_hum, str);    strcat(mqtt_hum, "/hum");
}




void onMqttDisconnect(AsyncMqttClientDisconnectReason reason) {
#ifdef GENERAL_DEBUG
  Log.println("[MQTT] Disconnected from MQTT.");
#endif
  if (WiFi.isConnected()) {
    mqttReconnectTimer.once(2, connectToMqtt);
  }
}

void onMqttSubscribe(uint16_t packetId, uint8_t qos) {

#ifdef GENERAL_DEBUG
  Log.println("[MQTT] Subscribe acknowledged.");
  Log.print("  packetId: ");
  Log.println(packetId);
  Log.print("  qos: ");
  Log.println(qos);
#endif
}

void onMqttUnsubscribe(uint16_t packetId) {
#ifdef GENERAL_DEBUG
  Log.println("[MQTT] Unsubscribe acknowledged.");
  Log.print("  packetId: ");
  Log.println(packetId);
#endif
}

void onMqttMessage(char* topic, char* payload, AsyncMqttClientMessageProperties properties, size_t len, size_t index, size_t total) {
#ifdef GENERAL_DEBUG
  Log.println("[MQTT] Publish received.");
  Log.print("  topic: ");
  Log.println(topic);
  Log.print("  qos: ");
  Log.println(properties.qos);
  Log.print("  dup: ");
  Log.println(properties.dup);
  Log.print("  retain: ");
  Log.println(properties.retain);
  Log.print("  len: ");
  Log.println(len);
  Log.print("  index: ");
  Log.println(index);
  Log.print("  total: ");
  Log.println(total);
#endif

}

void onMqttPublish(uint16_t packetId) {
#ifdef GENERAL_DEBUG
  Log.println("[MQTT] Publish acknowledged.");
  Log.print("  packetId: ");
  Log.println(packetId);
#endif
}
