#include <SoftwareSerial.h>
SoftwareSerial mySerial(10,11); // RX, TX
#define con 10
#define orderIn A0
#define Switch A1
#define orderBack A2
#define pushButtonA A3
#define pushButtonM A4
#define pushButtonU A5
#define orderOut1 3
#define orderOut2 4
#define dirPin 5
#define stepPin 6
#define stepU 500
#define stepA 500
unsigned char Re_buf[11], counter = 0;
unsigned char sign = 0;
//byte rgb[3] = {0};
byte g = {0};
int count=0,p=0,pb[60]={0},state=0;
void setup() 
{
pinMode(orderOut1,OUTPUT);
pinMode(orderOut2,OUTPUT);
pinMode(Switch,INPUT_PULLUP);
pinMode(orderBack,INPUT_PULLUP);
pinMode(orderIn,INPUT_PULLUP);
pinMode(pushButtonA,INPUT_PULLUP);
pinMode(pushButtonM,INPUT_PULLUP);
pinMode(pushButtonU,INPUT_PULLUP);
pinMode(dirPin,OUTPUT);
pinMode(stepPin,OUTPUT);
digitalWrite(dirPin,HIGH);
  Serial.begin(9600);
 // Serial.print("hello");
  mySerial.begin(9600);
  delay(1);
  mySerial.write(0XA5);
  mySerial.write(0X01);    //初始化,连续输出模式
  mySerial.write(0XA6);    //初始化,连续输出模式
  
}
void abandon()
{ Serial.println("ABandon");
  digitalWrite(orderOut1,LOW);
  digitalWrite(orderOut2,LOW);
  digitalWrite(dirPin,HIGH);
  delay(20);
  STEP(stepA);
  digitalWrite(dirPin,LOW);
  Serial.println("丢弃");

}
void use()
{ Serial.println("Use");
  analogWrite(orderOut1,LOW);
  analogWrite(orderOut2,LOW);
  digitalWrite(dirPin,LOW);
  delay(20);
  p=0;
  for(int i=0;i<60;i++)
  {
    p+=digitalRead(orderBack);
    delay(1);
  }
  if(p<15) ;//炮台就绪时拉低电位
  {STEP(stepU);
  digitalWrite(dirPin,HIGH);
  Serial.println("使用");}
}
void pink()
{ Serial.println("Pink");
  digitalWrite(orderOut1,LOW);
  digitalWrite(orderOut2,HIGH);
  digitalWrite(dirPin,LOW);
  delay(20);
  p=0;
  for(int i=0;i<60;i++)
  {
    p+=digitalRead(orderBack);
    delay(2);
  }
  if(p<15) //炮台就绪时拉低电位
  {STEP(stepU);
  digitalWrite(dirPin,HIGH);
  Serial.println("粉色");}
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
  void STEP(int t)
  {
    for(int k=0;k<t;k++)
    {
    digitalWrite(dirPin,HIGH);
    delayMicroseconds(800);
    digitalWrite(dirPin,LOW);
    delayMicroseconds(800);
    }
    Serial.print(t);
    Serial.println("电机");
    }

void Ready()
{
  Serial.println("Ready");  
  delay(2);
  int ba = 1, bu = 1, bm = 1;
  ba = digitalRead(pushButtonA);
  bu = digitalRead(pushButtonU);
  bm = digitalRead(pushButtonM);
  while(ba && bu && bm)
  {
      STEP(5);
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
          STEP(5);
          bu = digitalRead(pushButtonU);
          Serial.println("********A-M");
          }
      }
    else {
          if (!bu)
           {
            digitalWrite(dirPin, HIGH);
            while(bm)
        {
          STEP(5);
          bu = digitalRead(pushButtonU);
          Serial.println("********U-M");
          }
           }
         }
}
  /*
#define con 30
#define orderIn A0
#define Switch A1
#define orderBack A2
#define pushButtonA A3
#define pushButtonM A4
#define pushButtonU A5
#define orderOut1 3
#define orderOut2 4
  */
  
void judge()
{
  Ready();
  delay(500);
  int aver=0;
  for(int a=0;a<con;a++)
   {
    gather();
      delay(5);
      aver+=g;
   }
  aver/=con;
  if(abs(aver-g)>20)
   {
    judge();
   }
   else{
         Serial.println(g);
         if(g<120)  
           {
            Serial.print("黑色");
            if(state)
              {
                abandon();
              }
            else 
              {
                use();
              }
           }
           else if(g<210)
                {Serial.print("P ");
                  pink();
                  }
                else {Serial.print("W ");
                  if(state)
                     {
                      use();
                     
                      }
                      else
                      {
                        abandon();
                        
                        }
                        }
       /*
 *黑色球 r:51,g:64,b:63      178
 *白色球 r:255,g:255,b:255   765
 *粉色球 r:255,g:191,b:255   701
 */
       }
}
void loop()
{
  Serial.println("loop");
  digitalWrite(orderOut1, LOW);
  digitalWrite(orderOut2, LOW);
  float Data = 0;
  int i = 0;
  for (i = 0; i < 10; i++)
  {
    delay(5);
    Data += digitalRead(Switch);
  }
  if (Data >=5)state = 1;
  else state = 0;
  Data=0;
  for (i = 0; i < 10; i++)
  {
    delay(5);
    Data += digitalRead(orderIn);
  }
  if (Data<5)      
judge();
}
