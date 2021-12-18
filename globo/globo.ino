#include <Conceptinetics.h>

#define DMX_MASTER_CH   4 
#define RXEN_PIN        2

#define RELAY_1         8
#define RELAY_2         9
#define RELAY_3         10
#define RELAY_4         11

uint8_t relays[] = {RELAY_1,RELAY_2,RELAY_3,RELAY_4};

DMX_Master dmx_master ( DMX_MASTER_CH, RXEN_PIN );

void setup() {             

  pinMode(RELAY_1, OUTPUT);
  digitalWrite(RELAY_1, LOW);
  pinMode(RELAY_2, OUTPUT);
  digitalWrite(RELAY_2, LOW);
  pinMode(RELAY_3, OUTPUT);
  digitalWrite(RELAY_3, LOW);
  pinMode(RELAY_4, OUTPUT);
  digitalWrite(RELAY_4, LOW);
  
  dmx_master.enable ();  
  dmx_master.setChannelRange ( 1, 4, 0 );

  Serial.begin(9600);
}
uint16_t rit;

unsigned long t = 0;

void loop() 
{

  if ((millis() - t) < 60000) {
    digitalWrite(RELAY_1, LOW);
    digitalWrite(RELAY_2, LOW);
    digitalWrite(RELAY_3, LOW);
    digitalWrite(RELAY_4, LOW);

    rit = map(analogRead(A9),0,1024,0,255);
    
    delay(100);
    
    dmx_master.setChannelValue(1, rit);
    dmx_master.setChannelValue(2, rit);
    dmx_master.setChannelValue(3, rit);
    dmx_master.setChannelValue(4, rit);

  }
  else {
    if ((millis() - t) > 120000) {
      t = millis();
    }
    
    for (uint8_t i=0; i<4; i++) {
      rit = map(analogRead(A9),0,1024,100,10000);
      
      digitalWrite(relays[i],LOW);
      
      for (uint8_t j=0; j<255; j++) {
        dmx_master.setChannelValue(i+1, j);
        delayMicroseconds(rit);
      }
      for (uint8_t j=254; j>0; j--) {
        dmx_master.setChannelValue(i+1, j);
        delayMicroseconds(rit);
      }
      
      digitalWrite(relays[i],HIGH);
      
    }
  }


}
