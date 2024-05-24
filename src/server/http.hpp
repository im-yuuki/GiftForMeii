#pragma once
#include <Arduino.h>
#include <ESP8266WebServer.h>
#include "modules/persistent.hpp"
#include "modules/wifi.hpp"

class HttpServer {
    private:
    ESP8266WebServer server = ESP8266WebServer(80);
    Persistent* persistent;
    WiFiManager* wifi;


    void handleNotFound() {   
        server.send(404, "text/plain", "NOT FOUND");
    }

    public:
    HttpServer(Persistent &persistent, WiFiManager &wifiManager) {
        this->persistent = &persistent;
        this->wifi = &wifiManager;
    }

    void init() {
        server.onNotFound([this]() { this->handleNotFound(); });
    }

    /**
     * Loop task
    */
    void loop() {
        server.handleClient();
    }
};