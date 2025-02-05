



#include "reglages.h"
#include "init.h"

#include <EEPROM.h>

u8 temperature_demarrage = TEMPERATURE_DEMARRAGE;
u8 temperature_arret     = TEMPERATURE_ARRET;

u8 tempo_cycle   = TEMPO_CYCLE;
u8 tempo_ventilo = TEMPO_VENTILO;
u8 tempo_moteur  = TEMPO_MOTEUR;

u8 moteur_delai_inversion = MOTEUR_DELAI_INVERSION;
u8 moteur_duree_inversion = MOTEUR_DUREE_INVERSION;
u8 moteur_vitesse_min     = MOTEUR_VITESSE_MIN;
u8 moteur_blocage_max     = MOTEUR_BLOCAGE_MAX;

u8 temperature_secu = TEMP_SECU;
u8 temperature_vis_max =  TEMP_VIS;
u8 memorise_temperature =  -2;
u8 probe_select =  PROBE_SELECT;
u8 probe_nb     =  0;
u8 error        =  0;

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
int stat_cycle_total = 0;
int stat_chauffe_total = 0;
u8 reglage = REGLAGE_NONE;
int tmp_reglage  = -1;
u8 reglage_next = 0;

void restart_boiler(u8 e) {
    // restart boiler !
    mode = MODE_ON;
    etat = e;
    reglage = REGLAGE_NONE;
    tmp_reglage = -1;
    t = 0;
    MoteurVis.parametres( moteur_delai_inversion, moteur_duree_inversion, moteur_vitesse_min);
    MoteurVis.debloque();
  }

