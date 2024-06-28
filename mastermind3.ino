// Invicta Mastermind for Wio Terminal D51R
// Roni Bandini, June 2024
// MIT License


#include "TFT_eSPI.h" 
#include "Seeed_FS.h"  
#include "RawImage.h" 
#include "Free_Fonts.h"

TFT_eSPI tft; 

// random code
int rand1;
int rand2;
int rand3;

// user code
int user1=0;
int user2=0;
int user3=0;

int currentColumn=1;
int rightNumbersAndPositions=0;
int rightNumbers=0;
int counter=0;

void buzzerBeep()
{
    analogWrite(WIO_BUZZER, 20);  
    delay(150);  
    analogWrite(WIO_BUZZER, 0);  
}

void buzzerWin()
{
    analogWrite(WIO_BUZZER, 80);  
    delay(150);  
    analogWrite(WIO_BUZZER, 0);  
    analogWrite(WIO_BUZZER, 120);  
    delay(150);  
    analogWrite(WIO_BUZZER, 0);  
}

void displayInfo(){
  tft.fillScreen(TFT_BLACK);  

  tft.setFreeFont(&FreeMonoBold9pt7b);  
  tft.drawString("Mastermind was invented in ",0,0);
  tft.drawString("1970 by an Israeli teleco-",0,20);
  tft.drawString("mmunications expert named",0,40); 
  tft.drawString("Mordecai Meirowitz.",0,60); 

  tft.drawString("In this hame you are a code",0,100);
  tft.drawString("breaker attempting to guess",0,120); 
  tft.drawString("a 3-digit secret code.",0,140); 

  tft.drawString("Wio Terminal version",0,180); 
  tft.drawString("by Roni Bandini, June 2024",0,200); 
  tft.drawString("https://bandini.medium.com",0,220); 

  delay(5000);
}


void setup() {
  pinMode(WIO_BUZZER,OUTPUT);

  tft.begin();  


  // load bmp from sd card
  if(!SD.begin(SDCARD_SS_PIN, SDCARD_SPI)) {
        while (1);
    }
  
  buzzerBeep();
  drawImage<uint8_t>("mastermind.bmp", 0, 0); 

  tft.setRotation(3);  
  Serial.begin(115200);
  delay(5000);
  Serial.println("Mastermind started");
  randomSeed(analogRead(0));
  
  pinMode(WIO_5S_UP, INPUT); //set switch pin up as input
  pinMode(WIO_5S_DOWN, INPUT); //set switch pin down as input
  pinMode(WIO_5S_LEFT, INPUT); //set switch pin left as input
  pinMode(WIO_5S_RIGHT, INPUT); //set switch pin right as input
  pinMode(WIO_5S_PRESS, INPUT); //set switch pin press as input

  rand1 = random(9);
  rand2 = random(9);
  rand3 = random(9);

  Serial.println("Secret code:"+String(rand1)+String(rand2)+String(rand3));
  buzzerBeep();
  displayInfo();  
  updateScreen(); 

}
 
void upNumber(){    
  
  int myX;

  if (currentColumn==1){
    user1=user1+1;
    if (user1==10){user1=0;}
  }

  if (currentColumn==2){
    user2=user2+1;
    if (user2==10){user2=0;}
  }

  if (currentColumn==3){
    user3=user3+1;
    if (user3==10){user3=0;}
  }  
  
  updateScreen();  
}

void downNumber(){

  int myX;

  if (currentColumn==1){
     user1=user1-1;
    if (user1==-1){user1=9;}
  }

  if (currentColumn==2){
    user2=user2-1;
    if (user2==-1){user2=9;}
  }

  if (currentColumn==3){
    user3=user3-1;
    if (user3==-1){user3=9;}
  }  
  updateScreen();  
}

void left(){
  currentColumn=currentColumn-1;
  if (currentColumn==0){
    currentColumn=3;
  }

  updateScreen();  
}

void right(){
  currentColumn=currentColumn+1;
  if (currentColumn==4){
    currentColumn=1;
  }

  updateScreen();  

}

void pressButton(){
 buzzerBeep();
 counter++;
 calculateScores();
}

void calculateScores(){
  
  Serial.println("Secret code:"+String(rand1)+String(rand2)+String(rand3));
  
  rightNumbers=0;
  rightNumbersAndPositions=0;

  if (user1==rand1){rightNumbersAndPositions=rightNumbersAndPositions+1;Serial.println("Right number and position 1");}
  if (user2==rand2){rightNumbersAndPositions=rightNumbersAndPositions+1;Serial.println("Right number and position 2");}
  if (user3==rand3){rightNumbersAndPositions=rightNumbersAndPositions+1;Serial.println("Right number and position 3");}

  if (user1==rand2 or user1==rand3){rightNumbers=rightNumbers+1;Serial.println("Right number 1");}
  if (user2==rand1 or user2==rand3){rightNumbers=rightNumbers+1;Serial.println("Right number 2");}
  if (user3==rand1 or user3==rand2){rightNumbers=rightNumbers+1;Serial.println("Right number 3");}
  
  if (rightNumbersAndPositions==3){
    winScreen();
  }

  updateScreen();

}

void winScreen(){

  buzzerWin();

  tft.fillScreen(TFT_BLACK); 
  tft.setTextSize(3);  
  tft.setTextColor(TFT_WHITE);
  tft.drawString("Code found",60,80);
  tft.drawString(String(counter)+" attempts",60,110);
  delay(5000);

  rand1=random(9);
  rand2=random(9);
  rand3=random(9);

  user1=0;
  user2=0;
  user3=0;

  rightNumbersAndPositions=0;
  rightNumbers=0;
  counter=0; 
  currentColumn=1;

  updateScreen();

}

void updateScreen(){

  tft.setFreeFont(NULL);
  tft.fillScreen(TFT_BLACK); 
  tft.setTextSize(6);  
  tft.setTextColor(TFT_WHITE);
  

  int myX=60;
  if (currentColumn==1){tft.setTextColor(TFT_GREEN);}
  tft.drawString(String(user1),myX,100);
  
  tft.setTextColor(TFT_WHITE);
  if (currentColumn==2){tft.setTextColor(TFT_GREEN);}
  myX=myX+35;
  tft.drawString(String(user2),myX,100);
  
  tft.setTextColor(TFT_WHITE);
  if (currentColumn==3){tft.setTextColor(TFT_GREEN);}
  myX=myX+35;
  tft.drawString(String(user3),myX,100);

  tft.setTextColor(TFT_WHITE);
  myX=myX+55;
  tft.drawString(String(rightNumbersAndPositions),myX,100);
  myX=myX+35;
  tft.drawString(String(rightNumbers),myX,100); 

}

void loop() {

  if (digitalRead(WIO_5S_UP) == LOW) {  
   upNumber();
  }
  else if (digitalRead(WIO_5S_DOWN) == LOW) {
   downNumber();
  }
  else if (digitalRead(WIO_5S_LEFT) == LOW) {
   left();
  }
  else if (digitalRead(WIO_5S_RIGHT) == LOW) {
   right();
  }
  else if (digitalRead(WIO_5S_PRESS) == LOW) {
   pressButton();
  }

  delay(100);
}
