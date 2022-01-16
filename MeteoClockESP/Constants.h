#pragma once

// --- ESP -----------------------------
#define ESP_MODE_WIFI                (0U)                           // 0U - WiFi точка доступа, 1U - клиент WiFi (подключение к роутеру)
#define ESP_HTTP_PORT                (80U)                         // номер порта, который будет использоваться во время первой утановки имени WiFi сети (и пароля)
#define ESP_AP_CONNECT_TIMEOUT      (180U)                          //СЕК. ожидание подключения к точке доступа до рестарта
#define ESP_STA_CONNECT_TIMEOUT       (30U)                        //СЕК. ожидание подключения к роутеру доступа до перехода в STA
#define ESP_AP_CONNECT_RETRIES        (3U)                          // количество попыток на подключение к роутеру

#define ESP_AP_NAME                   "AP_ESP_Climate"
#define ESP_AP_PASSWORD               "password1"

// --- MQTT -------------------------------
#define MQTT_QOS (0U)

// ------------------------- НАСТРОЙКИ --------------------
#define RESET_CLOCK 0       // сброс часов на время загрузки прошивки (для модуля с несъёмной батарейкой). Не забудь поставить 0 и прошить ещё раз!
#define SENS_TIME 30000     // время обновления показаний сенсоров на экране, миллисекунд
#define LED_MODE 1          // тип RGB светодиода: 0 - главный катод, 1 - главный анод

// управление яркостью
#define BRIGHT_CONTROL 1      // 0/1 - запретить/разрешить управление яркостью (при отключении яркость всегда будет макс.)
#define BRIGHT_THRESHOLD 150  // величина сигнала, ниже которой яркость переключится на минимум (0-1023)
#define LED_BRIGHT_MAX 255    // макс яркость светодиода СО2 (0 - 255)
#define LED_BRIGHT_MIN 10     // мин яркость светодиода СО2 (0 - 255)
#define LCD_BRIGHT_MAX 255    // макс яркость подсветки дисплея (0 - 255)
#define LCD_BRIGHT_MIN 10     // мин яркость подсветки дисплея (0 - 255)

#define BLUE_YELLOW 0       // жёлтый цвет вместо синего (1 да, 0 нет) но из за особенностей подключения жёлтый не такой яркий
#define DISP_MODE 1         // в правом верхнем углу отображать: 0 - год, 1 - день недели, 2 - секунды
#define WEEK_LANG 1         // язык дня недели: 0 - английский, 1 - русский (транслит)
#define PRESSURE 1          // 0 - график давления, 1 - график прогноза дождя (вместо давления). Не забудь поправить пределы гроафика
#define CO2_SENSOR 1        // включить или выключить поддержку/вывод с датчика СО2 (1 вкл, 0 выкл)
#define DISPLAY_TYPE 1      // тип дисплея: 1 - 2004 (большой), 0 - 1602 (маленький)
#define DISPLAY_ADDR 0x27   // адрес платы дисплея: 0x27 или 0x3f. Если дисплей не работает - смени адрес! На самом дисплее адрес не указан

// пределы отображения для графиков
#define TEMP_MIN 15
#define TEMP_MAX 35
#define HUM_MIN 0
#define HUM_MAX 100
#define PRESS_MIN -100
#define PRESS_MAX 100
#define CO2_MIN 300
#define CO2_MAX 2000

// адрес BME280 жёстко задан в файле библиотеки Adafruit_BME280.h
// стоковый адрес был 0x77, у китайского модуля адрес 0x76.
// Так что если юзаете НЕ библиотеку из архива - не забудьте поменять

// если дисплей не заводится - поменяйте адрес (строка 54)

// пины
#define BACKLIGHT 10
#define PHOTO 9

#define MHZ_RX 14
#define MHZ_TX 12

#define LED_COM 13
#define LED_R 15
#define LED_G 3 
#define LED_B 1
#define BTN_PIN 2

// --- ОБНОВЛЕНИЕ ПО ВОЗДУХУ ----------------
#define OTA

#ifdef OTA
#define OTA_PORT                    (8266U)                       // номер порта, который будет "прослушиваться" в ожидании команды прошивки по воздуху
#endif

//************ ОТЛАДКА **************
//#define DEBUG 1            // вывод на дисплей лог инициализации датчиков при запуске. Для дисплея 1602 не работает! Но дублируется через порт!
#define GENERAL_DEBUG  
#define DEBUG_TELNET false

#if DEBUG_TELNET
#define TELNET_PORT           (23U)                         // номер telnet порта
WiFiServer telnetServer(TELNET_PORT);                       // telnet сервер
WiFiClient telnet;                                          // обработчик событий telnet клиента
bool telnetGreetingShown = false;                           // признак "показано приветствие в telnet"
#define Log                    telnet

#else
#define Log                   Serial
#endif
