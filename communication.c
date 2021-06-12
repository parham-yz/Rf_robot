#include "communication.h"
void belink(int led_port,int duration){
  pinMode(led_port,1);
  digitalWrite(led_port,1);
  delay(duration);
  digitalWrite(led_port,0);
}

