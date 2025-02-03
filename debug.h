
#ifndef __DEBUG__H__
#define __DEBUG__H__


unsigned long t_prev =0;
unsigned long total_s =0;

void mesure_timings(const char * s) {
  // Affiche le temps depuis que le programme a commencé
  unsigned long t_ = millis();
  
  
  Serial.print (s);
  Serial.println (t_ - t_prev);
  t_prev = t_;
}

#endif
