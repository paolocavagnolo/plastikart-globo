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

  randomSeed(analogRead(A5));
}

uint8_t mnm = 70;

uint8_t vel[] = {mnm,mnm,mnm,mnm};
bool rel[] = {0,0,0,0};

unsigned long ora = 0;

void loop() 
{
  spegni();

  uint8_t a = random(5);
  Serial.println(a);

  switch(a) {
    case 0:
      spegni();
      delay(4000);
      scosse(random(20,30));
      break;
    case 1:
      spegni();
      delay(6000);
      ondaSecca(random(20,30));
      break;
    case 2:
      spegni();
      delay(12000);
      salita();
      break;
    case 3:
      spegni();
      delay(5000);
      mare(random(20,45));
      break;
    case 4:
      spegni();
      delay(2000);
      vento(random(10,20));
      break;
  }

}

void mare(unsigned long minuti) {
  
  unsigned long durata = millis();
  unsigned long fine = minuti*1000;

  while ((millis() - durata) < fine){

    digitalWrite(relays[0],0);
    digitalWrite(relays[1],0);
    digitalWrite(relays[2],0);
    digitalWrite(relays[3],0);

    seqAll(85,1);
    seqAll(random(120,150),random(2,7));
    seqAll(85, random(2,7));
    spegni();
    delay(random(4000,8000));

  }

  spegni();
  
}

void ondaSecca(unsigned long minuti) {

  unsigned long durata = millis();
  unsigned long fine = minuti*1000;
  unsigned long dd = 0;
  unsigned long shock = 0;

  while ((millis() - durata) < fine){

    dd = random(300,1200);
    digitalWrite(relays[0],0);
    digitalWrite(relays[1],0);
    digitalWrite(relays[2],0);
    digitalWrite(relays[3],0);
    
    for (uint8_t j=0; j<random(2,4); j++) {

      for (uint8_t i=0; i<4;i++) {
        seq(i,255,dd);
        seq(i,mnm,dd);
      }

    }
    spegni();
    delay(random(10000,20000));

    if ((millis() - shock) > random(30000,40000)) {
      shock = millis();
      acceso();
      delay(1000);
      spegni();
      delay(6000);
      digitalWrite(relays[0],0);
      digitalWrite(relays[1],0);
      digitalWrite(relays[2],0);
      digitalWrite(relays[3],0);
    }

  }

  spegni();
}

void vento(unsigned long minuti) {
  
  unsigned long durata = millis();
  unsigned long fine = minuti*1000;

  unsigned long ferma = 0;

  while ((millis() - durata) < fine){

    for (uint8_t m=0; m<3; m++) {
      acceso();
      delay(random(500,1000));
      spegni();
      delay(random(1500,3000));
    }

    delay(random(6000,15000));

  }

  spegni();
}

void salita() {
  spegni();
  delay(2000);
  digitalWrite(relays[0],0);
  digitalWrite(relays[1],0);
  digitalWrite(relays[2],0);
  digitalWrite(relays[3],0);

  
  //sprint
    all(254);
    delay(100);

    //83 per un po di sec
    seqAll(83,5);
    delay(3000);
    //93 
    seqAll(93,5);
    delay(2000);
    //98
    seqAll(98,3);
    delay(1000);

    //102
    seqAll(102,2);
    delay(1000);

    //104
    seqAll(104,1);
    delay(1000);

    //120
    seqAll(120,1);
    delay(1000);

    //130
    seqAll(255,2);
    delay(10000);
}

void all(uint8_t v) {
  dmx_master.setChannelValue(1, v);
  dmx_master.setChannelValue(2, v);
  dmx_master.setChannelValue(3, v);
  dmx_master.setChannelValue(4, v);
}

void spegni() {

  digitalWrite(relays[0],1);
  digitalWrite(relays[1],1);
  digitalWrite(relays[2],1);
  digitalWrite(relays[3],1);
  dmx_master.setChannelValue(1, 0);
  dmx_master.setChannelValue(2, 0);
  dmx_master.setChannelValue(3, 0);
  dmx_master.setChannelValue(4, 0);
  
}

void acceso() {

  digitalWrite(relays[0],0);
  digitalWrite(relays[1],0);
  digitalWrite(relays[2],0);
  digitalWrite(relays[3],0);
  dmx_master.setChannelValue(1, 255);
  dmx_master.setChannelValue(2, 255);
  dmx_master.setChannelValue(3, 255);
  dmx_master.setChannelValue(4, 255);
  
}

void scosse(unsigned long minuti) {

  unsigned long durata = millis();
  unsigned long fine = minuti*1000;
  unsigned long shock = millis();

  while ((millis() - durata) < fine){

    for (uint8_t k = 0; k<random(2,8); k++) {
      uint8_t j=random(0,4);
  
      digitalWrite(relays[j],0);
      dmx_master.setChannelValue(j+1, 140);
      delay(600);
      digitalWrite(relays[j],1);
      dmx_master.setChannelValue(j+1, 0);
      
      delay(random(300,3000));
    }

    delay(random(5000,20000));

    if ((millis() - shock) > random(30000,45000)) {
      shock = millis();
      acceso();
      delay(1000);
      spegni();
      delay(6000);
    }
    
  }
  
}


void seq(uint8_t idMot, uint8_t v, unsigned long t) {

  if (v >= vel[idMot]) {

    unsigned long tt = t/(v - vel[idMot]);

    for (uint8_t i=vel[idMot]; i<v; i++) {

      vel[idMot] = i;
      dmx_master.setChannelValue(idMot+1, vel[idMot]);
      delay(tt);
      
    }
    
  }
  else {

    unsigned long tt = t/(vel[idMot] - v);

    for (uint8_t i=vel[idMot]; i>v; i--) {

      vel[idMot] = i;
      dmx_master.setChannelValue(idMot+1, vel[idMot]);
      delay(tt);
      
    }
    
  }


}

void seqAll(uint8_t v, unsigned long t) {

  if (v >= vel[0]) {

    unsigned long tt = (t*1000)/(v - vel[0]);
    
    for (uint8_t i=vel[0]; i<v; i++) {

      vel[0] = i;
      vel[1] = i;
      vel[2] = i;
      vel[3] = i;
      dmx_master.setChannelValue(0+1, vel[0]);
      dmx_master.setChannelValue(1+1, vel[1]);
      dmx_master.setChannelValue(2+1, vel[2]);
      dmx_master.setChannelValue(3+1, vel[3]);
      delay(tt);
      
    }
    
  }
  else {

    unsigned long tt = (t*1000)/(vel[0]-v);

    for (uint8_t i=vel[0]; i>v; i--) {

      vel[0] = i;
      vel[1] = i;
      vel[2] = i;
      vel[3] = i;
      dmx_master.setChannelValue(0+1, vel[0]);
      dmx_master.setChannelValue(1+1, vel[1]);
      dmx_master.setChannelValue(2+1, vel[2]);
      dmx_master.setChannelValue(3+1, vel[3]);
      delay(tt);
      
    }
    
  }


}

