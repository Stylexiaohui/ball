#define orderSend 2
#define stepPin 3
#define dirPin 4
#define orderPin A0
#define orderBack A1
#define pushButtonA A4
#define pushButtonM A3
#define pushButtonU A2
#define colorPin1 A5
#define colorPin2 6
#define orderRun 5


void setup() {
  Serial.begin(9600);
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
  pinMode(orderRun,OUTPUT);
  digitalWrite(orderRun,LOW);
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
        while(bm)
        {
          Step(50);
          bm = digitalRead(pushButtonM);
          Serial.println("********A-M");
          Serial.println(analogRead(pushButtonM));
          }
      }
    else {
          if (!bu)
           {
            digitalWrite(dirPin, HIGH);
            while(bm)
        {
          Step(50);
          bm = digitalRead(pushButtonM);
          Serial.println("********U-M");
          Serial.println(analogRead(pushButtonM));
          }
           }
         }
}
void judge()
{
  
  Serial.println("judge");
  Ready();
//  delay(300);
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
 /*      #define orderSend 2
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
************************************
*#define con 20
#define Switch A0
#define orderOut1 3
#define orderOut2 4
*/
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
