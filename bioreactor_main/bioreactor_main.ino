#include <dht.h>

dht DHT;

#define DHT11_PIN 7

void setup() {
  // put your setup code here, to run once:

}

void loop() {
  // put your main code here, to run repeatedly:
  int chk = DHT.read11(DHT11_PIN);
  delay(1000);

}
