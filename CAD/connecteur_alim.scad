// ============================================
// FICHIER : connecteur_alim.scad
// DESCRIPTION : Modèle 3D d'un connecteur d'alimentation.
//               Comprend un boîtier avec des trous de fixation et
//               un passage pour l'axe/câble central.
// ============================================

// ----------------------------
// 1. DIMENSIONS DU TROU CENTRAL
// ----------------------------
// Diamètre du trou central (pour axe ou câble, en mm)
d = 8;
// Diamètre des trous de fixation latéraux (en mm)
t = 2.8;

// ----------------------------
// 2. DIMENSIONS GÉNÉRALES
// ----------------------------
// Épaisseur des parois (en mm)
e = 2;
// Longueur du connecteur (en mm)
l = 15;
// Hauteur du connecteur (en mm)
h = 15;

// ----------------------------
// 3. QUALITÉ DE RENDU
// ----------------------------
$fn = 50;


// ============================================
// CONSTRUCTION PRINCIPALE
// ============================================

difference() {
  // ----------------------------
  // Forme de base : boîtier rectangulaire
  // ----------------------------
  // Cube principal avec marge (2*e) sur tous les côtés
  cube([l + 2*e, 2*l + 2*e, h + d]);
  
  // ----------------------------
  // Évidements : fentes latérales
  // ----------------------------
  // Fente latérale supérieure (pour le passage de câbles ou fixation)
  translate([e, l + e, e]) 
    cube([l, 2*l, h + d]);
  
  // Fente latérale inférieure (symétrique)
  translate([e, -l, e]) 
    cube([l, 2*l, h + d]);
  
  // ----------------------------
  // Évidement : trou central
  // ----------------------------
  // Trou central pour l'axe (cylindre horizontal)
  translate([e + l/2, 0, h]) 
    rotate([-90, 0, 0]) 
      cylinder(d = d, h = 100);
  
  // Renfort autour du trou central (cube pour maintenir la structure)
  translate([e/2 + l/2, 0, h]) 
    cube([e, 100, h]);
  
  // ----------------------------
  // Évidements : trous de fixation
  // ----------------------------
  // Trou de fixation supérieur (pour vis ou câble)
  translate([e + l/2, 6, -h]) 
    rotate([0, 0, 0]) 
      cylinder(d = t, h = 100);
  
  // Trou de fixation inférieur (symétrique)
  translate([e + l/2, 26, -h]) 
    rotate([0, 0, 0]) 
      cylinder(d = t, h = 100);
  
  // ----------------------------
  // Évidement : biseau ou dégagement
  // ----------------------------
  // Soustrait un cube incliné pour créer un dégagement
  translate([-e, 0, e]) 
    rotate([57, 0, 0]) 
      cube([2*l, 2*l, h + d]);
}
