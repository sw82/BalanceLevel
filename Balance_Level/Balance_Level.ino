/*
  Gyro-Stop
  
  modified 28 May 2022
  by Sebastian Winkler
  
  www.mindfuckbox.com

  some resources used: 
  - https://www.researchgate.net/figure/Gyroscope-sensor-and-accelerometer-MPU-6050_fig5_347285831
  
  
*/

#include<Wire.h>
#include "rgb_lcd.h"
const int MPU=0x68; 
int16_t AcX,AcY,AcZ,Tmp,GyX,GyY,GyZ;

int minVal=265;
int maxVal=402;

rgb_lcd lcd;

const int colorR = 255;
const int colorG = 0;
const int colorB = 0;

const double threadMin = 250;
const double threadMax = 282;

double z;
double x;
double y;

int record = 0; 
int timer = 0; 
bool go = 0;


void setup() {
  // initialize digital pin LED_BUILTIN as an output.
  pinMode(LED_BUILTIN, OUTPUT);
  
  Wire.begin();
  Wire.beginTransmission(MPU); 
  Wire.write(0x6B); 
  Wire.write(0);    
  Wire.endTransmission(true);
  Serial.begin(9600); 


  // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);
    
  lcd.setRGB(colorR, colorG, colorB);
    
  // Print a message to the LCD.
  lcd.print("It's record day!");
  lcd.setRGB(255, 255, 255);
  
}


void loop() {
  if (timer == 0)
  {
     lcd.setRGB(255, 255, 255);
  }
  
  digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on (HIGH is the voltage level)
  
  Wire.beginTransmission(MPU);
  Wire.write(0x3B);  
  Wire.endTransmission(false);
  Wire.requestFrom(MPU,12,true);  
  
  AcX=Wire.read()<<8|Wire.read();    
  AcY=Wire.read()<<8|Wire.read();  
  AcZ=Wire.read()<<8|Wire.read();  

  int xAng = map(AcX,minVal,maxVal,-90,90);
  int yAng = map(AcY,minVal,maxVal,-90,90);
  int zAng = map(AcZ,minVal,maxVal,-90,90);
  
  y= RAD_TO_DEG * (atan2(-xAng, -zAng)+PI); 
  x= RAD_TO_DEG * (atan2(-yAng, -zAng)+PI);
  z= RAD_TO_DEG * (atan2(-yAng, -xAng)+PI);


  if (y < threadMax && y > threadMin){
    timer = timer +1;
    //Serial.println(timer);
    digitalWrite(LED_BUILTIN, LOW);    // turn the LED off by making the voltage LOW
    delay(1000);
  }
  else {
    // Hooray, new record for the day.
    if (record < timer) { 
      record = timer;
      timer = 0;
      lcd.setCursor(0, 1);
      lcd.print(String("  "));

      lcd.setRGB(0, 255, 0); // turn green
      
      delay(2000);
    }
    else { 
      if(record > timer) {
        timer = 0 ;
        lcd.setCursor(0, 1);
        lcd.print(String("  "));  
      }
    }
  }
   
  // set the cursor to column 0, line 1
  // (note: line 1 is the second row, since counting begins with 0):
  lcd.setCursor(0, 1);
 
  lcd.print(timer);
  
  lcd.setCursor(5, 1);
  lcd.print(String("Today: ") + String(record));

  
}
