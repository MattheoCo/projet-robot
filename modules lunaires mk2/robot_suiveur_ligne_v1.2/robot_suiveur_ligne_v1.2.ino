/**
 * \par Copyright (C), 2012-2016, MakeBlock
 * @file    LineFollowerTest.ino
 * @author  MakeBlock
 * @version V1.0.0
 * @date    2015/09/09
 * @brief   Description: this file is sample code for Me line follower module.
 *
 * Function List:
 * 1. uint8_t MeLineFollower::readSensors(void)
 *
 * \par History:
 * <pre>
 * `<Author>`         `<Time>`        `<Version>`        `<Descr>`
 * Mark Yan         2015/09/09    1.0.0            Rebuild the old lib.
 * </pre>
 */
#include "MeOrion.h"

MeLineFollower lineFinder(PORT_4); /* Line Finder module can only be connected to PORT_3, PORT_4, PORT_5, PORT_6 of base shield. */

MeDCMotor motor1(M1);

MeDCMotor motor2(M2);

uint8_t motorSpeed = 100;


void setup()
{
  Serial.begin(9600);
}

void loop()
{
  int sensorState = lineFinder.readSensors();
  switch(sensorState)
  {
    case S1_IN_S2_IN: Serial.println("Sensor 1 and 2 are inside of black line"); motor1.stop(); motor2.stop(); break;
    case S1_IN_S2_OUT: Serial.println("Sensor 2 is outside of black line"); motor1.run(motorSpeed); motor2.run(motorSpeed); break;
    case S1_OUT_S2_IN: Serial.println("Sensor 1 is outside of black line"); motor1.run(-motorSpeed); motor2.run(-motorSpeed); break;
    case S1_OUT_S2_OUT: Serial.println("Sensor 1 and 2 are outside of black line"); motor1.run(motorSpeed); motor2.run(-motorSpeed); break;
    default: break;
  }
  delay(200);
}
