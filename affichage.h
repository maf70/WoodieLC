#ifndef __AFFICHAGE__H__
#define __AFFICHAGE__H__

#include "moteur.h"   // typedef u8

// Declaration anticipee : l'objet display est defini dans init.h
class Adafruit_SSD1306;

// Variables globales partagees avec l'unite principale (definies dans init.h)
extern Adafruit_SSD1306 display;
extern int optic_seconde;
extern u8 blocage;
extern int temperature_eau;
extern int temperature_vis;
extern int memorise_temperature_eau;
extern int memorise_temperature_vis;
extern u8 millis_overflow;
extern int tic;
extern int nb_cycle;
extern u8 temperature_error_eau_total;
extern u8 temperature_error_vis_total;
extern u8 temperature_error_same_total;
extern u8 sonde_error_0_total;
extern u8 sonde_error_1_total;
extern u8 sonde_error_X_total;
extern int stat_cycle;
extern int stat_repos;
extern int stat_cycle_total;
extern int stat_chauffe_total;

// Fonctions d'affichage
void affiche_header(void);
void affiche_reglage(const char * message, int valeur);
void affiche_date(const u8 detail);
void affiche_boiler(const u8 etat, const u8 screen);
void affiche_secu(const u8 secu);
void affiche_error(const u8 erreur);
void affiche_version(void);
void affiche_stop(void);
void affiche_stats(void);
void affiche_reglage(void);

#endif
