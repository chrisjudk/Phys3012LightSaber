#include <Adafruit_NeoPixel.h>

//LED output on pin 6
#define LED_PIN 6

//define number of LEDs on strip
#define LED_COUNT 120
#define BRIGHTNESS 255

bool bsA = false;
bool bsB = false;
bool bsC = false;
bool bsAprev= false;
bool bsBprev= false;
bool isOn = false;
int count = 0;
int red = 0;
int green = 0;
int blue = 0;

int t = 1;//delay time as we step through to get to the correct color

//pins
int iA = 2;//rotary encoder dataA pin
int iB = 4;//rotary encoder dataB pin
int iC = 3;//push button function


Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);
void setup()
{
  Serial.begin(38400);
  pinMode(iB, INPUT);
  pinMode(iC, INPUT);
  attachInterrupt(0,rotated,CHANGE);
  strip.begin(); //INIT
  strip.show(); //Turn OFF all pixels ASAP
  strip.setBrightness(BRIGHTNESS);
  strip.setPixelColor(1,255,0,0);
  strip.show();
  Serial.println("Setup done");
}

void loop() 
{
  isOn = digitalRead(iC);
  int first = 0;
  int last = 0;
  if(isOn)
  {
    last = strip.numPixels()-1;
    if((red + green + blue) == 0)
      red = 255;
  
    for(int i = 0; i < strip.numPixels();i++)
    {
      strip.setPixelColor(i,strip.Color(red,green,blue));
      strip.show();
      delay(t);
    }
  }//if turn on
  else if(!isOn)
  {
    for(int i = strip.numPixels()-1;i>=0;i--)
    {
      strip.setPixelColor(i,strip.Color(0,0,0));
      strip.show();
      delay(t);
    }
  }//turn off
}//loop()

void colorPickyBoy()
{
  delay(20);
  //red, orange(R,analog 128), yellow(RG), green, turquoise(BG), White(RGB)
  
  //select the color
  if (count == 0)
  {
    setColor(0xFF,0x0,0x0);
  }//if red
  
  else if (count == 1)
  {
    setColor(0xFF,0xA5,0x0);
  }//if orange

  else if (count == 2)
  {
     setColor(0xFF,0xFF,0x0);  
  }//if yellow

  else if (count == 3)
  {
    setColor(0x0,0xFF,0x0);
  }//if green

  else if (count == 4)
  {
    setColor(0x0,0xFF,0xFF);
  }//if cyan

  else if (count == 5)
  {
    setColor(0x0,0x0,0xFF);
    Serial.println("It should be blue");
  }//if blue

  else if (count == 6)
  {
    setColor(0x93,0x70,0xDB);
  }//if White
}//colorPickyBoy()

void setColor(int aRed, int aGreen, int aBlue)
{
  /*
  while((red != aRed) || (green != aGreen) || (blue != aBlue))
  {
    while(red != aRed)
    {
      if (red > aRed)
        red--;
      else
        red++;
    }//red
    
    while(green != aGreen)
    {
      if (green > aGreen)
        green--;
      else
        green++;
    }//green
    
    while(blue != aBlue)
    {
      if (blue > aBlue)
        blue--;
      else
        blue++;
    }//blue
    */
    red = aRed;
    green = aGreen;
    blue = aBlue;
    if(isOn)
    {
      for(int i=0; i < strip.numPixels(); i++)
      {
        strip.setPixelColor(i, strip.Color(red,green,blue));
      }//for
      delay(50);
      strip.show();
    }//if on
}//setColor

void rotated()
{
  if (isOn)
  {
    if(digitalRead(iA) == digitalRead(iB))
      count++;
    else
      count--;
    
    if (count < 0)
    {
      Serial.println("A");
      count = 6;
    }
    if(count > 6)
    {
      count = 0;
    }
    Serial.print("Count is: ");
    Serial.println(count);
    delay(20);
    colorPickyBoy();
  }
}//rotated()
