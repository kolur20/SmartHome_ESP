#pragma once

//===============НАСТРОЙКИ===========

// --- ESP -----------------------------
#define ESP_MODE_WIFI                (0U)                           // 0U - WiFi точка доступа, 1U - клиент WiFi (подключение к роутеру)
#define ESP_HTTP_PORT                (80U)                         // номер порта, который будет использоваться во время первой утановки имени WiFi сети (и пароля), к которой потом будет подключаться лампа в режиме WiFi клиента (лучше не менять)
#define ESP_AP_CONNECT_TIMEOUT      (180U)                          //СЕК. ожидание подключения к точке доступа до рестарта
#define ESP_STA_CONNECT_TIMEOUT       (30U)                        //СЕК. ожидание подключения к роутеру доступа до перехода в STA
#define ESP_AP_CONNECT_RETRIES        (3U)                          // количество попыток на подключение к роутеру

#define ESP_AP_NAME                   "AP_ESP_LIGHT-"
#define ESP_AP_PASSWORD               "password1"

// --- MQTT -------------------------------
#define MQTT_QOS (0U)





// --- КНОПКИ && СЕНСОРЫ  -------------------------------
#define BTN_SETINGS                 (13U)         //D7  кнопка запуска или зброса настроек

#define BTN_SWITCH_1_USE
#ifdef   BTN_SWITCH_1_USE
#define BTN_SWITCH_1                  (5U)         //D1 клавиша переключения света
#endif


//#define BTN_SWITCH_2_USE
#ifdef   BTN_SWITCH_2_USE
#define BTN_SWITCH_2                  (4U)         //D2 клавиша переключения света
#endif


//#define  BTN_SENSOR_1_USE
#ifdef BTN_SENSOR_1_USE
#define BTN_SENSOR_1            (14U)         //D5 клавиша переключения света
#endif


//#define BTN_SENSOR_2_USE
#ifdef BTN_SENSOR_2_USE
#define BTN_SENSOR_2            (12U)         //D6 клавиша переключения света
#endif


// --- ЛЕДЫ -----------------------------------
#define LED_1_USE
#ifdef LED_1_USE
#define LED_1                        (16U)         //D0 пин выхода на лампочку
#define LED_1_INVERT                 (false)       //инвертирование согнала на открытие реле
#endif

//#define LED_2_USE
#ifdef LED_2_USE
#define LED_2                        (2U)         //D4 пин выхода на лампочку
#define LED_2_INVERT                 (true)       //инвертирование согнала на открытие реле
#endif

#define START_STATUS_LED            (HIGH)         //для размыкания реле необходимы 3,3 вольта
// --- ОБНОВЛЕНИЕ ПО ВОЗДУХУ ----------------
#define OTA

#ifdef OTA
#define OTA_PORT                    (8266U)                       // номер порта, который будет "прослушиваться" в ожидании команды прошивки по воздуху
#endif
//************ ОТЛАДКА **************
#define GENERAL_DEBUG_TELNET  (false)                       // true - отладочные сообщения будут выводиться в telnet вместо Serial порта (для удалённой отладки без подключения usb кабелем)
#define GENERAL_DEBUG                                       // если строка не закомментирована, будут выводиться отладочные сообщения

#if defined(GENERAL_DEBUG) && GENERAL_DEBUG_TELNET
#define TELNET_PORT           (23U)                         // номер telnet порта
WiFiServer telnetServer(TELNET_PORT);                       // telnet сервер
WiFiClient telnet;                                          // обработчик событий telnet клиента
bool telnetGreetingShown = false;                           // признак "показано приветствие в telnet"
#define Log                    telnet

#else
#define Log                   Serial
#endif
