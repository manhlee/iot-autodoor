#include <Wire.h>


int CAM_BIEN = A0;
int LED = 9;


int muc_cu = 0;

void setup() {
  // put your setup code here, to run once:
  
  Serial.begin(9600);

  Wire.begin(8);                /* join i2c bus with address 8 */
//  Wire.onReceive(receiveEvent); /* register receive event */
  Wire.onRequest(requestEvent); /* register request event */
  
  pinMode(CAM_BIEN,INPUT);
  pinMode(LED,OUTPUT);
  
}

void loop() {
  
  // put your main code here, to run repeatedly:
  int muc_moi = analogRead(CAM_BIEN);
  Serial.println(muc_moi);
  int tmp = 0;
  
  if(muc_moi < 100) tmp = 100;
  else if(muc_moi < 200) tmp = 200;
  else if(muc_moi < 300) tmp = 300;
  else if(muc_moi < 400) tmp = 400;
  else if(muc_moi < 500) tmp = 500;
  else if(muc_moi < 600) tmp = 600;
  else if(muc_moi < 700) tmp = 700;
  else if (muc_moi < 800) tmp = 800;

  if(tmp != muc_cu){
    muc_cu = tmp;
  }
  delay(0);

}


void receiveEvent(int howMany) {
 while (0 <Wire.available()) {
    char c = Wire.read();      /* receive byte as a character */
    Serial.print(c);           /* print the character */
  }
 Serial.println();             /* to newline */
}

// function that executes whenever data is requested from master
void requestEvent() {

 
  char  buf[1024];
  snprintf(buf, sizeof(buf), "%d", muc_cu);
 
 Wire.write(buf);  /*send string on request */
}
