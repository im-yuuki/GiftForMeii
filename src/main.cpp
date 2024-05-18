#include <Arduino.h>
#include "modules/logging.h"

void setup() {
  logging::initLogging();
  logging::Logger logger = logging::Logger();
  logger.info("Testing");
}

void loop() {
  // put your main code here, to run repeatedly:
}