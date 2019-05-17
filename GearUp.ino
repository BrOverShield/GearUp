//#include <ODB2.h>
//
//#define REAR_END_LOW_GEAR_RATIO = 4.063;
//#define REAR_END_HIGH_GEAR_RATIO = 2.955;
//#define FIRST_GEAR_RATIO = 3.231;
//#define SECOND_GEAR_RATIO = 1.952;
//#define THIRD_GEAR_RATIO = 1.321;
//#define FORTH_GEAR_RATIO = 1.029;
//#define FIFTH_GEAR_RATIO = 1.129;
//#define SIXTH_GEAR_RATIO = 0.943;
//
//#define TIRE_DIAMETER = 25.65
//
//#define MAGIC_CONSTANT = 336 // Very unsure what it does
//
//void setup()
//{
//  Serial.begin(9600); //Set the baud rate of the bluetooth module
//}
//
//void loop()
//{
//  // put your main code here, to run repeatedly:
//
//}
//
//// speed must be in MPH
//float CalculateGearRatio(speed, rpm)
//{
//  float result = (rpm * TIRE_DIAMETER) / (speed * MAGIC_CONSTANT)
//}

// Copyright (c) Sandeep Mistry. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#include <CAN.h> // the OBD2 library depends on the CAN library
#include <OBD2.h>

void setup() {
  Serial.begin(9600);
  Serial1.begin(9600);
  while (!Serial);

  Serial.println(F("OBD2 data printer"));

  Serial.println();
  Serial.print("VIN = ");
  Serial.println(OBD2.vinRead());
  Serial.print("ECU Name = ");
  Serial.println(OBD2.ecuNameRead());
  Serial.println();
}

void loop()
{
  if (Serial.available()) {      // If anything comes in Serial (USB),
    Serial1.write(Serial.read());   // read it and send it out Serial1 (pins 0 & 1)
  }

  if (Serial1.available()) {     // If anything comes in Serial1 (pins 0 & 1)
    Serial.write(Serial1.read());   // read it and send it out Serial (USB)
  }
  // loop through PIDs 0 to 95, reading and printing the values
  for (int pid = 0; pid < 96; pid++) {
    processPid(pid);
  }
  Serial.println();

  // wait 5 seconds before next run
  delay(5000);
}

void processPid(int pid) {
  if (!OBD2.pidSupported(pid)) {
    // PID not supported, continue to next one ...
    return;
  }

  // print PID name
  Serial.print(OBD2.pidName(pid));
  Serial.print(F(" = "));

  if (OBD2.pidValueRaw(pid)) {
    // read the raw PID value
    unsigned long pidRawValue = OBD2.pidReadRaw(pid);

    Serial.print(F("0x"));
    Serial.print(pidRawValue, HEX);
  } else {
    // read the PID value
    float pidValue = OBD2.pidRead(pid);

    if (isnan(pidValue)) {
      Serial.print("error");
    } else {
      // print value with units

      Serial.print(pidValue);
      Serial.print(F(" "));
      Serial.print(OBD2.pidUnits(pid));
    }
  }

  Serial.println();
}
