#include "reglages.h"
#include "init.h"

#include <EEPROM.h>

// CORRECTION : volatile car ecrits dans l'ISR du Timer2, lus dans loop()
volatile u8 B_disable = 0;

// >>> NOUVELLES VARIABLES POUR DEBOUNCE <<<
volatile u8 BOK_st   = 0;
volatile u8 BMENU_st = 0;
volatile u8 BUP_st   = 0;
volatile u8 BDOWN_st = 0;
volatile uint8_t B_last[4] = {HIGH, HIGH, HIGH, HIGH};    // États précédents (0:MENU, 1:OK, 2:UP, 3:DOWN)
volatile uint8_t B_debounce_cpt[4] = {0};                 // Compteurs de stabilité
#define DEBOUNCE_THRESHOLD 3                               // 3 cycles = ~48ms (1 appel ISR = ~16ms avec prescaler 1024)

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

  // Lit les états actuels des boutons (avec pull-up: LOW = appuyé)
  uint8_t B_current[4] = {
    digitalRead(B_MENU),
    digitalRead(B_OK),
    digitalRead(B_UP),
    digitalRead(B_DOWN)
  };

  // Gère chaque bouton avec debounce
  for (uint8_t i = 0; i < 4; i++) {
    // Si l'état a changé, réinitialise le compteur de stabilité
    if (B_current[i] != B_last[i]) {
      B_debounce_cpt[i] = 0;
      B_last[i] = B_current[i];
    }
    // Sinon, incrémente le compteur
    else if (B_debounce_cpt[i] < DEBOUNCE_THRESHOLD) {
      B_debounce_cpt[i]++;
    }

    // Détection d'un appui VALIDE (front descendant + stabilité)
    if (B_current[i] == LOW && B_debounce_cpt[i] >= DEBOUNCE_THRESHOLD) {
      switch (i) {
        case 0: if (BMENU_st == 0) { BMENU_st = 1; B_disable = BUTTON_DISABLE; } break;
        case 1: if (BOK_st   == 0) { BOK_st   = 1; B_disable = BUTTON_DISABLE; } break;
        case 2: if (BUP_st   == 0) { BUP_st   = 1; B_disable = BUTTON_DISABLE; } break;
        case 3: if (BDOWN_st == 0) { BDOWN_st = 1; B_disable = BUTTON_DISABLE; } break;
      }
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

void restart_boiler(u8 e) {
    // restart boiler !
    mode = MODE_ON;
    etat = e;
    reglage = REGLAGE_NONE;
    tic = 0;
    tmp_reglage = -1;
    version_display = VERSION_TEMPO;  // Stay in version mode if no power on state
    boiler_screen = BOILER_SCREEN_DEFAULT;
    MoteurVis.parametres( moteur_delai_inversion, moteur_duree_inversion, moteur_vitesse_min);
    MoteurVis.debloque();
    temperature_eau = -127;
    temperature_vis = -127;
    temperature_error_eau = 0;
    temperature_error_vis = 0;
    temperature_error_same = 0;
    temperature_error_eau_total = 0;
    temperature_error_vis_total = 0;
    temperature_error_same_total = 0;
    sonde_error_0 = 0;
    sonde_error_0_total = 0;
    sonde_error_1 = 0;
    sonde_error_1_total = 0;
    sonde_error_X = 0;
    sonde_error_X_total = 0;
}

 
void affiche_header(void){ 
  //if (temperature_eau < -20) {
    display.print(temperature_eau, DEC);
  //} else {
  //  display.print("#");
  //}
  display.print(F(" "));
  //display.println(temperature_K);
  display.print(optic_seconde, DEC);
  display.print(F(" "));
  display.print(blocage, DEC);
  display.print(F(" "));
  //if (temperature_vis < -20) {
    display.println(temperature_vis, DEC);
  //} else {
   // display.println("#");
  //}
}

void affiche_reglage(const char * message, int valeur){
  display.print(message);
  display.println(": ");
  display.print(valeur);
}

void affiche_date(const u8 detail)
{
    long int uptime=millis()/1000 + millis_overflow * 4294967296;

    // up time since power on
  if ( uptime / 86400 >= 1 ) {
    display.print(uptime/86400);
    display.print("J ");
    if ( detail) {
      display.print(uptime%86400/3600);
      display.println("h ");
    }
  } else if ( uptime / 3600 >= 1 ) {
    display.print(uptime/3600);
    display.print("h ");
    if ( detail) {
      display.print((uptime%3600)/60);
      display.println("mn ");
    }
  } else if ( uptime / 60 >= 1 ) {
    display.print(uptime/60);
    display.print("mn ");
    if ( detail) {
      display.print(uptime%60);
      display.println("s ");
    }
  } else {
    display.print(uptime%60);
    display.print("s ");
  }
  display.print(millis_overflow);
  display.print(" ");
}

void affiche_boiler(const u8 etat, const u8 screen){
  switch ( etat) {
    case ETAT_REPOS:
      display.println("  REPOS");
    break;;
    case ETAT_FORCE_CHAUFFE:
      display.println("DEMARRAGE");
    break;;
    case ETAT_CHAUFFE:
      display.println(" CHAUFFE");
    break;;
    default:
      display.println("  UNDEF");
  }
  if (screen == BOILER_SCREEN_DEBUG ) display.setTextSize(1);      // 1:2 pixel scale
  display.print(tic, DEC);
  display.print(" ");
  display.print(nb_cycle, DEC);
  display.print(" ");
  if (screen == BOILER_SCREEN_DEBUG ) {
    display.println("");
    display.print(temperature_error_eau_total , DEC);
    display.print(" ");
    display.print(temperature_error_vis_total , DEC);
    display.print(" ");
    display.print(temperature_error_same_total , DEC);
    display.print("  ");
    display.print(sonde_error_0_total , DEC);
    display.print(" ");
    display.print(sonde_error_1_total , DEC);
    display.print(" ");
    display.println(sonde_error_X_total , DEC);
  }
  else
  {
    display.println(temperature_error_eau_total + temperature_error_vis_total \
                  + temperature_error_same_total + sonde_error_0_total        \
                  + sonde_error_1_total + sonde_error_X_total, DEC);
  }
  if (screen == BOILER_SCREEN_DEBUG ) {
    display.print(stat_cycle_total, DEC);
    display.print(" ");
    display.print(stat_chauffe_total, DEC);
    display.print(" ");
  }
  display.print(stat_cycle, DEC);
  display.print(" ");
  display.println(stat_repos, DEC);
  if (screen == BOILER_SCREEN_DEBUG ) {
    display.print(VERSION_NB);
    display.print(" ");
    affiche_date(1);
  }

}

void affiche_secu(const u8 secu){

  display.println(" SECURITE");
  switch ( secu ) {
    case ERROR_TEMP_SECU:
      display.print("Te");
      break;
    case ERROR_TEMP_VIS:
      display.print("Tv");
      break;
    default:
      display.print("S?");
  }
  display.print(" ");
  display.print(memorise_temperature_eau, DEC);
  display.print(" ");
  display.print(memorise_temperature_vis, DEC);
  display.print(" ");
  display.print(temperature_error_eau_total + temperature_error_vis_total + temperature_error_same_total, DEC);
  display.print(" ");
  display.print(sonde_error_0_total + sonde_error_1_total + sonde_error_X_total, DEC);
  display.print(" ");
  display.print(tic++);
}

void affiche_error(const u8 erreur){

  display.println(" ERREUR ");
  switch ( erreur ) {
    case ERROR_1_PROBE:
      display.print("1T");
      break;
    case ERROR_0_PROBE:
      display.print("0T");
      break;
    case ERROR_X_PROBE:
      display.print("xT");
      break;
    case ERROR_VIS:
      display.print("M");
      break;
    case ERROR_TEMP_ERROR:
      display.print("Tr");
      break;
    default:
      display.print("E?");
  }
  display.print(" ");
  display.print(temperature_error_eau_total, DEC);
  display.print(" ");
  display.print(temperature_error_vis_total, DEC);
  display.print(" ");
  display.print(temperature_error_same_total, DEC);
  display.print(" ");
  display.print(sonde_error_0_total, DEC);
  display.print(" ");
  display.print(sonde_error_1_total, DEC);
  display.print(" ");
  display.print(sonde_error_X_total, DEC);
  display.print(" ");
  display.print(tic++);
}


void affiche_version(void){
  display.println(" VERSION");
  display.println(VERSION_DATE);
  display.println(VERSION_NB);
}

void affiche_stop(void){
  display.println("  ARRET ");
  display.println("INV VENTIL");
  display.println("VIS  REGL.");
}

void affiche_stats(void){
  display.println("  STATS");
  affiche_date(1);
  display.println("");
  display.print(stat_cycle_total);
  display.print(" ");
  display.print(stat_chauffe_total);
}

void affiche_reglage(void){
    display.println(" REGLAGE");
}

void exit_reglage(void){
    mode = MODE_STATS;
}

// CORRECTION DES MACROS : syntaxe standard C++ (pas de \n, backslash uniquement pour continuation de ligne)
#define SET_PARAM( param , value, message, pmin, pmax)  \
        if      (BUP_st   > 0 ) { tmp_reglage++; } \
        else if (BDOWN_st > 0 ) { tmp_reglage--; } \
        else if (BOK_st   > 0 ) { value = tmp_reglage; \
                                  reglage_next = 1; \
                                } \
        else if (BMENU_st > 0 ) { exit_reglage(); } \
        if ( pmin > tmp_reglage ) { tmp_reglage = pmin; } \
        else if (pmax < tmp_reglage ) { tmp_reglage = pmax; } \
        affiche_reglage(message, tmp_reglage);
        
#define SET_NEXT_REGLAGE( pnext, snext)  \
        if (reglage_next == 1) { \
          tmp_reglage = pnext; \
          reglage = snext; \
          reglage_next = 0; \
        }

#define READ_REGLAGE( value, address, vmin, vmax, vdefaut)  \
        EEPROM.get(address, value); \
        if (value < vmin || value > vmax) { value = vdefaut; }

#define SAVE_REGLAGE( value, address)  \
        EEPROM.put(address, value);

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
    
  pinMode(OPTICAL_1, INPUT_PULLUP);
  // CORRECTION : digitalPinToInterrupt au lieu du numero d'interruption en dur
  attachInterrupt(digitalPinToInterrupt(OPTICAL_1), interruptC1, FALLING);
  
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

  // Display version state then start
  mode = MODE_POWER_ON;
}


void loop() {
  // Detection du rollover de millis() : horloge qui repart a zero ~49,7j
  unsigned long millis_now = millis();
  if (millis_now < millis_last) millis_overflow++;
  millis_last = millis_now;

  t_loop_debut = millis_now;
  
  display.clearDisplay();
  
  display.setTextSize(2);      // Normal 1:1 pixel scale
  display.setCursor(0, 0);     // Start at top-left corner

  digitalWrite(LED_BUILTIN, 1);
  
  // CORRECTION : lecture atomique du compteur optique.
  // OpticCount (int 16 bits) est modifie par l'interruption : on copie
  // et on remet a zero interruptions coupees, puis on travaille sur la copie.
  noInterrupts();
  optic_seconde = OpticCount;
  OpticCount = 0;
  interrupts();
  
  // You can have more than one IC on the same bus.
  // 0 refers to the first IC on the wire
  probe_nb = sensors.getDeviceCount();
  if ( probe_nb == 0 ) {
    mode = MODE_ERREUR;
    error = ERROR_0_PROBE;
    sonde_error_0++;
    if ( sonde_error_0_total < 255 ) sonde_error_0_total++;
    temperature_eau = -127;
    temperature_vis = -127;
  } else {
    // CORRECTION : conversion demandee aussi en cas d'anomalie sonde,
    // sinon getTempCByIndex() relit un scratchpad perime
    sensors.requestTemperatures(); // Send the command to get temperatures

    if ( probe_nb == 1 ) {
      mode = MODE_ERREUR;
      error = ERROR_1_PROBE;
      sonde_error_1++;
      if ( sonde_error_1_total < 255 ) sonde_error_1_total++;
      temperature_eau = -127;
      temperature_vis = -127;
    } else if ( probe_nb > 2 ) {
      mode = MODE_ERREUR;
      error = ERROR_X_PROBE;
      sonde_error_X++;
      if ( sonde_error_X_total < 255 ) sonde_error_X_total++;
      temperature_eau = -127;
      temperature_vis = -127;
    }
    else {
      if ( probe_select == 1 ) {
        temperature_eau = (int)sensors.getTempCByIndex(0);
        temperature_vis = (int)sensors.getTempCByIndex(1);
      } else {
        temperature_eau = (int)sensors.getTempCByIndex(1);
        temperature_vis = (int)sensors.getTempCByIndex(0);
      }
    }
  }
  
  // Check temperature probe errors threshold
  if ( sonde_error_0 >= PROBE_ERROR_MAX ) {
    mode = MODE_ERREUR;
    error = ERROR_0_PROBE;
  }
  if ( sonde_error_1 >= PROBE_ERROR_MAX ) {
    mode = MODE_ERREUR;
    error = ERROR_1_PROBE;
  }
  if ( sonde_error_X >= PROBE_ERROR_MAX ) {
    mode = MODE_ERREUR;
    error = ERROR_X_PROBE;
  }

  // Robustify temperature read
  if ( temperature_eau == -127 ) {
    temperature_error_eau++;
    if ( temperature_error_eau_total < 255 ) temperature_error_eau_total++;
  }
  if ( temperature_vis == -127 ) {
    temperature_error_vis++;
    if ( temperature_error_vis_total < 255 ) temperature_error_vis_total++;
  }
  else if ( temperature_vis == temperature_eau ) {
    // Should not occurs, don't take it in account
    temperature_error_same++;
    if ( temperature_error_same_total < 255 ) temperature_error_same_total++;
    temperature_vis = -127;  // Force la valeur à -1 au lieu de garder l'ancienne
  }

  // On accepte une erreur de lecture temperature par minute
  if (++Compteur_secondes >= 60 ) {
    if ( temperature_error_eau > 1) temperature_error_eau--;
    if ( temperature_error_vis > 1) temperature_error_vis--;
    if ( temperature_error_same > 1) temperature_error_same--;
    if ( sonde_error_0 > 1) sonde_error_0--;
    if ( sonde_error_1 > 1) sonde_error_1--;
    if ( sonde_error_X > 1) sonde_error_X--;
    Compteur_secondes = 0;
  }

  blocage = MoteurVis.getNB();
  // Read thermocouple K
  // Read the temp from the MAX6675
  //temperature_K = (int)temp.read_temp();
  //mesure_timings("max6675 lue : ");
  
  // Display header whatever the boiler mode
  affiche_header();

  switch( mode ) {
  case MODE_ON:
    // Action

    // Security check
    if ( temperature_vis > -20 && temperature_vis >= temperature_vis_max ) {
        mode = MODE_SECU;
        error = ERROR_TEMP_VIS;
        memorise_temperature_vis = temperature_vis;
        memorise_temperature_eau = temperature_eau;
      }
    if ( temperature_eau > -20 && temperature_eau <= temperature_secu && etat != ETAT_FORCE_CHAUFFE) {
        mode = MODE_SECU;
        error = ERROR_TEMP_SECU;
        memorise_temperature_vis = temperature_vis;
        memorise_temperature_eau = temperature_eau;
      }
    if ( temperature_error_eau + temperature_error_vis + temperature_error_same >= PROBE_READ_FAIL_MAX) {
        mode = MODE_ERREUR;
        error = ERROR_TEMP_ERROR;
        memorise_temperature_vis = temperature_vis;
        memorise_temperature_eau = temperature_eau;
      }
  
    switch( etat ) {
    case ETAT_REPOS:
     if (temperature_eau > -20 && temperature_eau <= temperature_demarrage) {
       MoteurVis.demarre(tempo_moteur, 1);
       Ventilo.demarre(tempo_ventilo);
       etat = ETAT_CHAUFFE;
       stat_chauffe_total++;
       stat_repos = tic;
       tic = 0;
       nb_cycle = 0;
     } else {
       tic += 1;
     }
     break;
  
    case ETAT_FORCE_CHAUFFE:
     // fallthrough voulu : le mode force enchaine sur la logique de chauffe
    case ETAT_CHAUFFE:
      if ( tic == 0 ){
        if (temperature_eau > -20 && temperature_eau < temperature_arret) {
          MoteurVis.demarre(tempo_moteur, 1);
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
        // CORRECTION : arret immediat des actionneurs, sans attendre
        // le tour de boucle suivant en MODE_ERREUR
        Ventilo.arret();
        MoteurVis.arret();
      } else {
        Ventilo.tic(1);
        MoteurVis.tic(1, optic_seconde);

        if (++tic >= tempo_cycle ) {
          tic = 0;
        }
      }
    break;

    default:
    break;
    }

    // Display
    affiche_boiler(etat, boiler_screen);

    // Button management
    if (BUP_st > 0 ) { boiler_screen = BOILER_SCREEN_DEFAULT ; }
    else if (BDOWN_st > 0 ) { boiler_screen = BOILER_SCREEN_DEBUG ; };
  
  break;

  case MODE_SECU:
    // Action
    Ventilo.arret();
    MoteurVis.arret();

    // Display
    affiche_secu(error);

    // Button management
    if (BOK_st > 0 ) {
      if (ERROR_TEMP_SECU == error) restart_boiler(ETAT_FORCE_CHAUFFE);
      else restart_boiler(ETAT_REPOS);
    }
  break;

  case MODE_ERREUR:
    // Action
    Ventilo.arret();
    MoteurVis.arret();

    // Display
    affiche_error(error);
    
    // Button management
    if (BOK_st > 0 ) {
      if (ERROR_TEMP_SECU == error) restart_boiler(ETAT_FORCE_CHAUFFE);
      else restart_boiler(ETAT_REPOS);
    }
  break;
  
  case MODE_STOP:
    // Action

    // Display
    affiche_stop();

    // Button management
    if (BDOWN_st > 0 ) {
      MoteurVis.debloque();
      MoteurVis.demarre(tempo_moteur, 1);
    } else if (BUP_st > 0 ) {
      MoteurVis.debloque();
      MoteurVis.demarre(tempo_moteur, 2);
    } else if (BOK_st > 0 ) {
      Ventilo.demarre(tempo_ventilo);
    }
    Ventilo.tic(1);
    MoteurVis.tic(1, optic_seconde);

  break;
  
  case MODE_REGLAGE:
    // Action
    Ventilo.arret();
    MoteurVis.arret();

    // Display
    affiche_reglage();

    // Button management
    switch ( reglage ) {
      case REGLAGE_INIT:
        reglage = REGLAGE_TEMP_START;
        tmp_reglage = temperature_demarrage;
        // fallthrough voulu

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
        SET_PARAM( MOTOR_INV , moteur_duree_inversion, "Invers.", TEMPO_MOTOR_I_MIN, TEMPO_MOTOR_I_MAX);
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
    // Action

    // Display
    affiche_stats();

    // Button management
    if (BOK_st > 0 ) {
      restart_boiler(ETAT_REPOS);
    }
  break;

  case MODE_POWER_ON:
    if (version_display > 0) version_display--;
  case MODE_VERSION:
    // Action

    // Display
    affiche_version();

    // Button management
    if (BOK_st > 0 || version_display == 0) {
      restart_boiler(ETAT_REPOS);
    }
  break;

  }
  
    // Button MENU management
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
  // mesure_timings("lcd refresh : ");

  display.display();

  digitalWrite(LED_BUILTIN, 0 );
  
  t_loop_fin = millis();
  t_loop_delai = t_loop_fin - t_loop_debut;

  // protect against millis overflow
  if (t_loop_fin < t_loop_debut) t_loop_delai = 0;

  // CORRECTION : si le tour de boucle a dure plus d'une seconde, on borne
  // le delai, sinon delay() recevrait un unsigned long enorme et la carte
  // se figerait pendant ~49 jours
  if (t_loop_delai > 1000) t_loop_delai = 1000;

  /* Rafraichissement une fois par seconde */ 
  delay(1000-t_loop_delai); 
  
}
