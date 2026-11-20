#if ARDUINO >= 100
#include "Arduino.h"
#else
extern "C" {
#include "WConstants.h"
}
#endif

#include <Wire.h>
#include <Adafruit_SSD1306.h>

#include "config.h"
#include "reglage.h"

// Lecture de tous les reglages a l'allumage
void lire_tous_reglages(void) {
  lire_reglage( temperature_demarrage , EEPROM_TEMPERATURE_DEMARRAGE , TEMPERATURE_DEMARRAGE_MIN, TEMPERATURE_DEMARRAGE_MAX, TEMPERATURE_DEMARRAGE );
  lire_reglage( temperature_arret     , EEPROM_TEMPERATURE_ARRET     , TEMPERATURE_ARRET_MIN    , TEMPERATURE_ARRET_MAX    , TEMPERATURE_ARRET     );
  lire_reglage( tempo_cycle           , EEPROM_TEMPO_CYCLE           , TEMPO_CYCLE_MIN          , TEMPO_CYCLE_MAX          , TEMPO_CYCLE           );
  lire_reglage( tempo_ventilo         , EEPROM_TEMPO_VENTILO         , TEMPO_VENTILO_MIN        , TEMPO_VENTILO_MAX        , TEMPO_VENTILO         );
  lire_reglage( tempo_moteur          , EEPROM_TEMPO_MOTEUR          , TEMPO_MOTOR_MIN          , TEMPO_MOTOR_MAX          , TEMPO_MOTEUR          );
  lire_reglage( moteur_duree_inversion, EEPROM_MOTEUR_DUREE_INVERSION, TEMPO_MOTOR_I_MIN        , TEMPO_MOTOR_I_MAX        , MOTEUR_DUREE_INVERSION);
  lire_reglage( moteur_vitesse_min    , EEPROM_MOTEUR_VITESSE_MIN    , COUNT_MOTOR_C_MIN        , COUNT_MOTOR_C_MAX        , MOTEUR_VITESSE_MIN    );
  lire_reglage( moteur_blocage_max    , EEPROM_MOTEUR_BLOCAGE_MAX    , COUNT_MOTOR_B_MIN        , COUNT_MOTOR_B_MAX        , MOTEUR_BLOCAGE_MAX    );
  lire_reglage( temperature_secu      , EEPROM_TEMP_SECU             , TEMP_SECU_MIN            , TEMP_SECU_MAX            , TEMP_SECU             );
  lire_reglage( temperature_vis_max   , EEPROM_TEMP_VIS              , TEMP_VIS_MIN             , TEMP_VIS_MAX             , TEMP_VIS              );
  lire_reglage( probe_select          , EEPROM_PROBE_SELECT          , PROBE_SELECT_MIN         , PROBE_SELECT_MAX         , PROBE_SELECT          );
}

// Machine a etats du menu reglage
void gerer_reglage(void) {
  switch ( reglage ) {
    case REGLAGE_INIT:
      reglage = REGLAGE_TEMP_START;
      tmp_reglage = temperature_demarrage;
      // fallthrough voulu

    case REGLAGE_TEMP_START:
      gerer_param( temperature_demarrage, "T Start", TEMPERATURE_DEMARRAGE_MIN, TEMPERATURE_DEMARRAGE_MAX, temperature_arret, REGLAGE_TEMP_STOP);
      ecrire_reglage( temperature_demarrage, EEPROM_TEMPERATURE_DEMARRAGE);
      break;

    case REGLAGE_TEMP_STOP:
      gerer_param( temperature_arret, "T Stop", TEMPERATURE_ARRET_MIN, TEMPERATURE_ARRET_MAX, tempo_cycle, REGLAGE_CYCLE);
      ecrire_reglage( temperature_arret, EEPROM_TEMPERATURE_ARRET);
      break;

    case REGLAGE_CYCLE:
      gerer_param( tempo_cycle, "Cycle", TEMPO_CYCLE_MIN, TEMPO_CYCLE_MAX, tempo_ventilo, REGLAGE_VENTILO);
      ecrire_reglage( tempo_cycle, EEPROM_TEMPO_CYCLE);
      break;

    case REGLAGE_VENTILO:
      gerer_param( tempo_ventilo, "Ventilo", TEMPO_VENTILO_MIN, tempo_cycle-1, tempo_moteur, REGLAGE_MOTOR);
      ecrire_reglage( tempo_ventilo, EEPROM_TEMPO_VENTILO);
      break;

    case REGLAGE_MOTOR:
      gerer_param( tempo_moteur, "Moteur", TEMPO_MOTOR_MIN, tempo_cycle-1, moteur_duree_inversion, REGLAGE_MOTOR_INV);
      ecrire_reglage( tempo_moteur, EEPROM_TEMPO_MOTEUR);
      break;

    case REGLAGE_MOTOR_INV:
      gerer_param( moteur_duree_inversion, "Invers.", TEMPO_MOTOR_I_MIN, TEMPO_MOTOR_I_MAX, moteur_vitesse_min, REGLAGE_MOTOR_COUNT);
      ecrire_reglage( moteur_duree_inversion, EEPROM_MOTEUR_DUREE_INVERSION);
      break;

    case REGLAGE_MOTOR_COUNT:
      gerer_param( moteur_vitesse_min, "Compteur", COUNT_MOTOR_C_MIN, COUNT_MOTOR_C_MAX, moteur_blocage_max, REGLAGE_MOTOR_BLOCK);
      ecrire_reglage( moteur_vitesse_min, EEPROM_MOTEUR_VITESSE_MIN);
      break;

    case REGLAGE_MOTOR_BLOCK:
      gerer_param( moteur_blocage_max, "Blocages", COUNT_MOTOR_B_MIN, COUNT_MOTOR_B_MAX, temperature_secu, REGLAGE_TEMP_SECU);
      ecrire_reglage( moteur_blocage_max, EEPROM_MOTEUR_BLOCAGE_MAX);
      break;

    case REGLAGE_TEMP_SECU:
      gerer_param( temperature_secu, "T Secu", TEMP_SECU_MIN, TEMP_SECU_MAX, temperature_vis_max, REGLAGE_TEMP_VIS);
      ecrire_reglage( temperature_secu, EEPROM_TEMP_SECU);
      break;

    case REGLAGE_TEMP_VIS:
      gerer_param( temperature_vis_max, "T Vis", TEMP_VIS_MIN, TEMP_VIS_MAX, probe_select, REGLAGE_PROBE_SELECT);
      ecrire_reglage( temperature_vis_max, EEPROM_TEMP_VIS);
      break;

    case REGLAGE_PROBE_SELECT:
      gerer_param( probe_select, "Sonde T", PROBE_SELECT_MIN, PROBE_SELECT_MAX, -1, REGLAGE_END);
      ecrire_reglage( probe_select, EEPROM_PROBE_SELECT);
      break;

    case REGLAGE_END:
      restart_boiler(ETAT_REPOS);
      break;
  }
}
