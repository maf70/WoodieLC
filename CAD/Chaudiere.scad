// ============================================
// FICHIER : Chaudiere.scad
// DESCRIPTION : Modèle 3D d'une chaudière avec corps, foyer, tuyaux,
//               charnières, pieds et porte amovible.
// ============================================

// ----------------------------
// 1. DIMENSIONS DU CORPS PRINCIPAL
// ----------------------------
// Longueur du corps (en mm)
cl = 50;
// Largeur du corps (en mm)
cL = 60;
// Hauteur du corps (en mm)
cH = 90;
// Épaisseur des parois (en mm)
ce = 2;
// Longueur de la face avant (en mm)
cf = 10;

// ----------------------------
// 2. DIMENSIONS DE L'AXE
// ----------------------------
// Diamètre de l'axe (en mm)
ad = 3.5;
// Hauteur de l'axe (en mm)
ah = 30;
// Diamètre de l'alésage/du support d'axe (en mm)
aD = 21;

// ----------------------------
// 3. DIMENSIONS DE LA SONDE
// ----------------------------
// Diamètre de la sonde (en mm)
sd = 7;

// ----------------------------
// 4. DIMENSIONS DU FOYER
// ----------------------------
// Diamètre du foyer (en mm)
fd = 37;
// Hauteur du foyer (en mm)
fh = 36;
// Position verticale du foyer (en mm)
fp = 20;

// ----------------------------
// 5. DIMENSIONS DU TUYAU
// ----------------------------
// Diamètre du tuyau (en mm)
td = 15;

// ----------------------------
// 6. DIMENSIONS DES CHARNIÈRES
// ----------------------------
// Épaisseur des charnières (en mm)
cht = 2.5;
// Hauteur des charnières (en mm)
chh = 5;

// ----------------------------
// 7. DIMENSIONS DES PIEDS
// ----------------------------
// Longueur des pieds (en mm)
pil = 10;
// Diamètre des trous dans les pieds (en mm)
pit = 2.8;

// ----------------------------
// 8. QUALITÉ DE RENDU
// ----------------------------
// Nombre de fragments pour les cercles et sphères
$fn = 50;


// ============================================
// MODULES
// ============================================

// ----------------------------
// MODULE : CHARNIÈRE TYPE 1
// ----------------------------
// Crée une charnière pour la fixation de la porte (côté corps de la chaudière).
// Paramètre :
//   - _h : décalage vertical pour l'alignement (en mm)
module charniere_1(_h) {
  // Longueur de la charnière (épaisseur paroi + épaisseur charnière)
  _l = ce + cht;
  // Épaisseur du pivot de la charnière
  _t = cht;

  // Positionne la charnière sur l'axe Y (centrée)
  translate([0, -_l/2, 0]) 
    difference() {
      union() {
        // Corps principal de la charnière (cylindre vertical)
        cylinder(d = _l, h = chh);
        
        // Renfort latéral gauche (cube)
        translate([-_l/2, 0, 0]) cube([_l, _l/2, chh]);
        
        // Pivot central (cylindre vertical)
        translate([0, 0, 0]) cylinder(d = _t, h = 2*chh - _t/2 + _h);
        
        // Boule en haut du pivot (pour un meilleur rendu visuel)
        translate([0, 0, 2*chh - _t/2 + _h]) sphere(d = _t);
      }
      
      // Soustrait un cube inclinée pour créer un dégagement dans la charnière
      rotate([-20, 0, 0]) 
        translate([0, 0, -chh/2]) 
          cube([2*_l, 2*_l, chh + 2], center = true);
    }
}


// ----------------------------
// MODULE : CHARNIÈRE TYPE 2
// ----------------------------
// Crée une charnière pour la fixation de la porte (côté porte).
// Paramètre :
//   - _a : angle de rotation de la charnière (en degrés)
module charniere_2(_a) {
  // Longueur de la charnière
  _l = ce + cht;
  // Épaisseur du pivot (légèrement plus épais que type 1)
  _t = cht + 0.5;

  // Positionne et tourne la charnière
  translate([0, -_l/2, chh]) 
    rotate([0, 0, _a]) 
      difference() {
        union() {
          // Corps principal de la charnière (cylindre vertical)
          cylinder(d = _l, h = chh);
          
          // Renfort latéral (plus large que pour type 1)
          translate([-_l/2, 0, 0]) cube([_l, 2*_l, chh]);
        }
        
        // Soustrait un cylindre pour créer le trou du pivot
        translate([0, 0, 0]) cylinder(d = _t, h = 2*chh - _t/2);
      }
}


// ----------------------------
// MODULE : PIED
// ----------------------------
// Crée un pied de support pour la chaudière.
// Paramètre :
//   - _l : longueur du pied (en mm)
module pied(_l) {
  difference() {
    union() {
      // Base du pied : cube allongé
      translate([0, -_l, 0]) cube([_l, _l + cL/2, ce]);
      
      // Renfort diagonal pour la stabilité
      rotate([-8, 0, 0]) 
        translate([0, -_l*0.4, 0]) 
          cube([_l, _l, _l*2]);
    }
    
    // Soustrait un cube pour créer un dégagement (évite les interférences)
    translate([0, 0, -20]) cube([40, 40, 40], center = true);
    
    // Trou de fixation dans le pied
    translate([_l/2, -_l/2 - _l*0.2, -1]) 
      cylinder(d = pit, h = _l);
  }
}


// ============================================
// CONSTRUCTION PRINCIPALE : CORPS + FOYER + TUYAUX
// ============================================

