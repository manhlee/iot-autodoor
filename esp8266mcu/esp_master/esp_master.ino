#include <Arduino_JSON.h>

#include <Wire.h>

#include <SocketIoClient.h>

#include <WebSockets2_Generic.h>

#include <WebSocketsServer.h>

#include <WebSockets.h>

#include <Arduino.h>

#include <ESP8266WiFi.h>


#define USER_SERIAL Serial


const char* ssid = "VietTel";
const char* pass = "Tenwifilamatkhau45";
const char* host ="192.168.1.2";

int port = 3000;

int n=0;
char tmp[3];
int i = 0;
 char muc_cu[3];


SocketIoClient webSocket;



void setup() {
  USER_SERIAL.begin(9600);

  
  
  searchWiFi();
  connectWiFi();

  webSocket.begin(host,port);
 
  webSocket.on("req_info_esp",res_sv);

  webSocket.on("status-ports", handlerOn);

  Wire.begin(D1, D2);
 

}

void loop() {
  webSocket.loop();
  webSocket.on("status-port-change", handlerOn);

      /* stop transmitting */

  Wire.requestFrom(8, 3); /* request & read data of size 13 from slave */
  while(Wire.available()){
    char c = Wire.read();
    tmp[i] = c;
    i++;
  }

  i=0;
  if(strcmp(tmp,muc_cu)!=0){

char str3[100];
  strcpy(str3, tmp);
  char str4[100] ="\"";
   strcat(str4, str3);
   strcat(str4, "\"");
  webSocket.emit("data-mucnuoc", str4);
  strcpy(muc_cu,tmp);
  }
  delay(0);
}



void res_sv(const char * payload, size_t length){
   webSocket.emit("get-ports", "\"{'name':'ESP'}\"");
}
void handlerOn(const char * payload, size_t length){

      USER_SERIAL.println((String)payload);
  
      JSONVar data_ports = JSON.parse((String)payload);
       
      if (JSON.typeof(data_ports) == "undefined") {
        Serial.println("Parsing input failed!");
        return;
      }

      JSONVar keys = data_ports.keys();

      for (int i = 0; i < keys.length(); i++) {
        JSONVar value = data_ports[keys[i]];

        Serial.print("GPIO: ");
        Serial.print(keys[i]);
        Serial.print(" - SET to: ");
        Serial.println(value);
        pinMode(atoi(keys[i]), OUTPUT);
        digitalWrite(atoi(keys[i]), atoi(value));
        
      }

      webSocket.emit("success-notify", "\"Done!\"");
  }

void searchWiFi(){
  int numberOfNetwork = WiFi.scanNetworks();
  USER_SERIAL.println("----");
  
  for(int i = 0; i < numberOfNetwork; i++ ){
    USER_SERIAL.print("Network name: ");
    USER_SERIAL.println(WiFi.SSID(i));
    USER_SERIAL.print("Signal strength: ");
    USER_SERIAL.println(WiFi.RSSI(i));
    USER_SERIAL.println("--------------");
  }
}


void connectWiFi(){
  WiFi.begin(ssid, pass);
  while(WiFi.status() != WL_CONNECTED){
    USER_SERIAL.print(".");
    delay(1000);
  }

  USER_SERIAL.print("");
  USER_SERIAL.println("WiFi connected");
  USER_SERIAL.print("IP Address : ");
  USER_SERIAL.println(WiFi.localIP());
  
}
