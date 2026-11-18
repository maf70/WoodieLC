/* Dépendances */
//#include <SPI.h>
#include <Wire.h>
#include <Adafruit_SSD1306.h>

#include <OneWire.h>
#include <DallasTemperature.h>
//#include <MAX6675.h>

#include "debug.h"

#include "moteur.h"

int temperature_eau = -127;
int temperature_vis = -127;
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
//long temperature_K   = -300;  // Temperature output variable

moteur MoteurVis(RELAIS_MOTEUR, RELAIS_MOTEUR_INV);
actionneur Ventilo(RELAIS_VENTILO);

// CORRECTION : volatile car modifie dans l'interruption interruptC1()
volatile int OpticCount = 0;
volatile unsigned long OpticStamp = 0;

int tic = 0;
u8 etat = ETAT_REPOS;   // etat repos
u8 mode = MODE_POWER_ON;  // Start boiler after version display

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

// Variables pour compenser le temps de cycle
unsigned long t_loop_debut = 0;
unsigned long t_loop_fin = 0;
unsigned long t_loop_delai = 0;