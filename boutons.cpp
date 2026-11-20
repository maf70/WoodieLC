#if ARDUINO >= 100
#include "Arduino.h"
#else
extern "C" {
#include "WConstants.h"
}
#endif

#include "config.h"
#include "boutons.h"

// CORRECTION : volatile car ecrits dans l'ISR du Timer2, lus dans loop()
volatile u8 B_disable = 0;

// >>> NOUVELLES VARIABLES POUR DEBOUNCE <<<
volatile u8 BOK_st   = 0;
volatile u8 BMENU_st = 0;
volatile u8 BUP_st   = 0;
volatile u8 BDOWN_st = 0;
volatile uint8_t B_last[4] = {HIGH, HIGH, HIGH, HIGH};    // États précédents (0:MENU, 1:OK, 2:UP, 3:DOWN)
volatile uint8_t B_debounce_cpt[4] = {0};                 // Compteurs de stabilité
#define DEBOUNCE_THRESHOLD 3                               // 3 cycles = ~48ms (1 appel ISR = ~16ms avec prescaler 1024)

void interruptC1()
{
  if ( (millis() - OpticStamp) > 20 )
  {
    OpticCount += 1 ;
    OpticStamp = millis();
  }
}

ISR( TIMER2_OVF_vect )
{
  if (B_disable > 0) {
    B_disable--;
    return;
  }

  // Lit les états actuels des boutons (avec pull-up: LOW = appuyé)
  uint8_t B_current[4] = {
    digitalRead(B_MENU),
    digitalRead(B_OK),
    digitalRead(B_UP),
    digitalRead(B_DOWN)
  };

  // Gère chaque bouton avec debounce
  for (uint8_t i = 0; i < 4; i++) {
    // Si l'état a changé, réinitialise le compteur de stabilité
    if (B_current[i] != B_last[i]) {
      B_debounce_cpt[i] = 0;
      B_last[i] = B_current[i];
    }
    // Sinon, incrémente le compteur
    else if (B_debounce_cpt[i] < DEBOUNCE_THRESHOLD) {
      B_debounce_cpt[i]++;
    }

    // Détection d'un appui VALIDE (front descendant + stabilité)
    if (B_current[i] == LOW && B_debounce_cpt[i] >= DEBOUNCE_THRESHOLD) {
      switch (i) {
        case 0: if (BMENU_st == 0) { BMENU_st = 1; B_disable = BUTTON_DISABLE; } break;
        case 1: if (BOK_st   == 0) { BOK_st   = 1; B_disable = BUTTON_DISABLE; } break;
        case 2: if (BUP_st   == 0) { BUP_st   = 1; B_disable = BUTTON_DISABLE; } break;
        case 3: if (BDOWN_st == 0) { BDOWN_st = 1; B_disable = BUTTON_DISABLE; } break;
      }
    }
  }
}

void clear_button(void) {
      B_disable = BUTTON_DISABLE;
      BMENU_st = 0;
      BOK_st   = 0;
      BUP_st   = 0;
      BDOWN_st = 0;

}
