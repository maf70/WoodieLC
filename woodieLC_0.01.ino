

#include "reglages.h"
#include "init.h"


long temperature_eau = -300;  // Temperature output variable
long temperature_K   = -300;  // Temperature output variable

DateTime_t now;
moteur MoteurVis(RELAIS_MOTEUR, RELAIS_MOTEUR_INV);
actionneur Ventilo(RELAIS_VENTILO);

int D220sectState = 0;
int D220secuState = 0;

int OpticState = 0;
int OpticCount = 0;


long t=0;
int v=0;  // clignotement led
int etat = 0;   //etat repos

void interruptC1()
{
  OpticCount += 1 ;
}


#define NUMFLAKES     10 // Number of snowflakes in the animation example

#define LOGO_HEIGHT   16
#define LOGO_WIDTH    16
static const unsigned char PROGMEM logo_bmp[] =
{ B00000000, B11000000,
  B00000001, B11000000,
  B00000001, B11000000,
  B00000011, B11100000,
  B11110011, B11100000,
  B11111110, B11111000,
  B01111110, B11111111,
  B00110011, B10011111,
  B00011111, B11111100,
  B00001101, B01110000,
  B00011011, B10100000,
  B00111111, B11100000,
  B00111111, B11110000,
  B01111100, B11110000,
  B01110000, B01110000,
  B00000000, B00110000 }; 

void setup() {
  Serial.begin(115200);
  
  /* Initialise le port I2C */
  Wire.begin();

  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C); // Address 0x3D for 128x64

  // Show initial display buffer contents on the screen --
  // the library initializes this with an Adafruit splash screen.
  display.display();
  delay(200); // Pause for 2 seconds

  // Clear the buffer
  display.clearDisplay();

  // Draw a single pixel in white
  display.drawPixel(10, 10, WHITE);
  display.display();
  delay(1000); // Pause for 2 seconds
  
  display.clearDisplay();
  
  display.setTextSize(2);      // Normal 1:1 pixel scale
  display.setTextColor(WHITE); // Draw white text
  display.setCursor(0, 0);     // Start at top-left corner
  display.cp437(true);         // Use full 256 char 'Code Page 437' font  
  
  // Not all the characters will fit on the display. This is normal.
  // Library will draw what it can and the rest will be clipped.
  for(int16_t i=0; i<38; i++) {
    if(i == '\n') display.write(' ');
    else          display.write(i+64);
  }

  display.display();
  delay(2000); // Pause for 2 seconds
  
  digitalWrite(LED_BUILTIN, v & 0x01); 
  mesure_timings("display pret : ");
  
    Serial.println(ARDUINO);
    Serial.println(ARDUINO);

/*    Vérifie si le module RTC est initialisé */
  if (read_current_datetime(&now)) 
  {
    Serial.println(F("Mise à l'heure"));
    
    // Reconfiguration avec une date et heure en dur (pour l'exemple)
    now.seconds = 0;
    now.minutes = 12;
    now.hours = 21; // 12h 0min 0sec
    now.is_pm = 0; 
    now.day_of_week = 2;
    now.days = 6; 
    now.months = 11;
    now.year = 18; // 1 dec 2016
    adjust_current_datetime(&now);
  }
  v = 0;
  mesure_timings("RTC init : ");
  
  // Start up the library
 sensors.begin();
  mesure_timings("Sensor init : ");

  //Serial.println(  sensors.getDeviceCount() );
  sensors.setResolution(9);
  mesure_timings("Sensor set res : ");

  pinMode(D220_SECTEUR, INPUT_PULLUP);
  pinMode(D220_SECURITE, INPUT_PULLUP);
    
  pinMode(OPTICAL_1, INPUT);
  attachInterrupt(0, interruptC1, FALLING);
  mesure_timings("interruption : ");
  
  //display.print("D");
 // display.display();
  
  mesure_timings("fin init : ");

  display.clearDisplay();
  display.display();
}

