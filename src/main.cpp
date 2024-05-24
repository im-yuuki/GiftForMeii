#include <Arduino.h>
#include "modules/logging.h"
#include "modules/persistent.h"
#include "modules/wifi.h"

class System {
	private:
	logging::Logger logger = logging::Logger();
	Persistent persistent = Persistent();
	WiFiManager wifi = WiFiManager(&persistent);

	public:
	/**
	 * Initalize the system instance
	*/
	void init() {
		logger.info("Starting system");
		persistent.init();
		wifi.init();
		logger.info("Finished initalize the system");
	}
	
	/**
	 * Loop for task
	*/
	void loop() {
		wifi.loop();
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