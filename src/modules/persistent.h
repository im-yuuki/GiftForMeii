#pragma once
#include <Arduino.h>
#include <EEPROM.h>
#include "modules/logging.h"

const uint8_t CONFIG_VERSION = 1; 

enum WiFiEncryption : uint8_t {
    DISABLED,
    OPEN,
    PSK_ENCRYPTED
};

enum AlarmMode : uint8_t {
    OFF,
    ONCE,
    ALL
};

struct DataLayout {
    uint8_t version = 1;
    char PASSWORD[16] = "meikayuu";
    WiFiEncryption WiFi_STATE = DISABLED;
    char WiFi_SSID[64] = "";
    char WiFi_PASSWORD[64] = "";
    // Structure: {mode, hour, minute}
    uint8_t Alarm1[3] = {OFF, 0, 0};
    uint8_t Alarm2[3] = {OFF, 0, 0};
};

class Persistent {
    private:
    DataLayout data;
    logging::Logger logger = logging::Logger("persistent");

    /**
     * Read data from EEPROM to instance
    */
    void readEEPROM() {
        byte *ptr = (byte*)(void*)&data;
        for (unsigned int i = 0; i < sizeof(data); i++) {
            *ptr++ = EEPROM.read(i);
        }
    }

    public:
    /**
     * Initalize the persistent instance
    */
    void init() {
        logger.info("Loading persistents");
        EEPROM.begin(1024);
        if (data.version != CONFIG_VERSION) {
            logger.warn("Fallback to default configuration"); 
            data = DataLayout();
        }
    }

    /**
     * Get pointer to configuration on instance, for raw access
    */
    DataLayout* getData() {
        return &data;
    }

    /**
     * Represent configuration in text format (Sensitive data won't be shown)
    */
    String repr() {
        String str = "VERSION: ";
        str += data.version;
        str += '\n';
        str += "WIFI: ";
        switch (data.WiFi_STATE) {
            case DISABLED:
                str += "DISABLED\n";
                break;
            case OPEN:
                str += data.WiFi_SSID;
                str += " OPEN\n";
                break;
            case PSK_ENCRYPTED:
                str += data.WiFi_SSID;
                str += " PASSWORD\n";
                break;
            default: break;
        }
        str += "ALARM 1: ";
        switch (data.Alarm1[0]) {
            case OFF:
                str += "OFF\n";
                break;
            case ONCE:
                str += data.Alarm1[1];
                str += ':';
                str += data.Alarm1[2];
                str += " ONCE\n";
                break;
            case ALL:
                str += data.Alarm1[1];
                str += ':';
                str += data.Alarm1[2];
                str += " ALL\n";
                break;
            default: break;
        }
        str += "ALARM 2: ";
        switch (data.Alarm2[0]) {
            case OFF:
                str += "OFF\n";
                break;
            case ONCE:
                str += data.Alarm2[1];
                str += ':';
                str += data.Alarm2[2];
                str += " ONCE\n";
                break;
            case ALL:
                str += data.Alarm2[1];
                str += ':';
                str += data.Alarm2[2];
                str += " ALL\n";
                break;
            default: break;
        }
        return str;
    }

    /**
     * Save configuration from instance to EEPROM
    */
    void commit() {
        const byte *ptr = (const byte*)(const void*)&data;
        for (unsigned int i = 0; i < sizeof(data); i++) {
            EEPROM.write(i, *ptr++);
        }
        EEPROM.commit();
    }

    /**
     * Mark an outdated flag to EEPROM, reboot the system with default configuration
    */
    void reset() {
        logger.info("Resetting system");
        EEPROM.write(0, 0);
        EEPROM.commit();
        ESP.reset();
    }
};