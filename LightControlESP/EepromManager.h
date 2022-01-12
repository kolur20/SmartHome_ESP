#pragma once

#include <EEPROM.h>

#define EEPROM_TOTAL_BYTES_USED              (20U)        // общий размер используемой EEPROM памяти (сумма всех хранимых настроек + 1 байт)
#define EEPROM_FIRST_RUN_MARK                (3U)         // метка, если ещё не записно в EEPROM_FIRST_RUN_ADDRESS, значит нужно проинициализировать EEPROM и записать все первоначальные настройки

#define EEPROM_FIRST_RUN_ADDRESS             (0U)         // адрес в EEPROM памяти для записи признака первого запуска (определяет необходимость первоначальной записи всех хранимых настроек)
#define EEPROM_ESP_MODE_ADDRESS              (1U)         // адрес в EEPROM памяти для записи режима работы модуля ESP (точка доступа/WiFi клиент)
#define EEPROM_USE_IK_ADDRESS                (2U)         // адрес в EEPROM памяти для записи режима работы IK датчика         

class EepromManager {

  public:



    static void InitEEPROMSettings(
      uint8_t* EEPROM_ESP_MODE,
      bool* EEPROM_USE_IK
    )
    {

      EEPROM.begin(EEPROM_TOTAL_BYTES_USED);
      delay(50);
      // записываем в EEPROM начальное состояние настроек, если их там ещё нет
      if (EEPROM.read(EEPROM_FIRST_RUN_ADDRESS) != EEPROM_FIRST_RUN_MARK)
      {
        SetDefaultValue();
        Log.println("SetDefaultValue in EEPROM!");
      }

      *EEPROM_ESP_MODE = (uint8_t)EEPROM.read(EEPROM_ESP_MODE_ADDRESS);
      *EEPROM_USE_IK = (bool)EEPROM.read(EEPROM_USE_IK_ADDRESS);

    }

    static void ResetEEPROM()
    {
      EEPROM.write(EEPROM_FIRST_RUN_ADDRESS, 0);
      EEPROM.commit();
    }



    static void SetDefaultValue()
    {

      EEPROM.write(EEPROM_FIRST_RUN_ADDRESS, EEPROM_FIRST_RUN_MARK);
      EEPROM.write(EEPROM_ESP_MODE_ADDRESS, ESP_MODE_WIFI);
      EEPROM.write(EEPROM_USE_IK_ADDRESS, 0);
      EEPROM.commit();
    }

    static void PrintEEPROMValues()
    {

      Log.printf_P(PSTR("Saved data in EEPROM\n"));

      Log.printf_P(PSTR("EEPROM_FIRST_RUN_MARK - %d\n"), EepromManager::ReadByte(EEPROM_FIRST_RUN_ADDRESS));
      Log.printf_P(PSTR("EEPROM_ESP_MODE - %d\n"), EepromManager::ReadByte(EEPROM_ESP_MODE_ADDRESS));
      Log.printf_P(PSTR("EEPROM_USE_IK - %d\n"), (bool)EepromManager::ReadByte(EEPROM_USE_IK_ADDRESS));


    }

    static void SetValueIk(bool flag)
    {
      Log.printf_P(PSTR("Save EEPROM_USE_IK - %d\n"), flag);

      if (flag != (bool)EepromManager::ReadByte(EEPROM_USE_IK_ADDRESS))
      {
        WriteByte(EEPROM_USE_IK_ADDRESS, flag);
      }
    }


    static uint8_t WriteByte(uint16_t address, uint8_t val) {
      EEPROM.write(address, val);
      EEPROM.commit();
    }

    static uint8_t ReadByte(uint16_t address) {
      return (uint8_t)EEPROM.read(address);
    }

    static uint16_t ReadUint16(uint16_t address)       {
      uint16_t val;
      uint8_t* p = (uint8_t*)&val;
      *p        = EEPROM.read(address);
      *(p + 1)  = EEPROM.read(address + 1);
      return val;
    }

    static void WriteUint16(uint16_t address, uint16_t val)      {
      uint8_t* p = (uint8_t*)&val;
      EEPROM.write(address, *p);
      EEPROM.write(address + 1, *(p + 1));
      EEPROM.commit();
    }

    static int16_t ReadInt16(uint16_t address)      {
      int16_t val;
      uint8_t* p = (uint8_t*)&val;
      *p        = EEPROM.read(address);
      *(p + 1)  = EEPROM.read(address + 1);
      return val;
    }

    static void WriteInt16(uint16_t address, int16_t val)      {
      uint8_t* p = (uint8_t*)&val;
      EEPROM.write(address, *p);
      EEPROM.write(address + 1, *(p + 1));
      EEPROM.commit();
    }

    static uint32_t ReadUint32(uint16_t address)      {
      uint32_t val;
      uint8_t* p = (uint8_t*)&val;
      *p        = EEPROM.read(address);
      *(p + 1)  = EEPROM.read(address + 1);
      *(p + 2)  = EEPROM.read(address + 2);
      *(p + 3)  = EEPROM.read(address + 3);
      return val;
    }

    static void WriteUint32(uint16_t address, uint32_t val)      {
      uint8_t* p = (uint8_t*)&val;
      EEPROM.write(address, *p);
      EEPROM.write(address + 1, *(p + 1));
      EEPROM.write(address + 2, *(p + 2));
      EEPROM.write(address + 3, *(p + 3));
      EEPROM.commit();
    }

    static int32_t ReadInt32(uint16_t address)      {
      int32_t val;
      uint8_t* p = (uint8_t*)&val;
      *p        = EEPROM.read(address);
      *(p + 1)  = EEPROM.read(address + 1);
      *(p + 2)  = EEPROM.read(address + 2);
      *(p + 3)  = EEPROM.read(address + 3);
      return val;
    }

    static void WriteInt32(uint16_t address, int32_t val)      {
      uint8_t* p = (uint8_t*)&val;
      EEPROM.write(address, *p);
      EEPROM.write(address + 1, *(p + 1));
      EEPROM.write(address + 2, *(p + 2));
      EEPROM.write(address + 3, *(p + 3));
      EEPROM.commit();
    }
};
