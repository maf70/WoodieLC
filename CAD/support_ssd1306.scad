// ============================================
// FICHIER : support_ssd1306.scad
// DESCRIPTION : Support pour écran OLED SSD1306 avec fixation.
//               Comprend des jambes de support, un plateau,
//               des emplacements pour écrous et des trous de fixation.
// ============================================

// ----------------------------
// 1. DIMENSIONS DES TROUS DE FIXATION
// ----------------------------
// Diamètre des trous (en mm)
trou_dia = 4.2;
// Distance entre les jambes (en mm)
trou_dis = 56;

// ----------------------------
// 2. DIMENSIONS DES ÉCROUS
// ----------------------------
// Diamètre de l'écrou (pour vis de 7mm, en mm)
ecrou_d = 8.6;
// Épaisseur de l'écrou (en mm)
ecrou_e = 3;

// ----------------------------
// 3. DIMENSIONS DE LA BASE
// ----------------------------
// Longueur de la base (en mm)
base_l = 10;
// Longueur horizontale de la jambe gauche (en mm)
base_Lh = 7;
// Longueur de la jambe droite depuis la base (en mm)
base_D_Lb = 27;
// Longueur de la jambe gauche depuis la base (en mm)
base_G_Lb = 15;

// ----------------------------
// 4. DIMENSIONS GÉNÉRALES
// ----------------------------
// Hauteur totale du support (en mm)
h = 25;

// Épaisseur du plateau supérieur (en mm)
dessus_e = 5;

// ----------------------------
// 5. DIMENSIONS DE L'ÉCRAN SSD1306
// ----------------------------
// Largeur de l'écran (en mm)
ecran_l = 28;
// Hauteur de l'écran (en mm)
ecran_h = 28.3;
// Épaisseur de l'écran (en mm)
ecran_e = 3;
// Marge autour de l'écran pour l logarithm (en mm)
ecran_s = 4;

// ----------------------------
// 6. DIMENSIONS DES VIS
// ----------------------------
// Diamètre des vis de fixation de l'écran (en mm)
vis_d = 1.5;

// ----------------------------
// 7. QUALITÉ DE RENDU
// ----------------------------
$fn = 50;


// ============================================
// CONSTRUCTION PRINCIPALE
// ============================================

difference() {
  // ----------------------------
  // Structure de base : jambes + plateau
  // ----------------------------
  union() {
    // Jambe Gauche : support vertical
    cube([base_Lh + base_G_Lb, base_l, h]);
    
    // Jambe Droite : support vertical (décalé)
    translate([0, trou_dis, 0]) 
      cube([base_Lh + base_D_Lb, base_l, h]);
    
    // Jambe Milieu : renfort central
    translate([base_Lh + base_D_Lb - base_l/2, base_l*1.5, 0]) 
      cube([base_l/2, base_l, h]);
    
    // Plateau supérieur : surface de support pour l'écran
    translate([0, 0, h - dessus_e]) 
      cube([base_Lh + base_D_Lb, base_l + trou_dis, dessus_e]);
  }
  
  // ----------------------------
  // Évidements : passage de vis et écrous (Droite)
  // ----------------------------
  // Passage de vis (jambe droite)
  translate([base_Lh, base_l/2, -0.1]) 
    cylinder(d = trou_dia, h = h + 5);
  
  // Emplacement pour écrou (jambe droite)
  translate([base_Lh, base_l/2, h - ecrou_e]) 
    cylinder(d = ecrou_d, h = h, $fn = 6);
  
  // ----------------------------
  // Évidements : passage de vis et écrous (Gauche)
  // ----------------------------
  // Passage de vis (jambe gauche)
  translate([0, trou_dis, 0]) 
    translate([base_Lh, base_l/2, -0.1]) 
      cylinder(d = trou_dia, h = h + 5);
  
  // Emplacement pour écrou (jambe gauche)
  translate([0, trou_dis, 0]) 
    translate([base_Lh, base_l/2, h - ecrou_e]) 
      cylinder(d = ecrou_d, h = h, $fn = 6);
  
  // ----------------------------
  // Évidement : emplacement de l'écran
  // ----------------------------
  // Emplacement pour l'écran SSD1306
  translate([(base_Lh + base_D_Lb - ecran_h)/2, (base_l + trou_dis - ecran_l)/2 + 7, h - ecran_e]) 
    cube([ecran_h, ecran_l, ecran_e * 2]);
  
  // Évidement sous l'écran (pour câbles ou composants)
  translate([(base_Lh + base_D_Lb - ecran_h)/2, (base_l + trou_dis - ecran_l)/2 + 7 + ecran_s]) 
    cube([ecran_h - ecran_s, ecran_l - 2*ecran_s, h * 2]);
  
  // ----------------------------
  // Trous de fixation pour l'écran
  // ----------------------------
  // 4 trous aux coins de l'écran pour fixation
  translate([(base_Lh + base_D_Lb - ecran_h)/2 + 2, (base_l + trou_dis - ecran_l)/2 + 7 + 2, 0]) 
    union() {
      // Trou 1 (coin supérieur gauche de l'écran)
      cylinder(d = vis_d, h = 2*h);
      
      // Trou 2 (coin supérieur droit de l'écran)
      translate([ecran_h - 4, 0, 0]) 
        cylinder(d = vis_d, h = 2*h);
      
      // Trou 3 (coin inférieur gauche de l'écran)
      translate([0, ecran_l - 4, 0]) 
        cylinder(d = vis_d, h = 2*h);
      
      // Trou 4 (coin inférieur droit de l'écran)
      translate([ecran_h - 4, ecran_l - 4, 0]) 
        cylinder(d = vis_d, h = 2*h);
    }
  
  // ----------------------------
  // Évidement : accès au connecteur de la sonde thermique
  // ----------------------------
  // Ouverture pour accéder au connecteur de la sonde de température
  translate([base_Lh - trou_dia/2 + 2 - 9, base_l/2 + trou_dia/2 + 0.5, -1]) 
    cube([9, 17.5, 2*h]);
  
  // ----------------------------
  // CODE COMMENTÉ (tests d'impression)
  // ----------------------------
  // Test impression gabarit
  // translate([-1,-1,-dessus_e-0.1])cube([trou_dis*2,trou_dis*2,h]);
  // translate([-1,-1,h-dessus_e+0.1])cube([trou_dis*2,trou_dis*2,h]);
  
  // Test impression réservation écrou
  // translate([-1,-1,-dessus_e])cube([trou_dis*2,trou_dis*2,h]);
  // translate([13,-1,0])cube([trou_dis*2,trou_dis*2,2*h]);
  // translate([-1,11,0])cube([trou_dis*2,trou_dis*2,2*h]);
}
