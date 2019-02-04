// scrolltext demo for Adafruit RGBmatrixPanel library.
// Demonstrates double-buffered animation on our 16x32 RGB LED matrix:
// http://www.adafruit.com/products/420

// Written by Limor Fried/Ladyada & Phil Burgess/PaintYourDragon
// for Adafruit Industries.
// BSD license, all text above must be included in any redistribution.

#include <RGBmatrixPanel.h>

// Most of the signal pins are configurable, but the CLK pin has some
// special constraints.  On 8-bit AVR boards it must be on PORTB...
// Pin 8 works on the Arduino Uno & compatibles (e.g. Adafruit Metro),
// Pin 11 works on the Arduino Mega.  On 32-bit SAMD boards it must be
// on the same PORT as the RGB data pins (D2-D7)...
// Pin 8 works on the Adafruit Metro M0 or Arduino Zero,
// Pin A4 works on the Adafruit Metro M4 (if using the Adafruit RGB
// Matrix Shield, cut trace between CLK pads and run a wire to A4).

#define CLK  8   // USE THIS ON ARDUINO UNO, ADAFRUIT METRO M0, etc.
//#define CLK A4 // USE THIS ON METRO M4 (not M0)
//#define CLK 11 // USE THIS ON ARDUINO MEGA
#define OE   9
#define LAT 10
#define A   A0
#define B   A1
#define C   A2

// Last parameter = 'true' enables double-buffering, for flicker-free,
// buttery smooth animation.  Note that NOTHING WILL SHOW ON THE DISPLAY
// until the first call to swapBuffers().  This is normal.
RGBmatrixPanel matrix(A, B, C, CLK, LAT, OE, false);
// Double-buffered mode consumes nearly all the RAM available on the
// Arduino Uno -- only a handful of free bytes remain.  Even the
// following string needs to go in PROGMEM:

// Similar to F(), but for PROGMEM string pointers rather than literals

#define F2(progmem_ptr) (const __FlashStringHelper *)progmem_ptr

const char str[] PROGMEM  = "Chuc mung nam moi 2019";
const char str2[] PROGMEM = " An khang thinh vuong";
const char str3[] PROGMEM = " Happy new year 2019";
int16_t    textX         = matrix.width(),
           textMin       = sizeof(str) * -12,
           hue           = 0;
int8_t ball[3][4] = {
  {  3,  0,  1,  1 }, // Initial X,Y pos & velocity for 3 bouncy balls
  { 17, 15,  1, -1 },
  { 27,  4, -1,  1 }
};
static const uint16_t PROGMEM ballcolor[3] = {
  0x0080, // Green=1
  0x0002, // Blue=1
  0x1000  // Red=1
};


//Tang thoi gian chay
float dem_mot = 0; int time_mot = 150;
float dem_hai = 0;

static const int8_t PROGMEM sinetab[256] = {
     0,   2,   5,   8,  11,  15,  18,  21,
    24,  27,  30,  33,  36,  39,  42,  45,
    48,  51,  54,  56,  59,  62,  65,  67,
    70,  72,  75,  77,  80,  82,  85,  87,
    89,  91,  93,  96,  98, 100, 101, 103,
   105, 107, 108, 110, 111, 113, 114, 116,
   117, 118, 119, 120, 121, 122, 123, 123,
   124, 125, 125, 126, 126, 126, 126, 126,
   127, 126, 126, 126, 126, 126, 125, 125,
   124, 123, 123, 122, 121, 120, 119, 118,
   117, 116, 114, 113, 111, 110, 108, 107,
   105, 103, 101, 100,  98,  96,  93,  91,
    89,  87,  85,  82,  80,  77,  75,  72,
    70,  67,  65,  62,  59,  56,  54,  51,
    48,  45,  42,  39,  36,  33,  30,  27,
    24,  21,  18,  15,  11,   8,   5,   2,
     0,  -3,  -6,  -9, -12, -16, -19, -22,
   -25, -28, -31, -34, -37, -40, -43, -46,
   -49, -52, -55, -57, -60, -63, -66, -68,
   -71, -73, -76, -78, -81, -83, -86, -88,
   -90, -92, -94, -97, -99,-101,-102,-104,
  -106,-108,-109,-111,-112,-114,-115,-117,
  -118,-119,-120,-121,-122,-123,-124,-124,
  -125,-126,-126,-127,-127,-127,-127,-127,
  -128,-127,-127,-127,-127,-127,-126,-126,
  -125,-124,-124,-123,-122,-121,-120,-119,
  -118,-117,-115,-114,-112,-111,-109,-108,
  -106,-104,-102,-101, -99, -97, -94, -92,
   -90, -88, -86, -83, -81, -78, -76, -73,
   -71, -68, -66, -63, -60, -57, -55, -52,
   -49, -46, -43, -40, -37, -34, -31, -28,
   -25, -22, -19, -16, -12,  -9,  -6,  -3
};
const float radius1 =65.2, radius2 =92.0, radius3 =163.2, radius4 =176.8,
            centerx1=64.4, centerx2=46.4, centerx3= 93.6, centerx4= 16.4,
            centery1=34.8, centery2=26.0, centery3= 56.0, centery4=-11.6;
