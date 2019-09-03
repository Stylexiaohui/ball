#include <SoftwareSerial.h>
SoftwareSerial mySerial(10, 11); // RX, TX
#define con 40
#define Switch A3
#define orderIn A0
#define orderOut1 3
#define orderOut2 4
unsigned char Re_buf[11], counter = 0;
unsigned char sign = 0;
//byte rgb[3] = {0};
byte g = {0};
//***********************************************添加部分****************************
int count = 0, p = 0, pb[60] = {0}, state = 0;
void setup()
{
  pinMode(orderOut1, OUTPUT);
  pinMode(orderOut2, OUTPUT);
  Serial.begin(9600);
  // Serial.print("hello");
  mySerial.begin(9600);
  delay(1);
  mySerial.write(0XA5);
  mySerial.write(0X01);    //初始化,连续输出模式
  mySerial.write(0XA6);    //初始化,连续输出模式
  pinMode(A4,INPUT);
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
      //      rgb[0] = Re_buf[4];
      g = Re_buf[5];
      //      rgb[2] = Re_buf[6];
      //      Serial.print("r:");
      //      Serial.print( rgb[0], DEC);
      //      Serial.print("g:");
      //      Serial.print( rgb[1], DEC);
      //      Serial.print(",b:");
      //      Serial.println( rgb[2], DEC);
      //           Serial.println(analogRead(A0));
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
  }
}
void judge()
{
  int aver = 0;
  for (int a = 0; a < con; a++)
  {
    gather();
    delay(2);
    aver += g;
    //aver+=rgb[1];
  }
  aver /= con;
  //if(abs(aver-rgb[1])>40)
  if (abs(aver - g) > 40)
  {
    judge();
  }
  else {
    Serial.println(aver);
    if (aver < 120)
    {
      Serial.print("黑色");
      if (state)
      {
        digitalWrite(orderOut1, LOW);
        digitalWrite(orderOut2, LOW);
      }
      else
      {
        digitalWrite(orderOut1, LOW);
        digitalWrite(orderOut2, HIGH);
      }
    }
    else if (aver < 210)
    {
      Serial.println("粉红");
      digitalWrite(orderOut1, HIGH);
      digitalWrite(orderOut2, HIGH);
    }
    else if (state)
    {
      Serial.println("白色");
      digitalWrite(orderOut1, LOW);
      digitalWrite(orderOut2, HIGH);

    }
    else
    {
      Serial.println("白色");

    }
    /*
      黑色球 r:51,g:64,b:63      178
      白色球 r:255,g:255,b:255   765
      粉色球 r:255,g:191,b:255   701
    */



  }
}
void loop()
{
/*  digitalWrite(orderOut1, LOW);
  digitalWrite(orderOut2, LOW);
  float Data = 0;
  int i = 0;
  for (i = 0; i < 20; i++)
  {
    delay(5);
    Data += digitalRead(Switch);
  }
  if (Data > 15)state = 1;
  else state = 0;
  Data=0;
  for (i = 0; i < 20; i++)
  {
    delay(5);
    Data += digitalRead(orderIn);
  }
  if (Data > 15)
    for (int r = 0; r < 60; r++)
    { digitalWrite(orderOut1, HIGH);
      delay(1);
      digitalWrite(orderOut1, LOW);
    }*/
  {Serial.println(analogRead(A4));

  //  judge();

  }
}

//19 17.5 24 27
