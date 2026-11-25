#include "config.h"
#include "init.h"
#include "affichage.h"
#include "reglage.h"
#include "boutons.h"

#include <EEPROM.h>


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

  // Lecture de tous les reglages en EEPROM avec bornes et defauts
  lire_tous_reglages();

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
        etat = ETAT_STOP;
      }
    if ( temperature_eau > -20 && temperature_eau <= temperature_secu && etat != ETAT_FORCE_CHAUFFE) {
        mode = MODE_SECU;
        error = ERROR_TEMP_SECU;
        memorise_temperature_vis = temperature_vis;
        memorise_temperature_eau = temperature_eau;
        etat = ETAT_STOP;
      }
    if ( temperature_error_eau + temperature_error_vis + temperature_error_same >= PROBE_READ_FAIL_MAX) {
        mode = MODE_ERREUR;
        error = ERROR_TEMP_ERROR;
        memorise_temperature_vis = temperature_vis;
        memorise_temperature_eau = temperature_eau;
        etat = ETAT_STOP;
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
    gerer_reglage();
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
    case MODE_SECU:
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
