#include <Adafruit_GFX.h>
#include <Adafruit_NeoMatrix.h>
#include <Adafruit_NeoPixel.h>
#ifndef PSTR
#define PSTR // Make Arduino Due happy
#endif

#define PIN 6

#define mw 34
#define mh 17

// MATRIX DECLARATION:
// Parameter 1 = width of NeoPixel matrix
// Parameter 2 = height of matrix
// Parameter 3 = pin number (most are valid)
// Parameter 4 = matrix layout flags, add together as needed:
//   NEO_MATRIX_TOP, NEO_MATRIX_BOTTOM, NEO_MATRIX_LEFT, NEO_MATRIX_RIGHT:
//     Position of the FIRST LED in the matrix; pick two, e.g.
//     NEO_MATRIX_TOP + NEO_MATRIX_LEFT for the top-left corner.
//   NEO_MATRIX_ROWS, NEO_MATRIX_COLUMNS: LEDs are arranged in horizontal
//     rows or in vertical columns, respectively; pick one or the other.
//   NEO_MATRIX_PROGRESSIVE, NEO_MATRIX_ZIGZAG: all rows/columns proceed
//     in the same order, or alternate lines reverse direction; pick one.
//   See example below for these values in action.
// Parameter 5 = pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_GRBW    Pixels are wired for GRBW bitstream (RGB+W NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)


// Example for NeoPixel Shield.  In this application we'd like to use it
// as a 5x8 tall matrix, with the USB port positioned at the top of the
// Arduino.  When held that way, the first pixel is at the top right, and
// lines are arranged in columns, progressive order.  The shield uses
// 800 KHz (v2) pixels that expect GRB color data.
Adafruit_NeoMatrix matrix = Adafruit_NeoMatrix(mh, mw, PIN,
                            NEO_MATRIX_BOTTOM     + NEO_MATRIX_RIGHT +
                            NEO_MATRIX_COLUMNS + NEO_MATRIX_ZIGZAG,
                            NEO_GRB            + NEO_KHZ800);
//Snake game Pseudocode
//1. Have a Rectangle that if the snake hits, he dies and game stops. BONUS: If snake goes over itself, the game stops,
//2. Make it so that a food particle goes in random places. BONUS: When Snake touches it, food goes to random place and snake gets longer by 1 length.
//Cant make new fruit where snake is.


const int buttonPin = 2;

// Variables will change:
int RbuttonPushCounter = 0;   // counter for the number of button presses
int RbuttonState = 0;         // current state of the button
int RlastButtonState = 0;     // previous state of the button

int snakeX = 7;
int snakeY = 7;

void setup() {
  pinMode(buttonPin, INPUT);
  matrix.begin();
  matrix.setTextWrap(false);
  Serial.begin(9600);
  matrix.setBrightness(30);
  makeFruit();
  makeSnake();
}

void loop()
{
  Buttons();
  RbuttonState = digitalRead(buttonPin);
  matrix.drawRect(0, 0, 17, 34, matrix.Color(255, 255, 255));
  matrix.show();
  Serial.println(RbuttonState);
}

void makeFruit() {
  int x = random(1, 16), y = random(0, 34);
  matrix.drawPixel(x, y, matrix.Color(255, 0, 99));
  matrix.show();
}

void makeSnake() {
  matrix.drawPixel(snakeX, snakeY, matrix.Color(0, 0, 128));
  matrix.show();
}

void Buttons() {
  RbuttonState = digitalRead(buttonPin);
  if (RbuttonState != RlastButtonState) {
    // if the state has changed, increment the counter
    if (RbuttonState == HIGH) {
      matrix.drawPixel(snakeX, snakeY, matrix.Color(0, 0, 0));
      snakeX--;
      makeSnake();
    }
  }
}
