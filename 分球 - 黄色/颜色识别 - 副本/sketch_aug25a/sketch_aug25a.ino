#include <SoftwareSerial.h>
SoftwareSerial mySerial(10,11); // RX, TX
unsigned char Re_buf[11], counter = 0;
unsigned char sign = 0;
byte r={0},g = {0};
#define con 20
#define Switch A0
#define State 2
#define orderOut1 3
#define orderOut2 4
int state=1;
//byte data[20][3] = {0};
//***********************************************添加部分****************************

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
  
  
/*
#define orderIn A0
#define Switch A1
#define orderOut1 3
#define orderOut2 4

*/
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

      g = Re_buf[6];
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
  for(int a=0;a<con;)
   {
    gather();
    delay(1);
    a++;
    aver+=g;
   }
  aver/=con;
  if(abs(aver-g)>10)
   {
    judge();
   }
   else{
//黑色 r:49,g:62,b:58
//粉色 r:255,g:181,b:236
//黄色 r:255,g:255,b:123
//白色 r:255,g:255,b:255
         Serial.println(aver);
         if(aver<90)  
           {
            Serial.print("黑色");
            if(state)
              {
                digitalWrite(orderOut1,LOW);
                digitalWrite(orderOut2,HIGH);
                Serial.print("丢弃");
              }
            else 
              {
                digitalWrite(orderOut1,HIGH);
                digitalWrite(orderOut2,LOW);
                Serial.print("使用");
              }
           }
           //黑色 r:49,g:62,b:58
//粉色 r:255,g:181,b:236
//黄色 r:255,g:255,b:123
//白色 r:255,g:255,b:255
           else {
                  if(aver<150)
                    {
                    digitalWrite(orderOut1,LOW);
                    digitalWrite(orderOut2,LOW);Serial.println("跑");
                    }
                  else 
                  {
                    if(aver<242)
                     {
                      Serial.println("粉红");
                      digitalWrite(orderOut1,HIGH);
                      digitalWrite(orderOut2,HIGH);
                     }
                    else  {
                      if(state)
                     {
                      Serial.println("白色");
                       digitalWrite(orderOut1,HIGH);
                       digitalWrite(orderOut2,LOW);
                      Serial.print("使用");
                      }
                      else
                      {
                        Serial.println("白色");
                        digitalWrite(orderOut1,LOW);
                       digitalWrite(orderOut2,HIGH);
                        Serial.println("丢弃");
                        
                        }
                    }
                    }
       /*
 *黑色球 r:51,g:64,b:63      178
 *白色球 r:255,g:255,b:255   765
 *粉色球 r:255,g:191,b:255   701
 */   
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
  judge();
  delay(10);
}
