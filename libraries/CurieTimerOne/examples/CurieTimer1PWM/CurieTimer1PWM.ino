/*
 * Copyright (c) 2016 Intel Corporation.  All rights reserved.
 * See the bottom of this file for the license terms.
 */

#include "CurieTimerOne.h"

void setup() {
  // Setup a PWM signal on pin 13, onboard LED, with a 25% duty cycle
  // of a 1 second period (1000000 usec), as follow (please note the
  // decimal point to indicate double):

  CurieTimerOne.pwmStart(13, 25.0, 1000000);

  // Or, use discrete number range, 0-1023, to define the duty period,
  // 255 is 24.9%, as follow:

  // CurieTimerOne.pwmStart(13, 255, 1000000);
}

void loop() {
  // put your main code here, to run repeatedly:

  delay( 10000 );
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
