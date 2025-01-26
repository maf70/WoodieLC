
/* Dépendances */
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_SSD1306.h>


Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);


#include "moteur.h"
#include "DS1307.h"

#include <OneWire.h>
#include <DallasTemperature.h>

#include <MAX6675.h>

#include "debug.h"


// Setup a oneWire instance to communicate with any OneWire devices 
// (not just Maxim/Dallas temperature ICs)
OneWire oneWire(ONE_WIRE_BUS);
 
// Pass our oneWire reference to Dallas Temperature.
DallasTemperature sensors(&oneWire);


// initialize the Thermocouple
//Adafruit_MAX31855 thermocouple(MAXCLK, MAXCS, MAXDO);
// Initialize the MAX6675 Library for our chip
MAX6675 temp(SONDEK_CS,SONDEK_SO,SONDEK_SCK,SONDEK_UNIT);


// Variables pour compenser le temps de cycle
unsigned long t_loop_debut = 0;
unsigned long t_loop_fin = 0;
unsigned long t_loop_delai = 0;


