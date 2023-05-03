# Electromagnetic-curved-gun

![1D8443702C919EDBCE6AD8C5E8576A87](https://user-images.githubusercontent.com/117464811/235859354-06c15df1-6630-468f-8b30-a5f220c59daf.jpg)

## Test debugging plus fill pits

### Projectile selection

Regarding the projectile material and shape, we directly chose steel balls. Maybe someone will use steel columns or bullet shapes?

There are some steel balls on Taobao that are not made of the same material, and the coil is not attractive to them. This steel ball looks brighter and does not rust so easily.

Then the diameter of the steel ball, we chose the same 10mm as the inner diameter of the tube at the beginning. After experiments, it was found that if the diameter of the steel ball is smaller, the accuracy is better. So we chose 6mm later, as for why not use 8mm, because 8mm bought fakes, can't suck ... My guess is: the steel ball is bigger, and there will be more friction with the tube, so the accuracy is affected.

### Initial position of the projectile

Where is it better to place the projectile at the beginning of the coil?

After trial and error, we found that half of the steel ball into the coil can hit the farthest under the same conditions.

When the position is selected, everyone must fix the limit to death. Otherwise, it will affect the curve fitting later

### Transmit mode selection and ADC voltage

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

### Curve fitting

Theoretically, the relationship between distance and angle is an implicit function. We tried to solve it, and then found that it was better to fit the big method.

![image](https://user-images.githubusercontent.com/117464811/231823639-f40643b3-4407-40a4-b69a-e8a65096db96.png)

We chose an angle of 10–30°, starting with a voltage that can hit two to three meters. Secondly, test each additional degree, the change of distance, each angle at least 10 experiments, take the average. Finally, it is fitted with MATLAB.

This step is very interesting because we have done a solid job of machinery and hardware, and there are not many problems. When testing, be sure to keep an eye on the electromagnetic gun, as soon as there is something wrong and immediately cut off, I guess you will not like to hear the sound of this capacitor exploding.

Because only DC regulated voltage can be used, the electromagnetic gun can only be powered by the student power supply, which should be noted that the power supply should be selected in parallel, otherwise the current supply cannot be supplied.
