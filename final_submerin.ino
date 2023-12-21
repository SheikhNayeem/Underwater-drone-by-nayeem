#include<Servo.h>
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

#include "DHT.h"

#define DHTPIN 7  
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE); 
LiquidCrystal_I2C lcd(0x20,16,2);
int VRx = A0;
int VRy = A1;
int fy = A2;
int SW = 2;

int xPosition = 0;
int yPosition = 0;
int fyPosition = 0;
int SW_state = 1;
int ph = A3;
int mapX = 0;
int mapY = 0;
int mapf = 0;
Servo ESCfront;
Servo ESCleft;
Servo ESCright;
Servo servo1;
void setup() {
  Serial.begin(9600);
  lcd.begin();
  lcd.backlight();
  dht.begin();
  // Print a message to the LCD.
  
  ESCfront.attach(11, 1000, 2000);
  ESCleft.attach(10, 1000, 2000);
  ESCright.attach(9, 1000, 2000);
  servo1.attach(6);
  servo1.write(10);
  pinMode(VRx, INPUT);
  pinMode(VRy, INPUT);
  pinMode(SW, INPUT_PULLUP);
  pinMode(fy, INPUT);
  pinMode(ph, INPUT);
}

void loop() {
  float t = dht.readTemperature();
  int p = phdata(analogRead(ph));
  lcd.setCursor(1,0);
  lcd.print("Temp:");
  lcd.setCursor(7,0);
  lcd.print(t);
  lcd.setCursor(1,1);
  lcd.print("PH:");
  lcd.setCursor(4,1);
  lcd.print(p);
  Serial.println("analog data:");
  Serial.println(p);
  //delay(500);
  xPosition = analogRead(VRx);
  yPosition = analogRead(VRy);
  fyPosition = analogRead (fy);
  SW_state = digitalRead(SW);
  mapX = map(xPosition, 0, 1023, -180, 180);
  mapY = map(yPosition, 0, 1023, -180, 180);
  mapf = map(fyPosition, 0, 1023, -180, 180);
  Serial.print("X: ");
  Serial.print(mapX);
  Serial.print(" | Y: ");
  Serial.print(mapY);
  Serial.print(" | Button: ");
  Serial.print(SW_state);
  Serial.print("Fy :");
  Serial.println(mapf);
  //ESC.write(mapX);
   //delay(2000);
  
  if (SW_state == 0)
  {
    servo1.write(56);
    //delay(1000);
  }
  if (SW_state == 1)
  {
    servo1.write(0);
    //delay(100);
  }

  if (mapf > 10)
  {
    ESCfront.write(mapf);
    Serial.print("motor Fy :");
    Serial.println(mapf);
  }
  if (mapf < 10)
  {
    ESCfront.write(0);
    Serial.print("motor Fy :");
    Serial.println("0");
  }
  if ( mapY > 10 && mapX < 10 && mapX > -10)
  {
    ESCleft.write(mapY);
    ESCright.write(mapY);
    Serial.print("motor left :");
    Serial.println(mapY);
    Serial.print("motor right :");
    Serial.println(mapY);

  }
  if ( mapY < 10 && mapX < 10 && mapX > -10)
  {
    ESCleft.write(0);
    ESCright.write(0);
    Serial.println("motor right and left stoped");


  }

  if ( mapY > -5 && mapX > 10 || mapX < -10)
  {
    ESCleft.write((mapX + mapY) / 2);
    ESCright.write((((-1)*mapX) + mapY) / 2);
    Serial.print("motor left :");
    Serial.println((mapX + mapY) / 2);
    Serial.print("motor right :");
    Serial.println((((-1)*mapX) + mapY) / 2);

  }
//delay(1000);
}
int phdata(int x)
{
  if(x < 600 & x>400)
  {
    return 5;
  }

   if(x > 600 & x<800)
  {
    return 6;
  }

   if(x > 800 & x<900)
  {
    return 7;
  }

   if(x > 900)
  {
    return 8;
  }
 if(x > 300 & x<400)
  {
    return 4;
  }

   if(x < 300)
  {
    return 3;
  }
  
}
