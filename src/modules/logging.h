#pragma once
#include <Arduino.h>
#include <RTClib.h>

namespace logging {
    static RTC_DS3231* rtcPointer = nullptr;

    enum LogLevel {
        INFO,
        WARNING,
        ERROR
    };

    String getTimeString() {
        String data = "";
        if (rtcPointer == nullptr) return data;
        RTC_DS3231 rtcInstance = *rtcPointer;
        DateTime now = rtcInstance.now();
        data += String(now.day());
        data += '-';
        data += String(now.month());
        data += '-';
        data += String(now.year());
        data += ' ';
        data += String(now.hour());
        data += ':';
        data += String(now.minute());
        data += ':';
        data += String(now.second());
        data += ' ';
        return data;
    }

    void append(String loggerName, LogLevel level, String msg) {
        if (!Serial.availableForWrite()) return;
        Serial.print(loggerName + ' ');
        switch (level) {
            case INFO:
                Serial.print("[INFO] ");
                break;
            
            case WARNING:
                Serial.print("[WARNING] ");
                break;

            case ERROR:
                Serial.print("[ERROR] ");
                break;

            default: break;
        }
        Serial.println(msg);
    }

    class Logger {
        private:
        String name;

        public:
        Logger() {
            this->name = "root";
        }

        Logger(String name) {
            this->name = name;
        }

        void connectRTC(RTC_DS3231 rtc) {
            rtcPointer = &rtc;
        }

        Logger getSubLogger(String name) {
            return Logger(this->name + '.' + name);
        }

        void info(String msg) {
            return append(this->name, INFO, msg);
        }

        void error(String msg) {
            return append(this->name, ERROR, msg);
        }

        void warn(String msg) {
            return append(this->name, WARNING, msg);
        }
    };
}