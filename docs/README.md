# Chrome Dino Automation Project

**Team Members** <br>
**Ruiyi Gao & Yixiao Hu** <br>

Georgia Institute of Technology <br>
ECE 4180 Section B (Fall 2022) Final Project

## Table of Content
* [Project Idea](#project-idea)
* [Demo Videos](#demo-videos)
* [Parts List](#parts-list)
* [Block Diagram and Schematic](#block-diagram-and-schematic)
* [Software Design](#software-design)
* [Future Direction](#future-direction)
* [Source Code](#source-code)

## Project Idea
Our project idea is fairly simple! <br>
We want to play the chrome dino game through various ways (using different peripherals), including the ultimate goal of letting mbed play the game automatically. <br>

Using peripherals we used in labs, we came up with the following ways to manually play dino game using mbed:
1. **Push Buttons** <br>
We used two push buttons to represent the actions of the dino. <br>
The top push button makes the dino jump and the bottom makes it crouch. <br>
2. **Navigation Switch** <br>
Similar to the button idea, if nagivation switch is toggleed upward, the dino would jump. <br>
If the nagivation switch is toggled downward, the dino would crouch.
3. **Microphone** <br>
If the user claps their hand, the dino would jump.
4. **LIDAR** <br>
The player could put their hand in front of the LIDAR sensor and move toward or away form it. <br>
If the distance between LIDAR is greater than 20cm, the dino would jump. <br>
If the distance between LIDAR is less than 10cm, the dino would crouch. <br>

## Demo Videos
We provided an all-in-one demo so you could see every feature in one place, but there are also separate videos for each feature if you want to explore more! <br>

**All In One Demo** <br>
https://youtu.be/9GH7aEYIwp4

**Push Button Demo** <br>
https://youtu.be/t7vQpgjw0i8

**Navigation Switch Demo** <br>
https://youtu.be/jdK-0nZb1SQ

**Microphone Demo** <br>
https://youtu.be/ZZIuClorhCs

**SONAR Demo** <br>
https://youtu.be/9gRKd20tYLc

## Parts List
MBed LPC1768, https://www.sparkfun.com/products/9564 <br>
SPST Push Buttons * 2, https://www.sparkfun.com/products/9190 <br>
Navigation Switch, https://www.sparkfun.com/products/retired/11187 <br>
SPW2430 Microphone, https://www.adafruit.com/product/2716 <br>
HDJD-S822 Color Sensor * 2, https://www.sparkfun.com/products/retired/10904 <br>
Mini USB Breakout, https://www.sparkfun.com/products/9966 <br>
HC-SR04 SONAR, https://www.sparkfun.com/products/15569 <br>

## Block Diagram and Schematic
Here is the schematic for all the components we used:
![block diagram](./assets/block_diagram.jpg)
**Note: All the peripherals used 5V as power, and we didn't use external power in our project.**

## Software Design

## Future Direction

## Source Code
