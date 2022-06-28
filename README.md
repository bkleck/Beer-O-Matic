# Beer-O-Matic
Automatic Beer Dispenser

<img src='https://user-images.githubusercontent.com/77097236/154419592-63ef2732-a856-435f-9c12-ad9126178d0e.png' width='400' height='220'>

## Table of Contents
* [Introduction](#introduction)
* [Mechanical Design](#mechanical-design)
* [Code Documentation](#code-documentation)
* [Logic Diagram](#logic-diagram)
* [Wiring Diagram](#wiring-diagram)

## Introduction
This project was done as part of the **_ME102B Mechatronics Design_** course during my exchange in UC Berkeley. We had to produce a working prototype by the end of the semester, that solves a problem in real life. We had to do both **_mechanical design and code design_** for our prototype to work.

We decided to do something fun, and decided to build a **_automatic beer dispenser_**. This solves the problem which one might experience in a crowded bar, trying to get the attention of the bartender but to no avail. With our product, you will be able to get a cold pint of beer without the need to ask anybody!

## Mechanical Design
<img src='https://user-images.githubusercontent.com/77097236/174054920-68d00b78-cf0b-4d1c-b6b6-8d86e0c52d02.jpg' width='500' height='270'>

- A **_rack and pinion_** was designed so that the motor can move the dispensing spout along the rack to get to its **_target destination_**.
- A **_cup holder_** is placed at every seat so that the user can put their glass into the holder when they require a drink. A small **_push against the in-built button_** will signal to the system that the user **_requires a refill_**, without the need for manual effort.
- The 2 main sensors we used were the **_limit switches and the IR sensors_**. The limit switches will be placed on each end to signal to our system that it has reached the corners and to change direction, **_preventing our spout from falling off_** the rack. The IR sensors will be placed at each of the cup holders. This will help to track the moving spout, as well as to tell the system to **_stop when it has reached the target holder_**.

## Code Documentation
### 1) Position tracking
On `Startup()`, we will turn the motor anticlockwise to move left, and stop when it has reached the left limit. Afterwards, the direction will be reversed and `currentPos` will be set to 0, representing the left limit switch. There will be **_3 IR sensors_** in our demo, so each of them will be labelled 1,2,3 respectively and the right limit switch will be 4.

The `ReadTarget()` function will be used to **_add a target position_** to the queue. We will be using a **_FIFO queue system_** so that users can signal to the system that they require a drink at any point in time, without needing to wait for their turn. Whenever a user **_presses the button_** on the cup holder, the system will **_read the pin as LOW_** because we are using **_PULLUP mode_**. We will then **_check if the position is already in queue_** using the boolean variable `elementInside`. This prevents any user from hogging the system and prevent spillage in the event the user accidentally presses more than once. If `elementInside = false`, we will **_add that element into our_** `targetQueue` **_array_** at the next possible location.

The `ReadIRSensor()` function is used to **_determine if the moving spout passes by any of the IR sensors_**. It uses `analogRead` to determine the **_amount of light the IR sensor is receiving_** at that point in time. We will use the initial IR reading during startup to determine that as no object passing by. If the **_current reading drops 100 points below initial reading_**, this means the moving spout **_moved past_** and it will return a **_LOW value_**.

Our `MoveToTarget()` function does exactly what is said. The `targetPos` variable is the next position the spout needs to move to, which is the **_first element_** of the `targetQueue` array. Next, we will make use of the **_current position's index and target position's index to tell the spout where to move_**. If `targetIndex > currentIndex`, the spout needs to move right to reach the target, and vice versa. Once `targetPos == currentPos`, we have reached our destination and will **_dispense beer_** using `measure_dispense()` function. The element will also be **_removed from the queue_**. Another safety feature we had was that if the spout ever **_hits the left or right limit switches_**, we will **_reverse the direction_** so that it can continue to find the target position. This happens sometimes due to bugs in the IR sensors.

### 2) Dispensing
Firstly, to **_calculate the distance between the cup and pump_**, we have an **_ultrasound sensor_** attached to the spout. In the `calc_distance()` function, we will turn the trigger pin of this sensor from LOW to HIGH to LOW, and calculate the **_duration for the ultrasound wave to emit and reflect back_** from the bottom of the cup. The distance is then calculated by deriving from the **_duration recorded and the speed of sound in air_**.

Next, we will use the `measure_dispense()` function to actually dispense the beer in the given time. Firstly, we model our cup using a standard size of **_500ml_**, and we want the cup to be completely filled in **_~3s_**. Hence, we calculate the **_flow rate using volume/time_**. Afterwards, we calculate the rate of height increase required using **_height_rate = flow rate/surface area_**. Lastly, the **_time required for the dispense will be (distance - final_distance)/height_rate_**. We will then activate the relay pin to turn on the valve for the given amount of time. Another safety feature here is that when the valve is open, the motor will be turned off, and vice versa, to **_prevent any spillage_** from movement during dispensing.


## Logic Diagram
<p align="center">
  <img src='https://user-images.githubusercontent.com/77097236/176107367-999a6f41-9dd7-4474-913a-12f6d676f4b9.jpg' width='450' height='400'>
  
  <img src='https://user-images.githubusercontent.com/77097236/176107897-e9a0bc9a-1457-4774-b63e-8c4274b12ab6.jpg' width='550' height='400'>
</p>

## Wiring Diagram
<p align="center">
  <img src='https://user-images.githubusercontent.com/77097236/176108375-bbb05da6-318a-4de0-8853-694d5d2277c4.jpg' width='1000' height='500'>
</p>
