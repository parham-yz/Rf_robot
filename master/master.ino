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

#define UP_PIN    6
#define DOWN_PIN  5
#define LEFT_PIN  4
#define RIGHT_PIN 3

#define DIST_LED0_PIN A0
#define DIST_LED1_PIN A1
#define DIST_LED2_PIN A2


RF24 radio(9, 10); // CE, CSN

const byte address[6] = "00001";
char command[32],ans[32],tempstr[32];
int distance =0;

void belink(int led_port,int duration){
  pinMode(led_port,OUTPUT);
  digitalWrite(led_port,1);
  delay(duration);
  digitalWrite(led_port,0);
}

void display_dist(int dist,int th1,int th2){
  
  if (dist<th1){
  analogWrite(DIST_LED0_PIN, 200);
  analogWrite(DIST_LED1_PIN, 0);
  analogWrite(DIST_LED2_PIN, 0);
    
  } else if (dist<th2){
  analogWrite(DIST_LED0_PIN, 200);
  analogWrite(DIST_LED1_PIN, 200);
  analogWrite(DIST_LED2_PIN, 0);
    
  } else {
  analogWrite(DIST_LED0_PIN, 200);
  analogWrite(DIST_LED1_PIN, 200);
  analogWrite(DIST_LED2_PIN, 200);
  }
}

void send_recive(char* message,char* answer){
    
    radio.stopListening();
    if (radio.write(message, 32)){
        radio.read(answer,32);
        belink(7,50);
        
    } else {
        belink(7,1000);
        sprintf(answer,"Failled");
        delay(1000);
    }

}

void read_buttons(char* res){
   sprintf(res,"%d%d%d%d",digitalRead(UP_PIN),
   digitalRead(DOWN_PIN),
   digitalRead(LEFT_PIN),
   digitalRead(RIGHT_PIN));
}

void setup() {
    radio.begin();
    Serial.begin(9600);
    radio.openWritingPipe(address);
    radio.setPALevel(RF24_PA_MIN);
    radio.setDataRate( RF24_250KBPS );
    radio.enableAckPayload();
    radio.setRetries(5,5);

    sprintf(command,"hallo");
    sprintf(ans,"empty");

    
    pinMode(UP_PIN,INPUT);
    pinMode(DOWN_PIN,INPUT);
    pinMode(LEFT_PIN,INPUT);
    pinMode(RIGHT_PIN,INPUT);
}

void loop() {


  read_buttons(tempstr);
  sprintf(command, "move: %s",tempstr);
  Serial.println(command);
  
  send_recive(command,ans);
  
  sscanf(ans,"dist: %d",&distance);
  display_dist(distance,20,100);
  
  delay(200);
}
