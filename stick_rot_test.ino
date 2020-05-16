/*===========================================================
 * Tiny Stick Rotation Test code
 *  
 * Test code for the tiny-stick board.  
 * Uses a reed switch instead of a button.
 * Rotation period test:
 *   Calculate how many ms per rotation.
 *   Display on the LEDs (yay binary!)
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
  pixels.show();
}

/* get_switch_state
 *  Initially, we're just going to use this as a passthrough, but if we
 *  need to debounce, we'll do so in this funciton.
 */
int get_switch_state( void )
{
  return(digitalRead(BUTTON_PIN));
}

//==============================================================================================
// FUNCTION:  write_and_latch_byte
//
// Writes an entire byte to the RGB LEDs.  Note this function
// goes MSB first.
//==============================================================================================
#define BITS_IN_BYTE 8
void write_and_latch_byte( int data, uint32_t color )
{
  int temp_bit;
  int temp_data;
  int i;

  // Make sure it's only one byte of data...mask off the rest.
  data &= 0xFF;
    
  temp_data = data;
  
  for (i = 0; i < BITS_IN_BYTE; i++)
  {
    // we only want the MSB
    temp_bit = temp_data & 0x80;
    temp_bit = temp_bit >> 7;

    if (temp_bit) pixels.setPixelColor(i, color);
    else pixels.setPixelColor(i, 0x00);
    
    // now shift our byte to get the next bit
    temp_data = temp_data << 1;     
  }

  pixels.show();

}

//==============================================================================================
// FUNCTION:  write_and_latch_byte_lsb_first
//
// Writes an entire byte to the rgb LEDs.  Note this function
// goes LSB first.
//==============================================================================================
void write_and_latch_byte_lsb_first( int data, uint32_t color)
{
  int temp_bit;
  int temp_data;
  int i;

  // Make sure we're just grabbing one byte
  data &= 0xFF;
    
  temp_data = data;
  
  for (i = 0; i < BITS_IN_BYTE; i++)
  {
    // we only want the LSB
    temp_bit = temp_data & 0x01;

    if (temp_bit) pixels.setPixelColor(i, color);
    else pixels.setPixelColor(i, 0x00);
    
    // now shift our byte to get the next bit
    temp_data = temp_data >> 1;     
  }

  pixels.show();
  
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
    delay(1000);

}

void loop()
{
  static uint32_t last_switch_ms=0;
  static int      last_switch_pos=HIGH;
  uint32_t current_ms;
  static uint32_t last_rot_ms=10000;
  int      current_switch_pos;
  uint32_t color;
  uint32_t rps;
  
  current_ms = millis();
  current_switch_pos = get_switch_state();
  
  /* if we just switched to low, flag this as the start of a rotation */
  if ((last_switch_pos == HIGH) && (current_switch_pos == LOW))
  {
    last_rot_ms = current_ms - last_switch_ms;
    last_switch_ms = current_ms;
  }

  last_switch_pos = current_switch_pos;

  /* Display how many MS/Rotation on the LED screen. 
   *  Note...this number *should* be in the hundreds of ms range.
   *  If it's bigger than 2000 ms, make all the lights yellow.  We're stopped.
   *  If it's smaller than 50, make all the lights red.  Debounce issue.
   *  Otherwise, lets calculate rotations per second and display that.
   */
  if (last_rot_ms > 2000) 
  {
    // consider this stopped.
    fillAll(COLOR_YELLOW); 
  }
  else if (last_rot_ms < 50)
  {
    //bounce occured....we're too fast.
    fillAll(COLOR_RED);
  }
  else
  {
    rps = 1000 / last_rot_ms;
    write_and_latch_byte(rps, COLOR_GREEN);
  }

}
