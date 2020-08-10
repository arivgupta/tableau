/*********************************************************************
  This is an example for our nRF51822 based Bluefruit LE modules

  Pick one up today in the adafruit shop!

  Adafruit invests time and resources providing this open source code,
  please support Adafruit and open-source hardware by purchasing
  products from Adafruit!

  MIT license, check LICENSE for more information
  All text above, and the splash screen below must be included in
  any redistribution
*********************************************************************/

#include <string.h>
#include <Arduino.h>
#include <SPI.h>
#include "Adafruit_BLE.h"
#include "Adafruit_BluefruitLE_SPI.h"
#include "Adafruit_BluefruitLE_UART.h"
#include <Adafruit_GFX.h>
#include <Adafruit_NeoMatrix.h>
#include <Adafruit_NeoPixel.h>

#include "BluefruitConfig.h"

#if SOFTWARE_SERIAL_AVAILABLE
#include <SoftwareSerial.h>
#endif

const int mw = 16;
const int mh = 16;

uint8_t buttnum;

//SETUP FOR SNAKE GAME BEGINS
int i; //declaring i for the for loops coming soon

int wait = 200; //created so we can easily make changes to our delay based on the size of the board

//creating variables to initialize fruit
int x = random(2, mw - 2);
int y = random(2, mh - 2);

const long MAX_SNAKE_LENGTH = 25; //creating a maximum length of snake to set a max value of the array

int snakeX[MAX_SNAKE_LENGTH];
int snakeY[MAX_SNAKE_LENGTH];

int game = 0;

int snakeLength = 1; //creating the initial length of snake, on which will be added to

int counter = 0; //initializing a counter for snake direction

void LButton();
void RButton();
void UButton();
void DButton();

//SETUP FOR SNAKE GAME ENDS
/*=========================================================================
    APPLICATION SETTINGS

      FACTORYRESET_ENABLE       Perform a factory reset when running this sketch
     
                                Enabling this will put your Bluefruit LE module
                              in a 'known good' state and clear any config
                              data set in previous sketches or projects, so
                                running this at least once is a good idea.
     
                                When deploying your project, however, you will
                              want to disable factory reset by setting this
                              value to 0.  If you are making changes to your
                                Bluefruit LE device via AT commands, and those
                              changes aren't persisting across resets, this
                              is the reason why.  Factory reset will erase
                              the non-volatile memory where config data is
                              stored, setting it back to factory default
                              values.
         
                                Some sketches that require you to bond to a
                              central device (HID mouse, keyboard, etc.)
                              won't work at all with this feature enabled
                              since the factory reset will clear all of the
                              bonding data stored on the chip, meaning the
                              central device won't be able to reconnect.
    MINIMUM_FIRMWARE_VERSION  Minimum firmware version to have some new features
    MODE_LED_BEHAVIOUR        LED activity, valid options are
                              "DISABLE" or "MODE" or "BLEUART" or
                              "HWUART"  or "SPI"  or "MANUAL"
    -----------------------------------------------------------------------*/
#define FACTORYRESET_ENABLE         1
#define MINIMUM_FIRMWARE_VERSION    "0.6.6"
#define MODE_LED_BEHAVIOUR          "MODE"
/*=========================================================================*/

// Create the bluefruit object, either software serial...uncomment these lines

/*SoftwareSerial bluefruitSS = SoftwareSerial(BLUEFRUIT_SWUART_TXD_PIN, BLUEFRUIT_SWUART_RXD_PIN);

  Adafruit_BluefruitLE_UART ble(bluefruitSS, BLUEFRUIT_UART_MODE_PIN,
                      BLUEFRUIT_UART_CTS_PIN, BLUEFRUIT_UART_RTS_PIN);
  /*

  /* ...or hardware serial, which does not need the RTS/CTS pins. Uncomment this line */
// Adafruit_BluefruitLE_UART ble(BLUEFRUIT_HWSERIAL_NAME, BLUEFRUIT_UART_MODE_PIN);

/* ...hardware SPI, using SCK/MOSI/MISO hardware SPI pins and then user selected CS/IRQ/RST */
Adafruit_BluefruitLE_SPI ble(BLUEFRUIT_SPI_CS, BLUEFRUIT_SPI_IRQ, BLUEFRUIT_SPI_RST);

/* ...software SPI, using SCK/MOSI/MISO user-defined SPI pins and then user selected CS/IRQ/RST */
//Adafruit_BluefruitLE_SPI ble(BLUEFRUIT_SPI_SCK, BLUEFRUIT_SPI_MISO,
//                             BLUEFRUIT_SPI_MOSI, BLUEFRUIT_SPI_CS,
//                             BLUEFRUIT_SPI_IRQ, BLUEFRUIT_SPI_RST);

Adafruit_NeoMatrix matrix = Adafruit_NeoMatrix(mh, mw, 13,
                            NEO_MATRIX_TOP     + NEO_MATRIX_RIGHT +
                            NEO_MATRIX_ROWS + NEO_MATRIX_ZIGZAG,
                            NEO_GRB            + NEO_KHZ800);

// A small helper
void error(const __FlashStringHelper*err) {
  Serial.println(err);
  while (1);
}

// function prototypes over in packetparser.cpp
uint8_t readPacket(Adafruit_BLE *ble, uint16_t timeout);
uint8_t readButtons(Adafruit_BLE *ble, uint16_t timeout);
float parsefloat(uint8_t *buffer);
void printHex(const uint8_t * data, const uint32_t numBytes);

