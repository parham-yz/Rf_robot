/*
* Arduino Wireless Communication Tutorial
*     Example 1 - Transmitter Code
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
char msg[32],ans[32];

void belink(int led_port,int duration){
  pinMode(led_port,OUTPUT);
  digitalWrite(led_port,1);
  delay(duration);
  digitalWrite(led_port,0);
}

void send_recive(char* message,char* answer){
  
  radio.stopListening();
  if (radio.write(message, 32)){
    radio.read(answer,32);
    Serial.println("caca");
    belink(7,100);
    
  } else {
    belink(7,1000);
    sprintf(answer,"Failled");
  }

}

void setup() {
  radio.begin();
  Serial.begin(9600);
  radio.openWritingPipe(address);
  radio.setPALevel(RF24_PA_MIN);
  radio.setDataRate( RF24_250KBPS );
  radio.enableAckPayload();
  radio.setRetries(5,5);
}

void loop() {

  sprintf(msg,"hallo");
  sprintf(ans,"empty");
  send_recive(msg,ans);
  Serial.println(ans);

  delay(1000);
}
