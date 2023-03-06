#include "MeMegaPi.h"

MeMegaPiDCMotor moteurG(PORT1B);

MeMegaPiDCMotor moteurD(PORT2B);

MeUltrasonicSensor USg(PORT_7);
MeUltrasonicSensor USa(PORT_8);
MeUltrasonicSensor USd(PORT_6);



void setup() {
moteurG.run(100);
moteurD.run(-80);
delay(500);
}

void loop() {
int g = USg.distanceCm();
int a = USa.distanceCm();
int d = USd.distanceCm();



 {
moteurG.run(100);
moteurD.run(-80);
 }

{
    moteurG.run(100);
    moteurD.run(-80);
    delay(1600);
    moteurG.stop();
    moteurD.stop();
    delay(1000);
    moteurG.run(100);
    moteurD.run(100);
    delay(2250);
    moteurG.stop();
    moteurD.stop();
    moteurG.run(100);
    moteurD.run(-80);
    delay(2000);
    moteurG.stop();
    moteurD.stop();
 }

 {
    moteurG.run(100);
    moteurD.run(-80);
    delay(1600);
    moteurG.stop();
    moteurD.stop();
    delay(1000);
    moteurG.run(100);
    moteurD.run(100);
    delay(2250);
    moteurG.stop();
    moteurD.stop();
    moteurG.run(100);
    moteurD.run(-80);
    delay(2000);
    moteurG.stop();
    moteurD.stop();
 }
 

  {
    moteurG.run(100);
    moteurD.run(-80);
    delay(1000);
    moteurG.stop();
    moteurD.stop();
    delay(1000);
    moteurG.run(-100);
    moteurD.run(-100);
    delay(2000);
    moteurG.stop();
    moteurD.stop();
    moteurG.run(100);
    moteurD.run(-80);
    delay(2000);
    moteurG.stop();
    moteurD.stop();
 
 }

 {
    moteurG.stop();
    moteurD.stop();
    delay(1000);
    moteurG.run(-100);
    moteurD.run(-100);
    delay(3400);
    moteurG.stop();
    moteurD.stop();
    moteurG.run(100);
    moteurD.run(-80);
    delay(2000);
    moteurG.stop();
    moteurD.stop();
 }
 
 
}
