#include <WiFi.h>
#include "FirebaseESP32.h"
#include <Wire.h>  
#define WIFI_SSID "Lupiswifi"   
#define WIFI_PASSWORD "288Blupis"
#define C "https://control-led-57624-default-rtdb.firebaseio.com/"
#define D "PttYJDJsTTGANxQHS6IzOs8DrxYUyPG2U22QvB1i"
#include <LiquidCrystal_I2C.h>
FirebaseData firebaseData;
FirebaseJson json;//Thư viện LCD

int lcdColumns = 16;
int lcdRows = 2;
LiquidCrystal_I2C lcd(0x27, lcdColumns, lcdRows);

int status_led1 = 0;
int status_led2 = 0;
int r1 = 6;
int r2 = 7;
const int trig = 19;  
const int echo = 18;

unsigned long thoigian; 
int khoangcach;

void setup() {
  Serial.begin(115200);
 
  pinMode(trig, OUTPUT);
  pinMode(echo, INPUT); 
 
    Wire.begin(4,5); 
  // initialize LCD
  lcd.init();
  // turn on LCD backlight                      
  lcd.backlight();

  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.println("Dang Ket Noi...");
  while(WiFi.status() !=WL_CONNECTED)
  {
    Serial.println("...");
    delay(200);
    }
    Serial.println(WiFi.localIP());
    Firebase.begin(C,D);
    pinMode(r1,OUTPUT);
    pinMode(r2,OUTPUT);
}

void scrollText(int row, String message, int delayTime, int lcdColumns) {
  for (int i=0; i < lcdColumns; i++) {
    message = " " + message;  
  } 
  message = message + " "; 
  for (int pos = 0; pos < message.length(); pos++) {
    lcd.setCursor(0, row);
    lcd.print(message.substring(pos, pos + lcdColumns));
    delay(delayTime);
  }
}

unsigned long timer = 0;
unsigned long timer_1 = 0;

void loop() {
  // put your main code here, to run repeatedly:


  // put your main code here, to run repeatedly:
  digitalWrite(trig,LOW);
  delayMicroseconds(2);

  digitalWrite(trig,HIGH);
  delayMicroseconds(10);

  digitalWrite(trig,LOW);
  thoigian = pulseIn(echo,HIGH);
//  Serial.print("thoi gian: ");
//  Serial.println( thoigian);
  
  khoangcach = int( thoigian / 2 / 29.412);
//  Serial.print("khoang cach: ");
//  Serial.println(khoangcach);
//  
////  digitalWrite(led,HIGH);
//    
//
//    lcd.setCursor(0, 0);
//
//// print static message
////  lcd.print("khoangcach:");
//
//  lcd.print(khoangcach);
 
// print scrolling message
//  scrollText(1,"khoang cach: ", 250, lcdColumns);
//  scrollText(1, khoangcach, 250, lcdColumns);


//  if(khoangcach >= 10) {
//    digitalWrite(r1,HIGH);
//    digitalWrite(r2,HIGH);                                                        // compare the input of led status received from firebase
//    Serial.println("Turned OFF");   
//  }
//  else {
//  digitalWrite(r1,LOW);
//  digitalWrite(r2,LOW);
//  Serial.println("Turned ON");      
//  }


//        Firebase.getInt(firebaseData,"/LED_STATUS1");
//  status_led1 = firebaseData.intData();
//
//  if (status_led1 == 1)
//  { 
//    digitalWrite(r1,0);
//  }
//  else
//  {
//    digitalWrite(r1,1);
//  }
//
//   Firebase.getInt(firebaseData,"/LED_STATUS2");
//  status_led2 = firebaseData.intData();
//   if (status_led2 == 2)
//  { 
//    digitalWrite(r2,0);
//  }
//  else
//  {
//    digitalWrite(r2,1);
//}              
  Firebase.getInt(firebaseData,"/LED_STATUS");
  status_led1 = firebaseData.intData();
if (status_led1 == 0)
{

  if ( (unsigned long) (millis() - timer) > 500)
  {
    lcd.clear();
    Serial.print("khoang cach: ");
    Serial.println(khoangcach);
    lcd.setCursor(0, 0);
    lcd.print(khoangcach);
    if(khoangcach >= 10)
    {
      digitalWrite(r1,HIGH);                                                        // compare the input of led status received from firebase
      digitalWrite(r2,HIGH); 
      Serial.println("Turned OFF");
    }
    timer = millis();   
  }

  
  if ( (unsigned long) (millis() - timer_1) > 1000) 
  {
    if(khoangcach < 10)
    {
      digitalWrite(r1,LOW);
      digitalWrite(r2,LOW); 
      Serial.println("Turned ON");
    }
    timer_1 = millis();
  }
    
  // compare the input of led status received from firebase
}

if (status_led1 == -1)
{
    digitalWrite(r1,HIGH);  
    digitalWrite(r2,HIGH); 
}



if (status_led1 == 1)
{
  digitalWrite(r1,LOW);
  digitalWrite(r2,LOW); 
}


}