int affiche_reglage(const char * message, int valeur){
  display.print(message);
  display.println(": ");
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

#define READ_REGLAGE( value, address, vmin, vmax, vdefaut)  \
        value = EEPROM.read(address);\
        if (value < vmin || value > vmax) { value = vdefaut;};

#define SAVE_REGLAGE( value, address)  \
        EEPROM.update(address, value);\

void setup() {
  //Serial.begin(115200);
  
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
  
  // Start up the library
  sensors.begin();

  //Serial.println(  sensors.getDeviceCount() );
  sensors.setResolution(9);
    
  pinMode(OPTICAL_1, INPUT);
  attachInterrupt(0, interruptC1, FALLING);
  
  pinMode(B_MENU, INPUT_PULLUP);
  pinMode(B_OK,   INPUT_PULLUP);
  pinMode(B_UP,   INPUT_PULLUP);
  pinMode(B_DOWN, INPUT_PULLUP);
  
    //    TIMER 2 CONTROL
  TCCR2A = 0;                      // No pin change on timer compare
  TCCR2B = _BV(CS22) | _BV(CS21) | _BV(CS20);  // prescaler = clk / 1024
  TIMSK2 = _BV(TOIE2);             // overflow interrupt only

  display.clearDisplay();
  display.display();

  READ_REGLAGE( temperature_demarrage , EEPROM_TEMPERATURE_DEMARRAGE , TEMPERATURE_DEMARRAGE_MIN, TEMPERATURE_DEMARRAGE_MAX, TEMPERATURE_DEMARRAGE );
  READ_REGLAGE( temperature_arret     , EEPROM_TEMPERATURE_ARRET     , TEMPERATURE_ARRET_MIN    , TEMPERATURE_ARRET_MAX    , TEMPERATURE_ARRET     );
  READ_REGLAGE( tempo_cycle           , EEPROM_TEMPO_CYCLE           , TEMPO_CYCLE_MIN          , TEMPO_CYCLE_MAX          , TEMPO_CYCLE           );
  READ_REGLAGE( tempo_ventilo         , EEPROM_TEMPO_VENTILO         , TEMPO_VENTILO_MIN        , TEMPO_VENTILO_MAX        , TEMPO_VENTILO         );
  READ_REGLAGE( tempo_moteur          , EEPROM_TEMPO_MOTEUR          , TEMPO_MOTOR_MIN          , TEMPO_MOTOR_MAX          , TEMPO_MOTEUR          );
  READ_REGLAGE( moteur_duree_inversion, EEPROM_MOTEUR_DUREE_INVERSION, TEMPO_MOTOR_I_MIN        , TEMPO_MOTOR_I_MAX        , MOTEUR_DUREE_INVERSION);
  READ_REGLAGE( moteur_vitesse_min    , EEPROM_MOTEUR_VITESSE_MIN    , COUNT_MOTOR_C_MIN        , COUNT_MOTOR_C_MAX        , MOTEUR_VITESSE_MIN    );
  READ_REGLAGE( moteur_blocage_max    , EEPROM_MOTEUR_BLOCAGE_MAX    , COUNT_MOTOR_B_MIN        , COUNT_MOTOR_B_MAX        , MOTEUR_BLOCAGE_MAX    );
  READ_REGLAGE( temperature_secu      , EEPROM_TEMP_SECU             , TEMP_SECU_MIN            , TEMP_SECU_MAX            , TEMP_SECU             );
  READ_REGLAGE( temperature_vis_max   , EEPROM_TEMP_VIS              , TEMP_VIS_MIN             , TEMP_VIS_MAX             , TEMP_VIS              );
  READ_REGLAGE( probe_select          , EEPROM_PROBE_SELECT          , PROBE_SELECT_MIN         , PROBE_SELECT_MAX         , PROBE_SELECT          );

  MoteurVis.parametres( moteur_delai_inversion, moteur_duree_inversion, moteur_vitesse_min);

  // Start in boiler mode
  mode = MODE_ON;
}


void loop() {
  t_loop_debut = millis();
  
  display.clearDisplay();
  display.setCursor(0, 0);     // Start at top-left corner

  led ^= 1;
  digitalWrite(LED_BUILTIN, led & 0x01);
  
  // You can have more than one IC on the same bus.
  // 0 refers to the first IC on the wire
  probe_nb = sensors.getDeviceCount();
  if ( probe_nb == 0 ) {
    mode = MODE_ERREUR;
    error = ERROR_0_PROBE;
    temperature_eau = -1;
    temperature_vis = -1;
  } else if ( probe_nb == 1 ) {
    mode = MODE_ERREUR;
    error = ERROR_1_PROBE;
    temperature_eau = (int)sensors.getTempCByIndex(0);
    temperature_vis = -1;
  } else if ( probe_nb > 2 ) {
    mode = MODE_ERREUR;
    error = ERROR_X_PROBE;
    temperature_eau = (int)sensors.getTempCByIndex(0);
    temperature_vis = (int)sensors.getTempCByIndex(1);
  }
  else {
    sensors.requestTemperatures(); // Send the command to get temperatures

    if ( probe_select == 1 ) {
      temperature_eau = (int)sensors.getTempCByIndex(0);
      temperature_vis = (int)sensors.getTempCByIndex(1);
    } else {
      temperature_eau = (int)sensors.getTempCByIndex(1);
      temperature_vis = (int)sensors.getTempCByIndex(0);
    }
  }

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
  display.print(blocage, DEC);
  display.print(F(" "));
  display.println(temperature_vis, DEC);

  switch( mode ) {
  case MODE_ON:

    // Security check
    if ( temperature_vis >= temperature_vis_max ) {
        mode = MODE_ERREUR;
        error = ERROR_TEMP_VIS;
        memorise_temperature = temperature_vis;
      };
    if ( temperature_eau <= temperature_secu && etat != ETAT_FORCE_CHAUFFE) {
        mode = MODE_ERREUR;
        error = ERROR_TEMP_SECU;
        memorise_temperature = temperature_eau;
      };
  
    switch( etat ) {
    case ETAT_REPOS:
     if (temperature_eau <= temperature_demarrage) {
       MoteurVis.demarre(tempo_moteur);
       Ventilo.demarre(tempo_ventilo);
       etat = ETAT_CHAUFFE;
       stat_chauffe_total++;
       stat_repos = t;
       t = 0;
       nb_cycle = 0;
     } else {
       t += 1;
     }
     display.println("  REPOS ");
     display.println(t, DEC);
     break;
  
    case ETAT_FORCE_CHAUFFE:
     display.println("DEMARRAGE");
    case ETAT_CHAUFFE:
     if ( etat != ETAT_FORCE_CHAUFFE) display.println(" CHAUFFE");
      if ( t == 0 ){
        if (temperature_eau <= temperature_arret) {
          MoteurVis.demarre(tempo_moteur);
          Ventilo.demarre(tempo_ventilo);
          nb_cycle++;
          stat_cycle_total++;
        }
        else {
          etat = ETAT_REPOS;
          stat_cycle = nb_cycle;
        }
      }

      if (blocage >= moteur_blocage_max) {
        mode = MODE_ERREUR;
        error = ERROR_VIS;
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
  
  case MODE_ERREUR:
    Ventilo.arret();
    MoteurVis.arret();
    display.println(" ERREUR ");
    switch ( error ) {
      case ERROR_1_PROBE:
        display.println("1 Sonde T");
        break;
      case ERROR_0_PROBE:
        display.println("0 Sonde T");
        break;
      case ERROR_X_PROBE:
        display.println("X Sonde T");
        break;
      case ERROR_VIS:
        display.println("Moteur");
        break;
      case ERROR_TEMP_SECU:
        display.print("T eau ");
        display.println(memorise_temperature, DEC);
        break;
      case ERROR_TEMP_VIS:
        display.print("T vis ");
        display.println(memorise_temperature, DEC);
        break;
      default:
        display.println("Inconnue");
    }
    display.print(t);
    
    if (BOK_st > 0 ) {
      if (ERROR_TEMP_SECU == error) restart_boiler(ETAT_FORCE_CHAUFFE);
      else restart_boiler(ETAT_REPOS);
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
        SET_PARAM( TEMP_START , temperature_demarrage, "T Start", TEMPERATURE_DEMARRAGE_MIN, TEMPERATURE_DEMARRAGE_MAX);
        SAVE_REGLAGE( temperature_demarrage, EEPROM_TEMPERATURE_DEMARRAGE);
        SET_NEXT_REGLAGE (temperature_arret, REGLAGE_TEMP_STOP);
        break;

      case REGLAGE_TEMP_STOP:
        SET_PARAM( TEMP_ARRET , temperature_arret, "T Stop", TEMPERATURE_ARRET_MIN, TEMPERATURE_ARRET_MAX);
        SAVE_REGLAGE( temperature_arret     , EEPROM_TEMPERATURE_ARRET);
        SET_NEXT_REGLAGE (tempo_cycle, REGLAGE_CYCLE);
        break;

      case REGLAGE_CYCLE:
        SET_PARAM( CYCLE , tempo_cycle, "Cycle", TEMPO_CYCLE_MIN, TEMPO_CYCLE_MAX);
        SAVE_REGLAGE( tempo_cycle           , EEPROM_TEMPO_CYCLE);
        SET_NEXT_REGLAGE (tempo_ventilo, REGLAGE_VENTILO);
        break;

      case REGLAGE_VENTILO:
        SET_PARAM( VENTILO , tempo_ventilo, "Ventilo", TEMPO_VENTILO_MIN, tempo_cycle-1);
        SAVE_REGLAGE( tempo_ventilo         , EEPROM_TEMPO_VENTILO);
        SET_NEXT_REGLAGE (tempo_moteur, REGLAGE_MOTOR);
        break;

      case REGLAGE_MOTOR:
        SET_PARAM( MOTOR , tempo_moteur, "Moteur", TEMPO_MOTOR_MIN, tempo_cycle-1);
        SAVE_REGLAGE( tempo_moteur          , EEPROM_TEMPO_MOTEUR);
        SET_NEXT_REGLAGE (moteur_duree_inversion, REGLAGE_MOTOR_INV);
        break;

      case REGLAGE_MOTOR_INV:
        SET_PARAM( MOTOR INV , moteur_duree_inversion, "Invers.", TEMPO_MOTOR_I_MIN, TEMPO_MOTOR_I_MAX);
        SAVE_REGLAGE( moteur_duree_inversion, EEPROM_MOTEUR_DUREE_INVERSION);
        SET_NEXT_REGLAGE (moteur_vitesse_min, REGLAGE_MOTOR_COUNT);
        break;

      case REGLAGE_MOTOR_COUNT:
        SET_PARAM( COUNTER , moteur_vitesse_min, "Compteur", COUNT_MOTOR_C_MIN, COUNT_MOTOR_C_MAX);
        SAVE_REGLAGE( moteur_vitesse_min    , EEPROM_MOTEUR_VITESSE_MIN);
        SET_NEXT_REGLAGE (moteur_blocage_max, REGLAGE_MOTOR_BLOCK);
        break;

      case REGLAGE_MOTOR_BLOCK:
        SET_PARAM( BLOCKING , moteur_blocage_max, "Blocages", COUNT_MOTOR_B_MIN, COUNT_MOTOR_B_MAX);
        SAVE_REGLAGE( moteur_blocage_max, EEPROM_MOTEUR_BLOCAGE_MAX);
        SET_NEXT_REGLAGE (temperature_secu, REGLAGE_TEMP_SECU);
        break;

      case REGLAGE_TEMP_SECU:
        SET_PARAM( TEMP_SECU , temperature_secu, "T Secu", TEMP_SECU_MIN, TEMP_SECU_MAX);
        SAVE_REGLAGE( temperature_secu      , EEPROM_TEMP_SECU);
        SET_NEXT_REGLAGE (temperature_vis_max, REGLAGE_TEMP_VIS);
        break;

      case REGLAGE_TEMP_VIS:
        SET_PARAM( TEMP_VIS, temperature_vis_max, "T Vis", TEMP_VIS_MIN, TEMP_VIS_MAX);
        SAVE_REGLAGE( temperature_vis_max   , EEPROM_TEMP_VIS);
        SET_NEXT_REGLAGE ( probe_select, REGLAGE_PROBE_SELECT);
        break;

      case REGLAGE_PROBE_SELECT:
        SET_PARAM( PROBE , probe_select, "Sonde T", PROBE_SELECT_MIN, PROBE_SELECT_MAX);
        SAVE_REGLAGE( probe_select          , EEPROM_PROBE_SELECT);
        SET_NEXT_REGLAGE ( -1, REGLAGE_END);
        break;

      case REGLAGE_END:
        restart_boiler(ETAT_REPOS);
        break;
    }
    clear_button();

  break;

  case MODE_STATS:
  {
    long int uptime=millis()/1000;

    display.println("  STATS ");
    // up time since power on
    if ( uptime / 86400 >= 1 ) {
      display.print(uptime/86400);
      display.print("J ");
      display.print(uptime%86400/3600);
      display.println("h");
    } else if ( uptime / 3600 >= 1 ) {
      display.print(uptime/3600);
      display.print("h ");
      display.print((uptime%3600)/60);
      display.println("mn");
    } else if ( uptime / 60 >= 1 ) {
      display.print(uptime/60);
      display.print("mn ");
      display.print(uptime%60);
      display.println("s");
    } else {
      display.print(uptime%60);
      display.println("s");
    }
  }
    display.print(stat_cycle_total);
    display.print(" ");
    display.println(stat_chauffe_total);

    if (BOK_st > 0 ) {
      restart_boiler(ETAT_REPOS);
    }
  break;

  case MODE_VERSION:
    display.println(" VERSION");
    display.println("05-Fev-25");
    display.println(" a85e463");

    if (BOK_st > 0 ) {
      restart_boiler(ETAT_REPOS);
    }
  break;

  }
//  display.println(total_s++);
  
  if (BMENU_st > 0 ) {
    switch( mode ) {
    case MODE_ON:
    case MODE_ERREUR:
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
      restart_boiler(ETAT_REPOS);
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
