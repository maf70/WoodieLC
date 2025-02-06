

#if ARDUINO >= 100
#include "Arduino.h"
#else
extern "C" {
#include "WConstants.h"
}
#endif

#include "reglages.h"
#include "moteur.h"

actionneur::actionneur(int p){
  pin=p;
  pinMode(pin, OUTPUT);
  digitalWrite(pin, HIGH); 
  }

void actionneur::demarre(long d){
  digitalWrite(pin, LOW); 
  duree = d;
  etat = 1;
  }

void actionneur::pause(int p){
  if(p)
  {
  digitalWrite(pin, HIGH);
  etat = 10;
  }
  else
  {
  digitalWrite(pin, LOW);
  etat = 1;
  }
}

void actionneur::arret(){
  digitalWrite(pin, HIGH); 
  etat = 0;
  }

void actionneur::tic(int t){
  if (etat != 10)
  {
    if (duree > t)
    {
      duree -= t;
    }
    else
    {
      duree = 0;
    }

    if (duree == 0) arret();
  }
  
}


moteur::moteur(int p, int pi) : actionneur (p) {
  pin_inverse=pi;
  nb_blocage=0;
  pinMode(pin_inverse, OUTPUT);
  digitalWrite(pin_inverse, HIGH); 
  }

void moteur::demarre(long d, u8 s ){
  // Demarrage : phase 1, on eteint les 2 sorties
  // on active la sortie normale au prochain tic
  digitalWrite(pin_inverse, HIGH); 
  digitalWrite(pin, HIGH); 
  duree = d;
  etat = 1;
  sens = s;
  }

void moteur::pause(int p){
  // Ne pas utiliser en l'etat
  if(p)
  {
  digitalWrite(pin, HIGH);
  etat = 10;
  }
  else
  {
  digitalWrite(pin, LOW);
  etat = 1;
  }
}

void moteur::arret(){
  digitalWrite(pin, HIGH); 
  digitalWrite(pin_inverse, HIGH); 
  etat = 0;
  }

void moteur::debloque(){
  nb_blocage = 0;
  }


void moteur::parametres(u8 delai, u8 duree, u8 vmin){
  moteur_delai_inversion = delai;
  moteur_duree_inversion = duree;
  moteur_vitesse_min = vmin;
  }
  
void moteur::tic(int t, int cpt){

  switch ( etat ) {
    case 0 :     // Arret
      break;
    case 1 :     // demarrage au prochain tic
      etat = 2;  
      break;
    case 2 :     // demarrage effectif
      etat = 3;  // etat 3 = fonctionnement normal
      if ( sens == 2 ) {
        // Start inverted
        digitalWrite(pin_inverse, LOW);
        digitalWrite(pin, HIGH);
      } else {
        digitalWrite(pin_inverse, HIGH);
        digitalWrite(pin, LOW);
      }
      break;
    case 3 :    // Fonctionnement normal
        
      if (duree > t)
      {
        duree -= t;
      }
      else
      {
        duree = 0;
      }

      if (duree == 0) 
      {
        digitalWrite(pin, HIGH);
        digitalWrite(pin_inverse, HIGH);
        etat = 0;
      }
      else if (cpt < moteur_vitesse_min)
      {
        // Blocage !
        nb_blocage++;
        etat = 4;
        tempo = moteur_delai_inversion;
        digitalWrite(pin_inverse, HIGH);
        digitalWrite(pin, HIGH);
      }
      break;
    case 4 :    // pause avant inversion
      if (--tempo <= 0)
      {
        etat = 5;
        tempo = moteur_duree_inversion;
        if ( sens == 2 ) {
          // Inverted is inverted => normal operation
          digitalWrite(pin_inverse, HIGH);
          digitalWrite(pin, LOW);
        } else {
          digitalWrite(pin_inverse, LOW);
          digitalWrite(pin, HIGH);
        }
      }
      break;
    case 5 :    // inversion
      if (cpt < moteur_vitesse_min)
      {
        // Blocage !
        nb_blocage++;
        tempo = 1;
      }
      if (--tempo <= 0)
      {
        etat = 6;
        tempo = moteur_delai_inversion;
        digitalWrite(pin_inverse, HIGH);
        digitalWrite(pin, HIGH);
      }
      break;
    case 6 :    // pause avant marche normale
      if (--tempo <= 0)
      {
        etat = 3;
        if ( sens == 2 ) {
          // Start inverted
          digitalWrite(pin_inverse, LOW);
          digitalWrite(pin, HIGH);
        } else {
          digitalWrite(pin_inverse, HIGH);
          digitalWrite(pin, LOW);
        }
      }
      break;
  }

  
}
