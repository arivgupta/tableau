/*
  Button

  Turns on and off a light emitting diode(LED) connected to digital pin 13,
  when pressing a pushbutton attached to pin 2.

  The circuit:
  - LED attached from pin 13 to ground
  - pushbutton attached to pin 2 from +5V
  - 10K resistor attached to pin 2 from ground

  - Note: on most Arduinos there is already an LED on the board
    attached to pin 13.

  created 2005
  by DojoDave <http://www.0j0.org>
  modified 30 Aug 2011
  by Tom Igoe

  This example code is in the public domain.

  http://www.arduino.cc/en/Tutorial/Button
*/

// constants won't change. They're used here to set pin numbers:

int counter = 0;
const int RbuttonPin = 12;
const int LbuttonPin = 4;
const int UbuttonPin = 8;
const int DbuttonPin = 7;

// Variables will change:
int RbuttonState = 0;         // current state of the button

int LbuttonState = 0;         // current state of the button

int UbuttonState = 0;         // current state of the button

int DbuttonState = 0;         // current state of the button


void setup() {
  // initialize the pushbutton pin as an input:
  pinMode(RbuttonPin, INPUT);
  pinMode(LbuttonPin, INPUT);
  pinMode(UbuttonPin, INPUT);
  pinMode(DbuttonPin, INPUT);
  Serial.begin(9600);
}

void loop() {
  RbuttonState = digitalRead(RbuttonPin);
  LbuttonState = digitalRead(LbuttonPin);
  UbuttonState = digitalRead(UbuttonPin);
  DbuttonState = digitalRead(DbuttonPin);

  if (RbuttonState == HIGH) {
    counter = 1;
    Serial.println(counter);
  }

  if (LbuttonState == HIGH) {
    counter = 2;
    Serial.println(counter);
  }

  if (UbuttonState == HIGH) {
    counter = 3;
    Serial.println(counter);
  }

  if (DbuttonState == HIGH) {
    counter = 4;
    Serial.println(counter);
  }

}
