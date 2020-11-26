#include <Wire.h>
#include <Stepper.h>
#define OPEN 1;
#define CLOSE 0;

const int stepsPerRevolution = 640;
Stepper myStepper(stepsPerRevolution, 2,4,3,5);

char prevVal = 'h';
int sensorPin = 8;
int sensorVal = 0;
int stateDoor = CLOSE;
bool isEnable = false;

void setup() {
  Serial.begin(9600);
  myStepper.setSpeed(50);
  pinMode(8,INPUT);
  pinMode(12,OUTPUT);
  Wire.begin(8); 

  //handler door at 'ban dem' :)
  Wire.onReceive(receiveEvent);
  
//   Wire.onRequest(requestDoor);
}

void loop() {
//Handler door at 'ban ngay' :)
  if(isEnable)
  {
    handleSensor();
  }
if(prevVal!='g')
{


  if(prevVal == '0') myStepper.step(-stepsPerRevolution); 
  else if(prevVal == '1') myStepper.step(stepsPerRevolution);
  else if(prevVal == '3') {
    isEnable = true; 
  }
  else if(prevVal == '4') {
    isEnable = false; 
//    delay(500);
    myStepper.step(stepsPerRevolution);
    stateDoor = CLOSE;
  }

  prevVal = 'g';
//  delay(200);
}
}

void handleSensor(){
  sensorVal = digitalRead(sensorPin);
  if(sensorVal == 0 && stateDoor != 1){
    openDoor();
    stateDoor = OPEN;
  }
  if(sensorVal == 1 && stateDoor != 0)
  {
    closeDoor();
    stateDoor = CLOSE;
  }  
}

void receiveEvent(int howMany) {

  char c;
  while (0 <Wire.available()) {
    c = Wire.read();

  }
 

//  openDoor();
prevVal = c;
//  if(!isEnable && c!='j'){
//    prevVal = c;
//    Serial.println("Run here");
//    if(prevVal != c) prevVal = c;
//    switch(prevVal){
//      case '0':
//        if(stateDoor != 0){
////           myStepper.step(-100);
//          Serial.println("Dong cong");  
//        }
//        
//        break;
//      case '1':
//        if(stateDoor != 1){
////           myStepper.step(100);
//          Serial.println("Mo cong");  
//        }
//        break;
//      default: break;  
//    }
//  }

//  if(c != 'j'){
//  
//    if(prevVal != c) prevVal = c;
//  
//    if(!isEnable)
//    {
//      Serial.println("run g");
//      if(prevVal == '1')// cua dang dong
//      {
//          openDoor();
//        stateDoor=OPEN;
//      }
//      if(prevVal == '0'){
//        closeDoor();
//        stateDoor=CLOSE;
//      }
//    }
  
//  }


  // if(preVal=='2')
  // {
  //  if(trangthai==1)
  //  {
  //  // co the dua ra canh bao roi dong cua
  //  closeDoor();
  //  trangthai=0;
  //  }
  //  ngat=0;
  //  Serial.print("dm");
  // }

  // if(preVal=='3')

}


// function that executes whenever data is requested from master
void requestDoor() {

}

void openDoor() {
  myStepper.step(stepsPerRevolution);
}

void closeDoor() {
  myStepper.step(-stepsPerRevolution);
}
