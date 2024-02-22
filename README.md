# FlyshipProject
<div id="header" align="center">
  <img src="/Photos/Preview.png"/>
</div>
## Overview:
The project implements realistic aircraft movements with convenient adjustment of physical properties.
Developed with Unreal Engine 4.27

> [!NOTE]
> Since the lift coefficient is calculated using real experiments and the process of creating a mathematical model is very complex, the code for calculating the lift coefficient was taken from here:
> https://ieeexplore.ieee.org/document/7152411

## Input:
- W/S: pitch
- A/D: yaw
- Mouse X: roll
- Space: flaps
- Shift: thrust
- C: free camera
- V: camera change
- Q: pause

## Adjustment:
In FlyMovementComponent the main settings for maximum speed and acceleration of the aircraft. 

<div id="header" align="center">
  <img src="/Photos/FlyComponentSettings.png"/>
</div>

The required number of components of aircraft parts is added to the actor. The settings for each aircraft are individual.
The location of aircraft parts is important.

<div id="header" align="center">
  <img src="/Photos/SurfaceWind.png""/>
</div>

The required number of wheels with the names of the model bones is added to the array of component wheels.

<div id="header" align="center">
  <img src="/Photos/Wheels.png""/>
</div>

Each wheel can be customized individually.

<div id="header" align="center">
  <img src="/Photos/Wheels_1.png""/>
</div>
<div id="header" align="center">
  <img src="/Photos/Wheels_2.png""/>
</div>
