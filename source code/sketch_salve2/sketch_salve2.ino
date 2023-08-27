// 此uno作为从机2,用于控制OLED显示屏和声音传感器（KV-037)
#include <Wire.h>
#include <Adafruit_GFX.h>//显示屏所需要的父类库
#include <Adafruit_SSD1306.h>//显示屏需要的子类库
Adafruit_SSD1306 oled(128, 64, &Wire, -1);//对显示屏进行定义
const int soundPin = A3;//作为声音分贝的获取的模拟接口
int num=1;//代表歌曲序

void setup() {
  analogWrite(5,100);//通过5号引脚，调节lcd1602背光度,这里由于2号从机引脚不够用，这里用三号机的5号引脚代替
  oled.begin(SSD1306_SWITCHCAPVCC,0X3C);//初始化显示屏,0X3C为iic通信的地址，通过调用iic_Scanner来获取
  Wire.begin(3);//作为2号从机，iic通信地址为3
  Wire.onReceive(receiveEvent);//该函数在从机端注册一个事件，当从机收到主机发送的数据时即被触发//是否放loop里？？？
  Serial.begin(9600); //初始化与计算机的串口通信
}

void loop() {
  oled.clearDisplay();//清除oled缓存
  oled.setTextSize(2);//设置字体大小为1
  oled.setTextColor(1);//设置字体颜色，默认为1（白色）

  oled.setCursor(0,0);//设置光标位置
  oled.print("sq:");//显示歌曲序的英文

  //歌曲为20首，对应歌曲数为1-20,新歌老歌各十首，1-10为新歌，11-20为老歌
  oled.setCursor(35,0);//显示歌曲序（1-20）
  oled.print(num);


  if(num<=10){//判断新老歌，小于或等于10为新歌
    oled.setCursor(90,0);
    oled.print("new");//显示新歌
  }
  else if(num>10){//判断新老歌，大于10为老歌
    oled.setCursor(90,0);
    oled.print("old");//显示老歌
  }

  int value =analogRead(soundPin);//检测麦克风分贝大小
  Serial.println(value);//显示麦克风音量
  delay(500);
  if(value<125){ //小于多少待测试!!!
  oled.setCursor(0,25);
  oled.print("low voice"); 
  }
  oled.setCursor(0,50);
  oled.print("db:");//反馈声音太小的英文
  

  oled.setCursor(35,50);
  oled.print(value);//显示音量的英文

  oled.display();
  delay(2000);
}

void receiveEvent(int howMany) {//当从机2接收到数据即被触发的事件，该事件带有一个int型参数（从主机读到的字节数）且没有返回值
  String s="";
  while(Wire.available()){
    char m=Wire.read(); 
    s+=(char)m;
    delay(10);
  }
  num=s.toInt();//用toInt()方法，把String转化为int
}