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
#include "affichage.h"

void affiche_header(void){
  // Sonde en defaut (-127) : affiche ## au lieu de la valeur crue
  if (temperature_eau < -20) display.print(F("# "));
  else                       display.print(temperature_eau, DEC);
  display.print(F(" "));
  //display.println(temperature_K);
  display.print(optic_seconde, DEC);
  display.print(F(" "));
  display.print(blocage, DEC);
  display.print(F(" "));
  if (temperature_vis < -20) display.println(F("# "));
  else                       display.println(temperature_vis, DEC);
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
    case ETAT_STOP:
      display.println("  STOP");
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
