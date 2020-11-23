#include <Wire.h>
#include <Stepper.h>
const int stepsPerRevolution = 700;  // hehe
 char preVal='h';
Stepper myStepper(stepsPerRevolution, 2,4,3,5);
int cambien=8;
int trangthai=0;
int ngat=1;
void setup() {
  // put your setup code here, to run once:
  myStepper.setSpeed(50);
  pinMode(8,INPUT);
  Wire.begin(8); 
   Wire.onReceive(receiveDoor);
   Wire.onRequest(requestDoor);
  Serial.begin(9600);
}

void loop() {

  if(ngat==1)
  {
    int cb=digitalRead(cambien);
    if(cb==0&&trangthai!=1){
      openDoor();
      trangthai=1;
    }
    if(cb==1&&trangthai!=0)
    {
      closeDoor();
      trangthai=0;
    }
  }
  // put your main code here, to run repeatedly:
}


void receiveDoor(int howMany) {
 
  char c;
   while (0 <Wire.available()) {
      c = Wire.read();
      if(preVal!=c){
        preVal=c;
      }
   }
   Serial.print("_____________");
   Serial.println(preVal);
    if(ngat==0)
    {
      Serial.println("chay1");
      if(preVal=='1')// cua dang dong
     {
      Serial.println("chay2");
       openDoor();
     }
     if(preVal=='0'){
       closeDoor();
     }
            /* print the character */  
    }
    if(preVal=='2')
    {
      if(trangthai==1)
      {
        // co the dua ra canh bao roi dong cua
        closeDoor();
        trangthai=0;
      }
      ngat=0;
      Serial.print("dm");
    }
    if(preVal=='3')
    {
       ngat=1;
        Serial.print("dm");
    }
}
// function that executes whenever data is requested from master
void requestDoor() {

}
void openDoor(){
  myStepper.step(stepsPerRevolution);
}
void closeDoor()
{
  myStepper.step(-stepsPerRevolution);
}
