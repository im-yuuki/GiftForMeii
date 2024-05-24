#pragma once
#include <Arduino.h>
#include "modules/logging.hpp"
#include "modules/persistent.hpp"
#include "modules/wifi.hpp"
#include "server/http.hpp"


class System {
	private:
	logging::Logger logger = logging::Logger();

	Persistent persistent = Persistent();
	WiFiManager wifi = WiFiManager(persistent);
	HttpServer http = HttpServer(persistent, wifi);

	public:
	/**
	 * Initalize the system instance
	*/
	void init() {
		logger.info("Starting system");
		persistent.init();
		wifi.init();
		http.init();
		logger.info("Finished initalize the system");
	}
	
	/**
	 * Loop for task
	*/
	void loop() {
		http.loop();
	}
};


System sys;

void setup() {
	Serial.begin(115200);
	Serial.println("\n\n\n\n\n\n\n\n\n-----------------------------------");
	sys.init();
}

void loop() {
	sys.loop();
}