float       angle1  = 0.0, angle2  = 0.0, angle3  =  0.0, angle4  =  0.0;
long        hueShift= 0;

#define FPS 15         // Maximum frames-per-second
uint32_t prevTime = 0; // For frame-to-frame interval timing

void setup() {
  matrix.begin();
  matrix.setTextWrap(false); // Allow text to run off right edge
  matrix.setTextSize(1);
}

void loop() {
  
  while(dem_mot<100)
  {
    dem_mot++;
     nam_plasma();
  }
  if (dem_mot==100) 
  {
    dem_mot=0;
   }
 
 ba_shape();

  while(dem_mot<time_mot)
  {
    dem_mot++;
    mot_chay_chu();
  }
  if (dem_mot==time_mot) 
  {
    dem_mot=0;
    textX = matrix.width();
  }

  while(dem_hai<100)
  {
    dem_hai++;
    hai_ball();
  }
  if (dem_hai==100) dem_hai=0;
  
  while(dem_mot<time_mot)
  {
    dem_mot++;
   bon_chu_ball();
  }
  if (dem_mot==time_mot) 
  {
    dem_mot=0;
    textX = matrix.width();
  }
}

void mot_chay_chu()
{


  // Clear background
  matrix.fillScreen(0);

  // Draw big scrolly text on top
  matrix.setTextColor(matrix.ColorHSV(hue, 255, 255, true));
  matrix.setCursor(textX, 0);
  matrix.print(F2(str));
 // delay(40);

  matrix.setCursor(textX, 8);
  matrix.print(F2(str2));
  delay(50);

  // Move text left (w/wrap), increase hue
  if((--textX) < textMin) textX = matrix.width();
  hue += 49;
  if(hue >= 1536) hue -= 1536;

#if !defined(__AVR__)
  // On non-AVR boards, delay slightly so screen updates aren't too quick.
  delay(20);
#endif

  // Update display
  matrix.swapBuffers(false);
}


void hai_ball ()
{
  byte i;
  matrix.fillScreen(0);

  // Bounce three balls around
  for(i=0; i<3; i++) {
    // Draw 'ball'
    matrix.fillCircle(ball[i][0], ball[i][1], 5, pgm_read_word(&ballcolor[i]));
    // Update X, Y position
    ball[i][0] += ball[i][2];
    ball[i][1] += ball[i][3];
    // Bounce off edges
    if((ball[i][0] == 0) || (ball[i][0] == (matrix.width() - 1)))
      ball[i][2] *= -1;
    if((ball[i][1] == 0) || (ball[i][1] == (matrix.height() - 1)))
      ball[i][3] *= -1;
   matrix.swapBuffers(false);
   delay(20);
  }
}

void ba_shape()
{
   matrix.begin();

  // draw a pixel in white
  matrix.drawPixel(0, 0, matrix.Color333(7, 7, 0));
  delay(500);
   
  // fix the screen with green
  matrix.fillRect(0, 0, 32, 16, matrix.Color333(0, 7, 0));
  delay(500);

  // draw a box in yellow
  matrix.drawRect(0, 0, 32, 16, matrix.Color333(7, 7, 0));
  delay(500);

  // draw an 'X' in red
  matrix.drawLine(0, 0, 31, 15, matrix.Color333(7, 0, 0));
  matrix.drawLine(31, 0, 0, 15, matrix.Color333(7, 0, 0));
  delay(500);

  // draw a blue circle
  matrix.drawCircle(7, 7, 7, matrix.Color333(0, 0, 7));
  delay(500);

  // fill a violet circle
  matrix.fillCircle(23, 7, 7, matrix.Color333(7, 0, 7));
  delay(500);

  // fill the screen with black
  matrix.fillScreen(matrix.Color333(0, 0, 0));

  // draw some text!
  matrix.setCursor(1, 0);  // start at top left, with one pixel of spacing
  matrix.setTextSize(1);   // size 1 == 8 pixels high

  // print each letter with a rainbow color
  matrix.setTextColor(matrix.Color333(0,0,7));
  matrix.print('-');
  matrix.setTextColor(matrix.Color333(7,7,0));
  matrix.print('T');
  matrix.setTextColor(matrix.Color333(7,7,7));
  matrix.print('E');
  matrix.setTextColor(matrix.Color333(4,7,0));
  matrix.print('T');
  matrix.setTextColor(matrix.Color333(0,0,7));
  matrix.print('-');

  matrix.setCursor(1, 9);  // next line
  matrix.setTextColor(matrix.Color333(7,7,7));
  matrix.print('A');
  matrix.setTextColor(matrix.Color333(7,4,7));
  matrix.print('M');
  matrix.setTextColor(matrix.Color333(0,0,7));
  matrix.print('-');
  matrix.setTextColor(matrix.Color333(4,0,7));
  matrix.print('A');
  matrix.setTextColor(matrix.Color333(7,0,4));
  matrix.print('P');
delay(3000);
}

