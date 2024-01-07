#include <Servo.h>   

Servo servo;  

const int buzzer = 9; //Isvestis buzzeriui
const int touchPin = 2; //Isvestis zaidimo kontaktui
const int gameLedPin = 13; //Isvestis zaliam ledui
const int nogameLedPin = 12; //Isvestis raudonam ledui
const int buttonPin = 3; //Isvestis mygtukui, kuris pradeda zaidima
bool isGameActive = false; //Pradedesime zaidima, kai paspausim mygtuka
const unsigned long gameDuration = 10000; // 7 sekundes zaidimui
unsigned long gameStartTime = 0; //Kintamasis skaiciuoti zaidimu laika
volatile bool wireContact = false; //Kintamasis stebeti zaidimo statusa
int angle = 96; //Zaidimo statuso pradine reiksme "I"

void setup() {
  servo.attach(8);
  servo.write(angle);
  pinMode(buzzer, OUTPUT);
  pinMode(gameLedPin, OUTPUT);
  pinMode(nogameLedPin, OUTPUT);
  pinMode(buttonPin, INPUT_PULLUP);
  pinMode(touchPin, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(buttonPin), startGame, FALLING);
  attachInterrupt(digitalPinToInterrupt(touchPin), touchWire, CHANGE);
}

void loop() {
    if (isGameActive && wireContact){
      tone(buzzer, 10000);
      delay(500);
      noTone(buzzer);
    }
    if (isGameActive && millis() >=  gameStartTime + gameDuration) { //Patikrina ar mygtukas buvo paspaustas ir veikia tol kol millis() yra maziau uz zaidimo trukme
      digitalWrite(gameLedPin, LOW); //Pasibaigus laikui isjungiam zalia LED
      isGameActive = false; //Pasibaigus laikui pakeiciam zaidimo busena i false
      noTone(buzzer);
      
      if(wireContact == true)
      {
        angle = 0;
      }
      else 
      {
        angle = 190;
      }
      servo.write(angle);
      delay(4000);
      wireContact = false;
      angle = 79;
      servo.write(angle);
    }
  
  // Raudonas LED sviecia, jeigu zaidimas nevyksta
  digitalWrite(nogameLedPin, !isGameActive);
  
}

void touchWire() {
  if(isGameActive)
  wireContact = true;
  }


void startGame() {
  digitalWrite(nogameLedPin, LOW);
  delay(100000);
  gameStartTime = millis(); //pradejus zaidima, nustatome gameStartTime i laika nuo programos paleidimo
  digitalWrite(gameLedPin, HIGH); //Ijungiam zalia LED
  isGameActive = true; //Pakeiciam zaidimo busena i true
}
