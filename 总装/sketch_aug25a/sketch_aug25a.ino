
#include <SoftwareSerial.h>
SoftwareSerial mySerial(10,11); // RX, TX
unsigned char Re_buf[11], counter = 0;
unsigned char sign = 0;
byte g = {0};
#define con 30
#define Switch 5//A0
#define State 2
#define orderOut1 7//3
#define orderOut2 8//4
int state=1;
//***********************************************
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

void setup() {
    Serial.begin(9600);
 // Serial.print("hello");
  mySerial.begin(9600);
  delay(1);
  mySerial.write(0XA5);
  mySerial.write(0X01);    //初始化,连续输出模式
  mySerial.write(0XA6);    //初始化,连续输出模式
  pinMode(Switch,INPUT_PULLUP);
  pinMode(orderOut1,OUTPUT);
  pinMode(orderOut2,OUTPUT);
  digitalWrite(orderOut1,LOW);
  digitalWrite(orderOut2,LOW);
  
  //************************************
  
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
  digitalWrite(stepPin, LOW);
  digitalWrite(dirPin, LOW);
  digitalWrite(orderSend, LOW);
  
}


void gather() 
{
  unsigned char i = 0, sum = 0;
  if (sign == 1)
  {
    //Serial.println("h");
    sign = 0;
    for (i = 0; i < 7; i++)
      sum += Re_buf[i];
    if (sum == Re_buf[i] )     //检查帧头，帧尾
    {

      g = Re_buf[5];
      Serial.print("g:");
      Serial.println(g, DEC);
    }
  }
  while (mySerial.available()) 
  {
    byte  readedchar = mySerial.read();
    Re_buf[counter] = (unsigned char)readedchar;
    if (counter == 0 && Re_buf[0] != 0x5A) return; // 检查帧头
    counter++;
    if (counter == 8)             //接收到数据
    {
      counter = 0;               //重新赋值，准备下一帧数据的接收
      sign = 1;
    }
  }}

  void judge()
{
  int aver=0;
  g=0;
  for(int a=0;a<con;)
   {
    gather();
    delay(2);
    a++;
    aver+=g;
   }
  aver/=con;
  if(abs(aver-g)<5)
   {
         Serial.println(aver);
         if(g<90)  
           {
            Serial.println("黑色");
            if(state)
              {
                digitalWrite(orderOut1,LOW);
                digitalWrite(orderOut2,HIGH);
                abandon();
                Serial.println("丢弃");
              }
            else 
              {
                digitalWrite(orderOut1,HIGH);
                digitalWrite(orderOut2,LOW);
                Serial.print("使用");
                use();
              }
           }
           else {
                  if(g<215)
                   {
                     Serial.println("粉红");
                     digitalWrite(orderOut1,HIGH);
                     digitalWrite(orderOut2,HIGH);pink();
                   }
                   else {Serial.println("白色");if(state)
                     {
                      
                       digitalWrite(orderOut1,HIGH);
                       digitalWrite(orderOut2,LOW);
                       use();
                      }
                      else
                      {
                        Serial.println("白色");
                        digitalWrite(orderOut1,LOW);
                       digitalWrite(orderOut2,HIGH);
                        Serial.println("丢弃");
                        abandon();
                        }}
       /*
 *黑色球 r:51,g:64,b:63      178
 *白色球 r:255,g:255,b:255   765
 *粉色球 r:255,g:191,b:255   701
 */
       }
}}
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
    int bu = digitalRead(pushButtonU);//Serial.println("123");
    int bm = digitalRead(pushButtonM);//Serial.println(digitalRead(pushButtonA));Serial.println(digitalRead(pushButtonA));
        while(ba && bu)
  {
      Step(50);//Serial.println("12356+");
      ba = digitalRead(pushButtonA);
      bu = digitalRead(pushButtonU);
      Serial.println(ba);
      Serial.println(bu);
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
       
          Serial.println(analogRead(pushButtonM));Serial.println(digitalRead(pushButtonA));
          }
           }
         }
}
void loop()
{
  float Data=0;
  int i=0;
  for(i=0;i<10;i++)
  {delay(2);Data+=digitalRead(Switch);}
  if(Data>5) state=1;
  else state=0;
  //***************************************************
//  Serial.println(digitalRead(dirPin));
  Serial.println("loop");//Serial.println(digitalRead(pushButtonA));Serial.println(digitalRead(pushButtonM));
 // Serial.println(digitalRead(pushButtonU));
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
  delay(1000);
  judge();

}
}
