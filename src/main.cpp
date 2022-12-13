#include <Arduino.h>

#define RED_LED 12

void setup() {
  pinMode(RED_LED, OUTPUT);
  digitalWrite(RED_LED, HIGH);
}
 

void loop() {
  digitalWrite(RED_LED, HIGH); 
  delay(850);
  digitalWrite(RED_LED, LOW); 
  delay(1000);
  digitalWrite(RED_LED, HIGH); 
  delay(250);
  digitalWrite(RED_LED, LOW);
  delay(250);
  digitalWrite(RED_LED, HIGH); 
  delay(250);
  digitalWrite(RED_LED, LOW);
  delay(250);
  digitalWrite(RED_LED, HIGH); 
  delay(250);
   digitalWrite(RED_LED, LOW);
  delay(250);
  digitalWrite(RED_LED, HIGH); 
  delay(250);
  digitalWrite(RED_LED, LOW); 
  delay(1000);
  digitalWrite(RED_LED, HIGH); 
  delay(250);
  digitalWrite(RED_LED, LOW); 
  delay(1000);
  digitalWrite(RED_LED, HIGH); 
  delay(850);
  digitalWrite(RED_LED, LOW);
  delay(250);
  digitalWrite(RED_LED, HIGH); 
  delay(850);
  digitalWrite(RED_LED, LOW);
  delay(250);
  digitalWrite(RED_LED, HIGH); 
  delay(250);
  digitalWrite(RED_LED, LOW); 
  delay(1000);
  digitalWrite(RED_LED, HIGH); 
  delay(250);
  digitalWrite(RED_LED, LOW);
  delay(250);
  digitalWrite(RED_LED, HIGH); 
  delay(850);
  digitalWrite(RED_LED, LOW);
  delay(1000);
  digitalWrite(RED_LED, HIGH); 
  delay(850);
  digitalWrite(RED_LED, LOW);
  delay(250);
  digitalWrite(RED_LED, HIGH); 
  delay(850);
  digitalWrite(RED_LED, LOW); 
  delay(1000);
  digitalWrite(RED_LED, HIGH); 
  delay(250);
  digitalWrite(RED_LED, LOW);
  delay(2000);
  
}