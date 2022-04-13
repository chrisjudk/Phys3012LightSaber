//LightSaber V3.3
#include <FastLED.h>


//define number of LEDs on strip
#define NUM_LEDS 120
//#define BRIGHTNESS 100 //max 255

//bool bsA = false;
//bool bsB = false;
//bool bsC = false;
//bool bsAprev= false;
//bool bsBprev= false;

//Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);



/*
################################################################################
configable Variables
*/
//pins
  #define LED_PIN 6
  //int iA = 2;//rotary encoder dataA
  //int iB = 4;//rotary encoder dataB
  #define TOGGLE 3//toggle on function
  #define ANALOG_IN A5//input from potentiometer


#define MY_BRIGHTNESS 180//brightness 0-255
#define NUM_COLORS 11
const int t = 5;//delay time as we step through to get to the correct color



/*
################################################################################
Variables
*/
CRGB leds[NUM_LEDS];
uint16_t pot = 0;
bool isOn = false;
bool wasOn = true;
CHSV myColor[NUM_COLORS];
CRGB off = CRGB(0,0,0);

/*
################################################################################
Setup
*/
void setup()
{
  Serial.begin(38400);
  pinMode(TOGGLE, INPUT);

  FastLED.addLeds<WS2812B, LED_PIN, GRB>(leds, NUM_LEDS);//setup the led strip
  FastLED.setBrightness(MY_BRIGHTNESS);//set brightness 0-255
  FastLED.setMaxPowerInMilliWatts(25000);
  FastLED.setCorrection(TypicalPixelString);//Might need this to get proper colors
  FastLED.showColor(off);//set all LEDs to black immediately
  leds[0]   = CRGB(255,0,0);//set 1st led red
  leds[1]   = CRGB(0,255,0);//second green
  leds[2]   = CRGB(0,0,255);//third blue
  leds[117] = CRGB(0,0,255);//3rd to last blue
  leds[118] = CRGB(0,255,0);//2nd to last green
  leds[119] = CRGB(255,0,0);//set last led red
  FastLED.show();
  //array length is NUM_COLORS. This variable stores our array length
  myColor[0]  = CHSV(0,255,142);//Red
  myColor[1]  = CHSV(24,255,255);//Orange
  myColor[2]  = CHSV(64,255,250);//Yellow
  myColor[3]  = CHSV(80,255,225);//lime
  myColor[4]  = CHSV(96,255,255);//Green
  myColor[5]  = CHSV(140,255,255);//cyan
  myColor[6]  = CHSV(165,255,255);//Blue
  myColor[7]  = CHSV(180,160,250);//purple
  myColor[8]  = CHSV(210,255,255);//Magenta
  myColor[9]  = CHSV(220,140,250);//Pinkish
  myColor[10] = CHSV(0,0,255);//white
  FastLED.delay(1000);
  FastLED.showColor(off);
  FastLED.show();
  Serial.println("Setup done!");
}//setup()



/*
################################################################################
loop()
*/
void loop()
{
  isOn = digitalRead(TOGGLE);
  if(isOn)
  {
    pot = analogRead(ANALOG_IN);//read in potentiometer value //[0,1023]
    uint8_t index = map(pot, 0, 1023, 0, NUM_COLORS - 1);
    // Serial.print("Index: ");
    // Serial.print(index);
    // Serial.print(", pot: ");
    // Serial.print(pot);
    // Serial.print(", rgb: ");
    Serial.print(index);
    Serial.print("/");
    Serial.println(NUM_COLORS - 1);
    if (index < NUM_COLORS)
    {
      // Serial.println(myColor[index]);
      ColorPickyBoy(myColor[index]);
    }
    else
    {
      Serial.print("Array index trying to go out of bounds! Index was: ");
      Serial.println(index);
    }//else
    wasOn = true;
  }//if on
  if(!isOn && wasOn)
  {
    int i = NUM_LEDS/2 - 1;
    int j = NUM_LEDS/2;
    while(i >= 0)
    {
      leds[i] = off;
      leds[j] = off;
      FastLED.show();
      FastLED.delay(t);//show and delay for t milliseconds
      fadeToBlackBy(leds,NUM_LEDS,(NUM_LEDS/2)/MY_BRIGHTNESS);
      Serial.print("Off Mode:\ni is: ");
      Serial.print(i);
      Serial.print(", j is: ");
      Serial.println(j);
      i--;
      j++;
    }
    wasOn = false;
  }//turn off
}//loop()



/*
################################################################################
Methods
*/
void ColorPickyBoy(CHSV color)
{
  if (isOn)
  {
    int i = 0;
    int j = NUM_LEDS-1;
    while(i < NUM_LEDS/2)
    {
      leds[i] = color;
      leds[j] = color;
      FastLED.show();
      FastLED.delay(t);//show and delay for t milliseconds
      Serial.print("On Mode:\ni is: ");
      Serial.print(i);
      Serial.print(", j is: ");
      Serial.println(j);
      i++;
      j--;
    }//while
    Serial.println("\n--------------------------------------------------");
  }//if on
}//colorPickyBoy()
/*
CHSV ColorHSLtoHSV(uint16_t hue, uint8_t sat, uint8_t level)
{
  uint8_t tHue = (255/360) * hue;
  uint8_t tSat = (255/100) * sat;
  uint8_t tValue = (255/100) * level;
  return CHSV(tHue, tSat, tValue);
}
*/
