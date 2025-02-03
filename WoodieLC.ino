



#include "reglages.h"
#include "init.h"

u8 temperature_demarrage = TEMPERATURE_DEMARRAGE;
u8 temperature_arret     = TEMPERATURE_ARRET;

int tempo_cycle   = TEMPO_CYCLE;
int tempo_ventilo = TEMPO_VENTILO;
int tempo_moteur  = TEMPO_MOTEUR;

int moteur_delai_inversion = MOTEUR_DELAI_INVERSION;
int moteur_duree_inversion = MOTEUR_DUREE_INVERSION;
u8 moteur_vitesse_min     = MOTEUR_VITESSE_MIN;
u8 moteur_blocage_max     = MOTEUR_BLOCAGE_MAX;

u8 B_disable = 0;

u8 BOK_cpt   = 0;
u8 BMENU_cpt = 0;
u8 BUP_cpt   = 0;
u8 BDOWN_cpt = 0;

u8 BOK_st   = 0;
u8 BMENU_st = 0;
u8 BUP_st   = 0;
u8 BDOWN_st = 0;

void interruptC1()
{
  if ( (millis() - OpticStamp) > 20 )
  {
    OpticCount += 1 ;
    OpticStamp = millis();
  }
}

ISR( TIMER2_OVF_vect )
{
  if (B_disable > 0) {
    B_disable--;
    return;
  }
  
  if ( BMENU_st + BOK_st + BUP_st + BDOWN_st < 1) {

    if (digitalRead(B_MENU) == 0)
    {
      if( ++BMENU_cpt > BUTTON_VALID) { BMENU_st = 1; BMENU_cpt = 0;}
      BOK_cpt   = 0;
      BUP_cpt   = 0;
      BDOWN_cpt = 0;
    }
    else if (digitalRead(B_OK) == 0)
    {
      BMENU_cpt = 0;
      if( ++BOK_cpt > BUTTON_VALID) { BOK_st = 1; BOK_cpt = 0;}
      BUP_cpt   = 0;
      BDOWN_cpt = 0;
    }
    else if (digitalRead(B_UP) == 0)
    {
      BMENU_cpt = 0;
      BOK_cpt   = 0;
      if( ++BUP_cpt > BUTTON_VALID) { BUP_st = 1; BUP_cpt = 0;}
      BDOWN_cpt = 0;
    }
    else if (digitalRead(B_DOWN) == 0)
    {
      BMENU_cpt = 0;
      BOK_cpt   = 0;
      BUP_cpt   = 0;
      if( ++BDOWN_cpt > BUTTON_VALID) { BDOWN_st = 1; BDOWN_cpt = 0;}
    }
    else
    {
      BMENU_cpt = 0;
      BOK_cpt   = 0;
      BUP_cpt   = 0;
      BDOWN_cpt = 0;
    }
   }
}

void clear_button(void) {
      B_disable = BUTTON_DISABLE;
      BMENU_st = 0;
      BOK_st   = 0;
      BUP_st   = 0;
      BDOWN_st = 0;
  
}

u8 blocage = 0;
int nb_cycle = 0;
int stat_cycle = 0;
int stat_repos = 0;
u8 reglage = REGLAGE_NONE;
int tmp_reglage  = -1;
u8 reglage_next = 0;

void restart_boiler(void) {
    // restart boiler !
    mode = MODE_ON;
    etat = ETAT_REPOS;
    reglage = REGLAGE_NONE;
    tmp_reglage = -1;
    t = 0;
    MoteurVis.debloque();
  }

int affiche_reglage(const char * message, int valeur){
  display.print(message);
  display.print(": ");
  display.print(valeur);
}

int affiche_Chaudiere(const char * message){
  display.println(message);
}

int affiche_arret(const char * message, int valeur){
  display.println("  ARRET");
}

void exit_reglage(void){
    mode = MODE_STATS;
}

#define SET_PARAM( param , value, message, pmin, pmax)  \
        if      (BUP_st   > 0 ) { tmp_reglage++; }\
        else if (BDOWN_st > 0 ) { tmp_reglage--; }\
        else if (BOK_st   > 0 ) { value = tmp_reglage; \
                                  reglage_next = 1; \
                                  }\
        else if (BMENU_st > 0 ) { exit_reglage(); }\
