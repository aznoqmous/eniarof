#include <Arduino.h>
#include <Adafruit_Protomatter.h>
#include "game.cpp"

const int MONITOR_SPEED = 115200;
const int GAME_SPEED_CAP = 10;

Game game = Game();

void setup() {
  Serial.begin(MONITOR_SPEED);
  Serial.println("Setup complete");
  game.start();
}

  
void loop(void) {
  game.update();
  delay(GAME_SPEED_CAP);
}