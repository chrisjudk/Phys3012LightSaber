#define RA 2
#define RB 3

int counter = 0;
int currentStateRA;
int lastStateRA;
String rotDir = "";
void setup()
{
  Serial.begin(38400);
  while(!Serial)
    delay(100);
  Serial.println();
  Serial.print("Setting up pinModes . . . ");
  pinMode(RA, INPUT);
  pinMode(RB, INPUT);
  Serial.println("done!");

  lastStateRA = digitalRead(RA);

  Serial.print("Attaching interrupts . . . ");
  attachInterrupt(0, encoder, CHANGE);
  attachInterrupt(1, encoder, CHANGE);
  Serial.println("done!");
}
void loop()
{

}

void encoder()
{
  currentStateRA = digitalRead(RA);
  if (currentStateRA != lastStateRA && currentStateRA == 1)
  {
    if(digitalRead(RB) != currentStateRA)
    {
      counter--;
      rotDir = "CCW";
    }
    else
    {
      counter++;
      rotDir = "CW";
    }
    Serial.print("Counter: ");
    Serial.print(counter);
    Serial.print(" | Dir: ");
    Serial.println(rotDir);
  }
  lastStateRA = currentStateRA;
}
