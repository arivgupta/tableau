#include <Adafruit_NeoPixel.h>
//Hacked from the original Adafruit library demo
 
#define PIN 6   //my control pin
 
// Parameter 1 = number of pixels in strip
// Parameter 2 = pin number (most are valid)
// Parameter 3 = pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
Adafruit_NeoPixel strip = Adafruit_NeoPixel(700, PIN, NEO_GRB + NEO_KHZ800);


void setup() {
  strip.begin();
  strip.setBrightness(10);
  strip.show(); // Initialize all pixels to 'off'
}



void rainbow_cycle(){
    //Start out with a purple color
  strip.fill(strip.Color(255, 0 , 0),0);
  strip.show();
  delay(5000);
  strip.fill(strip.Color(255, 127, 0),0);
  strip.show();
  delay(5000);
  strip.fill(strip.Color(255, 255, 0),0);
  strip.show();
  delay(5000);
  strip.fill(strip.Color(0, 255, 0),0);
  strip.show();
  delay(5000);
  strip.fill(strip.Color(0, 0, 255),0);
  strip.show();
  delay(5000);
  strip.fill(strip.Color(46, 43, 95),0);
  strip.show();
  delay(5000);
  strip.fill(strip.Color(139, 0, 255),0);
  strip.show();
  delay(5000);
}


// Rainbow cycle along whole strip. Pass delay time (in ms) between frames.
void rainbow(int wait) {
  // Hue of first pixel runs 5 complete loops through the color wheel.
  // Color wheel has a range of 65536 but it's OK if we roll over, so
  // just count from 0 to 5*65536. Adding 256 to firstPixelHue each time
  // means we'll make 5*65536/256 = 1280 passes through this outer loop:
  for(long firstPixelHue = 0; firstPixelHue < 5*65536; firstPixelHue += 256) {
    for(int i=0; i<strip.numPixels(); i++) { // For each pixel in strip...
      // Offset pixel hue by an amount to make one full revolution of the
      // color wheel (range of 65536) along the length of the strip
      // (strip.numPixels() steps):
      int pixelHue = firstPixelHue + (i * 65536L / strip.numPixels());
      // strip.ColorHSV() can take 1 or 3 arguments: a hue (0 to 65535) or
      // optionally add saturation and value (brightness) (each 0 to 255).
      // Here we're using just the single-argument hue variant. The result
      // is passed through strip.gamma32() to provide 'truer' colors
      // before assigning to each pixel:
      strip.setPixelColor(i, strip.gamma32(strip.ColorHSV(pixelHue)));
    }
    strip.show(); // Update strip with new contents
    delay(wait);  // Pause for a moment
  }
}

  void colorWipe(uint32_t color, int wait) {
  for(int i=0; i<strip.numPixels(); i++) { // For each pixel in strip...
    strip.setPixelColor(i, color);         //  Set pixel's color (in RAM)
    strip.show();                          //  Update strip to match
    delay(wait);                           //  Pause for a moment
  }
}

void fadeStrip(int r1, int g1, int b1, int r2, int g2, int b2) {
  int steps = strip.numPixels();
  for (int i = 0; i < steps; i++) {
    uint8_t red = (((r1 * (steps - i)) + (r2 * i)) / steps);
    uint8_t green = (((g1 * (steps - i)) + (g2 * i)) / steps);
    uint8_t blue = (((b1 * (steps - i)) + (b2 * i)) / steps);
    // Sets the pixels to the color adjusted in the fade
    strip.setPixelColor(i, red, green, blue);
  }
    strip.show();
}


 
void loop() {
  rainbow(5);
}
