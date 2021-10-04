/*NAMA    : DANIEL MORANTHA
NIM     : 2019230088*/

#include <Servo.h>
Servo servo1;
int led1=12;   //LED MERAH
int led2=11;   //LED HIJAU
int trig=4;
int echo=5;
int servo1_pin=6; //PINTU
int duration=0,dist=0;
void setup()
{
  Serial.begin(9600);
  
pinMode(led1,OUTPUT);
pinMode(led2,OUTPUT);
pinMode(trig,OUTPUT);
pinMode(echo,INPUT);
  servo1.attach(servo1_pin);
  servo1.write(90);
}

void loop()
{
  digitalWrite(trig, HIGH);
delayMicroseconds(10);
digitalWrite(trig, LOW);
pinMode(echo, INPUT);
duration = pulseIn(echo, HIGH);
dist = (duration*0.034) / 2;    //obtain distance

if(dist<20)
{
  Serial.println("Door Open");
  digitalWrite(led2,HIGH);
  digitalWrite(led1,LOW);
  servo1.write(90);
  delay(2000);
 
}
else
{
  Serial.println("Door Closed");
  digitalWrite(led1,HIGH);
  digitalWrite(led2,LOW);
  servo1.write(0);
  delay(1000);
}
}
