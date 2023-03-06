/**
 * \par Copyright (C), 2012-2016, MakeBlock
 * @file    DCMotorDriverTest.ino
 * @author  MakeBlock
 * @version V1.0.0
 * @date    2015/09/09
 * @brief   Description: this file is sample code for Me DC motor device.
 *
 * Function List:
 *    1. void MeDCMotor::run(int16_t speed)
 *    2. void MeDCMotor::stop(void)
 *
 * \par History:
 * <pre>
 * <Author>     <Time>        <Version>      <Descr>
 * Mark Yan     2015/09/09    1.0.0          rebuild the old lib
 * </pre>
 */
#include "MeOrion.h"

MeDCMotor motor1(PORT_1);

MeDCMotor motor2(PORT_2);

MeDCMotor motor3(M1);

MeDCMotor motor4(M2);

uint8_t motorSpeed = 255;

void setup()
{
}

void loop()
{
  for (int i=0; i<3; i++){
    motor3.run(-motorSpeed);
    motor4.run(motorSpeed);
    delay(1500);
    motor3.stop();
    motor4.stop();
    delay(500);
    motor3.run(-motorSpeed);
    motor4.run(-motorSpeed);
    delay(875);
    motor3.stop();
    motor4.stop();
    delay(500);
  }
  for (int i=0; i<2; i++){
    motor2.run(-motorSpeed);
    delay(1000);
    motor2.stop();
    delay(500);
    motor2.run(motorSpeed);
    delay(1000);
    motor2.stop();
    delay(500);
  }
  
}

// for joystick gdhb -> controler carte avec 1boutton : diriger 
// 2 boutton : enlever objet avec pince



//  motor1.stop();
//  motor2.stop();
//  motor3.stop();
//  motor4.stop();
