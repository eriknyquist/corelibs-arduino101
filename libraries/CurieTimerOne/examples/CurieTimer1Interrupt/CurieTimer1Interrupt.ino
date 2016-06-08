/*
  Sketch: Timer1Interrupt.ino

  This sketch demonstrates the usage of the Curie Timer One Library.
  It uses timer-1 to blink the onboard LED, pin 13, at different
  intervals (speed) in four steps.

  You can see the time interval and the number of interrupt counted
  in 10 seconds if you keep serial logging active, but this may require
  a MASTER_RESET to reprogram the board. 

  Blinking of the LED will start only when you open the  Serial Monitor
  unless you comment the "#define SERIAL_PORT_LOG_ENABLE 1"; don't
  forget to uncomment "CurieTimerOne.restart(time);"

  Modified 14 March 2016
  by Simone Majocchi

  Copyright (c) 2016 Intel Corporation.  All rights reserved.
  See the bottom of this file for the license terms.
*/

#include "CurieTimerOne.h"

// Comment the following statement to disable logging on serial port.
#define SERIAL_PORT_LOG_ENABLE 1

const int oneSecInUsec = 1000000;   // A second in mirco second unit.
bool toggle = 0;                    // The LED status toggle
int time;                           // the variable used to set the Timer

void timedBlinkIsr()   // callback function when interrupt is asserted
{
  digitalWrite(13, toggle);
  toggle = !toggle;  // use NOT operator to invert toggle value
}

void setup() {

#ifdef SERIAL_PORT_LOG_ENABLE
  Serial.begin(115200);
  while (!Serial);  //  wait for the serial monitor to open
#endif

  // Initialize pin 13 as an output - onboard LED.
  pinMode(13, OUTPUT);
}

void loop() {

  for (int i = 1; i < 9; i = i * 2) {
    // We set a blink rate of 1000000, 500000, 250000, 125000 microseconds
    time = oneSecInUsec / i; // time is used to toggle the LED is divided by i
    CurieTimerOne.start(time, &timedBlinkIsr);  // set timer and callback

#ifdef SERIAL_PORT_LOG_ENABLE
    Serial.print("The blink period: ");
    Serial.println(time);
#endif

    delay(10000);  // 10 seconds of delay, regularly 'interrupted' by the timer interrupt

#ifdef SERIAL_PORT_LOG_ENABLE
    Serial.print("Total number of ticks in 10 seconds: ");
    Serial.println(CurieTimerOne.rdRstTickCount());  // Reads and Resets tick count
    Serial.println("----");
#endif
    // Uncomment the following line if the serial logging is disabled
    // CurieTimerOne.restart(time);   // Restarts Timer
  }
}

/*
   Copyright (c) 2016 Intel Corporation.  All rights reserved.

   This library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public
   License as published by the Free Software Foundation; either
   version 2.1 of the License, or (at your option) any later version.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Lesser General Public License for more details.

   You should have received a copy of the GNU Lesser General Public
   License along with this library; if not, write to the Free Software
   Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
*/
