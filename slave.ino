/*
* Arduino Wireless Communication Tutorial
*       Example 1 - Receiver Code
*                
* by Dejan Nedelkovski, www.HowToMechatronics.com
* 
* Library: TMRh20/RF24, https://github.com/tmrh20/RF24/
*/

#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

RF24 radio(9, 10); // CE, CSN

const byte address[6] = "00001";

void belink(int led_port,int duration){
  pinMode(led_port,OUTPUT);
  digitalWrite(led_port,1);
  delay(duration);
  digitalWrite(led_port,0);
}   

void recive_send(char* message,char* answer,int channel){
  
  radio.writeAckPayload(channel, &answer, 32);
  
  radio.read(message, 32);
}

void setup() {
  Serial.begin(9600);
  radio.begin();
  radio.openReadingPipe(1, address);
  radio.setPALevel(RF24_PA_MIN);
  radio.setDataRate( RF24_250KBPS );
  radio.enableAckPayload();
}

void loop() {
  char msg[32],ans[32];
  int counter = 0;
  radio.startListening();
  sprintf(ans,"blank");
  if (radio.available()){
    belink(7,250);
    recive_send(msg,ans,1);
    strcpy(ans,msg);
    ans[1]='0'+counter;
    counter = (counter+1)%10;
  }
}
