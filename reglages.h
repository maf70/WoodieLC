

/********************************************************
 * Constantes
 ********************************************************/

// DEFAULT values
#define TEMPERATURE_DEMARRAGE  30
#define TEMPERATURE_ARRET      33

#define TEMPO_CYCLE   60      // unite : second
#define TEMPO_VENTILO 50
#define TEMPO_MOTEUR   10

#define MOTEUR_DELAI_INVERSION   2
#define MOTEUR_DUREE_INVERSION   2
#define MOTEUR_VITESSE_MIN       5
#define MOTEUR_BLOCAGE_MAX       3

#define TEMP_SECU               27
#define TEMP_VIS                26
#define PROBE_SELECT             1

// MIN / MAX values
#define TEMPERATURE_DEMARRAGE_MIN  28
#define TEMPERATURE_DEMARRAGE_MAX  65
#define TEMPERATURE_ARRET_MIN      33
#define TEMPERATURE_ARRET_MAX      66

#define TEMPO_CYCLE_MIN            30
#define TEMPO_CYCLE_MAX           254
#define TEMPO_VENTILO_MIN           2
#define TEMPO_VENTILO_MAX         254
#define TEMPO_MOTOR_MIN             2
#define TEMPO_MOTOR_MAX           100
#define TEMPO_MOTOR_I_MIN           1
#define TEMPO_MOTOR_I_MAX           5

#define COUNT_MOTOR_C_MIN           5
#define COUNT_MOTOR_C_MAX          30
#define COUNT_MOTOR_B_MIN           1
#define COUNT_MOTOR_B_MAX          15

#define TEMP_SECU_MIN              25
#define TEMP_SECU_MAX              50
#define TEMP_VIS_MIN               25
#define TEMP_VIS_MAX               70

#define PROBE_SELECT_MIN            1
#define PROBE_SELECT_MAX            2

// EEPROM storage address
#define EEPROM_TEMPERATURE_DEMARRAGE   0
#define EEPROM_TEMPERATURE_ARRET       1

#define EEPROM_TEMPO_CYCLE             2
#define EEPROM_TEMPO_VENTILO           3
#define EEPROM_TEMPO_MOTEUR            4

#define EEPROM_MOTEUR_DUREE_INVERSION  5
#define EEPROM_MOTEUR_VITESSE_MIN      6
#define EEPROM_MOTEUR_BLOCAGE_MAX      7

#define EEPROM_TEMP_SECU               8
#define EEPROM_TEMP_VIS                9
#define EEPROM_PROBE_SELECT           10

/********************************************************
 * Defines
 ********************************************************/

// Boiler state
#define ETAT_REPOS   0
#define ETAT_CHAUFFE 1

// Boiler mode
#define MODE_ON      1
#define MODE_STOP    2
#define MODE_REGLAGE 3
#define MODE_STATS   4
#define MODE_VERSION 5
#define MODE_BLOCAGE 6

#define BUTTON_VALID   6
#define BUTTON_DISABLE 20


// Setting state
#define REGLAGE_NONE          0
#define REGLAGE_INIT          1
#define REGLAGE_TEMP_START    2
#define REGLAGE_TEMP_STOP     3
#define REGLAGE_CYCLE         4
#define REGLAGE_VENTILO       5         //FAN
#define REGLAGE_MOTOR         6
#define REGLAGE_MOTOR_INV     7
#define REGLAGE_MOTOR_COUNT   8
#define REGLAGE_MOTOR_BLOCK   9
#define REGLAGE_TEMP_SECU    10
#define REGLAGE_TEMP_VIS     11
#define REGLAGE_PROBE_SELECT 12
#define REGLAGE_END          13

#define ERROR_1_PROBE     1
#define ERROR_0_PROBE     2
#define ERROR_X_PROBE     3
#define ERROR_VIS         4
#define ERROR_TEMP_SECU   5
#define ERROR_TEMP_VIS    6

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

#define B_MENU    A0        // S1
#define B_OK      A1        // S2
#define B_UP      A3        // S4
#define B_DOWN    A2        // S3

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
#define OLED_RESET     4 // Reset pin # (or -1 if sharing Arduino reset pin)

// Data wire is plugged into pin D4 on the Arduino
#define ONE_WIRE_BUS 4
