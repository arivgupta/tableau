//Snake Game
//By: Ariv Gupta, @arivgups
//For Neopixel Matrix Rigged up with Strips or Just a Plain Old Matrix (buttons might change based on which you decide to use)
//Utilizes Pushbuttons. Would love to see your iteration!
//thanks to adityamittal13 for his conceptual help!

#include <Adafruit_GFX.h>
#include <Adafruit_NeoMatrix.h>
#include <Adafruit_NeoPixel.h>

#ifndef PSTR
#define PSTR // Make Arduino Due happy
#endif

#define PIN 6

#define mw 16 //Width of Matrix, In my case, the Y Axis
#define mh 16//Height of Matrix, In my case, the X Axis

// MATRIX DECLARATION(Taken from Neopixel Library):
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

//initialize it the way it suits you

Adafruit_NeoMatrix matrix = Adafruit_NeoMatrix(mh, mw, PIN,
                            NEO_MATRIX_TOP     + NEO_MATRIX_RIGHT +
                            NEO_MATRIX_COLUMNS + NEO_MATRIX_ZIGZAG,
                            NEO_GRB            + NEO_KHZ800);

int i; //declaring i for the for loops coming soon

int wait = 250; //created so we can easily make changes to our delay based on the size of the board

//creating variables to initialize fruit
int x = random(2, mw - 2); 
int y = random(2, mh - 2); 

//labeling the pins that each button is connected to
const int RbuttonPin = 12;
const int LbuttonPin = 4;
const int UbuttonPin = 8; //flip if using normal matrix
const int DbuttonPin = 7;

int RbuttonState = 0;         // current state of the button

int LbuttonState = 0;         // current state of the button

int UbuttonState = 0;         // current state of the button

int DbuttonState = 0;         // current state of the button

const long MAX_SNAKE_LENGTH = 25; //creating a maximum length of snake to set a max value of the array

int snakeX[MAX_SNAKE_LENGTH];
int snakeY[MAX_SNAKE_LENGTH];

int snakeLength = 1; //creating the initial length of snake, on which will be added to

int counter = 0; //initializing a counter for snake direction

void setup() {
  pinMode(RbuttonPin, INPUT);
  pinMode(LbuttonPin, INPUT);
  pinMode(UbuttonPin, INPUT);
  pinMode(DbuttonPin, INPUT);
  matrix.begin();
  matrix.setTextWrap(false);
  Serial.begin(9600);
  matrix.setBrightness(60);
  initializeFruit(); //check below, this initializes the fruit for the snake to eat
  matrix.drawRect(0, 0, mw, mh, matrix.Color(255, 0, 0)); //this creates a boundary
  snakeX[0] = 4;
  snakeY[0] = 7;
  makeSnake();
}

void loop() //running code to check buttons. From there, code will loop through and check for other values
{
  RButton();
  LButton();
  UButton();
  DButton();
  matrix.show();
}

void initializeFruit() {
  matrix.drawPixel(x, y, matrix.Color(0, 255, 0));
  matrix.show();
}


void makeSnake() {
  for (i = 0; i <= snakeLength - 1; i++) {
    matrix.drawPixel(snakeX[i], snakeY[i], matrix.Color(255, 0, 99));
    matrix.show();
  }
}

void resetFruit() { //resets the the fruit
  x = 0;
  y = 0;
  x = random(2, mw - 2); 
  y = random(2, mh - 2); 
  matrix.drawPixel(x, y, matrix.Color(0, 255, 0));
  matrix.show();
}


void GameOver() { //code checking if the user lost
  if (snakeX[0] == 0 || snakeX[0] == mw || snakeY[0] == 0 || snakeY[0] == mh) { //checking whether the snake hit the border
    matrix.clear();
    matrix.fillRect(0, 0, mw, mh, matrix.Color(246, 70, 91));
    matrix.show();
    exit(1);
  }
  else { //checking wether the snake hit itself
    for (i = 1; i <= snakeLength - 1; i++) {
      if (snakeX[0] == snakeX[i] && snakeY[0] == snakeY[i]) {
        matrix.clear();
        matrix.fillRect(0, 0, mw, mh, matrix.Color(246, 70, 91));
        matrix.show();
        exit(1);
      }
      else {
        continue;
      }
    }
  }
}


