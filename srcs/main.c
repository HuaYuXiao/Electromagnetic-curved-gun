//蓝牙仅用串口
#include <Servo.h>
#include <SoftwareSerial.h>

// RX, TX
SoftwareSerial softSerial(10, 11); 
//定义2个舵机  1为底座控制偏转角度   2为竖轴控制曲射距离
Servo servo_1,servo_2;  
const int baseMin = 60;
//限制舵机旋转角度
const int baseMax = 120;    
const int rArmMin = 0;
const int rArmMax = 180;

//控制步长
int Step=15;  
//自动模式下的步长
int AutoStep=10; 
//充电按钮
int rechargePin = 3; 
//发射按钮
int launchPin = 2;    
char serialData=0; 
float Distance=0;  
int Find=0;   

typedef struct
{
  int data[1][2] = {{0,0}};
  int len = 0;
}List;
List list;

void setup() {
   // 按键模式定义
   pinMode(rechargePin, OUTPUT);  
   pinMode(launchPin, OUTPUT);
   // 连接舵机
   servo_1.attach(5);  
   servo_2.attach(6);
   Serial.begin(9600); 
   softSerial.begin(9600);
   Serial.println("请输入选择模式......");
}

void loop() {
  //如果软件串口有HC-06发来的数据
if( Serial.available()>0 ){   
     char inChar  =  Serial.read();  
     Mode_selection(inChar);
                         }
}  
 
//*******************以下为自定义************************************

void Mode_selection(char inChar){
     if(inChar=='a'||inChar=='b'){
     switch(inChar){ 
         //模式一a  输入距离d和角度a
       case 'a':    
                Mode1();        
                break;
         //模式二b 自动搜寻 搜到即停下
       case 'b':    
                Mode2();         
                break;       
      default:
               return;      
       }
     }
  //清空重新输入
   inChar = "";  
} 

//模式1  手动
void Mode1(){
  float serialData=0.0;
  long T1=0;
  int Angle;
  int K=0;
  Serial.println("Mode1");
  Serial.println("请输入距离");
  //等待输入时间
  delay(5000);         
  if( Serial.available()>0 ) {   
  serialData =  Serial.parseFloat(); 
  Distance = serialData; 
  Serial.print("距离： ");
  Serial.println(Distance);
  serialData=0.0;
  K=1;
  }       
  if(K==1)   {            
  Serial.println("请输入选角度");
    //等待输入时间
  delay(5000);         
  if( Serial.available()>0 ) {   
   Angle  =  Serial.parseInt();  
   Serial.print("角度： ");
   Serial.println(Angle); 
   K=2;}   
       }           
if(K==2) { 
  //计算调整炮台位置
     T1=Battery(Distance,Angle);  
     delay(500);
  //充能
     Charging(T1);               
     delay(200);
  //发射
     Launch();                 
     delay(500);
  //然后复位,使舵机复位两个都90°
     Reset();                                      
     T1=0;
  }
  else{Serial.println("未输入");}
  K=0;
  }

//模式2 自动
void Mode2(){
  int temp=0;
  long T2=0;
  int K=0;
  Serial.println("Mode2");
  Serial.println("开始");
  //转到120°角  以90为中轴
  for(int k=90;k<=baseMax;k++){ 
  servo_1.write(k);
    //这里的步长可以改  不取照片可以加快
  delay (Step);
    //确定初始化转到120°  则赋值1
  if(k==120) {temp=1;}           
   }                
if(temp==1){
  //自动搜寻，调整炮台位置，返回充能时间，且找到赋值Find为1
     T2 = Autosearch();      
     delay(50);
 if(Find==1){ 
   //充能
     Charging(T2);              
     delay(200);
   //发射
     Launch();                  
     Serial.println("射击完毕");
     delay(5000);
   //初始化   复位,使舵机复位两个都90° 
     Find=0; 
             } else{Serial.println("未找到目标");}
     Reset();                    
               }
     temp=0;  
     delay(10000);
}

