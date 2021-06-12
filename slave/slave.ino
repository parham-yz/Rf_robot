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

#define ultra_pingPin 3
#define ultra_echoPin 4

#define MOTOR_DIR1 5
#define MOTOR_DIR2 7
#define MOTOR_POWER 6



RF24 radio(9, 10); // CE, CSN
char master_command[32],ans[32],tempstr[32];
int counter = 0;
const byte address[6] = "00001";

void belink(int led_port,int duration){
  pinMode(led_port,1);
  digitalWrite(led_port,1);
  delay(duration);
  digitalWrite(led_port,0);
}

void recive_send(char* message,char* answer,int channel){
  
  radio.writeAckPayload(channel, answer, 32);
  radio.read(message, 32);
  belink(7,50);
}

int measure_distance(int pingPin,int echoPin){
  long duration;
  pinMode(pingPin, OUTPUT);
  digitalWrite(pingPin, LOW);
  delayMicroseconds(2);
  digitalWrite(pingPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(pingPin, LOW);
  pinMode(echoPin, INPUT);
  duration = pulseIn(echoPin, HIGH);
  return (duration/ 29 / 2);
}

void move_dc(char* dir, int power, int duration) {

  Serial.println(dir);
  if (!strcmp(dir,"0000")){
    analogWrite(MOTOR_POWER,0);
    delay(duration);
    return;
  } else {
    analogWrite(MOTOR_POWER,power);
  }

  
  if (dir[0] == '1'){
    digitalWrite(MOTOR_DIR1,1);
    digitalWrite(MOTOR_DIR2,1);
    Serial.println('1');
  }
  else if (dir[1] == '1'){
    digitalWrite(MOTOR_DIR1,0);
    digitalWrite(MOTOR_DIR2,0);
    Serial.println('2');
  }
  else if (dir[2] == '1'){
    digitalWrite(MOTOR_DIR1,1);
    digitalWrite(MOTOR_DIR2,0);
    Serial.println('3');
  }
  else if (dir[3] == '1'){
    digitalWrite(MOTOR_DIR1,0);
    digitalWrite(MOTOR_DIR2,1);
    Serial.println('4');
  }
  delay(duration);
  
}


void setup() {
	Serial.begin(9600);
	radio.begin();
	radio.openReadingPipe(1, address);
	radio.setPALevel(RF24_PA_MIN);
	radio.setDataRate( RF24_250KBPS );
	radio.enableAckPayload();
  radio.startListening();

	sprintf(ans,"blank");

  pinMode(MOTOR_DIR1,OUTPUT);
  pinMode(MOTOR_DIR2,OUTPUT);
  pinMode(MOTOR_POWER,OUTPUT);
}

void loop() {

	if (radio.available()){
		sprintf(ans,"dist: %d",measure_distance(ultra_pingPin,ultra_echoPin));
		recive_send(master_command,ans,1);
    sscanf(master_command,"move: %s",tempstr);
    Serial.println(tempstr);
    move_dc(tempstr,250,100);
    
	} else {
    delay(25);
    return;
	}




}
