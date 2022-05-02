//LightSaber V3.3
#include <FastLED.h>
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>
#include <utility/imumaths.h>
//#include <DFRobotDFPlayerMini.h>
#include <DFPlayerMini_Fast.h>
#include <SoftwareSerial.h> //RX and TX lines
//define number of LEDs on strip
#define NUM_LEDS 120

/*
################################################################################
configable Variables
*/

//pins
  #define LED_PIN 6
  #define PBU 2
  #define PBD 3
  #define TOGGLE 4//toggle on function
  #define MP3RX 10
  #define MP3TX 11

#define MY_BRIGHTNESS 128//brightness 0-255
#define NUM_COLORS 11
const int t = 5;//delay time as we step through to get to the correct color

/*
################################################################################
Variables
*/

CRGB leds[NUM_LEDS];
volatile uint8_t count = 0;
uint32_t prevIntUp = 0;
uint32_t prevIntDown = 0;
bool isOn = false;
bool wasOn = true;
CHSV myColor[NUM_COLORS];
CRGB off = CRGB(0,0,0);
SoftwareSerial mySoftwareSerial(MP3RX, MP3TX); //Setup communication with MP3 Module
//DFRobot
DFPlayerMini_Fast myDFPlayer; // Naming the module
Adafruit_BNO055 bno = Adafruit_BNO055(55);

/*
################################################################################
Setup
*/

void setup()
{
  pinMode(TOGGLE, INPUT);
  pinMode(PBU, INPUT);
  pinMode(PBD, INPUT);

  attachInterrupt(digitalPinToInterrupt(PBU), NextColorBoy, RISING);
  attachInterrupt(digitalPinToInterrupt(PBD), PrevColorBoy, RISING);

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

  //moving on to mp3 Setup
  bno.begin();
  /* Initialise the sensor */
  bno.setExtCrystalUse(true);
  mySoftwareSerial.begin(9600);
  myDFPlayer.begin(mySoftwareSerial);
  myDFPlayer.stop();
  myDFPlayer.volume(30); // setting volume from 1-30
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
    if (!wasOn)
    {
      myDFPlayer.play(1);// on sound
      while (myDFPlayer.isPlaying());
    }
    if (!myDFPlayer.isPlaying())
      myDFPlayer.play(2); // constant while on sound
    if (count < NUM_COLORS)
    {
      ColorPickyBoy(myColor[count]);
    }
    isMoving();
    wasOn = true;
  }//if on
  if(!isOn && wasOn)
  {
    myDFPlayer.play(3); // off sound
    int i = NUM_LEDS/2 - 1;
    int j = NUM_LEDS/2;
    while(i >= 0)
    {
      leds[i] = off;
      leds[j] = off;
      FastLED.show();
      FastLED.delay(t);//show and delay for t milliseconds
      fadeToBlackBy(leds,NUM_LEDS,(NUM_LEDS/2)/MY_BRIGHTNESS);
      i--;
      j++;
    }
    while (myDFPlayer.isPlaying());
    wasOn = false;
  }//turn off
}//loop()

/*
################################################################################
Methods
*/
void isMoving()
{
  imu::Vector<3> acc = bno.getVector(Adafruit_BNO055::VECTOR_ACCELEROMETER);

  if(acc.z() > 10)
  {
    //Lightsaber Swing Noise
    myDFPlayer.play(4); //swing sound
    while(myDFPlayer.isPlaying());
  }//fi
}//isMoving()

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
      i++;
      j--;
    }//while
  }//if on
}//colorPickyBoy()

void NextColorBoy()
{
  uint32_t interruptTime = millis();
  if (interruptTime - prevIntUp > 50)
  {
    if (count < 10)
      count++;
    else
      count = 0;
  }
  prevIntUp = interruptTime;
}

void PrevColorBoy()
{
  uint32_t interruptTime = millis();
  if (interruptTime - prevIntDown > 50)
  {
    if (count > 0)
      count--;
    else
      count = 10;
  }
  prevIntDown = interruptTime;
}
