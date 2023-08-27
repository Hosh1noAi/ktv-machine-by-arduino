//此uno作为作为从机1，用于控制LED1602  
#include <LiquidCrystal.h>//LED1602显示屏所需要的库
#include <Wire.h>
#include <SD.h>
#include <SPI.h>
LiquidCrystal lcd(9,8,4,5,6,7);//定义LED1602
const int chipSelect = 10;
File myFile;
int num=1;//代表歌曲序
bool start=false;
void setup() { 
  lcd.begin(16,2);//初始化LED1602显示屏的宽度和高度
  Wire.begin(2);//初始化iic通信，作为1号从机，iic通信地址为2
  Wire.onReceive(receiveEvent);//该函数在从机端注册一个事件，当从机收到主机发送的数据时即被触发
  Serial.begin(9600); //与计算机进行串口通信
  attachInterrupt(0,pause,RISING);//初始化中断引脚，中断编号为0，实际引脚为D2，中断触发方式为RISING（下降沿触发，即高电平变低电平）
  
}

void loop() {
  if(start) sd(num);
}
void receiveEvent(int howMany) {//当从机1接收到数据即被触发的事件，该事件带有一个int型参数（从主机读到的字节数）且没有返回值
  start=true;
  String s="";
  while(Wire.available()){
    char m=Wire.read(); 
    s+=(char)m;
    delay(10);
  }
  num=s.toInt();//用toInt()方法，把String转化为int
  sd(num);
}

void pause(){//中断函数
  lcd.clear();
  lcd.begin(16,2);
  lcd.setCursor(0,0);
  lcd.print("Song Pause");
  while(digitalRead(2) == HIGH){}
  lcd.clear();
}

void sd(int num){//用于从sd卡中获取歌词
  int count=0;//代表一行的脚标
  int start=0;//将第一个时间节点单独考虑
  String b="";//用于存取歌词
  String time1="";//时间节点1
  String time2="";//时间节点2
  int time=0;//时间节点1和时间节点2的差值，即一句歌词出现在LCD的持续时间
  while (!Serial);
  Serial.print("Initializing SD card...");
  pinMode(chipSelect, OUTPUT);
  if (!SD.begin(chipSelect)) {
    Serial.println("initialization failed.");
    return;
  }
  Serial.println("initialization done.");
  myFile = SD.open("song"+(String)num+".txt");
  if (myFile) {
    Serial.println("song"+(String)num+".txt");
    while(myFile.available()){
      char a=myFile.read();
      if(start==0){
        if(a==(char)'[') count=0;
        if(0<count&count<9)  time1+=a;
        else if(count==9)  start=1;
      }
      if(count>9&&a!=(char)'[') b+=a;
      if(a==(char)'[') {
        count=0;
        count++;
        continue;
      }
      if(0<count&count<9)  time2+=a;
      time =timecount(time2)-timecount(time1);
      if(count==9&&start==1){
      lcd.clear();//清除屏幕上的内容，并将光标定位到屏幕左上角
      lcd.begin(16,2);
      lcd.print(b);
      Serial.print(b);
      for(int i=0;i<13;i++){//滚动歌词
        lcd.scrollDisplayLeft();
        delay(time/13);
      }
      time1=time2;
      time2="";
      b="";
      }
      count++;
      }
    
    myFile.close();
    Serial.println("done.");
  } 
  else {
    Serial.println("error opening test.txt");
  }
}

int timecount(String time){//用于转换时间
  int times=0;
  times+=(int)time.charAt(1)*60000;
  int i=(int)time.charAt(3)*10+(int)time.charAt(4);
  times+=i*1000;
  int j=(int)time.charAt(6)*10+(int)time.charAt(7);
  times+=i*10;
  return times;
}
