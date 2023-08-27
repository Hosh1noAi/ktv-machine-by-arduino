// 使用SV5W模块，此uno作为主机，只发送数据给从机，不接受从机的信息。且通过创造软串口与SV5W模块进行串口通信
#include<SoftwareSerial.h>//使用软串口
//原因：uno 只有0（RX)和1（TX)一组硬件串口，这组串口常用于与计算机进行通信，如果需要与SV5W模块进行串口通信需要使用软件模拟串口（软串口）
#include <Wire.h>//使用iic通信
bool mp3_control = false;//bool变量，语音模块是否处于播放状态的状态量
SoftwareSerial softSerialsv(10,11);//定义软串口，10为RX，11为TX
unsigned char data1=0x00;//使用无符号数来代替0X00，直接写0X00会造成write（）方法重载，原因不详？？？
String recipt="";//用于接受SV5W模块的返回值
int num=0;
void setup()
{	
  Wire.begin();//初始化iic通信，括号内没有写地址值，即作为主机接入
  Serial.begin(9600);//与计算机进行串口通信
  softSerialsv.begin(9600);//与sv5w进行软串口通信
  softSerialsv.listen();//监听软串口softSerialsv
//模块状态控制传感器引脚（按键）
	pinMode(2, INPUT);//控制播放与暂停，接下来的2,3,4,5,6,7的功能都要在flag==true时，才能实现
  pinMode(3, INPUT);//切换上一首
  pinMode(4, INPUT);//切换下一首
  pinMode(5, INPUT);//控制音量加
  pinMode(6, INPUT);//控制音量减
  pinMode(7, INPUT);//新歌老歌分类，这里采用指定歌曲来代替分类功能
  pinMode(8,OUTPUT);//初始化8号引脚，用于给1号从机发送中断触发方式
  digitalWrite(8,LOW);//默认为low，为high时1号从机执行中断函数
}

void loop()
{
	//控制模块播放歌曲（按键传感器高电平触发）
	if((digitalRead(2) == HIGH)&&(mp3_control == false))
	{
		
		delay(500);//延时去抖
		
		
		mp3_control = true;//修改状态量mp3_control为true（播放状态）
    
		//发送播放指令  AA 02 00 AC
		softSerialsv.write(0xAA);
		softSerialsv.write(0x02);
		softSerialsv.write(data1);
		softSerialsv.write(0xAC);
		delay(1000);
		
		//发送（当前目录）顺序播放指令 AA 18 01 07 CA
		softSerialsv.write(0xAA);
		softSerialsv.write(0x18);
		softSerialsv.write(0x01);
		softSerialsv.write(0x07);
		softSerialsv.write(0xCA);
    if(num==0){
      writer2_1();
      writer3_1();
      num++;
  }
  inspect_songQequence();
    digitalWrite(8,LOW);
	}
	//控制模块暂停播放（按键传感器高电平触发）
	if((digitalRead(2) == HIGH)&&(mp3_control == true))
	{
		
		delay(500);//延时去抖
		mp3_control = false;//修改状态量mp3_control为false（暂停状态）

		//发送暂停指令 AA 03 00 AD
		softSerialsv.write(0xAA);
		softSerialsv.write(0x03);
		softSerialsv.write(data1);
		softSerialsv.write(0xAD);
    
     digitalWrite(8,HIGH);
        
	}
//控制模块播放上一首（按键传感器高电平触发）
  if((digitalRead(3) == HIGH)&&(mp3_control == true))
	{
		
		delay(500);//延时去抖
		

		//发送上一首指令 AA 05 00 AF
		softSerialsv.write(0xAA);
		softSerialsv.write(0x05);
		softSerialsv.write(data1);
		softSerialsv.write(0xAF);

    inspect_songQequence();
    
	}
//控制模块播放下一首（按键传感器高电平触发）
   if((digitalRead(4) == HIGH)&&(mp3_control == true))
	{
	
		delay(500);	//延时去抖
		

		//发送下一首指令 AA 06 00 B0
		softSerialsv.write(0xAA);
		softSerialsv.write(0x06);
		softSerialsv.write(data1);
		softSerialsv.write(0xB0);

    inspect_songQequence();
    
	}
  //控制模块音量加（按键传感器高电平触发）
   if((digitalRead(5) == HIGH)&&(mp3_control == true))
	{
	
		delay(500);	//延时去抖
		

		//发送音量加指令 AA 14 00 BE
		softSerialsv.write(0xAA);
		softSerialsv.write(0x14);
		softSerialsv.write(data1);
		softSerialsv.write(0xBE);
	}
  //控制模块音量减（按键传感器高电平触发）
  if((digitalRead(6) == HIGH)&&(mp3_control == true))
	{
		
		delay(500);//延时去抖
		

		//发送音量减指令 AA 15 00 BF
		softSerialsv.write(0xAA);
		softSerialsv.write(0x15);
		softSerialsv.write(data1);
		softSerialsv.write(0xBF);
	}//控制模块新老歌分类（按键传感器高电平触发），这里采用指定歌曲来代替分类功能，后三位都是要改的
  if((digitalRead(7) == HIGH))
	{
		
		delay(500);//延时去抖
		

		//发送播放指定歌曲指令 AA 07 02 00 0B BE
		softSerialsv.write(0xAA);
		softSerialsv.write(0x07);
		softSerialsv.write(0x02);
		softSerialsv.write(data1);
    softSerialsv.write(0x0B);
    softSerialsv.write(0xBE);

     inspect_songQequence();
	}
 
}

