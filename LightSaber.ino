//LightSaber V2.0
#include <Adafruit_NeoPixel.h>

//LED output on pin 6
#define LED_PIN 6

//define number of LEDs on strip
#define LED_COUNT 120
#define BRIGHTNESS 255 //max 255

//bool bsA = false;
//bool bsB = false;
//bool bsC = false;
//bool bsAprev= false;
//bool bsBprev= false;

Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);

//################################################################
//Variables
  
  //pins
  //int iA = 2;//rotary encoder dataA pin
  //int iB = 4;//rotary encoder dataB pin
  int iC = 3;//toggle on function
  int analogIn = A5;

//configable
  const int numColors = 11;
  const int t = 5;//delay time as we step through to get to the correct color

uint32_t myColor[numColors];
uint32_t off = strip.Color(0,0,0);
bool isOn = false;
bool wasOn = true;
int pot = 0;

void setup()
{
  Serial.begin(38400);
  pinMode(iC, INPUT);
  strip.begin(); //INIT
  strip.show(); //Turn OFF all pixels ASAP
  strip.setBrightness(BRIGHTNESS);
  strip.setPixelColor(strip.numPixels()-1,strip.gamma32(strip.ColorHSV(0,100,50)));
  strip.setPixelColor(1,strip.gamma32(strip.ColorHSV(0,100,50)));
  strip.show();
  //array length is numColors. This variable stores our array length
  myColor[0] = strip.gamma32(ColorHSLtoHSV(0,100,55));//Red
  myColor[1] = strip.gamma32(ColorHSLtoHSV(24,100,100));//Orange
  myColor[2] = strip.gamma32(ColorHSLtoHSV(42,100,98));//Yellow
  myColor[3] = strip.gamma32(ColorHSLtoHSV(67,100,85));//lime
  myColor[4] = strip.gamma32(ColorHSLtoHSV(120,100,100));//Green
  myColor[5] = strip.gamma32(ColorHSLtoHSV(180,100,100));//cyan
  myColor[6] = strip.gamma32(ColorHSLtoHSV(240,100,100));//Blue
  myColor[7] = strip.gamma32(ColorHSLtoHSV(271,50,40));//purple
  myColor[8] = strip.gamma32(ColorHSLtoHSV(300,100,100));//Magenta
  myColor[9] = strip.gamma32(ColorHSLtoHSV(336,54,98));//Pinkish
  myColor[10] = strip.gamma32(ColorHSLtoHSV(0,0,100));//white
  delay(1000);
  strip.setPixelColor(1,off);
  strip.setPixelColor(118,off);
  Serial.println("Setup done!");
}//setup()

void loop() 
{
  isOn = digitalRead(iC);
  if(isOn)
  {
    pot = analogRead(analogIn);//read in potentiometer value //[0,1023]
    int index = pot/(1023/numColors+1);
    Serial.print("Index: ");
    Serial.print(index);
    Serial.print(", pot: ");
    Serial.print(pot);
    Serial.print(", rgb: ");
    if (index < numColors)
    {
      Serial.println(myColor[index]);
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
    int i = strip.numPixels()/2 - 1;
    int j = strip.numPixels()/2;
    while(i >= 0)
    {
      strip.setPixelColor(i,off);
      strip.setPixelColor(j,off);
      strip.show();
      delay(t);
      Serial.print("i is: ");
      Serial.print(i);
      Serial.print(", j is: ");
      Serial.println(j);
      i--;
      j++;
    }
    wasOn = false;
  }//turn off
}//loop()

void ColorPickyBoy(uint32_t color)
{
  if (isOn)
  {
    int i = 0;
    int j = strip.numPixels()-1;
    while(i < 60)
    {
      strip.setPixelColor(i, color);
      strip.setPixelColor(j,color);
      delay(t);
      strip.show();
      i++;
      j--;
    }//while
  }//if on
}//colorPickyBoy()

uint32_t ColorHSLtoHSV(int hue, int sat, int level)
{
  unsigned int tHue = (65536/360) * hue;
  unsigned int tSat = (255/100) * sat;
  unsigned int tValue = (255/100) * level;
  return strip.ColorHSV(tHue, tSat, tValue);
}
