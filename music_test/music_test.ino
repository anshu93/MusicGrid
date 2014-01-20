#include <TimerOne.h>


#define dim 5  //Dimension of LED matrix
#define Input A0 //Input analog pin
#define Feedback 13 //pin 13 will flash if threshold is exceeded
int row = 0;  //Current row being processed
byte* CurrentFrame; //Pointer to the byte where the actual frame is stored
int FrameIndex = 0;  //Index of frame being displayed
byte mask = B00000011;  //Mask of which leds to light
int time = 32768;      //Initialize time to be a 16bit number

//To set an led high cols pin = HIGH and rows = LOW
int cols[dim] = {12,11,10,9,8};  //Column leds
int rows[dim] = {7,6,5,4,3};     //Row leds

typedef byte Frame[dim];
#include "frames.h"

int thresh = 40;  //Threshold above which the board will respond
int WaitTime = 2000; //Down time between responding to bass
int Sync = 1; // 0=board operated by timer. 1=music
int FrameTime = 400; //time for changing frames in ms
int threshCrossed = 0; //trigger status

void BlankScreen(){
 for (int i = 0; i < dim; i++) {
    digitalWrite(cols[i], LOW);
  }

  for (int j = 0; j < dim; j++) {
    digitalWrite(rows[j], HIGH);
  }
}

void UpdateFrame(int index){  //Function that updates the frame required in time for the interrupt
 noInterrupts(); //Make sure the interrupt doesn't screw up iteration
 CurrentFrame = Frames[index]; 
 interrupts();  //Resume interrupting
}

void CreateScreen(){
  digitalWrite(rows[row], HIGH);  //Turn row off
  if(mask == B00000011){
   mask = B00001100; 
  }
  else if(mask == B00001100){
   mask = B00010000; 
  }
  else if(mask == B00010000){
    row++;      //Go to next row
    if(row == dim){  //if filled up all the rows
     row = 0; 
    }
  mask = B00000011;
  }
  
  //PORTB is the register that controls columns
  PORTB = PORTB & B11100000;  //Retain all but the bottom 5 bits
  PORTB = PORTB | (CurrentFrame[row] & mask);       //turn on 2 allowed leds
  
  digitalWrite(rows[row], LOW);  //Allow the leds to go on
}


void setup(){
 for(int i = 0; i < dim; i++){
  pinMode(rows[i], OUTPUT);
  pinMode(cols[i], OUTPUT);
 } 
 
  pinMode(Input, INPUT);
  pinMode(Feedback, OUTPUT);
 
 BlankScreen(); //Create a dark display
 
 Timer1.initialize(500);    //Initialise timer interrupts to 1000us
 Timer1.attachInterrupt(CreateScreen);  //Make the timer run CreateSreen() 
}

void loop(){
  if(Sync == 1){
    int InputVal = analogRead(Input); //Read incoming data
    time++;  //Increment times read
    if(InputVal > thresh){  //If thresh exceeded
     if((threshCrossed == 0) && (time > WaitTime)){//If not already above threshold and enough time has passed since last trigger
      threshCrossed = 1;//Update status
      time = 0;  //Reset waittime
      
      if(FrameIndex == FRAMECNT){  //Loop the display
      FrameIndex = 0;
      } 
      UpdateFrame(FrameIndex);  //Display frame
      FrameIndex++;                //Increment to next frame
     } 
     digitalWrite(Feedback, HIGH);  //Turn on 13 to show threshold is exceeded
    }
    else{  //If thresh not exceeded
      digitalWrite(Feedback, LOW);
      if(threshCrossed == 1){
       threshCrossed = 0;  //Update trigger status 
      }
    }
  }
    
  else{ //If not syncing to music
    if(FrameIndex == FRAMECNT){  //Loop the display
  FrameIndex = 0;
 } 
  UpdateFrame(FrameIndex);  
  delay(FrameTime);
  FrameIndex++;                //Increment to next frame
  }
}









