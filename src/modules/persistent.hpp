#pragma once
#include <Arduino.h>
#include <EEPROM.h>
#include "modules/logging.hpp"

const uint8_t CONFIG_VERSION = 2; 

enum WiFiEncryption : uint8_t {
    DISABLED,
    OPEN,
    PSK_ENCRYPTED
};

struct DataLayout {
    char PASSWORD[16];
    WiFiEncryption WiFi_STATE;
    char WiFi_SSID[64];
    char WiFi_PASSWORD[64];
};


const DataLayout DEFAULT_DATA {
    "meikayuu",
    DISABLED,
    "", "",
};

/**
 * Represent configuration in text format (Sensitive data won't be shown)
*/
String dataRepr(DataLayout &data) {
    String str = "VERSION: ";
    str += CONFIG_VERSION;
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
    return str;
}


class Persistent {
    private:
    DataLayout data;
    logging::Logger logger = logging::Logger("persistent");

    /**
     * Read data from EEPROM to instance
    */
    uint8_t readVersionByte() {
        return EEPROM.read(0);
    }

    void readEEPROM() {
        byte *ptr = (byte*)(void*)&data;
        for (unsigned int i = 1; i <= sizeof(data); ++i) {
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
        if (readVersionByte() != CONFIG_VERSION) {
            logger.warn("Fallback to default configuration"); 
            data = DEFAULT_DATA;
        }
        else {
            readEEPROM();
            logger.info("Persistents loaded sucessful");
        }
        logger.info(dataRepr(data));
    }

    /**
     * Get pointer to configuration on instance, for raw access
    */
    DataLayout* getData() {
        return &data;
    }

    /**
     * Save configuration from instance to EEPROM
    */
    void commit() {
        const byte *ptr = (const byte*)(const void*)&data;
        for (unsigned int i = 1; i <= sizeof(data); ++i) {
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