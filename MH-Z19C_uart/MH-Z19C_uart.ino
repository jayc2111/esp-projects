#include "MH-Z19C.hpp"

co2_sensor co2(4, 2);

//#define DEBUG 1
//#define Serial if(DEBUG)Serial


// the setup routine runs once when you press reset:
void setup() {
  Serial.begin(115200);

  while (!Serial);
    Serial.println("");

  Serial.println("--- init done ---");
}


void loop() {
  co2.readCo2();  
  delay(30000);
}
