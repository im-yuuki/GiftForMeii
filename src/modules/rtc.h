#pragma once
#include <Arduino.h>
#include <RTClib.h>

#include "logging.h"

class RTC {
    private:
    logging::Logger logger = logging::Logger("rtc");
    bool rtc_initalized = false;
    RTC_DS3231 rtc;

    public:
    void init(bool force) {
        if (!force && this->rtc_initalized) {
            logger.warn("RTC has already initalized.");
            return;
        }
        logger.info("Initalizing RTC");
        while (!this->rtc.begin()) {
            logger.error("Cannot find RTC connected to system. Retrying.");
        }
        if (this->rtc.lostPower()) logger.warn("RTC power lost detected. Time will not correct.");
        this->rtc_initalized = true;
        logger.connectRTC(rtc);
    }

    DateTime now() {
        if (this->rtc.lostPower()) logger.warn("RTC power lost detected. Time will not correct.");
        return this->rtc.now();
    }

    String getTimeString() {
        String data = "";
        if (!this->rtc_initalized) return data;
        DateTime now = this->now();
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
};