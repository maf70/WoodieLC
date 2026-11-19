/* Dépendances */
//#include <SPI.h>
#include <Wire.h>
#include <Adafruit_SSD1306.h>

#include <OneWire.h>
#include <DallasTemperature.h>
//#include <MAX6675.h>

#include "debug.h"

#include "moteur.h"

// Temperatures management
int temperature_eau = -127;
int temperature_vis = -127;
u8 temperature_demarrage = TEMPERATURE_DEMARRAGE;
u8 temperature_arret     = TEMPERATURE_ARRET;
u8 temperature_secu = TEMP_SECU;
u8 temperature_vis_max =  TEMP_VIS;
// CORRECTION : int au lieu de u8, pour memoriser correctement -1
int memorise_temperature_eau = 0;
int memorise_temperature_vis = 0;

// Temperature probes error management
u8 temperature_error_eau = 0;
u8 temperature_error_eau_total = 0;
u8 temperature_error_vis = 0;
u8 temperature_error_vis_total = 0;
u8 temperature_error_same = 0;
u8 temperature_error_same_total = 0;
u8 sonde_error_0 = 0;
u8 sonde_error_0_total = 0;
u8 sonde_error_1 = 0;
u8 sonde_error_1_total = 0;
u8 sonde_error_X = 0;
u8 sonde_error_X_total = 0;
u8 Compteur_secondes       =  0;

// Temperature probes management
u8 probe_select =  PROBE_SELECT;
u8 probe_nb     =  0;
u8 error        =  0;

// Motor settings
u8 moteur_delai_inversion = MOTEUR_DELAI_INVERSION;
u8 moteur_duree_inversion = MOTEUR_DUREE_INVERSION;
u8 moteur_vitesse_min     = MOTEUR_VITESSE_MIN;
u8 moteur_blocage_max     = MOTEUR_BLOCAGE_MAX;
u8 blocage = 0;
int tic = 0;

// Boiler management
uint16_t tempo_cycle   = TEMPO_CYCLE;
uint16_t tempo_ventilo = TEMPO_VENTILO;
u8 tempo_moteur  = TEMPO_MOTEUR;
u8 version_display =  VERSION_TEMPO;
u8 boiler_screen   =  BOILER_SCREEN_DEFAULT;
u8 etat = ETAT_REPOS;   // etat repos
u8 mode = MODE_POWER_ON;  // Start boiler after version display

// Variables for statistics
int nb_cycle = 0;
int stat_cycle = 0;
int stat_repos = 0;
int stat_cycle_total = 0;
int stat_chauffe_total = 0;

// Variables for settings
u8 reglage = REGLAGE_NONE;
int tmp_reglage  = -1;
u8 reglage_next = 0;

// Loop and timing management
unsigned long t_loop_debut = 0;
unsigned long t_loop_fin = 0;
unsigned long t_loop_delai = 0;
// Compteur de debordements de millis() (rollover tous les ~49,7 jours)
unsigned long millis_last = 0;
u8 millis_overflow = 0;

// interruption interruptC1() management
volatile int OpticCount = 0;
volatile unsigned long OpticStamp = 0;
int optic_seconde;

moteur MoteurVis(RELAIS_MOTEUR, RELAIS_MOTEUR_INV);
actionneur Ventilo(RELAIS_VENTILO);

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

// Setup a oneWire instance to communicate with any OneWire devices 
// (not just Maxim/Dallas temperature ICs)
OneWire oneWire(ONE_WIRE_BUS);
 
// Pass our oneWire reference to Dallas Temperature.
DallasTemperature sensors(&oneWire);

// initialize the Thermocouple
//Adafruit_MAX31855 thermocouple(MAXCLK, MAXCS, MAXDO);
// Initialize the MAX6675 Library for our chip
//MAX6675 temp(SONDEK_CS,SONDEK_SO,SONDEK_SCK /*,SONDEK_UNIT*/);