void RButton() { //reading the right button
  RbuttonState = digitalRead(RbuttonPin);
  if (RbuttonState == HIGH) {
    counter = 1;
    Serial.println(counter);
    while (counter = 1) {
      matrix.drawPixel(snakeX[0], snakeY[0], matrix.Color(0, 0, 0));
      snakeX[0]--;
      for (i = 1; i <= snakeLength - 1; i++) {
        matrix.drawPixel(snakeX[i], snakeY[i], matrix.Color(0, 0, 0));
        if (snakeX[i] - snakeX[i - 1] == 2) {
          snakeX[i]--;
        }
        if (snakeX[i] - snakeX[i - 1] == -2) {
          snakeX[i]++;
        }
        else if (snakeY[i] - snakeY[i - 1] == -2 || snakeY[i] - snakeY[i - 1] == -1) {
          snakeY[i]++;
        }
        else if (snakeY[i] - snakeY[i - 1] == 2 || snakeY[i] - snakeY[i - 1] == 1) {
          snakeY[i]--;
        }
      }
      makeSnake();
      delay(wait);
      checkFruit();
      LButton();
      UButton();
      DButton();
      GameOver();
    }
  }
}
void LButton() { //reading the left button
  LbuttonState = digitalRead(LbuttonPin);
  if (LbuttonState == HIGH) {
    counter = 2;
    Serial.println(counter);
    while (counter = 2) {
      matrix.drawPixel(snakeX[0], snakeY[0], matrix.Color(0, 0, 0));
      snakeX[0]++;
      for (i = 1; i <= snakeLength - 1; i++) {
        matrix.drawPixel(snakeX[i], snakeY[i], matrix.Color(0, 0, 0));
        if (snakeX[i] - snakeX[i - 1] == -2) {
          snakeX[i]++;
        }
        if (snakeX[i] - snakeX[i - 1] == 2) {
          snakeX[i]--;
        }
        else if (snakeY[i] - snakeY[i - 1] == -2 || snakeY[i] - snakeY[i - 1] == -1) {
          snakeY[i]++;
        }
        else if (snakeY[i] - snakeY[i - 1] == 2 || snakeY[i] - snakeY[i - 1] == 1) {
          snakeY[i]--;
        }
      }
      makeSnake();
      delay(wait);
      checkFruit();
      RButton();
      UButton();
      DButton();
      GameOver();
    }
  }
}
void UButton() { //reading the up button

  UbuttonState = digitalRead(UbuttonPin);

  if (UbuttonState == HIGH) {
    counter = 3;
    Serial.println(counter);
    while (counter = 3) {
      matrix.drawPixel(snakeX[0], snakeY[0], matrix.Color(0, 0, 0));
      snakeY[0]++;
      for (i = 1; i <= snakeLength - 1; i++) {
        matrix.drawPixel(snakeX[i], snakeY[i], matrix.Color(0, 0, 0));
        if (snakeY[i] - snakeY[i - 1] == -2) {
          snakeY[i]++;
        }
        if (snakeY[i] - snakeY[i - 1] == 2) {
          snakeY[i]--;
        }
        else if (snakeX[i] - snakeX[i - 1] == 2 || snakeX[i] - snakeX[i - 1] == 1) {
          snakeX[i]--;
        }
        else if (snakeX[i] - snakeX[i - 1] == -2 || snakeX[i] - snakeX[i - 1] == -1) {
          snakeX[i]++;
        }
      }
      makeSnake();
      delay(wait);
      checkFruit();
      RButton();
      LButton();
      DButton();
      GameOver();
    }
  }
}
void DButton() { //reading the doen button
  DbuttonState = digitalRead(DbuttonPin);

  if (DbuttonState == HIGH) {
    counter = 4;
    Serial.println(counter);
    while (counter = 4) {
      matrix.drawPixel(snakeX[0], snakeY[0], matrix.Color(0, 0, 0));
      snakeY[0]--;
      for (i = 1; i <= snakeLength - 1; i++) {
        matrix.drawPixel(snakeX[i], snakeY[i], matrix.Color(0, 0, 0));
        if (snakeY[i] - snakeY[i - 1] == 2) {
          snakeY[i]--;
        }
        if (snakeY[i] - snakeY[i - 1] == -2) {
          snakeY[i]++;
        }
        else if (snakeX[i] - snakeX[i - 1] == 2 || snakeX[i] - snakeX[i - 1] == 1) {
          snakeX[i]--;
        }
        else if (snakeX[i] - snakeX[i - 1] == -2 || snakeX[i] - snakeX[i - 1] == -1) {
          snakeX[i]++;
        }
      }
      makeSnake();
      delay(wait);
      checkFruit();
      RButton();
      LButton();
      UButton();
      GameOver();
    }
  }
}

void checkFruit() {
  if (snakeX[0] == x && snakeY[0] == y) {
    snakeLength++;
    wait = wait - snakeLength * 3; //the longer the snake, the longer the code takes to iterate over and "move" the snakes individual parts, thus the delay amount gets quicker
    
    //next function shifts all values of snake 1 ahead to create room to add 1 value of the snake to the front.
    //at the moment there is a design flaw in our work that caused us to alter the new location of each fruit. see if you can find it! would love your ideas on how to solve it.
    int temp = snakeX[0];
    int temp2;
    int temp3 = snakeY[0];
    int temp4;
    for (i = 0; i <= snakeLength - 1; i++) {
      temp2 = snakeX[i + 1];
      snakeX[i + 1] = temp;
      temp = temp2;
      temp4 = snakeY[i + 1];
      snakeY[i + 1] = temp3;
      temp3 = temp4;
    }

    //this series of if statements check which direction the snake is moving in before setting a value for the head of the snake
    
    if (counter == 1) { 
      snakeX[0] = snakeX[1] - 1;
    }
    else if (counter == 2) {
      snakeX[0] = snakeX[1] + 1;
    }
    else if (counter == 3) {
      snakeY[0] = snakeY[1] + 1;
    }
    else if (counter == 4) {
      snakeY[0] = snakeY[1] - 1;
    }
    makeSnake();
    resetFruit();
    delay(500);
    matrix.show();
  }
  else {
    return;
  }
}