if (1) 
  difference() {
    union() {
      // ----------------------------
      // Base : Corps + Dessus + Tuyau
      // ----------------------------
      difference() {
        union() {
          // Corps principal : cube rectangulaire
          cube([cl, cL, cH]);
          
          // Dôme supérieur : cylindre horizontal à l'arrière
          translate([cl/2, cL, cH - cl/3]) 
            rotate([90, 0, 0]) 
              cylinder(d = cl, h = cL);
          
          // Dessus : plaque de fermeture
          translate([0, 0, cH]) 
            cube([cl, cL, ce]);
          
          // Embout du tuyau de sortie (supérieur)
          translate([cl/2, 2*cL/3, cH + ce]) 
            cylinder(d = td + 2*ce, h = 10);
          
          // Ajout des 4 pieds aux coins
          pied(pil);
          translate([cl - pil, 0, 0]) pied(pil);
          translate([pil, cL, 0]) rotate([0, 0, 180]) pied(pil);
          translate([cl, cL, 0]) rotate([0, 0, 180]) pied(pil);
        }
        
        // ----------------------------
        // Évidements intérieurs
        // ----------------------------
        // Cavité intérieure du corps (espace vide)
        translate([ce, ce, fp + ce + sd]) 
          cube([cl - 2*ce, cL - 2*ce, cH - fp - cl/2]);
        
        // Évidement du dôme supérieur (intérieur)
        translate([cl/2, cL - ce, cH - cl/3]) 
          rotate([90, 0, 0]) 
            cylinder(d = cl - 2*ce, h = cL - 2*ce);
        
        // Foyer : cylindre vertical au centre
        translate([cl/2, cL/2, ce + 2*sd + 2]) 
          cylinder(d = fd, h = fh);
        
        // Tuyau de sortie (partie intérieure, dans le corps)
        translate([cl/2, 2*cL/3, cH - ce]) 
          cylinder(d = td, h = fh);
        
        // ----------------------------
        // Sonde et alimentation
        // ----------------------------
        // Sonde : cylindre horizontal traversant la paroi gauche
        translate([-ce, cL/2, ce + 2*sd]) 
          rotate([0, 90, 0]) 
            cylinder(d = sd, h = fd/2 + cl/2 + ce);
        
        // Alimentation : petit tuyau à l'arrière
        translate([3*cl/4, cL + ce, fp + 2*ce + sd]) 
          rotate([90, 0, 0]) 
            cylinder(d = 3, h = fd/2 + cl/2 + ce);
        
        // ----------------------------
        // Porte : ouverture avant
        // ----------------------------
        // Ouverture rectangulaire pour la porte
        translate([-fd/2 + cl/2, -ce, ce + ah + aD/2]) 
          cube([fd, 3*ce, fh]);
      }
      
      // ----------------------------
      // Face avant avec supports de vis
      // ----------------------------
      // Panneau avant (avec épaisseur pour les fixations)
      cube([cl, cf, ah + aD/2 + ce]);
      
      // ----------------------------
      // Charnières côté corps (pour la porte)
      // ----------------------------
      // Charnière supérieure droite
      translate([cht/2 + ce/2 + fd + (cl - fd)/2 + ce/2, 0, ce + ah + aD/2]) 
        union() {
          charniere_1(0);
          translate([0, 0, fh - 2*chh]) charniere_1(0);
        }
      
      // Charnière inférieure gauche
      translate([-cht/2 - ce/2 + (cl - fd)/2 - ce/2, 0, ce + ah + aD/2 + fd/2 - chh]) 
        charniere_1(-ce);
    }
    
    // ----------------------------
    // Axes de fixation
    // ----------------------------
    // Axe principal (vis horizontale traversant la face avant)
    translate([cl/2, cf + ce, ah]) 
      rotate([90, 0, 0]) 
        cylinder(d = ad, h = 2*cf);
    
    // Support/alésage pour l'axe
    translate([cl/2, 2*ce, ah]) 
      rotate([90, 0, 0]) 
        cylinder(d = aD, h = 2*cf);
    
    // ----------------------------
    // Évidements supplémentaires (accès ou aération ?)
    // ----------------------------
    // Évidement arrière (horizontal)
    translate([cl/2, cL + ce, -cl/3 - sd/2]) 
      rotate([90, 0, 0]) 
        cylinder(d = cl, h = cL + 2*ce);
    
    // Évidement latéral gauche (vertical)
    translate([-ce, cL/2, -cl/3 - sd/2]) 
      rotate([0, 90, 0]) 
        cylinder(d = cl, h = cL + 2*ce);
  }


// ============================================
// PORTE AMOVIBLE (en vert)
// ============================================

if (1) 
  color("green") 
    union() {
      // ----------------------------
      // Charnières côté porte
      // ----------------------------
      // Charnière supérieure droite (sur la porte)
      translate([cht/2 + ce/2 + fd + (cl - fd)/2 + ce/2, 0, ce + ah + aD/2]) 
        union() {
          charniere_2(90);
          translate([0, 0, fh - 2*chh]) charniere_2(90);
        }
      
      // Charnière inférieure gauche (sur la porte)
      translate([-cht/2 - ce/2 + (cl - fd)/2 - ce/2, 0, ce + ah + aD/2 + fd/2 - chh]) 
        charniere_2(-90);
      
      // ----------------------------
      // Porte : panneau principal
      // ----------------------------
      // Panneau de la porte (légèrement plus grand que l'ouverture)
      translate([-fd/2 + cl/2 - ce/4, -ce, ce + ah + aD/2]) 
        cube([fd + ce/2, ce, fh]);
    }
