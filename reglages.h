

/********************************************************
 * Constantes
 ********************************************************/
#define TEMPO_CYCLE   60      // unite : second
#define TEMPO_VENTILO 50
#define TEMPO_MOTEUR   10

#define TEMPERATURE_DEMARRAGE  25
#define TEMPERATURE_ARRET      33

/********************************************************
 * Brochage carte
 ********************************************************/

#define RELAIS_MOTEUR     7
#define RELAIS_MOTEUR_INV 8
#define RELAIS_VENTILO    9

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

