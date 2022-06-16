# PissBot
Automatic Beer Dispenser

<img src='https://user-images.githubusercontent.com/77097236/154419592-63ef2732-a856-435f-9c12-ad9126178d0e.png' width='400' height='220'>

## Table of Contents
* [Introduction](#introduction)
* [Mechanical Design](#mechanical-design)
* [Documentation](#documentation)

## Introduction
This project was done as part of the ME102B Mechatronics Design course during my exchange in UC Berkeley. We had to produce a working prototype by the end of the semester, that solves a problem in real life. We had to do both mechanical design and code design for our prototype to work.

We decided to do something fun, and decided to build a automatic beer dispenser. This solves the problem which one might experience in a crowded bar, trying to get the attention of the bartender but to no avail. With our product, you will be able to get a cold pint of beer without the need to ask anybody!

## Mechanical Design
<img src='https://user-images.githubusercontent.com/77097236/174054920-68d00b78-cf0b-4d1c-b6b6-8d86e0c52d02.jpg' width='500' height='270'>

- A rack and pinion was designed so that the motor can move the dispensing spout along the rack to get to its target destination.
- A cup holder is placed at every seat so that the user can put their glass into the holder when they require a drink. A small push against the in-built button will signal to the system that the user requires a refill, without the need for manual effort.
- The 2 main sensors we used were the limit switches and the IR sensors. The limit switches will be placed on each end to signal to our system that it has reached the corners and to change direction, preventing our spout from falling off the rack. The IR sensors will be placed at each of the cup holders. This will help to track the moving spout, as well as to tell the system to stop when it has reached the target holder.
