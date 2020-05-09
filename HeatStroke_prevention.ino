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
  while(1)      //infinite loop.
  {
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
void check() //for checking the presence of a person.
{
   int start_time=millis();
   long duration,cm;
   digitalWrite(pingPin, LOW);
   delayMicroseconds(2);
   digitalWrite(pingPin, HIGH);
   delayMicroseconds(10);
   digitalWrite(pingPin, LOW);
   duration = pulseIn(echoPin, HIGH);
   cm = microsecondsToCentimeters(duration);    //gets the distance in cm from obstacle using data obtained from distance sensor.
   Serial.println(cm);
   if(millis()<500)   //distance sensor data collection variable initialization condition.
   {
    previous=cm;
   }
   if(previous-cm>.2)   //if current distance has changed changed or not
   {
    digitalWrite(12,1);
    start_time=millis();
    while(millis()-start_time<=300000)    //checks the temperature inside car for next 300 seconds.
    {
      DHT.read11(DHT11_PIN);
      Serial.println(DHT.temperature);    //for testing purpose
      if(DHT.temperature>=33)
      {
        digitalWrite(12,1);   //can be configured to perform an action.
        sound_alarm();
        break;
      }
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
  check();
}