//自动搜索
long Autosearch(){
  double Length=0;
  int Findaim=0;
  long T2_2=0;
  int n=1;
  int angle=120;
  int Angle;
  while(!(Findaim)){
    if(softSerial.available())
    {
       Serial.println("2");
      //把X水平偏移存在list.data[i][0]，把收到的距离作为返回值
       Length = getList();    
       Serial.println(Length);
        Serial.println("3"); 
        Serial.println("找到目标");
        Distance = Length;
      //把当前舵机角度赋值给它
        Angle = servo_1.read(); 
      //计算调整炮台位置
        T2_2=Battery(Distance,Angle);  
        // Serial.println(T2_2);
        delay(500);
        Findaim=1; 
        Find=1;
        clearList();
    }  
      angle=angle-2;
      servo1(angle);
      delay(200);
      if(angle<=60){angle=120;}
  }
  return T2_2;
}

//炮台
long Battery(float Distance, int Angle){  
  int angle1;
  int angle2=120;
  int t=0.0;
  double T=0.0;
  float a=5.1258 ;
  float b=0.6353 ;
  float c=0.1758 ;
  //需要将角度转弧度
   angle1 = Angle ;   
  //带入拟合函数
   T = a*pow(Distance,b)+c;
  //毫秒转为微秒
   t = T;       
   servo1(angle1);
   delay(50);
   servo2(angle2);
   delay(50);
 return t;
}

//舵机1 、2底座
void servo1(int Angle) {
  int fromPos=0;
  int toPos=0;
  //还需计算转角度  需要去测试数据才能确定
   toPos = Angle;   
   fromPos = servo_1.read();

   if (fromPos >= toPos  ){      
   for (int i=fromPos; i>=toPos; i--){
    servo_1.write(i);
    delay (Step);
   }
  }
   else {   
   for (int i=fromPos; i<=toPos; i++){
    servo_1.write(i);
    delay (Step);
   }
  } 
}

//曲射距离 俯仰
void servo2(int Angle) {    
  int fromPos=0;
  int toPos=0;
  //还需计算转角度  需要去测试数据才能确定
     toPos = Angle;   
     fromPos = servo_2.read();

     if (fromPos >= toPos  ){      
     for (int i=fromPos; i>=toPos; i--){
      servo_2.write(i);
      delay (Step);
     }
    }
     else{   
     for (int i=fromPos; i<=toPos; i++){
      servo_2.write(i);
      delay (Step);
     }
    } 
}  

//充能
void Charging(long t){
  if(t>100&&t<300){
    //按键3
    digitalWrite(rechargePin,HIGH);  
    //控制充电时间，改变电压从而改变距离
    delay(t);     
    digitalWrite(rechargePin,LOW);
    Serial.print("充能时间（um）： ");
    Serial.println(t);
    Serial.println("充能就绪");
  }else{Serial.println("充能超出范围！");}
}

void Launch(){
    digitalWrite(launchPin,HIGH);
    delay(200);             
    digitalWrite(launchPin,LOW);
    Serial.println("发射");
}

//复位
void Reset(){
  servo1(90);
  servo2(90);
  delay(50);
}

double getList()
{
  double b=0,sum=0,num;
  int a=0;
   //读到大括号里的字符
  String s = detectString(); 
  String numStr = "";
  for(int i = 0; i<s.length(); i++)
  {
    if(s[i]=='['){
      numStr = "";
    }
    else if(s[i] == ','){
      //变为整形
      list.data[list.len][0] = numStr.toInt();
      numStr = "";
    }
    else if(s[i]==']'){
    //Serial.println(numStr.length());
    if(numStr.length()==5){num=1;}
    if(numStr.length()==6){num=10;}
    if(numStr.length()==7){num=100;}
     for(int j=0;j<=numStr.length();j++)
   {
    a=numStr[j]-48;
    if(a<=9&&a>=0){
    //Serial.println(a);
    b=a*num;
    num=num/10;
    sum=sum+b;
    }
   }
      numStr = "";
      list.len++;
    }
    else{
      numStr += s[i];
    }
  }
  return sum;
}

String detectString()
{
  while(softSerial.read() != '{');
  return(softSerial.readStringUntil('}'));
}
void clearList()
{
  memset(list.data, sizeof(list.data),0);
  list.len = 0;
}
