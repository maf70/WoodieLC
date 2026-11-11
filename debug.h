#ifndef __DEBUG__H__
#define __DEBUG__H__

// CORRECTION : static pour eviter les erreurs de link ("multiple definition")
// si ce header est inclus par plusieurs unites de compilation.
static unsigned long t_prev = 0;
static unsigned long total_s = 0;

static void mesure_timings(const char * s) {
  // Affiche le temps depuis que le programme a commencé
  unsigned long t_ = millis();
  
  (void)s;   // parametre reserve : les Serial.print sont desactives
  
  // Serial.print (s);
  // Serial.println (t_ - t_prev);
  t_prev = t_;
}

#endif