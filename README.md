# Electromagnetic-curved-gun-in-Arduino

## 电磁炮制作
No matter what the scoring point is, the electromagnetic cannon is a must. But we took advantage of it and bought a finished kit directly online. As shown in the picture.
![image](https://user-images.githubusercontent.com/117464811/231820150-0d2388d1-ac9a-4043-a3d9-c709343d382c.png)

The principle is still relatively simple, the boost charges the capacitor, and the capacitor generates a magnetic field through the coil, and the magnetic field attracts the steel ball. But after the stuff came back, we made some changes.
![image](https://user-images.githubusercontent.com/117464811/231820176-466c884c-217b-484c-8af9-958f97e4cc85.png)

### 线圈
Because the inner diameter of the tube equipped by the merchant is not 10mm (the title requires 10mm-15mm), we have to rewind. The plastic tube used for the tube. We decided to use a 10mm inner diameter after a simple discussion, and then the projectile also used a 10mm steel ball, because the first feeling is that the smaller the better, the smaller the less energy required. It turns out that sometimes the first feeling is not necessarily optimal. you can install a laser pointer on the barrel to facilitate debugging.

![image](https://user-images.githubusercontent.com/117464811/231820351-0566b2e8-c7fd-4cfa-990d-da94fd772923.png)

#### coil parameters
we decided to go around 8 layers, each layer 30 turns, a total of 240 turns, and then after the guidance of the great god, this number of turns is within a reasonable range. If you go around, 200-300 is fine. If the number of turns is large, the resistance will also be large, and the effect is not necessarily better.
#### The diameter of the enameled wire 
we chose is 1mm, and some students use 0.8mm, which has been proved to be usable.

If the enameled wire is not easy to fix, you can put on 502 glue, as for both sides, you can try electrical glue, or 3M tape stacked, or other things that can be stuck. We ended up carving the cards with an engraving machine.

### 主控及模块

#### 主控 
我们自己画了一个STM32F407核心板。没有的同学完全可以用网上买的核心板。

#### 视觉 
毫无疑问的我们买了OPENMV，比赛的时候我们用的长焦镜头。the image processing uses a simple algorithm to find color patches. The test is responsible for receiving the data of the module.

#### 测距 
这个用的激光测距模块，有点小贵。有人用超声波模块，可能效果不太好。OpenMV and laser ranging are placed on the first stage servo disk, only the yaw angle, and the laser ranging we rack to be about the same height as the marker.

#### 键盘 
一般人都用的矩阵键盘，我们买了串口屏。串口屏的好处是，简单易上手，不用写矩阵键盘的行列扫描程序。

![image](https://user-images.githubusercontent.com/117464811/231821414-18186a1d-1d53-4c85-b650-2d34a519d3a3.png)

#### 云台 
这个网上有一个做的不错的。事实上，普通的两个舵机的也能用，因为我们基本上都选择了只把炮口放最后一级舵机上。所以重量不是很大，它对系统的要求是，射的时候要稳，毕竟有一些小小的冲击力，会让精度下降。
不过由于我们学校比较有钱，所以我们组最后选择了用两个42步进伺服电机做动力，优点是力矩大，缺点是得有起始点的卡位，而舵机的脉冲宽和角度直接对应。餐桌转盘轴承的确好用。

![image](https://user-images.githubusercontent.com/117464811/231821549-74fd50f2-a9c8-4ee3-9dfb-d0b5e30f064c.png)

## 测试调试加填坑

### 弹丸选择
Regarding the projectile material and shape, we directly chose steel balls. Maybe someone will use steel columns or bullet shapes?

There are some steel balls on Taobao that are not made of the same material, and the coil is not attractive to them. This steel ball looks brighter and does not rust so easily.

Then the diameter of the steel ball, we chose the same 10mm as the inner diameter of the tube at the beginning. After experiments, it was found that if the diameter of the steel ball is smaller, the accuracy is better. So we chose 6mm later, as for why not use 8mm, because 8mm bought fakes, can't suck ... My guess is: the steel ball is bigger, and there will be more friction with the tube, so the accuracy is affected.

### 弹丸初始位置

Where is it better to place the projectile at the beginning of the coil?

After trial and error, we found that half of the steel ball into the coil can hit the farthest under the same conditions.

When the position is selected, everyone must fix the limit to death. Otherwise, it will affect the curve fitting later

### 发射模式选择与adc采电压
Launch mode, there are currently two options. From the experimental data, both can be fitted with. 
One is to change the voltage at a fixed angle. 

![image](https://user-images.githubusercontent.com/117464811/231822309-a35beae1-3b57-4249-9cdb-e1e2029c8b34.png)

The other is the constant voltage change angle.

![image](https://user-images.githubusercontent.com/117464811/231822361-92c695b7-c1a8-4976-bb83-9ecfc93fb873.png)

the last piece of data for distance and angle, and it can be seen that it is approximately linear at about 20°. In fact, we chose a range of 10°–30°.

![image](https://user-images.githubusercontent.com/117464811/231823404-6ce263d7-d894-4e55-94c9-f7c38579050f.png)

most of us use the constant voltage variable angle mode. Students who did it in another mode did well.

Both modes rely heavily on one thing: accurate and timely measurement of voltage. When we do, the waveform noise of the AD sampled data is very large, and at its worst, under the same conditions, it has three drop points. 

The solution is, first, the power supply of the ADC is stable, and the average number of filters of the AD software is a little more. Our approach is not to use ADC, use a comparator, one end of the comparator is connected to the voltage of the capacitor, the other end is set with a sliding rheostat (after setting, there is no need to change), the output of the comparator is connected to a solenoid valve, and the solenoid valve is used as a control switch. Perfect to avoid the pit of AD sampling.

### 曲线拟合
Theoretically, the relationship between distance and angle is an implicit function. We tried to solve it, and then found that it was better to fit the big method.

![image](https://user-images.githubusercontent.com/117464811/231823639-f40643b3-4407-40a4-b69a-e8a65096db96.png)

We chose an angle of 10–30°, starting with a voltage that can hit two to three meters. Secondly, test each additional degree, the change of distance, each angle at least 10 experiments, take the average. Finally, it is fitted with MATLAB.

This step is very interesting because we have done a solid job of machinery and hardware, and there are not many problems. When testing, be sure to keep an eye on the electromagnetic gun, as soon as there is something wrong and immediately cut off, I guess you will not like to hear the sound of this capacitor exploding.

Because only DC regulated voltage can be used, the electromagnetic gun can only be powered by the student power supply, which should be noted that the power supply should be selected in parallel, otherwise the current supply cannot be supplied.
