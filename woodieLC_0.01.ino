



#include "reglages.h"
#include "init.h"

void interruptC1()
{
  if ( (millis() - OpticStamp) > 20 )
  {
    OpticCount += 1 ;
    OpticStamp = millis();
  }
}

void setup() {
  Serial.begin(115200);
  
  /* Initialise le port I2C */
  Wire.begin();

  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C); // Address 0x3D for 128x64

  // Show initial display buffer contents on the screen --
  // the library initializes this with an Adafruit splash screen.
  display.display();
  delay(1000); // Pause for 1 seconds
  
  display.clearDisplay();
  
  display.setTextSize(2);      // Normal 1:1 pixel scale
  display.setTextColor(WHITE); // Draw white text
  display.setCursor(0, 0);     // Start at top-left corner
  display.cp437(true);         // Use full 256 char 'Code Page 437' font  

  mesure_timings("display pret : ");
  
  // Start up the library
  sensors.begin();
  mesure_timings("Sensor init : ");

  //Serial.println(  sensors.getDeviceCount() );
  sensors.setResolution(9);
  mesure_timings("Sensor set res : ");
    
  pinMode(OPTICAL_1, INPUT);
  attachInterrupt(0, interruptC1, FALLING);
  mesure_timings("interruption : ");
  
  mesure_timings("fin init : ");

  display.clearDisplay();
  display.display();
}

void loop() {
  t_loop_debut = millis();
  
  display.clearDisplay();
  display.setCursor(0, 0);     // Start at top-left corner
  
  mesure_timings("############################ : ");
  led ^= 1;
  digitalWrite(LED_BUILTIN, led & 0x01);
  
  // call sensors.requestTemperatures() to issue a global temperature
  // request to all devices on the bus
  //Serial.print(" Requesting temperatures...");
  sensors.requestTemperatures(); // Send the command to get temperatures
  mesure_timings("Sonde db180 lue : ");

  // You can have more than one IC on the same bus. 
  // 0 refers to the first IC on the wire
  temperature_eau = (int)sensors.getTempCByIndex(0);
  Serial.print (temperature_eau);

  // Read thermocouple K
  // Read the temp from the MAX6675
  //temperature_K = (int)temp.read_temp();
  //mesure_timings("max6675 lue : ");
  
  // Not all the characters will fit on the display. This is normal.
  // Library will draw what it can and the rest will be clipped.
  
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
 // mesure_timings("Gestion chaudiere : ");
 
  display.println(temperature_eau);
  //display.println(temperature_K);
  display.print(OpticCount);
  display.print(" ");
  display.println(MoteurVis.getNB());
  display.println(total_s++);
  display.display();

  //mesure_timings("lcd refresh : ");

  OpticCount = 0;
  
  t_loop_fin = millis();
  t_loop_delai = t_loop_fin - t_loop_debut;

  //if (t_loop_delai >= 1000) t_loop_delai = 900;
  
  /* Rafraichissement une fois par seconde */ 
  delay(1000-t_loop_delai); 

  t += 1;

  if (t >= TEMPO_CYCLE ) {
    t = 0; 
  }
  
}
