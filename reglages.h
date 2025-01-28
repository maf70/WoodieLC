

/********************************************************
 * Constantes
 ********************************************************/

#define TEMPERATURE_DEMARRAGE  30
#define TEMPERATURE_ARRET      35

#define TEMPO_CYCLE   60      // unite : second
#define TEMPO_VENTILO 50
#define TEMPO_MOTEUR   10

#define MOTEUR_DELAI_INVERSION   2
#define MOTEUR_DUREE_INVERSION   2
#define MOTEUR_VITESSE_MIN       5
#define MOTEUR_BLOCAGE_MAX       3


/********************************************************
 * Defines
 ********************************************************/

#define ETAT_REPOS 0
#define ETAT_CHAUFFE 1
#define ETAT_BLOCAGE 2
#define ETAT_REGLAGE 3

/********************************************************
 * Brochage carte
 ********************************************************/

#define RELAIS_MOTEUR     8
#define RELAIS_MOTEUR_INV 9
#define RELAIS_VENTILO    7

#define D220_SECTEUR  5
#define D220_SECURITE 6

#define OPTICAL_1 2

#define SONDEK_CS   11           // CS pin on MAX6675
#define SONDEK_SO   10           // SO pin of MAX6675
#define SONDEK_SCK  12           // SCK pin of MAX6675
#define SONDEK_UNIT  1           // Units to readout temp (0 = raw, 1 = ˚C, 2 = ˚F)


#define I2C_EXT_EEPROM 0x50
#define I2C_EXT_SIZE   32768

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
#define OLED_RESET     4 // Reset pin # (or -1 if sharing Arduino reset pin)

// Data wire is plugged into pin D4 on the Arduino
#define ONE_WIRE_BUS 4
