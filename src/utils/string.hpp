#pragma once
#include <Arduino.h>

/**
 * Check if a character is in the whitelist (accepted character) or not.
 * DO NOT USE IT TOO MUCH AS IT IS REALLY HEAVY FOR SMALL SYSTEM!
 * @param a character to check
 * @param whiteList character whitelist
*/
bool isAcceptedChar(char a, String whiteList) {
    for (unsigned int i = 0; i < whiteList.length(); ++i) if (a == whiteList[i]) return true;
    return false;
}

/**
 * Check if all characters in the string are accepted characters or not.
 * DO NOT USE IT TOO MUCH AS IT IS REALLY HEAVY FOR SMALL SYSTEM!
 * @param str string to check
 * @param whiteList character whitelist
*/
bool isAcceptedString(String &str, String whiteList) {
    for (unsigned int i = 0; i < str.length(); ++i) if (!isAcceptedChar(str[i], whiteList)) return false;
    return true;
}