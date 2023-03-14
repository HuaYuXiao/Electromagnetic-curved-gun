import sensor, image, time, math
from pyb import UART
import json
import ustruct

sensor.reset()
sensor.set_framesize(sensor.QVGA)
sensor.set_pixformat(sensor.RGB565)
sensor.skip_frames(time = 2000)
#sensor.set_auto_gain(False) # #关闭自动增益
#sensor.set_auto_whitebal(False) # 关闭白平衡
#sensor.set_auto_exposure(False,2000);  #控制曝光时间,单位为us
sensor.set_hmirror(True); #水平镜像翻转
sensor.set_vflip(True); #垂直方向翻转
#sensor.set_windowing((22,7,110,105));#在要处理的图像中提取出的要处理的区域 感兴趣区

clock = time.clock()
uart = UART(3,9600)   #定义串口3变量    P4 TX<-->PA10  P5 RX<-->PA9
uart.init(9600, bits=8, parity=None, stop=1) # init with given parameters

#binary_threshold = (0, 156)
find_threshold = (39, 77, 38, 85, -95, 86)
K = 5350;  #自己选取一个合适的校准值

def find_max(blobs):    #定义寻找色块面积最大的函数
    blobs.sort(key=lambda x:x.pixels(),reverse=True); #排序
    max_blob={}             #定义一个变量存储最大像素
    length=len(blobs);
    if length>0:            #长度大于0则有
        max_blob=blobs[0];
    return max_blob;

def sending_data(cx_max,cy_max):  #发送数据
    global uart;
    #frame=[0x2C,18,cx%0xff,int(cx/0xff),cy%0xff,int(cy/0xff),0x5B];
    #data = bytearray(frame)
    data = ustruct.pack("<bbhhb",              #格式为俩个字符俩个短整型(2字节)
                   0x2C,                       #帧头1
                   0x12,                       #帧头2
                   int(cx_max), # up sample by 4    #数据1
                   int(cy_max), # up sample by 4    #数据2LCD_ShowStringLCD_ShowString
                   0x5B);
    uart.write(data);   #必须要传入一个字节数组

while(True):
    clock.tick()
    img = sensor.snapshot()
    blobs = img.find_blobs([find_threshold],area_threshold=150)
    if blobs:
        max_blob=find_max(blobs)
        img.draw_rectangle(max_blob.rect(),color=(0,0,255))#找到的最大区域用红色矩形框起来
        img.draw_cross(max_blob.cx(), max_blob.cy(),color=(0,0,255))#中心十字
        img.draw_cross(160, 120,color=(0,0,255)) # 在中心点画标记    有利于观察两个中心重合
        img.draw_line((160,120,max_blob.cx(),max_blob.cy()), color=(0,0,255));
        print('position:',max_blob.cx(),max_blob.cy())
        print('Z:',max_blob.cx()-160,max_blob.cy()-120)
        phi = (max_blob.w() + max_blob.h())/2;
        length = K/phi;         #获得距标靶距离
        print("Length=",length);
        data=[]
        data_out="{[%d,%.3f]}" % (max_blob.cx(),length) #方式1
        #data_out = json.dumps(set(data))  #方式2
        #print("pixels",max_blob.pixels());
        if(abs(160-max_blob.cx())<=20):
            if(max_blob.pixels()>270):
             uart.write(data_out +'\n')
             print('you send:',data_out)
        else:
           print("not found!")




