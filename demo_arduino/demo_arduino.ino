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
}

void loop() {
	//Handler door at 'ban ngay' :)
	if(isEnable)
	{
		handleSensor();
	}
	if(prevVal!='g')
	{


	if(prevVal == '0') {
		myStepper.step(-stepsPerRevolution);
		stateDoor = CLOSE;

	}
	else if(prevVal == '1') {
		myStepper.step(stepsPerRevolution);
		stateDoor = OPEN;
	}
	else if(prevVal == '3') {
		isEnable = true; 
	}
	else if(prevVal == '4') {
		isEnable = false;
		if(stateDoor==OPEN){
		myStepper.step(stepsPerRevolution);
		stateDoor = CLOSE;
		}
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

	prevVal = c;

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
