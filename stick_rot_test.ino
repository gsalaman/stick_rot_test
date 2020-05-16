/*===========================================================
 * Tiny Stick Rotation Test code
 *  
 * Test code for the tiny-stick board.  
 * Uses a reed switch instead of a button.
 * First test:  Lights go red when near magnet.  Green when away.
 */

#include <Adafruit_NeoPixel.h>

#define LED_PIN    3
#define BUTTON_PIN 0      
#define NUMPIXELS  8
#define POT_PIN    2   // Tiny

Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, LED_PIN, NEO_GRB+NEO_KHZ800);

#define COLOR_RED     0xFF0000
#define COLOR_GREEN   0x00FF00
#define COLOR_BLUE    0x0000FF
#define COLOR_MAGENTA 0xFF00FF
#define COLOR_YELLOW  0xFFFF00
#define COLOR_CYAN    0x00FFFF
#define COLOR_BLACK   0
#define COLOR_WHITE   0xFFFFFF



/*================================================================================
 * fillAll
 */
void fillAll( uint32_t color )
{
  int i;

  for (i=0; i<NUMPIXELS; i++)
  {
    pixels.setPixelColor(i, color);
  }
}




void setup()
{
    int i;
    uint32_t led_color=COLOR_BLUE;

    #if 0
    // Uno debug 
    Serial.begin(9600);
    pinMode(BUTTON_PIN,INPUT_PULLUP);
    #endif
    
    pixels.begin();

    for (i=0; i<NUMPIXELS; i++)
    {
      pixels.setPixelColor(i,led_color);
      delay(50);
      pixels.show();
    }
    delay(100);
    
    pixels.show();
    delay(1000);
}

void loop()
{
  uint32_t color;
  
  if (digitalRead(BUTTON_PIN) == HIGH)
  {
    color = COLOR_RED;
  }
  else
  {
    color = COLOR_GREEN;
  }
  
  fillAll(color);
  pixels.show();

}
