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
  int etat;
  long duree;
};

class moteur : public actionneur {
  public:
  moteur(int p, int pi);
  void demarre(long d );
  void pause(int p);
  void arret();
  void parametres(int delai, int duree, int vmin);
  void tic(int t, int cpt);
  int getNB() {return nb_blocage;};

  private:
  int moteur_delai_inversion;
  int moteur_duree_inversion;
  int moteur_vitesse_min;
  int pin_inverse;
  int nb_blocage;
  int tempo;
};

#endif
