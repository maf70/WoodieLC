#ifndef __MOTEUR__H__
#define __MOTEUR__H__

class actionneur  {
  public:
  actionneur(int p);
  void demarre(long d );
  void pause(int p);
  void arret();
  void tic(int t);
  
  protected:
  int pin;
  u8  etat;
  long duree;
};

class moteur : public actionneur {
  public:
  moteur(int p, int pi);
  void demarre(long d );
  void pause(int p);
  void arret();
  void debloque();
  void parametres(u8 delai, u8 duree, u8 vmin);
  void tic(int t, int cpt);
  u8 getNB() {return nb_blocage;};

  private:
  u8 moteur_delai_inversion;
  u8 moteur_duree_inversion;
  u8 moteur_vitesse_min;
  u8 pin_inverse;
  u8 nb_blocage;
  int tempo;
};

#endif