// 查询当前曲目 ，适用于0-99首
void inspect_songQequence(){
  softSerialsv.write(0xAA);
	softSerialsv.write(0x0D);
	softSerialsv.write(data1);
  softSerialsv.write(0xB7);
  //将串口的十六进制转换十进制
  int i,j;
  String sq=""; 
  while (softSerialsv.available()) {
  int in = (char)softSerialsv.read();
  recipt+=in;
  recipt+=',';
  delay(2);
  }
  Serial.println(recipt);
  if(recipt.length()==17) sq=(String)recipt.charAt(11);//1-9首的情况  //这两行用（String）来转换的原因是：如果将两个字符类型的变量相加，结果会是一个整数，字符相加时，实际是将它们的ASCII码相加。
  else sq=(String)recipt.charAt(11)+(String)recipt.charAt(12);//10-20首的情况
  Serial.println(sq);
  writer2(sq);//将歌曲序传给slave1
  writer3(sq);//将歌曲序传给slave2
  recipt="";
}

//  给二号从机发送信息（开始播放）//考虑删除！！！
void writer2_1(){
  Wire.beginTransmission(2);//开始传输数据
    
  Wire.write('1');//发送开始显示字幕的信号
    
  Wire.endTransmission(); //结束传输

}

void writer3_1(){
  Wire.beginTransmission(3);//开始传输数据
    
  Wire.write('1');//发送开始显示字幕的信号
    
  Wire.endTransmission(); //结束传输

}

// 给二号从机发送信息（歌曲序）
void writer2(String sq){
  Wire.beginTransmission(2);//开始传输数据
  if(sq.length()==1) Wire.write(sq.charAt(0));//歌曲位数差别
  else {
    Wire.write(sq.charAt(0));
    Wire.write(sq.charAt(1));
  }
    
  Wire.endTransmission(); //结束传输

 }
 //给三号从机发送信息（歌曲序）
void writer3(String sq){
  Wire.beginTransmission(3); //开始传输数据
  if(sq.length()==1) Wire.write(sq.charAt(0));//歌曲位数差别
  else {
    Wire.write(sq.charAt(0));
    Wire.write(sq.charAt(1));
  }
  Wire.endTransmission(); //结束传输

 }
 