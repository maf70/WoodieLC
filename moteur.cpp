
#include "moteur.h"

#if ARDUINO >= 100
#include "Arduino.h"
#else
extern "C" {
#include "WConstants.h"
}
#endif


actionneur::actionneur(int p){
  pin=p;
  pinMode(pin, OUTPUT);
  digitalWrite(pin, HIGH); 
  }

void actionneur::demarre(long d){
  digitalWrite(pin, LOW); 
  duree = d;
  }

void actionneur::arret(){
  digitalWrite(pin, HIGH); 
  
  }

void actionneur::tic(int t){
  if (duree > t)
  {
    duree -= t;
  }
  else
  {
    duree = 0;
  }

  if (duree == 0) digitalWrite(pin, HIGH);
  
}


moteur::moteur(int p, int pi) : actionneur (p) {
  pin_inverse=pi;
  pinMode(pin_inverse, OUTPUT);
  digitalWrite(pin_inverse, HIGH); 
  }

void moteur::tic(int t, int cpt){
  if (duree > t)
  {
    duree -= t;
  }
  else
  {
    duree = 0;
  }

  if (duree == 0) digitalWrite(pin, HIGH);
  
}