void bon_chu_ball()
{
  byte i;

  // Clear background
  matrix.fillScreen(0);

  // Bounce three balls around
  for(i=0; i<3; i++) {
    // Draw 'ball'
    matrix.fillCircle(ball[i][0], ball[i][1], 5, pgm_read_word(&ballcolor[i]));
    // Update X, Y position
    ball[i][0] += ball[i][2];
    ball[i][1] += ball[i][3];
    // Bounce off edges
    if((ball[i][0] == 0) || (ball[i][0] == (matrix.width() - 1)))
      ball[i][2] *= -1;
    if((ball[i][1] == 0) || (ball[i][1] == (matrix.height() - 1)))
      ball[i][3] *= -1;
  }

  // Draw big scrolly text on top
  matrix.setTextColor(matrix.ColorHSV(hue, 255, 255, true));
  matrix.setCursor(textX, 1);
  matrix.print(F2(str3));
  delay(30);

  // Move text left (w/wrap), increase hue
  if((--textX) < textMin) textX = matrix.width();
  hue += 57;
  if(hue >= 1536) hue -= 1536;

#if !defined(__AVR__)
  // On non-AVR boards, delay slightly so screen updates aren't too quick.
  delay(20);
#endif

  // Update display
  matrix.swapBuffers(false);
}

void nam_plasma()
{
   int           x1, x2, x3, x4, y1, y2, y3, y4, sx1, sx2, sx3, sx4;
  unsigned char x, y;
  long          value;

  sx1 = (int)(cos(angle1) * radius1 + centerx1);
  sx2 = (int)(cos(angle2) * radius2 + centerx2);
  sx3 = (int)(cos(angle3) * radius3 + centerx3);
  sx4 = (int)(cos(angle4) * radius4 + centerx4);
  y1  = (int)(sin(angle1) * radius1 + centery1);
  y2  = (int)(sin(angle2) * radius2 + centery2);
  y3  = (int)(sin(angle3) * radius3 + centery3);
  y4  = (int)(sin(angle4) * radius4 + centery4);

  for(y=0; y<matrix.height(); y++) {
    x1 = sx1; x2 = sx2; x3 = sx3; x4 = sx4;
    for(x=0; x<matrix.width(); x++) {
      value = hueShift
        + (int8_t)pgm_read_byte(sinetab + (uint8_t)((x1 * x1 + y1 * y1) >> 4))
        + (int8_t)pgm_read_byte(sinetab + (uint8_t)((x2 * x2 + y2 * y2) >> 4))
        + (int8_t)pgm_read_byte(sinetab + (uint8_t)((x3 * x3 + y3 * y3) >> 5))
        + (int8_t)pgm_read_byte(sinetab + (uint8_t)((x4 * x4 + y4 * y4) >> 5));
      matrix.drawPixel(x, y, matrix.ColorHSV(value * 3, 255, 255, true));
      x1--; x2--; x3--; x4--;
    }
    y1--; y2--; y3--; y4--;
  }

  angle1   += 0.03;
  angle2   -= 0.07;
  angle3   += 0.13;
  angle4   -= 0.15;
  hueShift += 2;

  // To ensure that animation speed is similar on AVR & SAMD boards,
  // limit frame rate to FPS value (might go slower, but never faster).
  // This is preferable to delay() because the AVR is already plenty slow.
  uint32_t t;
  while(((t = millis()) - prevTime) < (1000 / FPS));
  prevTime = t;

  matrix.swapBuffers(false);
}

