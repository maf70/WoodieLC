#ifndef __BOUTONS__H__
#define __BOUTONS__H__

#include "moteur.h"   // typedef u8

// Etats des boutons : ecrits par l'ISR du Timer2, lus dans loop()
// (definis dans boutons.cpp)
extern volatile u8 B_disable;
extern volatile u8 BOK_st;
extern volatile u8 BMENU_st;
extern volatile u8 BUP_st;
extern volatile u8 BDOWN_st;

// Compteur optique : ecrit par l'interruption OPTICAL_1, lu dans loop()
// (definis dans init.h)
extern volatile int OpticCount;
extern volatile unsigned long OpticStamp;

void clear_button(void);
void interruptC1(void);

#endif
