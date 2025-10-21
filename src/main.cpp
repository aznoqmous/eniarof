#include <Arduino.h>
#include <Adafruit_Protomatter.h>
#include "game.cpp"

const int MONITOR_SPEED = 115200;

Game game = Game();

void setup() {
  Serial.begin(MONITOR_SPEED);
  game.start();
}

void draw(){
  
  // matrix.show(); // Copy data to matrix buffers
}

void loop(void) {
  
  game.update();
  delay(200);
}