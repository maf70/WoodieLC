// ============================================
// FICHIER : support_pcb.scad
// DESCRIPTION : Support pour carte PCB avec trous de fixation,
//               évidements pour composants, renforts et languette.
// ============================================

// ----------------------------
// 1. DIMENSIONS DES TROUS DE FIXATION DU PCB
// ----------------------------
// Diamètre des trous de fixation (en mm)
trou_dia = 2.5;
// Distance gauche depuis l'origine (en mm)
trou_dis_g = 25;
// Distance entre trous (en mm)
trou_dis = 56;
// Distance droite depuis l'origine (en mm)
trou_dis_d = 19;
// Distance bas depuis l'origine (en mm)
trou_dis_b = 5.5;
// Diamètre des trous de fixation du support (en mm)
trou_fixation = 3;

// ----------------------------
// 2. DIMENSIONS DU SUPPORT PRINCIPAL
// ----------------------------
// Marge/bordure autour du PCB (en mm)
bord = 2;
// Épaisseur du fond (en mm)
fond = 1.5;
// Hauteur du support (en mm)
h = 16.5;
// Longueur du PCB (en mm)
pcb_l = 100.5;
// Largeur du PCB (en mm)
pcb_L = 100.5;
// Épaisseur du PCB (en mm)
pcb_e = 1.5;

// ----------------------------
// 3. DIMENSIONS DE LA LANGUETTE
// ----------------------------
// Longueur de la languette (en mm)
languette = 10;

// ----------------------------
// 4. DIMENSIONS DES SUPPORTS À VISSER
// ----------------------------
// Longueur du support à visser (en mm)
support_l = 10;
// Largeur du support à visser (en mm)
support_L = 18;
// Diamètre des trous dans les supports à visser (en mm)
support_trou = 2.5;


// ============================================
// CONSTRUCTION PRINCIPALE : BOÎTIER PRINCIPAL
// ============================================

difference() {
  // ----------------------------
  // Forme de base : boîtier extérieur
  // ----------------------------
  union() {
    // Cube principal avec bordure autour du PCB
    translate([-bord, -bord, 0]) 
      cube([pcb_L + 2*bord, pcb_L + 2*bord, h + pcb_e]);
  }
  
  // ----------------------------
  // Évidements pour le PCB
  // ----------------------------
  // Évidement principal pour loger le PCB
  translate([0, 0, h]) 
    cube([pcb_L, pcb_L, 2*h]);
  
  // Évidement intérieur (cavité sous le PCB)
  translate([bord, bord, fond]) 
    cube([pcb_L - 2*bord, pcb_L - 2*bord, 2*h]);
  
  // Évidement central (pour composants traversants ?)
  translate([10*bord, 10*bord, -1]) 
    cube([pcb_L - 20*bord, pcb_L - 20*bord, 2*h]);
  
  // ----------------------------
  // Évidements pour composants spécifiques
  // ----------------------------
  // Évidement pour la languette de connexion
  translate([10, pcb_L - 5*bord, 3*fond]) 
    cube([languette, 10*bord, 2*h]);
  
  // Évidement pour relais
  translate([10 + languette + 5, pcb_L - 12, -1]) 
    cube([pcb_l, 10*bord, 2*h]);
  
  // Évidement pour RTC (module horloge temps réel)
  translate([-2*bord, -2*bord, -1]) 
    cube([24.5, 15, 2*h]);
  
  // Évidement pour secteur (interrupteur ou connecteur)
  translate([pcb_l - 10 + 2*bord, 22, -1]) 
    cube([10, 55, 2*h]);
  
  // Évidement pour Nano (Arduino Nano ?)
  translate([-2*bord, 30, 3*fond]) 
    cube([10, 40, 2*h]);
  
  // ----------------------------
  // Trous de fixation du PCB
  // ----------------------------
  // Trou 1 (coin supérieur gauche)
  translate([12, 25, -fond]) 
    cylinder(d = trou_fixation, h = 3*fond, $fn = 20);
  
  // Trou 2 (coin supérieur droit)
  translate([pcb_L - 8, 12, -fond]) 
    cylinder(d = trou_fixation, h = 3*fond, $fn = 20);
  
  // Trou 3 (coin inférieur gauche)
  translate([12, pcb_L - 8, -fond]) 
    cylinder(d = trou_fixation, h = 3*fond, $fn = 20);
  
  // Trou 4 (coin inférieur droit)
  translate([pcb_L - 12, pcb_L - 25, -fond]) 
    cylinder(d = trou_fixation, h = 3*fond, $fn = 20);
}


// ============================================
// RENFORTS CARRÉS AU FOND
// ============================================

difference() {
  // Renfort principal au centre du fond
  translate([9*bord, 9*bord, 0]) 
    cube([pcb_L - 18*bord, pcb_L - 18*bord, 3*fond]);
  
  // Soustrait l'évidement central déjà créé
  translate([10*bord, 10*bord, -1]) 
    cube([pcb_L - 20*bord, pcb_L - 20*bord, 2*h]);
}


// ============================================
// RENFORTS TRANSVERSE
// ============================================

// Renfort transversal supérieur
translate([0, 8*bord, 0]) 
  cube([pcb_l, 2*bord, 3*fond]);

// Renfort transversal inférieur
translate([0, 10*bord + pcb_L - 20*bord, 0]) 
  cube([pcb_l, 2*bord, 3*fond]);


// ============================================
// SUPPORTS À VISSER
// ============================================

// Supports à visser pour fixation externe (côté gauche)
translate([trou_dis_g - 5 + 0.5, 0, 0]) 
  difference() {
    // Bloc sous les vis (deux cubes côte à côte)
    union() {
      cube([support_l, support_L, h]);
      translate([trou_dis, 0, 0]) 
        cube([support_l, support_L, h]);
    }
    
    // Trous de vis dans les supports
    // Trou 1 (gauche)
    translate([support_l/2, trou_dis_b, 5]) 
      cylinder(d = support_trou, h = h, $fn = 20);
    
    // Trou 2 (droit)
    translate([trou_dis + support_l/2, trou_dis_b, 5]) 
      cylinder(d = support_trou, h = h, $fn = 20);
  }


// ============================================
// LANGUETTE DE CONNEXION
// ============================================

// Base de la languette (pour connexion électrique ou mécanique)
translate([11, pcb_L, 0]) 
  cube([languette - 2, bord, pcb_e + h + 0.3]);

// Forme de la languette (profil incliné)
hull() {
  // Base de la languette (large)
  translate([11, pcb_L - bord, pcb_e + h + 0.3]) 
    cube([languette - 2, 2*bord, 0.1]);
  
  // Extrémité de la languette (plus haute)
  translate([11, pcb_L, pcb_e + h + 0.3 + h]) 
    cube([languette - 2, bord, 0.1]);
}
