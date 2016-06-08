/*
 * Copyright (c) 2016 Intel Corporation.  All rights reserved.
 * See the bottom of this file for the license terms.
 */

#include <CurieTimerOne.h>

const int blinkPin = 13;

void setup(void)
{
  CurieTimerOne.initialize(50000);
  Serial.begin(9600);
}

void loop(void)
{
  unsigned int range;

  Serial.println("PWM blink: low -> high duty cycle");

  for (float dutyCycle = 5.0; dutyCycle <= 100.0; dutyCycle++) {
    range = (dutyCycle / 100) * 1023;
    CurieTimerOne.pwm(blinkPin, range);
    delay(50);
  }

  Serial.println("PWM blink: high -> low duty cycle");

  for (float dutyCycle = 100.0; dutyCycle > 5.0; dutyCycle--) {
    range = (dutyCycle / 100) * 1023;
    CurieTimerOne.setPwmDuty(blinkPin, range);
    delay(50);
  }
}

/*
 * Copyright (c) 2016 Intel Corporation.  All rights reserved.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 */
