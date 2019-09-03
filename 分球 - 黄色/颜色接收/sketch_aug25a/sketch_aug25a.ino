#define orderSend 2
#define stepPin 3
#define dirPin 4
#define orderRun 5
#define orderPin A0
#define orderBack A1
#define pushButtonA A4
#define pushButtonM A3
#define pushButtonU A2
#define colorPin1 A5
#define colorPin2 6
//黑色 r:49,g:62,b:58
//粉色 r:255,g:181,b:236
//黄色 r:255,g:255,b:123
//白色 r:255,g:255,b:255
void setup() {
  Serial.begin(9600);
  pinMode(stepPin, OUTPUT);   
  pinMode(dirPin, OUTPUT);
  pinMode(orderSend, OUTPUT);
  pinMode(orderRun, OUTPUT);
  pinMode(orderPin, INPUT_PULLUP);
  pinMode(orderBack, INPUT_PULLUP);
  pinMode(pushButtonA, INPUT_PULLUP);
  pinMode(pushButtonM, INPUT_PULLUP);
  pinMode(pushButtonU, INPUT_PULLUP);
  pinMode(colorPin1, INPUT_PULLUP);
  pinMode(colorPin2, INPUT_PULLUP);
  digitalWrite(orderRun, LOW);
  digitalWrite(stepPin, LOW);
  digitalWrite(dirPin, LOW);
  digitalWrite(orderSend, LOW);
}
void Step(int s)
{
  for (int a = 0; a < s; a++)
  {
    digitalWrite(stepPin, HIGH);
    delayMicroseconds(1000);
    digitalWrite(stepPin, LOW);
    delayMicroseconds(1000);
  }
  Serial.print("电机:  ");Serial.println(s);
}
void abandon()
{
  digitalWrite(dirPin, LOW);
  delay(5);
  int ba = digitalRead(pushButtonA);
  int bu = digitalRead(pushButtonU);
  int bm = digitalRead(pushButtonM);
  while(ba && bu)
  {
      Step(50);
      ba = digitalRead(pushButtonA);
      bu = digitalRead(pushButtonU);
      Serial.println("Abandon");
    }
  digitalWrite(dirPin, HIGH);
  Serial.println("丢弃");
  digitalWrite(orderSend,LOW);
  digitalWrite(orderRun, LOW);
}
void use()
{
  digitalWrite(dirPin, HIGH);
  if (digitalRead(orderBack))//炮台就绪时拉高电位
  {
    int ba = digitalRead(pushButtonA);
    int bu = digitalRead(pushButtonU);
    int bm = digitalRead(pushButtonM);
    while(ba && bu)
  {
      Step(50);
      ba = digitalRead(pushButtonA);
      bu = digitalRead(pushButtonU);
      Serial.println("Use");
    }
    digitalWrite(dirPin, LOW);
    Serial.println("使用");
    digitalWrite(orderSend,LOW);
  }
    Serial.println("Use");
    digitalWrite(orderRun, LOW);
}
void pink()
{
  Serial.println("Pink");
  digitalWrite(dirPin, HIGH);Serial.println(digitalRead(orderBack));
  if (digitalRead(orderBack))//炮台就绪时拉高电位 
  {
    int ba = digitalRead(pushButtonA);
    int bu = digitalRead(pushButtonU);
    int bm = digitalRead(pushButtonM);
        while(ba && bu)
  {
      Step(50);
      ba = digitalRead(pushButtonA);
      bu = digitalRead(pushButtonU);
    }
    digitalWrite(dirPin, LOW);
    Serial.println("粉色");
    digitalWrite(orderSend,LOW);
  }
  digitalWrite(orderRun, LOW);
}
void Ready()
{
  Serial.println("Ready");  
  delay(2);
  int ba = digitalRead(pushButtonA);
  int bu = digitalRead(pushButtonU);
  int bm = digitalRead(pushButtonM);
  while(ba && bu && bm)
  {
      Step(50);
      ba = digitalRead(pushButtonA);
      bu = digitalRead(pushButtonU);
      bm = digitalRead(pushButtonM);
      Serial.println("***************");
    }
  if (!ba)
      {
        digitalWrite(dirPin, LOW);
        while(bm&&bu)
        {
          Step(50);
          bm = digitalRead(pushButtonM);
          bu = digitalRead(pushButtonU);
          Serial.println("********A-M");
          Serial.println(analogRead(pushButtonM));
          }
      }
    else {
          if (!bu)
           {
            digitalWrite(dirPin, HIGH);
            while(bm&&ba)
        {
          Step(50);
          bm = digitalRead(pushButtonM);
          ba = digitalRead(pushButtonA);
          Serial.println("********U-M");
          Serial.println(analogRead(pushButtonM));
          }
           }
         }
}

void loop()
{Serial.println(digitalRead(dirPin));
  Serial.println("loop");
  delay(2);
  int data = 0;
  for (int i = 0; i < 10; i++)
  {
    delay(2);
    data += digitalRead(orderPin);
  }
  if (data < 5)
  {Serial.println("judge");
  Ready();
  delay(3000);
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
      else {digitalWrite(orderRun,HIGH);Serial.println("跑");}
    }
  else {
         digitalWrite(orderSend,HIGH);
         if(color2<10)
         {
          use();
         }
         else pink();
       }}
    
}
