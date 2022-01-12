
#include <ESP8266WiFi.h>
//needed for library
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <WiFiManager.h>

#include <GyverButton.h>

#include "Constants.h"
//#include "EepromManager.h"
#include "FSManager.h"
#include "OtaManager.h"


#include <AsyncMqttClient.h>
#include <Ticker.h>
#include "Convert.h"

///---------ИНИЦИАЛИЗАЦИЯ ОБЪЕКТОВ---------------
static WiFiManager wifiManager;
static FSManager fsManager;
AsyncMqttClient mqttClient;
Ticker mqttReconnectTimer;

char ch1_cmd[60];
char ch1_status[60];
char ch2_cmd[60];
char ch2_status[60];

#ifdef OTA
OtaManager otaManager;
#endif
/// --- ИНИЦИАЛИЗАЦИЯ ПЕРЕМЕННЫХ -------
uint8_t Esp_mode = ESP_MODE_WIFI;                                // ESP_MODE может быть сохранён в энергонезависимую память и изменён в процессе работы контроллера без необходимости её перепрошивки
bool shouldSaveConfig = false;


// --- ИНИЦИАЛИЗАЦИЯ КНОПОК -------

GButton btn_settings(BTN_SETINGS, HIGH_PULL, NORM_OPEN);

#ifdef BTN_SENSOR_1_USE
GButton btn_sensor_1(BTN_SENSOR_1, HIGH_PULL, NORM_OPEN);
#endif

#ifdef BTN_SENSOR_2_USE
GButton btn_sensor_2(BTN_SENSOR_2, HIGH_PULL, NORM_OPEN);
#endif

#ifdef BTN_SWITCH_1_USE
GButton btn_switch_1(BTN_SWITCH_1);
#endif


#ifdef BTN_SWITCH_2_USE
GButton btn_switch_2(BTN_SWITCH_2 );
#endif

void setup() {

  Serial.begin(115200);
  ESP.wdtEnable(WDTO_8S);




  // TELNET
#if defined(GENERAL_DEBUG) && GENERAL_DEBUG_TELNET
  Log.println("[GENERAL_DEBUG_TELNET]");
  telnetServer.begin();
  for (uint8_t i = 0; i < 100; i++)                         // пауза 10 секунд в отладочном режиме, чтобы успеть подключиться по протоколу telnet до вывода первых сообщений
  {
    handleTelnetClient();
    delay(100);
    ESP.wdtFeed();
  }
#endif

  //FileSystem

  Log.println("Mounting FS...");

  if (!SPIFFS.begin()) {
    Log.println("Failed to mount file system");
    SPIFFS.format();
    //    EepromManager::ResetEEPROM();
    wifiManager.resetSettings();
    wifiManager.erase();
    return;
  }
  if (!fsManager.LoadConfig()) {
    Log.println("Failed to load config");
    if (!fsManager.SaveConfig()) {
      Log.println("Failed to save config..... FS Format...");
      SPIFFS.format();
      ESP.restart();
      delay(1000);
    } else {
      Log.println("Config saved");
    }
  } else {
    Log.println("Config loaded");
  }


  // EEPROM
  // инициализация EEPROM; запись начального состояния настроек, если их там ещё нет; инициализация настроек значениями из EEPROM
  //  EepromManager::InitEEPROMSettings(
  //    &Esp_mode,
  //    &use_ik
  //  );
  //  EepromManager::PrintEEPROMValues();

  // ---- КНОПКИ и пины
  //D6 - 12
#ifdef LED_1_USE
  pinMode(LED_1, OUTPUT);
  digitalWrite(LED_1, START_STATUS_LED);
#endif

#ifdef LED_2_USE
  pinMode(LED_2, OUTPUT);
  digitalWrite(LED_2, START_STATUS_LED);
#endif
  buttonBegin();


  // WI-FI
  connectToWifi();

#ifdef OTA
  otaManager.Begin(strcat(ESP_AP_NAME, fsManager.device_name));
#endif

  //MQTT
  connectToMqtt();
}



void loop() {
  // put your main code here, to run repeatedly:
#ifdef OTA
  otaManager.Handle();
#endif


#if defined(GENERAL_DEBUG) && GENERAL_DEBUG_TELNET
  handleTelnetClient();
#endif

  buttonTick();





  ESP.wdtFeed();
}