\
        if ( pmin > tmp_reglage ) { tmp_reglage = pmin; }\
        else if (pmax < tmp_reglage ) { tmp_reglage = pmax; }\
\
        affiche_reglage(message, tmp_reglage);
        
#define SET_NEXT_REGLAGE( pnext, snext)  \
        if (reglage_next == 1) { \
          tmp_reglage = pnext;\
          reglage = snext;\
          reglage_next = 0; \
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
  
  pinMode(B_MENU, INPUT_PULLUP);
  pinMode(B_OK,   INPUT_PULLUP);
  pinMode(B_UP,   INPUT_PULLUP);
  pinMode(B_DOWN, INPUT_PULLUP);
  
    //    TIMER 2 CONTROL
  TCCR2A = 0;                      // No pin change on timer compare
  TCCR2B = _BV(CS22) | _BV(CS21) | _BV(CS20);  // prescaler = clk / 1024
  TIMSK2 = _BV(TOIE2);             // overflow interrupt only

  mesure_timings("fin init : ");

  display.clearDisplay();
  display.display();

  MoteurVis.parametres( moteur_delai_inversion, moteur_duree_inversion, moteur_vitesse_min);

  // Start in boiler mode
  mode = MODE_ON;
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

  blocage = MoteurVis.getNB();
  // Read thermocouple K
  // Read the temp from the MAX6675
  //temperature_K = (int)temp.read_temp();
  //mesure_timings("max6675 lue : ");
  
  // Not all the characters will fit on the display. This is normal.
  // Library will draw what it can and the rest will be clipped.
  
  display.print(temperature_eau, DEC);
  display.print(F(" "));
  //display.println(temperature_K);
  display.print(OpticCount, DEC);
  display.print(F(" "));
  display.println(blocage, DEC);

  switch( mode ) {
  case MODE_ON:
  
    switch( etat ) {
    case ETAT_REPOS:
     if (temperature_eau <= temperature_demarrage) {
       MoteurVis.demarre(tempo_moteur);
       Ventilo.demarre(tempo_ventilo);
       etat = ETAT_CHAUFFE;
       stat_repos = t;
       t = 0;
       nb_cycle = 0;
     } else {
       t += 1;
     }
     display.println(t, DEC);
     break;
  
    case ETAT_CHAUFFE:
      if ( t == 0 ){
        if (temperature_eau <= temperature_arret) {
          MoteurVis.demarre(tempo_moteur);
          Ventilo.demarre(tempo_ventilo);
          nb_cycle++;
        }
        else {
          etat = ETAT_REPOS;
          stat_cycle = nb_cycle;
        }
      }

      if (blocage >= moteur_blocage_max) {
        mode = MODE_BLOCAGE;
      } else {
        Ventilo.tic(1);
        MoteurVis.tic(1, OpticCount);

        if (++t >= tempo_cycle ) {
          t = 0; 
        }
      }
      display.print(t, DEC);
      display.print(F(" "));
      display.println(nb_cycle, DEC);
    break;
    }
    if ( stat_cycle != 0) display.print(stat_cycle, DEC);
    display.print(" ");
    if ( stat_repos != 0) display.print(stat_repos, DEC);
  
  break;
  
  case MODE_BLOCAGE:
    Ventilo.arret();
    MoteurVis.arret();
    display.print(" BLOCAGE ");
    display.print(t);
    
    if (BOK_st > 0 ) {
      restart_boiler();
    }
  break;
  
  case MODE_STOP:
    display.println("  ARRET ");
    display.println("INV VENTIL");
    display.println("VIS  REGL.");
    
    if (BDOWN_st > 0 ) {
      MoteurVis.debloque();
      MoteurVis.demarre(tempo_moteur);
    } else if (BOK_st > 0 ) {
      Ventilo.demarre(tempo_ventilo);
    }

    Ventilo.tic(1);
    MoteurVis.tic(1, OpticCount);

  break;
  
  case MODE_REGLAGE:
    display.println(" REGLAGE");
    Ventilo.arret();
    MoteurVis.arret();

    switch ( reglage ) {
      case REGLAGE_INIT:
        reglage = REGLAGE_TEMP_START;
        tmp_reglage = temperature_demarrage;

      case REGLAGE_TEMP_START:
        SET_PARAM( TEMP_START , temperature_demarrage, "Start", TEMPERATURE_DEMARRAGE_MIN, TEMPERATURE_DEMARRAGE_MAX);
        SET_NEXT_REGLAGE (temperature_arret, REGLAGE_TEMP_STOP);
        break;

      case REGLAGE_TEMP_STOP:
        SET_PARAM( TEMP_ARRET , temperature_arret, "Stop", TEMPERATURE_ARRET_MIN, TEMPERATURE_ARRET_MAX);
        SET_NEXT_REGLAGE (tempo_cycle, REGLAGE_CYCLE);
        break;

      case REGLAGE_CYCLE:
        SET_PARAM( CYCLE , tempo_cycle, "Cycle", TEMPO_CYCLE_MIN, TEMPO_CYCLE_MAX);
        SET_NEXT_REGLAGE (tempo_ventilo, REGLAGE_VENTILO);
        break;

      case REGLAGE_VENTILO:
        SET_PARAM( VENTILO , tempo_ventilo, "Ventilo", TEMPO_VENTILO_MIN, tempo_cycle-1);
        SET_NEXT_REGLAGE (tempo_moteur, REGLAGE_MOTOR);
        break;

      case REGLAGE_MOTOR:
        SET_PARAM( MOTOR , tempo_moteur, "MOTEUR", TEMPO_MOTOR_MIN, tempo_cycle-1);
        SET_NEXT_REGLAGE (moteur_duree_inversion, REGLAGE_MOTOR_INV);
        break;

      case REGLAGE_MOTOR_INV:
        SET_PARAM( MOTOR INV , moteur_duree_inversion, "", TEMPO_MOTOR_I_MIN, TEMPO_MOTOR_I_MAX);
        SET_NEXT_REGLAGE (moteur_vitesse_min, REGLAGE_MOTOR_COUNT);
        break;

      case REGLAGE_MOTOR_COUNT:
        SET_PARAM( COUNTER , moteur_vitesse_min, "COMPTEUR", TEMPO_MOTOR_C_MIN, TEMPO_MOTOR_C_MAX);
        SET_NEXT_REGLAGE (moteur_blocage_max, REGLAGE_MOTOR_BLOCK);
        break;

      case REGLAGE_MOTOR_BLOCK:
        SET_PARAM( BLOCKING , moteur_blocage_max, "BLOCAGES", TEMPO_MOTOR_B_MIN, TEMPO_MOTOR_B_MAX);
        SET_NEXT_REGLAGE (-1, REGLAGE_END);
        break;

      case REGLAGE_END:
        restart_boiler();
        break;
    }
    clear_button();

  break;

  case MODE_STATS:
    display.println("  STATS ");
    display.println(millis()/1000);

    if (BOK_st > 0 ) {
      restart_boiler();
    }
  break;

  case MODE_VERSION:
    display.println("VERSION");
    display.println("03-Fev-25");
    display.println("526c78b");

    if (BOK_st > 0 ) {
      restart_boiler();
    }
  break;

  }
//  display.println(total_s++);
  
  if (BMENU_st > 0 ) {
    switch( mode ) {
    case MODE_ON:
    case MODE_BLOCAGE:
      mode = MODE_STOP;
      Ventilo.arret();
      MoteurVis.arret();
      break;
    case MODE_STOP:
      mode = MODE_REGLAGE;
      reglage = REGLAGE_INIT;
      break;
    case MODE_REGLAGE:
      mode = MODE_STATS;
      break;
    case MODE_STATS:
      mode = MODE_VERSION;
      break;
    case MODE_VERSION:
      restart_boiler();
      break;
    }  
  } 

  clear_button();
  
 // mesure_timings("Gestion chaudiere : ");
  //mesure_timings("lcd refresh : ");

  OpticCount = 0;
  display.display();
  
  t_loop_fin = millis();
  t_loop_delai = t_loop_fin - t_loop_debut;

  // protect against millis overflow
  if (t_loop_fin < t_loop_debut) t_loop_delai = 0;

  
  /* Rafraichissement une fois par seconde */ 
  delay(1000-t_loop_delai); 
  
}