void loop() {
  t_loop_debut = millis();
  
  display.clearDisplay();
  //display.display();
  //display.setTextSize(1);      // Normal 1:1 pixel scale
  //display.setTextColor(WHITE); // Draw white text
  display.setCursor(0, 0);     // Start at top-left corner
  //display.cp437(true);         // Use full 256 char 'Code Page 437' font  
  
  mesure_timings("############################ : ");
  digitalWrite(LED_BUILTIN, v & 0x01); 
  delay(10);
  
  /* Lit la date et heure courante */
  if (read_current_datetime(&now)) {
    Serial.println(F("L'horloge du module RTC n'est pas active !"));
  }
  else {
    Serial.println(F("L'horloge du module RTC OK!"));
  }
  mesure_timings("date lue : ");
  
  //display.clearDisplay();
  //display.setCursor(0, 0);     // Start at top-left corner
  //display.print(F("A"));
/*  delay(10);
  display.clearDisplay();
  display.display();
  delay(10);*/
  
  
  // call sensors.requestTemperatures() to issue a global temperature
  // request to all devices on the bus
  //Serial.print(" Requesting temperatures...");
  sensors.requestTemperatures(); // Send the command to get temperatures
  mesure_timings("Sonde db180 lue : ");

  // You can have more than one IC on the same bus. 
  // 0 refers to the first IC on the wire
  temperature_eau = (int)sensors.getTempCByIndex(0);

  // Read thermocouple K
  // Read the temp from the MAX6675
  temperature_K = (int)temp.read_temp();
  mesure_timings("max6675 lue : ");
  
  // Not all the characters will fit on the display. This is normal.
  // Library will draw what it can and the rest will be clipped.
  
  display.print(now.days);
  display.print(F("/"));
  display.print(now.months);
  display.print(F(" "));
  display.print(now.hours);
  display.print(F(":"));
  display.println(now.minutes);
     display.print(temperature_eau);
     display.print(F(" "));
     display.println(temperature_K);
/*  for(int16_t i=0; i<t; i++) {
    if(i == '\n') display.write(' ');
    else          display.write(i+32);
  }*/
  display.display();
  
  // read the state of 220v detection
  D220sectState = digitalRead(D220_SECTEUR);
  D220secuState = digitalRead(D220_SECURITE);
  
  mesure_timings("d220 lus : ");
  
   if ( t == 0 ){
    if (etat == 0){   // REPOS
     if (temperature_eau <= TEMPERATURE_DEMARRAGE) {
      MoteurVis.demarre(TEMPO_MOTEUR);
      Ventilo.demarre(TEMPO_VENTILO);
      etat = 1;
     }
    }
   else {
      if (etat == 1) { // CHAUFFE
        if (temperature_eau <= TEMPERATURE_ARRET) {
          MoteurVis.demarre(TEMPO_MOTEUR);
          Ventilo.demarre(TEMPO_VENTILO);
        }
        else {
          etat = 0;
        }
        
      }
//  mesure_timings("CHAUFFE : ");

    }
   }
   if ( t > 0 && etat == 0){
    if (temperature_eau <= TEMPERATURE_DEMARRAGE) {
      t = -1;  // Nouveau cycle a la prochaine boucle
      etat = 0;
    }
  }
  
  
  Ventilo.tic(1);
  MoteurVis.tic(1, OpticCount);
  OpticCount = 0;
 // mesure_timings("Gestion chaudiere : ");
 
 // display.display();
 delay(1);
  mesure_timings("lcd refresh : ");
 delay(1);
  
  t_loop_fin = millis();
  t_loop_delai = t_loop_fin - t_loop_debut;

  
  Serial.println (t_loop_delai);
  if (t_loop_delai >= 1000) t_loop_delai = 900;
  Serial.println (t_loop_delai);
  
  /* Rafraichissement une fois par seconde */ 
  delay(1000-t_loop_delai); 
  //delay(0); 


  t += 1;

  if (t >= TEMPO_CYCLE ) {
    t = 0; 
  }
  
}


