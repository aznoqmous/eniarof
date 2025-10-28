#include <Arduino.h>
#include <Adafruit_Protomatter.h>
#include "game.cpp"

const int MONITOR_SPEED = 115200;

Game game = Game();

const int hallPin1 = 3;
const int hallPin2 = 9;

void setup() {
  Serial.begin(MONITOR_SPEED);
  
  Serial.println("Setup complete");
  game.start();
}

  
void loop(void) {
  game.update();
  delay(100);
}