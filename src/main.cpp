#include <Arduino.h>
// #include <Wire.h>
#include "modules/logging.h"
// #include "modules/rtc.h"

logging::Logger logger = logging::Logger();
// RTC rtc = RTC();

void setup() {
	Serial.begin(115200);
	// Wire.begin();
	Serial.println("\n\n\n\n\n\n\n\n\n-----------------------------------");
	// rtc.init(false);
	logger.info("Testing");
}

void loop() {
	// put your main code here, to run repeatedly:
}