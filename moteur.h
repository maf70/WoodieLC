#ifndef __MOTEUR__H__
#define __MOTEUR__H__

class actionneur  {
  public:
  actionneur(int p);
  void demarre(long d );
  void arret();
  void tic(int t);
  
  protected:
  int pin;
  long duree;
};

class moteur : public actionneur {
  public:
  moteur(int p, int pi);
  void tic(int t, int cpt);

  private:
  int pin_inverse;
};

#endif
