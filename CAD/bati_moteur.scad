// ============================================
// FICHIER : bati_moteur.scad
// DESCRIPTION : Modèle 3D d'un bâti mécanique avec un moteur intégré.
//               Comprend un socle, un bâti principal, et des trous de fixation.
// ============================================

// ----------------------------
// 1. DIMENSIONS DU MOTEUR
// ----------------------------
// Diamètre du moteur (en mm)
mD = 12;
// Épaisseur du moteur (en mm)
mE = 10;
// Longueur du moteur (en mm)
mL = 12;

// ----------------------------
// 2. DIMENSIONS DU BÂTI
// ----------------------------
// Hauteur du bâti (en mm)
bH = 30;
// Longueur des rebords latéraux du bâti (en mm)
bL = 2;
// Épaisseur des parois du bâti (en mm)
bT = 3;

// ----------------------------
// 3. DIMENSIONS DU SOCLE
// ----------------------------
// Longueur du socle (en mm)
sL = 30;
// Épaisseur du socle (en mm)
sE = 2;
// Diamètre des trous de fixation dans le socle (en mm)
sT = 2.8;

// ----------------------------
// 4. QUALITÉ DE RENDU
// ----------------------------
// Nombre de fragments pour les cercles (plus le nombre est élevé, plus le rendu est lisse)
$fn = 100;

// ----------------------------
// 5. MODULE : MOTEUR
// ----------------------------
// Crée un moteur cylindrique avec deux encoches rectangulaires (pour les axes ou fixations).
// Paramètres :
//   - d : diamètre du cylindre
//   - l : longueur du cylindre
//   - e : épaisseur des encoches
module moteur(d, l, e) {
  difference() {
    // Crée un cylindre horizontal (roté de 90° autour de l'axe Y)
    rotate([0, 90, 0]) cylinder(d = d, h = l, center = true);

    // Soustrait deux cubes pour créer des encoches dans le cylindre :
    //   - Encoche supérieure
    translate([0, d/2 + e/2, 0]) cube([2*l, d, d], center = true);
    //   - Encoche inférieure
    translate([0, -d/2 - e/2, 0]) cube([2*l, d, d], center = true);
  }
}

// ----------------------------
// 6. CONSTRUCTION PRINCIPALE : BÂTI + MOTEUR
// ----------------------------
difference() {
  // Union des éléments de base : socle + bâti
  union() {
    // Socle : cube plat centré, avec une épaisseur sE
    translate([0, 0, sE/2]) cube([mL, sL, sE], center = true);

    // Bâti principal : cube vertical, centré sur le socle
    // Largeur = mE (épaisseur du moteur) + 2*bL (rebords latéraux)
    // Hauteur = bH (hauteur du bâti) + mD (diamètre du moteur)
    translate([0, 0, bH/2 + mD/2]) cube([mL, mE + 2*bL, bH + mD], center = true);
  }

  // Soustrait les éléments suivants pour créer des vides/tranches :

  // 1. Emplacement du moteur : soustrait le volume du moteur à l'intérieur du bâti
  //    Positionné à une hauteur bH (hauteur du bâti sans le moteur)
  translate([0, 0, bH]) moteur(mD, mL + 2, mE);

  // 2. Fente verticale dans le bâti (pour le passage d'un axe ou d'une courroie ?)
  //    Largeur = 1 mm, hauteur = 2*bH (traverse tout le bâti)
  translate([0, 0, bH + sE]) cube([mL + 2, 1, 2*bH], center = true);

  // 3. Trous de fixation dans le socle (deux trous alignés sur l'axe Y) :
  //    - Trou supérieur : positionné à mi-chemin entre le moteur et le bord du socle
  translate([0, mE/2 + bL + (sL/2 - mE/2 - bL)/2, 0]) cylinder(d = sT, h = 4*sE, center = true);
  //    - Trou inférieur : symétrique du trou supérieur
  translate([0, -(mE/2 + bL + (sL/2 - mE/2 - bL)/2), 0]) cylinder(d = sT, h = 4*sE, center = true);

  // 4. Trous horizontaux dans le bâti (pour le passage de câbles ou d'axes) :
  //    - Trou supérieur : centré à une hauteur bH + mD/2 + 3 (au-dessus du moteur)
  //      Roté de 90° pour être horizontal (le long de l'axe X)
  translate([0, 0, bH + mD/2 + 3]) rotate([90, 0, 0]) cylinder(d = bT, h = 100, center = true);
  //    - Trou inférieur : symétrique du trou supérieur, en dessous du moteur
  translate([0, 0, bH - mD/2 - 3]) rotate([90, 0, 0]) cylinder(d = bT, h = 100, center = true);
}
