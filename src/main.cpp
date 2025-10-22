#include <Arduino.h>
#include <Adafruit_Protomatter.h>
#include "game.cpp"

const int MONITOR_SPEED = 115200;

Game game = Game();

void setup() {
  Serial.begin(MONITOR_SPEED);

  Serial.println("Setup complete");
  game.start();

  // Adafruit_Protomatter matrix = game.renderer.matrix;
  // // matrix.show();
  // Serial.println("MAIN");
  // matrix.drawLine(0, 0, 32, 32, matrix.color565(128, 128, 128));
  // matrix.show();
  
}
void loop(void) {
  // Serial.println("Setup complete");
  game.update();
  delay(100);
}