// the packet buffer
extern uint8_t packetbuffer[];



/**************************************************************************/
/*!
    @brief  Sets up the HW an the BLE module (this function is called
            automatically on startup)
*/
/**************************************************************************/
void setup(void)
{
  //while (!Serial);  // required for Flora & Micro
  //delay(500);
  matrix.begin();
  matrix.show();
  matrix.setBrightness(50);
  matrix.setTextWrap(false);
  Serial.begin(115200);
  Serial.println(F("Adafruit Bluefruit App Controller Example"));
  Serial.println(F("-----------------------------------------"));

  /* Initialise the module */
  Serial.print(F("Initialising the Bluefruit LE module: "));

  if ( !ble.begin(VERBOSE_MODE) )
  {
    error(F("Couldn't find Bluefruit, make sure it's in CoMmanD mode & check wiring?"));
  }
  Serial.println( F("OK!") );

  if ( FACTORYRESET_ENABLE )
  {
    /* Perform a factory reset to make sure everything is in a known state */
    Serial.println(F("Performing a factory reset: "));
    if ( ! ble.factoryReset() ) {
      error(F("Couldn't factory reset"));
    }
  }


  /* Disable command echo from Bluefruit */
  ble.echo(false);

  Serial.println("Requesting Bluefruit info:");
  /* Print Bluefruit information */
  ble.info();

  Serial.println(F("Please use Adafruit Bluefruit LE app to connect in Controller mode"));
  Serial.println(F("Then activate/use the sensors, color picker, game controller, etc!"));
  Serial.println();

  ble.verbose(false);  // debug info is a little annoying after this point!

  /* Wait for connection */
  while (! ble.isConnected()) {
    delay(500);
  }

  Serial.println(F("******************************"));

  // LED Activity command is only supported from 0.6.6
  if ( ble.isVersionAtLeast(MINIMUM_FIRMWARE_VERSION) )
  {
    // Change Mode LED Activity
    Serial.println(F("Change LED activity to " MODE_LED_BEHAVIOUR));
    ble.sendCommandCheckOK("AT+HWModeLED=" MODE_LED_BEHAVIOUR);
  }

  // Set Bluefruit to DATA mode
  Serial.println( F("Switching to DATA mode!") );
  ble.setMode(BLUEFRUIT_MODE_DATA);

  Serial.println(F("******************************"));

}

/**************************************************************************/
/*!
    @brief  Constantly poll for new command or response data
*/
/**************************************************************************/

void general();

void loop() {
  general();
}

void general() {
  while (true)
  {
    /* Wait for new data to arrive */
    uint8_t len = readPacket(&ble, BLE_READPACKET_TIMEOUT);
    if (len == 0) return;

    /* Got a packet! */
    // printHex(packetbuffer, len);
    // Color

    if (packetbuffer[1] == 'C') {
      uint8_t red = packetbuffer[2];
      uint8_t green = packetbuffer[3];
      uint8_t blue = packetbuffer[4];
      matrix.fillScreen(matrix.Color(red, green, blue));
      matrix.show();
      Serial.println("Color sent!");
    }

    // Buttons
    if (packetbuffer[1] == 'B') {
      buttnum = packetbuffer[2] - '0';
      boolean pressed = packetbuffer[3] - '0';
      Serial.print ("Button "); Serial.print(buttnum);
      if (game == 1)
      {
        if (buttnum == 7)
        {
          counter = 1;
          RButton();
        }
        else if (buttnum == 8)
        {
          counter = 2;
          LButton();
        }
        else if (buttnum == 6)
        {
          counter = 3;
          UButton();
        }
        else if (buttnum == 5)
        {
          counter = 1;
          DButton();
        }
        matrix.show();
      }
      if (buttnum == 1)
      {
        if (pressed) {
          game = 1;
          snakeBegin();
        }
      }
    }
  }
}

void snakeBegin() {
  matrix.clear();
  initializeFruit(); //check below, this initializes the fruit for the snake to eat
  matrix.drawRect(0, 0, mw, mh, matrix.Color(255, 0, 0)); //this creates a boundary
  snakeX[0] = 4;
  snakeY[0] = 7;
  makeSnake();
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


void GameOver();
void checkFruit();

void RButton() { //reading the right button
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
    general();
    GameOver();
  }
}
void LButton() { //reading the left button
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
    general();
    GameOver();
  }
}

void UButton() { //reading the up button
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
    general();
    GameOver();
  }
}

void DButton() { //reading the down button
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
    general();
    GameOver();
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
    RButton();
    LButton();
    UButton();
    DButton();
  }
  else {
    return;
  }
}

void GameOver() { //code checking if the user lost
  if (snakeX[0] == 0 || snakeX[0] == mw || snakeY[0] == 0 || snakeY[0] == mh) { //checking whether the snake hit the border
    matrix.clear();
    matrix.fillRect(0, 0, mw, mh, matrix.Color(246, 70, 91));
    matrix.show();
    game = 0;
    delay(500);
    matrix.clear();
    general();
  }
  else { //checking whether the snake hit itself
    for (i = 1; i <= snakeLength - 1; i++) {
      if (snakeX[0] == snakeX[i] && snakeY[0] == snakeY[i]) {
        matrix.clear();
        matrix.fillRect(0, 0, mw, mh, matrix.Color(246, 70, 91));
        matrix.show();
        game = 0;
        delay(500);
        matrix.clear();
        general();
      }
      else {
        continue;
      }
    }
  }
}
