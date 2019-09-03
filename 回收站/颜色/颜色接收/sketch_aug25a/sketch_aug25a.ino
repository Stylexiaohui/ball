#define colorIn1 A0
#define colorIn2 A1
#define stepPin 2
#define dirPin 3
#define pushButtonA A2
#define pushButtonM A3
#define pushButtonU A4
#define orderPin A5
#define stepA 500
#define stepU 500
void setup() {

  pinMode(colorIn1,INPUT_PULLUP);
  pinMode(colorIn2,INPUT_PULLUP);
  pinMode(stepPin,OUTPUT);
  pinMode(dirPin,OUTPUT);
  pinMode(pushButtonA,INPUT_PULLUP);
  pinMode(pushButtonM,INPUT_PULLUP);
  pinMode(pushButtonU,INPUT_PULLUP);
  pinMode(orderPin,INPUT_PULLUP);
  digitalWrite(stepPin,LOW);
  digitalWrite(dirPin,LOW);

}
void Step(int s)
{
  for(int a=0;a<s;a++)
  {
    digitalWrite(stepPin,HIGH);
    delayMicroseconds(800);
    digitalWrite(stepPin,HIGH);
  }
}
void abandon()
{ Serial.println("Abandon");
  digitalWrite(dirPin,HIGH);
  delay(20);
  Step(stepA);
  digitalWrite(dirPin,LOW);
  Serial.println("丢弃");
}
void use()
{ Serial.println("Use");
  digitalWrite(dirPin,LOW);
  delay(20);
  p=0;
  for(int i=0;i<60;i++)
  {
    p+=digitalRead(orderBack);
    delay(1);
  }
  if(p>15) use();//炮台就绪时拉低电位
  Step(stepU);
  digitalWrite(dirPin,HIGH);
  Serial.println("使用");
}
void pink()
{ Serial.println("Pink");
  digitalWrite(dirPin,LOW);
  delay(20);
  p=0;
  for(int i=0;i<60;i++)
  {
    p+=digitalRead(orderBack);
    delay(1);
  }
  if(p>15) pink();//炮台就绪时拉低电位
  Step(stepU);
  digitalWrite(dirPin,HIGH);
  Serial.println("粉色");
}
void Ready()
{
  Step(2);Serial.print("******");Serial.println(analogRead(pushButtonM));
  int ba=1,bu=1,bm=1;
  ba=digitalRead(pushButtonA);
  bu=digitalRead(pushButtonU);
  bm=digitalRead(pushButtonM);
  if(ba&&bu&&bm) Ready();
  if(!ba) 
  {
    digitalWrite(dirPin,LOW);
    Ready();
    }
  else if(!bu) 
  {
    digitalWrite(dirPin,HIGH);
    Ready();
    }
}
  void judge()
{
  Ready();
  delay(300);
  
  
}
void loop()
{
  delay(2);
  int Data=0;
  for (i = 0; i < 20; i++)
  {
    delay(2);
    Data += digitalRead(orderIn);
  }
  if (Data<8)
  shoot();
}
