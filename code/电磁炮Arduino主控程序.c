//蓝牙仅用串口
#include <Servo.h>
#include <SoftwareSerial.h>

SoftwareSerial softSerial(10, 11); // RX, TX
Servo servo_1,servo_2;  //定义2个舵机  1为底座控制偏转角度   2为竖轴控制曲射距离
const int baseMin = 60;
const int baseMax = 120;    //限制舵机旋转角度
const int rArmMin = 0;
const int rArmMax = 180;

int Step=15;   //控制步长
int AutoStep=10; //自动模式下的步长
int rechargePin = 3;  //充电按钮
int launchPin = 2;    //发射按钮
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
   pinMode(rechargePin, OUTPUT);  // 按键模式定义
   pinMode(launchPin, OUTPUT); 
   servo_1.attach(5);  // 连接舵机
   servo_2.attach(6);
   Serial.begin(9600); 
   softSerial.begin(9600);
   Serial.println("请输入选择模式......");
}

void loop() {
if( Serial.available()>0 ){   //如果软件串口有HC-06发来的数据
     char inChar  =  Serial.read();  
     Mode_selection(inChar);
                         }
}  
 
//*******************以下为自定义************************************

void Mode_selection(char inChar){
     if(inChar=='a'||inChar=='b'){
     switch(inChar){     
       case 'a':    
                Mode1();        //模式一a  输入距离d和角度a
                break;
       case 'b':    
                Mode2();        //模式二b 自动搜寻 搜到即停下 
                break;       
    //   default:
    //            return;      
       }
     }
   inChar = "";  //清空重新输入
} 

//模式1  手动
void Mode1(){
float serialData=0.0;
long T1=0;
int Angle;
int K=0;
                Serial.println("Mode1");
                Serial.println("请输入距离");
                delay(5000);         //等待输入时间
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
                delay(5000);         //等待输入时间
                if( Serial.available()>0 ) {   
                 Angle  =  Serial.parseInt();  
                 Serial.print("角度： ");
                 Serial.println(Angle); 
                 K=2;}   
                     }           
              if(K==2) { 
                   T1=Battery(Distance,Angle);  //计算调整炮台位置
                   delay(500);
                   Charging(T1);               //充能
                   delay(200);
                   Launch();                 //发射
                   delay(500);
                   Reset();                  //然后复位,使舵机复位两个都90°                    
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
          for(int k=90;k<=baseMax;k++){ //转到120°角  以90为中轴
          servo_1.write(k);
          delay (Step);      //这里的步长可以改  不取照片可以加快
          if(k==120) {temp=1;}  // //确定初始化转到120°  则赋值1                  
                                 }                
     if(temp==1){
             T2 = Autosearch();      //自动搜寻，调整炮台位置，返回充能时间，且找到赋值Find为1
             delay(50);
         if(Find==1){ 
             Charging(T2);              //充能
             delay(200);
             Launch();                  //发射
             Serial.println("射击完毕");
             delay(5000);
             Find=0; //初始化   复位,使舵机复位两个都90° 
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
int angle=120; //120°
int Angle;
while(!(Findaim)){
    
    //Serial.println("1");
    if(softSerial.available())
    {
       Serial.println("2");
       Length = getList();    //把X水平偏移存在list.data[i][0]，把收到的距离作为返回值
       Serial.println(Length);
      //  if( abs(160-list.data[0][0])<=20 )
      // {
        Serial.println("3"); 
        Serial.println("找到目标");
        Distance = Length;
        Angle = servo_1.read(); //把当前舵机角度赋值给他
        T2_2=Battery(Distance,Angle);  //计算调整炮台位置
        // Serial.println(T2_2);
        delay(500);
        Findaim=1; 
        Find=1;
        
      //  } 
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
             angle1 = Angle ;  //需要将角度转弧度 
             T = a*pow(Distance,b)+c; ;//带入拟合函数
             t = T;       //毫秒转为微秒
             servo1(angle1);
             delay(50);
             servo2(angle2);
             delay(50);
             //Serial.print("t:");  Serial.println(t);
 return t;
}

//舵机1 、2
void servo1(int Angle) {    //底座
int fromPos=0;
int toPos=0;
     toPos = Angle;   //还需计算转角度  需要去测试数据才能确定
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
void servo2(int Angle) {    //曲射距离 俯仰
int fromPos=0;
int toPos=0;
     toPos = Angle;   //还需计算转角度  需要去测试数据才能确定
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
    digitalWrite(rechargePin,HIGH);  //按键3
    delay(t);    //控制充电时间，改变电压从而改变距离 
    digitalWrite(rechargePin,LOW);
    Serial.print("充能时间（um）： ");
    Serial.println(t);
    Serial.println("充能就绪  (๑╹ヮ╹๑)ﾉ ");
  }else{Serial.println("充能超出范围！ (乂｀д´)");}
  
}

void Launch(){
    digitalWrite(launchPin,HIGH);
    delay(200);             
    digitalWrite(launchPin,LOW);
    Serial.println("发射 ✈");
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
  String s = detectString();  //读到大括号里的字符
  String numStr = "";
  for(int i = 0; i<s.length(); i++)
  {
    if(s[i]=='['){
      numStr = "";
    }
    else if(s[i] == ','){
      list.data[list.len][0] = numStr.toInt();//变为整形
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
    //Serial.println(sum);
    }
   }
     // list.data[list.len][1] = numStr.toInt();
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
