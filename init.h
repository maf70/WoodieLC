
/* Dépendances */
//#include <SPI.h>
#include <Wire.h>
#include <Adafruit_SSD1306.h>

#include <OneWire.h>
#include <DallasTemperature.h>
//#include <MAX6675.h>

#include "debug.h"

#include "moteur.h"

u8 temperature_eau = 0;  // Temperature output variable
//long temperature_K   = -300;  // Temperature output variable

moteur MoteurVis(RELAIS_MOTEUR, RELAIS_MOTEUR_INV);
actionneur Ventilo(RELAIS_VENTILO);

int OpticCount = 0;
unsigned long OpticStamp =0;

int t=0;
u8 led=0;  // clignotement led
u8 etat = ETAT_REPOS;   // etat repos
u8 mode = MODE_ON;  // Start boiler

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
