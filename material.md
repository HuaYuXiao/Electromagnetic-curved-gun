# Material

## electromagnetic cannon

![image](https://user-images.githubusercontent.com/117464811/231820176-466c884c-217b-484c-8af9-958f97e4cc85.png)

No matter what the scoring point is, the electromagnetic cannon is a must. But we took advantage of it and bought a finished kit directly online. As shown in the picture.

![image](https://user-images.githubusercontent.com/117464811/231820150-0d2388d1-ac9a-4043-a3d9-c709343d382c.png)

### capacitor

450V, 1000uF.

![F4719D554E784693D3D8C2C2186DCA70](https://user-images.githubusercontent.com/117464811/235860992-d28bf8cc-412b-4626-ba98-9b112e179643.jpg)

### tube

Because the inner diameter of the tube equipped by the merchant is not 10mm (the title requires 10mm-15mm), we have to rewind. The plastic tube used for the tube. We decided to use a 10mm inner diameter after a simple discussion.

### Coil

We decided to go around 8 layers, each layer 30 turns, a total of 240 turns. Under the guidance of the great god, this number of turns is within a reasonable range, 200-300 is fine. If the number of turns is large, the resistance will also be large, and the effect is not necessarily better.We chose is 1mm, which has been proved to be usable.

![26597C230C3883EC2BD37DC52F5990F7](https://user-images.githubusercontent.com/117464811/235859535-16c2a066-5e58-4a23-bc2d-08ae5fee5144.jpg)

### bullet

The projectile also used a 10mm steel ball, because the first feeling is that the smaller the better, the smaller the less energy required. It turns out that sometimes the first feeling is not necessarily optimal. you can install a laser pointer on the barrel to facilitate debugging. 

### Gimbal

This online one has a nice job done. Ordinary two-servo ones can also be used, because we basically chose to put only the muzzle on the last stage of the servo. So the weight is not very large, and its requirements for the system are to be stable when shooting, after all, there are some small impacts, which will reduce the accuracy. Our group finally chose to use two 42-step servo motors as power, the advantage is that the torque is large, the disadvantage is that there must be a starting point of the card position, and the pulse width and angle of the servo directly correspond. Table turntable bearings are indeed easy to use.

![D62F10B048DE785D39F48ED5253581F2](https://user-images.githubusercontent.com/117464811/235860286-2bf3ea61-4c32-4e13-86eb-bc0e11bd639c.jpg)

## Master 

Arduino UNO

![167B1D58CE3C4456E40D55EEB5AD0711](https://user-images.githubusercontent.com/117464811/235858074-f358eb22-c40f-4afb-a13e-b8e2ccf6925f.jpg)

## Visual

OPENMV. The image processing uses a simple algorithm to find color patches. The test is responsible for receiving the data of the module.

![416749AA12AD3B9682436BE9F18F6B28](https://user-images.githubusercontent.com/117464811/235858359-e7f18565-ae3d-4d0c-89b9-99cbb667de74.jpg)

## Ranging

This is the laser ranging module. Some people use ultrasonic modules, which may not work well. OpenMV and laser ranging are placed on the first stage servo disk, only the yaw angle, and the laser ranging we rack to be about the same height as the marker.


