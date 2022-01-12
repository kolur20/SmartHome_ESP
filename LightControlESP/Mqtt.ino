


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
  strcpy(ch1_cmd, fsManager.mqtt_topic);  strcat(ch1_cmd, "/lamp/ch1/cmd");
  strcpy(ch2_cmd, fsManager.mqtt_topic);  strcat(ch2_cmd, "/lamp/ch2/cmd");

  strcpy(ch1_status, fsManager.mqtt_topic);  strcat(ch1_status, "/lamp/ch1/status");
  strcpy(ch2_status, fsManager.mqtt_topic);  strcat(ch2_status, "/lamp/ch2/status");

  //подписка
#ifdef GENERAL_DEBUG
  Log.printf_P(PSTR("[MQTT] subscribe: %s , qos = %d\n"), ch1_cmd, MQTT_QOS);
#endif

  mqttClient.subscribe(ch1_cmd, MQTT_QOS);

#ifdef GENERAL_DEBUG

  Log.printf_P(PSTR("[MQTT] subscribe: %s , qos = %d\n"), ch2_cmd, MQTT_QOS);
#endif

  mqttClient.subscribe(ch2_cmd, MQTT_QOS);

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

  if (!strcmp_P(topic, ch1_cmd))
  {
    LedChangeStatus_1(atoi(payload));

  }
  else  if (!strcmp_P(topic, ch2_cmd))
  {
    LedChangeStatus_2(atoi(payload));
  }
}

void onMqttPublish(uint16_t packetId) {
#ifdef GENERAL_DEBUG
  Log.println("[MQTT] Publish acknowledged.");
  Log.print("  packetId: ");
  Log.println(packetId);
#endif
}
