#pragma once



#ifdef OTA

#include <ArduinoOTA.h>
#include <ESP8266mDNS.h>




class OtaManager
{
  public:
    void Begin(const char* nameOta)
    {
      // Port defaults to 8266
      ArduinoOTA.setPort(8266);

      // Hostname defaults to esp8266-[ChipID]
      ArduinoOTA.setHostname(nameOta);

      // No authentication by default
      // ArduinoOTA.setPassword((const char *)"123");
      ArduinoOTA.setRebootOnSuccess(true);
      ArduinoOTA.onStart([]() {
#ifdef GENERAL_DEBUG
        Log.println("OTA: Start");
        Log.println("OTA: SPIFFS is close");
#endif
        SPIFFS.end();
      });
      ArduinoOTA.onEnd([]() {
#ifdef GENERAL_DEBUG
        Log.println("OTA: End");
#endif
      });
      ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
#ifdef GENERAL_DEBUG
        Log.printf("OTA: Progress: %u%%\r", (progress / (total / 100)));
#endif
      });
      ArduinoOTA.onError([](ota_error_t error) {
#ifdef GENERAL_DEBUG
        Log.printf("Error[%u]: ", error);
        if (error == OTA_AUTH_ERROR) Log.println("OTA: Auth Failed");
        else if (error == OTA_BEGIN_ERROR) Log.println("OTA: Begin Failed");
        else if (error == OTA_CONNECT_ERROR) Log.println("OTA: Connect Failed");
        else if (error == OTA_RECEIVE_ERROR) Log.println("OTA: Receive Failed");
        else if (error == OTA_END_ERROR) Log.println("OTA: End Failed");
#endif
      });


      ArduinoOTA.begin();
    }



    void Handle()
    {
      ArduinoOTA.handle();
    }


};

#endif
