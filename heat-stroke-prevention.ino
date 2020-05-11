/*
	Vehicular Heat Stroke Prevention System
   
   It's a prevention system in cars which trigger the parents through alarm, if the car temperature is beyond the given threshold and save the child
   which is inside the car from heatstroke.
	
   Components used:
   
                                DHT-11 :-     digital pin 10
   echo pin of ultrasonic sound sensor :-     digital pin 6
   trigger pin of ultrasonic sound sensor :-  digital pin 4

	Requirements:
      + DHT library of arduino
      + DHT11 temperature and humidity sensor
      + Ultrasonic Distance sensor
      + Buzzer for alarm

	Created December 2018
	By Harsh Sinha & Sushil Kumar Singh

	https://github.com/justarandomcontributor/Vehicular_heatstroke_prevention_system

*/

// Source Code

#include <dht.h>
#define DHT11_PIN 10

dht DHT;

const int pingPin = 4; // Trigger Pin of Ultrasonic Sensor
const int echoPin = 6; // Echo Pin of Ultrasonic Sensor
const int soundPin = 8;   //buzzer pin
const int resetPin = 9;


void setup()
{
   Serial.print("Welcome to the debuging part of HeatStroke prevention System.");
   pinMode(pingPin, OUTPUT);
   pinMode(echoPin, INPUT);
   pinMode(soundPin,OUTPUT);
   pinMode(resetPin,OUTPUT);
   pinMode(12,OUTPUT);         //for indicating that movement has been detected
   pinMode(3,OUTPUT);
   pinMode(4,OUTPUT);
   Serial.begin(9600);
   digitalWrite(12,0);
}

int previous=0;


void sound_alarm()      //activates the alarm
{
  int x=100;
   
  while(1) {
    digitalWrite(8,1);    //buzzer activated
    delay(x);
    digitalWrite(8,0);
    delay(x);
    if(x==100)
      x=10;
    else
      x=100;
  }
}

//for checking the presence of a person
void check() 
{
   int start_time=millis();
   long duration,cm;
   digitalWrite(pingPin, LOW);
   delayMicroseconds(2);
   digitalWrite(pingPin, HIGH);
   delayMicroseconds(10);
   digitalWrite(pingPin, LOW);
   duration = pulseIn(echoPin, HIGH);
   cm = microsecondsToCentimeters(duration); //gets the distance in cm from obstacle using data obtained from distance sensor
   Serial.println(cm);
   
   //distance sensor data collection variable initialization condition.
   if( millis() < 500){
    previous=cm;}
   
   //if current distance has changed changed or not
   if( previous-cm > 0.2){
    digitalWrite(12,1);
    start_time=millis();
      
    //checks the temperature inside car for next 300 seconds.
    while( (millis() - start_time) <= 300000){
      DHT.read11(DHT11_PIN);
      Serial.println(DHT.temperature);    //for testing purpose
      
       if(DHT.temperature>=33){
        digitalWrite(12,1);   //can be configured to perform an action.
        sound_alarm();
        break; }
       
      delay(2000);
     }
   }
   
   previous=cm;
}


long microsecondsToCentimeters(long microseconds) 
{
   return microseconds / 29 / 2;
}


void loop()
{
  digitalWrite(13,0);
  Wire.beginTransmission(MPU);
  Wire.write(0x3B);  
  Wire.endTransmission(false);
  Wire.requestFrom(MPU,12,true);
  x=AcX;
  y=AcY;
  z=AcZ;  
  AcX=Wire.read()<<8|Wire.read();  
  AcY=Wire.read()<<8|Wire.read();  
  AcZ=Wire.read()<<8|Wire.read();  
    
  
  Serial.print("Accelerometer: ");          //for testing purpose
  Serial.print("X = "); Serial.print(x-AcX);
  Serial.print(" | Y = "); Serial.print(y-AcY);
  Serial.print(" | Z = "); Serial.println(z-AcZ); 
  
  if( x-AcX > 3000 || y-AcY > 3000 || z-AcZ > 3000 || x-AcX <- 3000 || y-AcY <- 3000 || z-AcZ <- 3000 ) //detects vibration
    digitalWrite(13,1);
  else{
    digitalWrite(13,0);
    check();
  }
  delay(333);
}
