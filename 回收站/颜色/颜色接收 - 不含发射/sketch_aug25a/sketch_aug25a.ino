#define orderSend 2
#define stepPin 3
#define dirPin 4
#define orderPin A0
#define orderBack A1
#define pushButtonA A2
#define pushButtonM A3
#define pushButtonU A4
#define colorPin1 A5
#define colorPin2 A6
#define stepA 500
#define stepU 500



void setup() {
  pinMode(stepPin, OUTPUT);
  pinMode(dirPin, OUTPUT);
  pinMode(orderSend, OUTPUT);
  pinMode(orderPin, INPUT_PULLUP);
  pinMode(orderBack, INPUT_PULLUP);
  pinMode(pushButtonA, INPUT_PULLUP);
  pinMode(pushButtonM, INPUT_PULLUP);
  pinMode(pushButtonU, INPUT_PULLUP);
  pinMode(colorPin1, INPUT_PULLUP);
  pinMode(colorPin2, INPUT_PULLUP);
  digitalWrite(stepPin, LOW);
  digitalWrite(dirPin, LOW);
  digitalWrite(orderSend, LOW);
}
void Step(int s)
{
  for (int a = 0; a < s; a++)
  {
    digitalWrite(stepPin, HIGH);
    delayMicroseconds(800);
    digitalWrite(stepPin, LOW);
    delayMicroseconds(800);
  }
}
void abandon()
{
  Serial.println("Abandon");
  digitalWrite(dirPin, HIGH);
  delay(5);
  Step(stepA);
  digitalWrite(dirPin, LOW);
  Serial.println("丢弃");
  digitalWrite(orderSend,LOW);
}
void use()
{
  Serial.println("Use");
  digitalWrite(dirPin, LOW);
  int p = 0;
  for (int i = 0; i < 60; i++)
  {
    p += digitalRead(orderBack);
    delay(1);
  }
  if (p < 15) judge(); //炮台就绪时拉高电位
  Step(stepU);
  digitalWrite(dirPin, HIGH);
  Serial.println("使用");
  digitalWrite(orderSend,LOW);
}
void pink()
{
  Serial.println("Pink");
  digitalWrite(dirPin, LOW);
  int p = 0;
  for (int i = 0; i < 30; i++)
  {
    p += digitalRead(orderBack);
    delay(1);
  }
  if (p < 15) judge(); //炮台就绪时拉高电位
  Step(stepU);
  digitalWrite(dirPin, HIGH);
  Serial.println("粉色");
  digitalWrite(orderSend,LOW);
}
void Ready()
{
  Step(2);
  int ba = 1, bu = 1, bm = 1;
  ba = digitalRead(pushButtonA);
  bu = digitalRead(pushButtonU);
  bm = digitalRead(pushButtonM);
  if (ba && bu && bm) judge();
  if (!ba)
  {
    digitalWrite(dirPin, LOW);
    Ready();
  }
  else if (!bu)
  {
    digitalWrite(dirPin, HIGH);
    Ready();
  }
}
void judge()
{
  Ready();
  delay(300);
//  digitalWrite(orderSend,HIGH);
  int color1=0,color2=0;
  for (int a = 0; a < 20; a++)
  {
    delay(2);
    color1 += digitalRead(colorPin1);
    color2 += digitalRead(colorPin2);
  }
  if (color1 <10)
    {
      if(color2>=10)
      abandon();
    }
  else {
         digitalWrite(orderSend,HIGH);
         if(color2<10)
         {
          use();
         }
         else pink();
       }
}
void loop()
{
  delay(2);
  int data = 0;
  for (int i = 0; i < 20; i++)
  {
    delay(2);
    data += digitalRead(orderPin);
  }
  if (data < 8)
    judge();
}
