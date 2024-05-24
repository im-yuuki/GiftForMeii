#pragma once
#include <Arduino.h>
#include "utils/string.hpp"

const static char HEX_CHAR[] = "0123456789ABCDEF";

char* macToString(const std::array<uint8_t, 6> &macByteArray) {
    static char repr[18]; // Static buffer to hold the MAC address string
    char* iterator = repr;
    for (uint8_t i = 0; i < 6; ++i) {
        if (i != 0) *(iterator++) = ':';
        *(iterator++) = HEX_CHAR[macByteArray[i] / 16];
        *(iterator++) = HEX_CHAR[macByteArray[i] % 16];
    }
    repr[17] = '\0'; // Null-terminate the string
    return repr;
}

bool vaildateMacString(String &macString) {
    if (macString.length() != 17) return false;
    for (unsigned int i = 0; i < 17; ++i) {
        if (i % 3 == 2) if (macString[i] != ':') return false;
        else if (!isAcceptedChar(macString[i], HEX_CHAR)) return false;
    }
    return true;
}

// Please validate before use this, there is no error thrown :gg:
uint8_t* macToByteArray(String &macString) {

}