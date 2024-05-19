#pragma once
#include <Arduino.h>
#include <ESP8266WiFi.h>
#include "modules/logging.h"
#include "modules/constant.h"
#include "modules/persistent.h"

const char AP_NAME[] = "Meikayuu [CONFIGURATION]";

class WiFiManager {
    private:
    logging::Logger logger = logging::Logger("wifi");
    Persistent* persistent;

    void onConnected(const WiFiEventStationModeConnected &event) {
        logger.info(String("AP connected. IP address: ") + WiFi.localIP().toString()); 
    }

    public:
    WiFiManager(Persistent* persistent) {
        this->persistent = persistent;
        logger.info(String("Starting AP: ") + AP_NAME);
        WiFi.softAP(AP_NAME, DEVICE_ID, 10, 0, 2);
        WiFi.enableAP(true);
        WiFi.enableInsecureWEP(true);
        // std::function<void(const WiFiEventStationModeConnected &)> func = onConnected;

        WiFi.onStationModeConnected(func);
    }

    void scan() {
        // TODO: Code it
    }

    void connect() {
        WiFi.disconnect(true);
        DataLayout data = *((*persistent).getData());
        switch (data.WiFi_STATE) {
            case DISABLED:
                logger.info("Disabled connection to AP");
                break;
            case OPEN:
                logger.info(String("Connecting to open AP: ") + data.WiFi_SSID);
                WiFi.begin(data.WiFi_SSID);
                WiFi.enableSTA(true);
                break;
            case PSK_ENCRYPTED:
                logger.info(String("Connecting to password-protected AP: ") + data.WiFi_SSID);
                WiFi.begin(data.WiFi_SSID, data.WiFi_PASSWORD);
                WiFi.enableSTA(true);
                break;
            default: break;
        }
    }

    void connect(WiFiEncryption type, const char ssid[64], const char password[64]) {
        Persistent config = *persistent;
        DataLayout data = *(config.getData());
        data.WiFi_STATE = type;
        for (uint8_t i = 0; i < 64; ++ i) {
            data.WiFi_SSID[i] = ssid[i];
            data.WiFi_PASSWORD[i] = password[i];
        }
        config.commit();
        connect();
    }
};