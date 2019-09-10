/**
 * @file sketch_aug25a.ino
 * @author your name (you@domain.com)
 * @brief 颜色识别板
 * @version 0.1
 * @date 2019-09-04
 * 
 * @copyright Copyright (c) 2019
 * 
 */
#include <SoftwareSerial.h>
SoftwareSerial mySerial(10, 11); // RX, TX
#define con 20
#define Switch A0
#define State 2
//!信号输出引脚
#define orderOut1 3
#define orderOut2 4
unsigned char Re_buf[11], counter = 0;
unsigned char sign = 0;
//!从GY33读来的颜色值
byte r = 0, g = 0,b=0;

//!表示队伍号
int state = 1;

//***********************************************添加部分****************************

void setup()
{
  Serial.begin(9600);
  // Serial.print("hello");
  mySerial.begin(9600);
  delay(1);
  mySerial.write(0XA5);
  mySerial.write(0X01); //初始化,连续输出模式
  mySerial.write(0XA6); //初始化,连续输出模式
  pinMode(Switch, INPUT_PULLUP);
  pinMode(orderOut1, OUTPUT);
  pinMode(orderOut2, OUTPUT);
  digitalWrite(orderOut1, LOW);
  digitalWrite(orderOut2, LOW);
}

/**
 * @brief 接收颜色
 * 
 */
void gather()
{
  unsigned char i = 0, sum = 0;
  if (sign == 1)
  {
    //Serial.println("h");
    sign = 0;
    for (i = 0; i < 7; i++)
      sum += Re_buf[i];
    if (sum == Re_buf[i]) //检查帧头，帧尾
    {
      r = Re_buf[4];
      g = Re_buf[5];
      b = Re_buf[6];
    }
  }
  while (mySerial.available())
  {
    byte readedchar = mySerial.read();
    Re_buf[counter] = (unsigned char)readedchar;
    if (counter == 0 && Re_buf[0] != 0x5A)
      return; // 检查帧头
    counter++;
    if (counter == 8) //接收到数据
    {
      counter = 0; //重新赋值，准备下一帧数据的接收
      sign = 1;
    }
  }
}

/**
 * @brief 判断颜色并发送信号给步进电机控制板
 * @notes 引脚信号表
 * --------------------------------------------------------
 *    引脚\意义   |  丢弃  |   使用  |  粉色   |   跑    | 
 *    orderOut1  |   LOW  |  HIGH  |   HIGH  |   LOW   |
 *    orderOut2  |   HIGH |  LOW   |   HIGH  |   LOW   |
 * --------------------------------------------------------
 */
void judge()
{
  //!计算20次读取G的平均值
  int aver = 0;
  for (int a = 0; a < con;a++)
  {
    gather();
    delay(1); 
    aver += g;
  }
  aver /= con;

  //!读值稳定了
  if (abs(aver - g) <= 10)
  {
    //黑色 r:49,g:62,b:58
    //粉色 r:255,g:181,b:236
    //跑 r:255,g:78,b:107
    //白色 r:255,g:255,b:255
//    Serial.println(aver);

    //!是黑色的话
    
    if(aver<90)
    {
      if(r<90)//黑色
        {
          if (state)//abandon
          {
            digitalWrite(orderOut1, LOW);
            digitalWrite(orderOut2, HIGH);
            Serial.print("黑色1");Serial.println(digitalRead(orderOut1));Serial.println(digitalRead(orderOut2));
          }
          else //use
          {
            digitalWrite(orderOut1, HIGH);
            digitalWrite(orderOut2, LOW);Serial.println(digitalRead(orderOut1));Serial.println(digitalRead(orderOut2));
            Serial.print("黑色2");
          }
        }
        else 
        {
          digitalWrite(orderOut1,LOW);
          digitalWrite(orderOut2, LOW);
     //     Serial.println("无球");
        }
    }
    else
    {
      if(aver<190)
      {
          digitalWrite(orderOut1,HIGH);
          digitalWrite(orderOut2, HIGH);
          Serial.println("Pink");
      }
      else
      {
        if(state)
        {
          digitalWrite(orderOut1,HIGH);
          digitalWrite(orderOut2, LOW);
          Serial.println("白色1");
          Serial.println(digitalRead(orderOut1));
          Serial.println(digitalRead(orderOut2));
        }
        else
        {
          digitalWrite(orderOut1,LOW);
          digitalWrite(orderOut2,HIGH);
          Serial.println("白色2");
          Serial.println(digitalRead(orderOut1));
          Serial.println(digitalRead(orderOut2));
          Serial.println(digitalRead(orderOut1));Serial.println(digitalRead(orderOut2));
        }
      }
    }
  }
}
     









/*     
    if (aver < 90&& r<90)
    {
      Serial.print("黑色");
      if (state)
      {
        digitalWrite(orderOut1, LOW);
        digitalWrite(orderOut2, HIGH);
        Serial.print("丢弃");
      }
      else
      {
        digitalWrite(orderOut1, HIGH);
        digitalWrite(orderOut2, LOW);
        Serial.print("使用");
      }
    }
    else
    {
      if (r> 170&&(g+b)<300)
      {
        digitalWrite(orderOut1, LOW);
        digitalWrite(orderOut2, LOW);
        Serial.println("跑");
      }
      else
      {
        if (aver < 220)
        {
          Serial.println("粉红");
          digitalWrite(orderOut1, HIGH);
          digitalWrite(orderOut2, HIGH);
        }
        else
        {
          if (state)
          {
            Serial.println("白色");
            digitalWrite(orderOut1, HIGH);
            digitalWrite(orderOut2, LOW);
            Serial.print("使用");
          }
          else
          {
            Serial.println("白色");
            digitalWrite(orderOut1, LOW);
            digitalWrite(orderOut2, HIGH);
            Serial.println("丢弃");
          }
        }
      }
    }
  }
}
*/
void loop()
{
        Serial.print("r:");
      Serial.print( r, DEC);
      Serial.print(",g:");
      Serial.print( g, DEC);
      Serial.print(",b:");
      Serial.println(b, DEC);
  float Data = 0;
  int i = 0;
  for (i = 0; i < 10; i++)
  {
    delay(2);
    Data += digitalRead(Switch);
  }
  if (Data > 5)
  {  state = 1;  Serial.println("抽到红方球");
  }
  else
   { state = 0;  Serial.println("抽到蓝方");
   }
  judge();
  delay(10);
}
