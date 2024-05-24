#pragma once
#include <Arduino.h>

namespace logging {
    enum LogLevel {
        INFO,
        WARNING,
        ERROR
    };


    void append(String &loggerName, LogLevel level, String &msg) {
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

        /**
         * Get child of current logger
         * @param name Child logger name
        */
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