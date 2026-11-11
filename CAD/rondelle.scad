// ============================================
// FICHIER : rondelle.scad
// DESCRIPTION : Modèle 3D d'une rondelle plate.
//               Utilisée pour répartir la charge sous une vis.
// ============================================

// ----------------------------
// 1. DIMENSIONS DE LA RONDELLE
// ----------------------------
// Diamètre extérieur de la rondelle (en mm)
dx = 9;

// Diamètre du trou central (en mm)
dt = 4.2;

// Épaisseur de la rondelle (en mm)
de = 1;

// ----------------------------
// 2. QUALITÉ DE RENDU
// ----------------------------
$fn = 100;


// ============================================
// CONSTRUCTION PRINCIPALE
// ============================================

difference() {
  // ----------------------------
  // Forme de base : cylindre (rondelle)
  // ----------------------------
  cylinder(d = dx, h = de);
  
  // ----------------------------
  // Évidement : trou central
  // ----------------------------
  // Trou traversant (3 fois l'épaisseur pour garantir la pénétration)
  translate([0, 0, -de]) 
    cylinder(d = dt, h = 3*de);
}
