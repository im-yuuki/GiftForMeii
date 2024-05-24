#pragma once
#include <Arduino.h>
#include <ESP8266WiFi.h>
#include "modules/logging.hpp"
#include "modules/constant.hpp"
#include "modules/persistent.hpp"

const char AP_NAME[] = "Meikayuu <Setup>";

class WiFiManager {
    private:
    logging::Logger logger = logging::Logger("wifi");
    Persistent* persistent;

    void handleOnSTAConnected(const WiFiEventStationModeConnected &event) {
        logger.info(String("Connected to AP: ") + event.ssid + ". Channel: " + event.channel + ". IP Address: "+ WiFi.localIP().toString()); 
    }

    void handleOnSTADisconnected(const WiFiEventStationModeDisconnected &event) {
        logger.info("Connection to the AP has been lost");
    }

    void handleOnClientConnected(const WiFiEventSoftAPModeStationConnected &event) {
        logger.info(String("New client: ") + event.aid + '(' + event.mac + ')');
    }

    void handleOnClientDisconnected(const WiFiEventSoftAPModeStationDisconnected &event) {
        // logger.info();
    }

    public:
    WiFiManager(Persistent &persistent) {
        this->persistent = &persistent;
        WiFi.onStationModeConnected([this](const WiFiEventStationModeConnected &event) { this->handleOnSTAConnected(event); });
        WiFi.onStationModeDisconnected([this](const WiFiEventStationModeDisconnected &event) { this->handleOnSTADisconnected(event); });
        WiFi.onSoftAPModeStationConnected([this](const WiFiEventSoftAPModeStationConnected &event) { this->handleOnClientConnected(event); });
        WiFi.onSoftAPModeStationDisconnected([this](const WiFiEventSoftAPModeStationDisconnected &event) { this->handleOnClientDisconnected(event); });
    }
    
    /**
     * Initalize management system
    */
    void init() {
        logger.info(String("Starting AP: ") + AP_NAME);
        WiFi.softAP(AP_NAME, DEVICE_ID, 10, 0, 2);
        WiFi.enableAP(true);
        WiFi.enableInsecureWEP(true);
        logger.info("WiFiManager successfully initalized");
        connect();
    }

    void startScanTasks() {
        WiFi.scanNetworks(true, true, 0);
        // TODO: Code it
    }

    void getScanResults() {

    }

    // Reload credential and start connecting flow
    void connect() {
        WiFi.disconnect(true);
        DataLayout data = *(persistent->getData());
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

    /**
     * Connect to WiFi AP with new credentials
     * @param type encryption type of AP
     * @param ssid AP SSID (Leave blank if type is DISABLED)
     * @param password AP password (Leave blank if type is DISABLED or OPEN)
    */
    void connect(WiFiEncryption type, const char ssid[64], const char password[64]) {
        DataLayout data = *(persistent->getData());
        data.WiFi_STATE = type;
        // Copy value
        strncpy(data.WiFi_SSID, ssid, sizeof(data.WiFi_SSID) - 1);
        strncpy(data.WiFi_PASSWORD, password, sizeof(data.WiFi_PASSWORD) - 1);
        // Ensuring null termination
        data.WiFi_SSID[sizeof(data.WiFi_SSID) - 1] = '\0';
        data.WiFi_PASSWORD[sizeof(data.WiFi_PASSWORD) - 1] = '\0';
        //
        persistent->commit();
        connect();
    }
};