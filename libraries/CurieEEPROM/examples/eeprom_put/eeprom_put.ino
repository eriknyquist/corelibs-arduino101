/*
    eeprom_put example.

    This shows how to use the EEPROM.put() method.
    Also, this sketch will pre-set the EEPROM data for the
    example sketch eeprom_get.

    Note, unlike the single byte version EEPROM.write(),
    the put method will use update semantics. As in a byte
    will only be written to the EEPROM if the data is actually
    different.

    Written by Christopher Andrews 2015

    Copyright (c) 2016 Intel Corporation.  All rights reserved.
    See the bottom of this file for the license terms.
*/

#include <CurieEEPROM.h>

struct MyObject {
  float field1;
  byte field2;
  char name[10];
};

void setup() {

  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }

  float f = 123.456f;  //Variable to store in EEPROM.
  int eeAddress = 0;   //Location we want the data to be put.


  //One simple call, with the address first and the object second.
  EEPROM.put(eeAddress, f);

  Serial.println("Written float data type!");

  /** Put is designed for use with custom structures also. **/

  //Data to store.
  MyObject customVar = {
    3.14f,
    65,
    "Working!"
  };

  eeAddress++;
  EEPROM.put(eeAddress, customVar);
  Serial.print("Written custom data type! \n\nView the example sketch eeprom_get to see how you can retrieve the values!");
}

void loop() {
  /* Empty loop */
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
