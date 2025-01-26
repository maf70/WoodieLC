#ifndef DS1307_H
#define DS1307_H

/* Rétro-compatibilité avec Arduino 1.x et antérieur */
/*#if ARDUINO >= 100
#define Wire_write(x) Wire.write(x)
#define Wire_read() Wire.read()
#else
#define Wire_write(x) Wire.send(x)
#define Wire_read() Wire.receive()
#endif*/

/** Adresse I2C du module RTC DS1307 */
const uint8_t DS1307_ADDRESS = 0x68;

/** Adresse du registre de contrôle du module RTC DS1307 */
const uint8_t DS1307_CTRL_REGISTER = 0x07;

/** Adresse et taille de la NVRAM du module RTC DS1307 */
const uint8_t DS1307_NVRAM_BASE = 0x08;
const uint8_t DS1307_NVRAM_SIZE = 56;


/** Structure contenant les informations de date et heure en provenance ou à destination du module RTC */
typedef struct {
  uint8_t seconds; /**!< Secondes 00 - 59 */
  uint8_t minutes; /**!< Minutes 00 - 59 */
  uint8_t hours;  /**!< Heures 00 - 23 (format 24h), 01 - 12 (format 12h) */
  uint8_t is_pm; /**!< Vaut 1 si l'heure est en format 12h et qu'il est l'aprés midi, sinon 0 */
  uint8_t day_of_week;  /**!< Jour de la semaine 01 - 07, 1 = lundi, 2 = mardi, etc.  */
  uint8_t days; /**!< Jours 01 - 31 */
  uint8_t months;  /**!< Mois 01 - 12 */
  uint8_t year;  /**!< Année au format yy (exemple : 16 = 2016) */
} DateTime_t;


/** Mode de fonctionnement pour la broche SQW */
typedef enum {
  SQW_1_HZ = 0, /**!< Signal à 1Hz sur la broche SQW */
  SQW_4096_HZ,  /**!< Signal à 4096Hz sur la broche SQW */
  SQW_8192_HZ,  /**!< Signal à 8192Hz sur la broche SQW */
  SQW_32768_HZ, /**!< Signal à 32768Hz sur la broche SQW */
  SQW_DC /**!< Broche SQW toujours à LOW ou HIGH */
} DS1307_Mode_t;

/** Fonction de conversion BCD -> decimal */
byte bcd_to_decimal(byte bcd) {
  return (bcd / 16 * 10) + (bcd % 16); 
}

/** Fonction de conversion decimal -> BCD */
byte decimal_to_bcd(byte decimal) {
  return (decimal / 10 * 16) + (decimal % 10);
}
/** 
 * Fonction récupérant l'heure et la date courante à partir du module RTC.
 * Place les valeurs lues dans la structure passée en argument (par pointeur).
 * N.B. Retourne 1 si le module RTC est arrêté (plus de batterie, horloge arrêtée manuellement, etc.), 0 le reste du temps.
 */
byte read_current_datetime(DateTime_t *datetime) {
  
  /* Début de la transaction I2C */
  Wire.beginTransmission(DS1307_ADDRESS);
  Wire.write((byte) 0); // Lecture mémoire à l'adresse 0x00
  Wire.endTransmission(); // Fin de la transaction I2C
 
  /* Lit 7 octets depuis la mémoire du module RTC */
  Wire.requestFrom(DS1307_ADDRESS, (byte) 7);
  byte raw_seconds = Wire.read();
  datetime->seconds = bcd_to_decimal(raw_seconds);
  datetime->minutes = bcd_to_decimal(Wire.read());
  byte raw_hours = Wire.read();
  if (raw_hours & 64) { // Format 12h
    datetime->hours = bcd_to_decimal(raw_hours & 31);
    datetime->is_pm = raw_hours & 32;
  } else { // Format 24h
    datetime->hours = bcd_to_decimal(raw_hours & 63);
    datetime->is_pm = 0;
  }
  datetime->day_of_week = bcd_to_decimal(Wire.read());
  datetime->days = bcd_to_decimal(Wire.read());
  datetime->months = bcd_to_decimal(Wire.read());
  datetime->year = bcd_to_decimal(Wire.read());
  
  /* Si le bit 7 des secondes == 1 : le module RTC est arrêté */
  return raw_seconds & 128;
}


/** 
 * Fonction ajustant l'heure et la date courante du module RTC à partir des informations fournies.
 * N.B. Redémarre l'horloge du module RTC si nécessaire.
 */
void adjust_current_datetime(DateTime_t *datetime) {
  
  /* Début de la transaction I2C */
  Wire.beginTransmission(DS1307_ADDRESS);
  Wire.write((byte) 0); // Ecriture mémoire à l'adresse 0x00
  Wire.write(decimal_to_bcd(datetime->seconds) & 127); // CH = 0
  Wire.write(decimal_to_bcd(datetime->minutes));
  Wire.write(decimal_to_bcd(datetime->hours) & 63); // Mode 24h
  Wire.write(decimal_to_bcd(datetime->day_of_week));
  Wire.write(decimal_to_bcd(datetime->days));
  Wire.write(decimal_to_bcd(datetime->months));
  Wire.write(decimal_to_bcd(datetime->year));
  Wire.endTransmission(); // Fin de transaction I2C
}


#endif /* DS1307_H */
