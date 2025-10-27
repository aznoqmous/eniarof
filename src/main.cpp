#include <Arduino.h>
#include <Adafruit_Protomatter.h>
#include "game.cpp"

const int MONITOR_SPEED = 115200;

Game game = Game();

const int hallPin1 = 3;
const int hallPin2 = 9;

// void inputForward(ESPRotary& r){
//   Serial.println("FORWARD");
//   game.inputForward();
// }
// void inputBackward(ESPRotary& r){
//   Serial.println("BACKWARD");
//   game.inputBackward();
// }

void setup() {
  Serial.begin(MONITOR_SPEED);
  
  Serial.println("Setup complete");
  game.start();
  
  // Adafruit_Protomatter matrix = game.renderer.matrix;
  // // matrix.show();
  // Serial.println("MAIN");
  // matrix.drawLine(0, 0, 32, 32, matrix.color565(128, 128, 128));
  // matrix.show();
  
  // pinMode(hallPin, INPUT_PULLUP);  // use internal pull-up

    // rotary.begin(hallPin1, hallPin2, 1);
    // rotary.setLeftRotationHandler(inputForward);
    // rotary.setRightRotationHandler(inputBackward);
}

  
void loop(void) {
  // rotary.loop();
  game.update();
  game.inputs();
  delay(100);
}