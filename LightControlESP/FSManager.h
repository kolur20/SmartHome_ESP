// Example: storing JSON configuration file in flash file system
//
// Uses ArduinoJson library by Benoit Blanchon.
// https://github.com/bblanchon/ArduinoJson
//
// Created Aug 10, 2015 by Ivan Grokhotkov.
//
// This example code is in the public domain.
#pragma once

#include <ArduinoJson.h>
#include "FS.h"

#define FileNameConfig        ("/config.json")
class FSManager
{
  public:
    //переменныe для передачи данных
    char mqtt_server[20] = "api.example.com";
    char mqtt_port[6] = "8023";
    char mqtt_login[60] = "admin";
    char mqtt_password[60] = "root";
    char mqtt_topic[60] = "wallswitch";
    char device_name[30] = "newKithen";
    char use_sensor_1[2] = "0";
    char use_sensor_2[2] = "0";
    

    

    bool LoadConfig();
    bool SaveConfig();
    void PrintFSValues();
    bool UseSensor_1 = false;
    bool UseSensor_2 = false;

};

bool FSManager::LoadConfig() {
  File configFile = SPIFFS.open(FileNameConfig, "r");
  if (!configFile) {
#ifdef GENERAL_DEBUG
    Log.println("Failed to open config file");
#endif
    return false;
  }

  size_t size = configFile.size();
  if (size > 1024) {
#ifdef GENERAL_DEBUG
    Log.println("Config file size is too large");
#endif
    return false;
  }

  // Allocate a buffer to store contents of the file.
  std::unique_ptr<char[]> buf(new char[size]);

  // We don't use String here because ArduinoJson library requires the input
  // buffer to be mutable. If you don't use ArduinoJson, you may as well
  // use configFile.readString instead.
  configFile.readBytes(buf.get(), size);

  DynamicJsonBuffer jsonBuffer;
  JsonObject& json = jsonBuffer.parseObject(buf.get());

  if (!json.success()) {
#ifdef GENERAL_DEBUG
    Log.println("Failed to parse config file");
#endif
    return false;
  }

  strcpy(mqtt_server, json["mqtt_server"]);
  strcpy(mqtt_port, json["mqtt_port"]);
  strcpy(mqtt_login, json["mqtt_login"]);
  strcpy(mqtt_password, json["mqtt_password"]);
  strcpy(mqtt_topic, json["mqtt_topic"]);
  strcpy(device_name, json["device_name"]);
  strcpy(use_sensor_1, json["use_sensor_1"]);
  strcpy(use_sensor_2, json["use_sensor_2"]);
  UseSensor_1 = strncmp(use_sensor_1, "1", 1) ==  0 ? true : false;
  UseSensor_2 = strncmp(use_sensor_2, "1", 1) ==  0 ? true : false;
  
#ifdef GENERAL_DEBUG
  json.prettyPrintTo(Log);
  Log.println("- patametrs is load!");
#endif
  configFile.close();
  return true;
}

bool FSManager::SaveConfig() {
  DynamicJsonBuffer jsonBuffer;
  JsonObject& json = jsonBuffer.createObject();

  json["mqtt_server"] = mqtt_server;
  json["mqtt_port"] = mqtt_port;
  json["mqtt_login"] = mqtt_login;
  json["mqtt_password"] = mqtt_password;
  json["mqtt_topic"] = mqtt_topic;
  json["device_name"] = device_name;
  json["use_sensor_1"] = use_sensor_1;
  json["use_sensor_2"] = use_sensor_2;
  UseSensor_1 = strncmp(use_sensor_1, "1", 1) ==  0 ? true : false;
  UseSensor_2 = strncmp(use_sensor_2, "1", 1) ==  0 ? true : false;
  
  File configFile = SPIFFS.open(FileNameConfig, "w");
  if (!configFile) {
#ifdef GENERAL_DEBUG
    Log.println("Failed to open config file for writing");
#endif
    return false;
  }

  json.printTo(configFile);
#ifdef GENERAL_DEBUG
  json.prettyPrintTo(Log);
  Log.println("- patametrs is save!");
#endif
  configFile.close();
  //PrintFSValues();
  return true;
}

void FSManager::PrintFSValues() {
#ifdef GENERAL_DEBUG
  Log.printf_P(PSTR("Data in FS\n"));
  Log.printf_P(PSTR("MQTT_SERVER: %s\n"), mqtt_server);
  Log.printf_P(PSTR("MQTT_PORT: %s\n"), mqtt_port);
  Log.printf_P(PSTR("MQTT_LOGIN: %s\n"), mqtt_login);
  Log.printf_P(PSTR("MQTT_PASSWORD: %s\n"), mqtt_password);
  Log.printf_P(PSTR("MQTT_TOPIC: %s\n"), mqtt_topic);
  Log.printf_P(PSTR("DEVICE_NAME: %s\n"), device_name);
  Log.printf_P(PSTR("USE_SENSOR_1: %s\n"), use_sensor_1);
  Log.printf_P(PSTR("USE_SENSOR_2: %s\n"), use_sensor_2);
#endif
}
