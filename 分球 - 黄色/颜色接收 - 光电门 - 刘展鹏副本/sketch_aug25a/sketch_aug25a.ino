/**
 * @file sketch_aug25a.ino
 * @author your name (you@domain.com)
 * @brief 分球主机板
 * @version 0.1
 * @date 2019-09-04
 * 
 * @copyright Copyright (c) 2019
 * 
 */
#define orderSend 2 //本机————>炮台|“送球完毕”
#define stepPin 3     //步进电机脉冲输入引脚
#define dirPin 4      //步进电机转动方向引脚
#define orderRun 5    //本机————>主机|“分球结束，继续跑动”
#define orderPin A0   //主机————>本机|“靠边”
#define orderBack A1 //炮台————>本机|“炮台就绪”
#define pushButtonM A3 //分球原点光电门
#define pushButtonU A2 //送球限位
#define colorPin1 A5 //颜色识别板————>本机
#define colorPin2 6 //颜色识别板————>本机
void setup() {
  Serial.begin(9600);
  
  IO_init();

  Ready();
}

void loop()
{
  ///主机发来靠边信号
  int data = 0;
  for (int i = 0; i < 10; i++)
  {
    delay(2);
    data += digitalRead(orderPin);
  }
  data = 1;
  //!如果现在是靠边状态
  if (data < 5)
  {
    Serial.println("judge");
    int color1=0,color2=0;
    for (int a = 0; a < 20; a++)
    {
      delay(2);
      color1 += digitalRead(colorPin1);
      color2 += digitalRead(colorPin2);
    }
/**
 * --------------------------------------------------------
 *    引脚\意义   |  丢弃  |   使用  |  粉色   |   跑    | 
 *    orderOut1  |   LOW  |  HIGH  |   HIGH  |   LOW   |
 *    orderOut2  |   HIGH |  LOW   |   HIGH  |   LOW   |
 * --------------------------------------------------------
 */
    if (color1 <10)
    {
      if(color2>=10)
      {//!丢弃
        pink();
      }
      else 
      {//!跑
        //digitalWrite(orderRun,HIGH);
        Serial.println("跑");
      }
    }
    else
    { 
      if(color2<10)
      {//!我方球
        use();
      }
      else 
      {//!粉色
        pink();
      }
    }
  }
}

/**
 * @brief 端口初始化
 * 
 */
void IO_init()
{
  //!步进电机
  pinMode(stepPin, OUTPUT);   
  pinMode(dirPin, OUTPUT);
  digitalWrite(stepPin, LOW);
  digitalWrite(dirPin, HIGH);

  //！通信引脚
  pinMode(orderSend, OUTPUT);
  pinMode(orderRun, OUTPUT);
  pinMode(orderPin, INPUT_PULLUP);
  digitalWrite(orderRun, LOW);
  digitalWrite(orderSend, LOW);
  pinMode(orderBack, INPUT_PULLUP);
  pinMode(colorPin1, INPUT_PULLUP);
  pinMode(colorPin2, INPUT_PULLUP);

  //!限位和光电门
  pinMode(pushButtonM, INPUT_PULLUP);
  pinMode(pushButtonU, INPUT_PULLUP);
}

/**
 * @brief 步进电机走步函数
 * 
 * @param s 要走的脉冲数
 */

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

/**
 * @brief 步进电机走步函数
 * 
 * @param dir 转向
 *        0-left
 *        1-right
 * @param s 要走的脉冲数
 */
void Step(int dir,int s)
{
  digitalWrite(dirPin, dir);
  for (int a = 0; a < s; a++)
  {
    digitalWrite(stepPin, HIGH);
    delayMicroseconds(1000);
    digitalWrite(stepPin, LOW);
    delayMicroseconds(1000);
  }
  Serial.print("电机:  ");Serial.println(s);
}

/**
 * @brief 送有用的球上炮台
 * 
 */
void use()
{
  //！要求射球启动
  digitalWrite(orderSend,HIGH);

  //!程序卡住等待炮台就绪时拉高电位 
  while (digitalRead(orderBack)) {}

  //炮台就绪后送球上去
  int bu = digitalRead(pushButtonU);
  int bm = !digitalRead(pushButtonM);
  while(bu)
  {
    Step(0,50);
    bu = digitalRead(pushButtonU);
  }
    Serial.println("使用");

    //送球完毕
    digitalWrite(orderSend,LOW);
}

/**
 * @brief 送粉球上炮台
 * 
 */
void pink()
{
  //！要求射球启动
  digitalWrite(orderSend,HIGH);

  Serial.println("Pink");
  Serial.println(digitalRead(orderBack));

  //!程序卡住等待炮台就绪时拉高电位 
  while(!digitalRead(orderBack)){}
  
  //炮台就绪后送球上去
    int bu = digitalRead(pushButtonU);
    int bm = !digitalRead(pushButtonM);
    while(bu)
    {
      Step(0,50);
      bu = digitalRead(pushButtonU);
    }
    Serial.println("粉色");

    //送球完毕
    digitalWrite(orderSend,LOW);
}

/**
 * @brief 准备函数
 * @notes 先移动到最右的限位
 *        再寻找光电门
 *        以此到达原点位置
 */
void Ready()
{
  Serial.println("Ready");  
  delay(2);
  int bu = digitalRead(pushButtonU);
  int bm = !digitalRead(pushButtonM);

  ///寻找最右限位开关
  while(bu)
  {
      Step(1,10);
      bu = digitalRead(pushButtonU);
      bm = !digitalRead(pushButtonM);
      Serial.println("***************");
  }

  ///如果找到最右限位就反转找光电门
    while(bm)
    {
      Step(0,10);
      bm = !digitalRead(pushButtonM);
      Serial.println("********U-M");
      Serial.println(digitalRead(pushButtonM));
    }
}