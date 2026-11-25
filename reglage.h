#ifndef __REGLAGE__H__
#define __REGLAGE__H__

#include <EEPROM.h>
#include "affichage.h"   // affiche_reglage(), typedef u8 (via moteur.h)

// Variables d'etat de l'UI, definies dans init.h
extern u8 reglage;
extern int tmp_reglage;
extern u8 reglage_next;
extern u8 mode;
extern volatile u8 BOK_st;
extern volatile u8 BMENU_st;
extern volatile u8 BUP_st;
extern volatile u8 BDOWN_st;

// Valeurs reglables, definies dans init.h
extern u8 temperature_demarrage;
extern u8 temperature_arret;
extern uint16_t tempo_cycle;
extern uint16_t tempo_ventilo;
extern u8 tempo_moteur;
extern u8 moteur_duree_inversion;
extern u8 moteur_vitesse_min;
extern u8 moteur_blocage_max;
extern u8 temperature_secu;
extern u8 temperature_vis_max;
extern u8 probe_select;

// Defini dans WoodieLC.ino
void restart_boiler(u8 e);

// Lecture d'un reglage en EEPROM, avec bornes et valeur par defaut
// (remplace la macro READ_REGLAGE)
template<class T>
void lire_reglage(T& value, int address, long vmin, long vmax, long vdefaut) {
  EEPROM.get(address, value);
  if (value < vmin || value > vmax) { value = (T)vdefaut; }
}

// Sauvegarde d'un reglage en EEPROM (remplace la macro SAVE_REGLAGE)
template<class T>
void ecrire_reglage(const T& value, int address) {
  EEPROM.put(address, value);
}

// Lecture de tous les reglages a l'allumage
void lire_tous_reglages(void);

// Gestion des boutons sur un parametre du menu reglage
// (remplace les macros SET_PARAM + SET_NEXT_REGLAGE)
//
// REPETITION : la boucle tourne exactement 1x/seconde. En plus du drapeau
// pose par l'ISR (appui initial), on lit directement la broche : une touche
// UP/DOWN maintenue compte donc 1 increment par tour de boucle, soit
// ~1x/seconde, sans depends d'aucune synchronisation d'horloges.
template<class T>
void gerer_param(T& value, const char* message, int pmin, int pmax,
                 int valeur_suivante, u8 etat_suivant) {
  u8 up   = (BUP_st   > 0) || (digitalRead(B_UP)   == LOW);
  u8 down = (BDOWN_st > 0) || (digitalRead(B_DOWN) == LOW);
  if      (up   > 0 ) { tmp_reglage++; }
  else if (down > 0 ) { tmp_reglage--; }
  else if (BOK_st   > 0 ) { value = (T)tmp_reglage; reglage_next = 1; }
  else if (BMENU_st > 0 ) { mode = MODE_STATS; }
  if      (pmin > tmp_reglage) { tmp_reglage = pmin; }
  else if (pmax < tmp_reglage) { tmp_reglage = pmax; }
  affiche_reglage(message, tmp_reglage);

  if (reglage_next == 1) {
    tmp_reglage = valeur_suivante;
    reglage = etat_suivant;
    reglage_next = 0;
  }
}

// Machine a etats du menu reglage
void gerer_reglage(void);

#endif
