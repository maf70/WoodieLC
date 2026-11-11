// ============================================
// FICHIER : vis.scad
// DESCRIPTION : Modèle 3D d'une vis à filetage et fentes radiales.
//               Comprend un arbre central avec méplat pour l'entraînement.
// ============================================

// ----------------------------
// 1. DIMENSIONS GÉNÉRALES
// ----------------------------
// Longueur totale de la vis (en mm)
L = 70;

// ----------------------------
// 2. DIMENSIONS DE LA ROUE/DE LA VIS
// ----------------------------
// Diamètre extérieur de la roue (en mm)
D = 50;
// Diamètre intérieur (corps de la vis, en mm)
d = 20;

// ----------------------------
// 3. DIMENSIONS DES FENTES
// ----------------------------
// Nombre de fentes radiales (réparties sur 360°)
n = 13;

// ----------------------------
// 4. DIMENSIONS DE L'AXE MOTEUR
// ----------------------------
// Diamètre de l'axe moteur (en mm)
md = 3.4;
// Largeur du méplat sur l'axe (en mm)
mm = 2.8;
// Longueur de l'axe (en mm)
ml = 10;

// ----------------------------
// 5. DIMENSIONS DU FILETAGE
// ----------------------------
// Pas de la vis (distance entre deux filets, en mm)
vp = 30;
// Épaisseur du filet (en mm)
ve = 1;

// ----------------------------
// 6. DIMENSIONS DU ROULEMENT
// ----------------------------
// Diamètre du roulement (en mm)
rd = 3;
// Longueur du roulement (en mm)
rl = 10;

// ----------------------------
// 7. QUALITÉ DE RENDU
// ----------------------------
// Nombre de fragments pour les cercles et courbes
$fn = 100;


// ============================================
// MODULES
// ============================================

// ----------------------------
// MODULE : VIS (FILETAGE HÉLICOÏDAL)
// ----------------------------
// Crée un filetage hélicoïdal sur un cylindre.
// Paramètres :
//   - diametre : diamètre du cylindre de base (en mm)
//   - filet : épaisseur du filet (en mm)
//   - pas : pas de la vis (distance entre deux spires, en mm)
//   - longueur : longueur totale du filetage (en mm)
//
// Principe :
//   - Le filet est créé par une série de cubes inclinés et tournés
//   - Chaque cube représente une section du filet
//   - L'angle de rotation (a) dépend du pas : plus le pas est petit, plus l'angle par mm est grand
module vis(diametre, filet, pas, longueur) {
  // Calcule l'angle de rotation par incrément de 0.1 mm
  // (360° / (pas / 0.1)) = angle pour avancer de 0.1 mm verticalement
  a = 360 / (pas / 0.1);
  
  // Calcule le nombre d'itérations nécessaires pour couvrir la longueur
  // (longueur - filet/2 - 0.1) / 0.1 = nombre de segments de 0.1 mm
  ni = (longueur - filet/2 - 0.1) / 0.1;
  
  // Boucle pour créer chaque segment du filet
  for (i = [0:ni]) {
    hull() {
      // Cube supérieur : positionné à (i+1)*0.1 mm, tourné de a*(i+1) degrés
      translate([0, 0, filet/2 + (i+1)/10]) 
        rotate([0, 0, a*(i+1)]) 
          cube([diametre, filet, filet], center = true);
      
      // Cube inférieur : positionné à i*0.1 mm, tourné de a*i degrés
      translate([0, 0, filet/2 + i/10]) 
        rotate([0, 0, a*i]) 
          cube([diametre, filet, filet], center = true);
    }
  }
}


// ----------------------------
// MODULE : ARBRE (AVEC MÉPLAT)
// ----------------------------
// Crée un arbre cylindrique avec un méplat pour l'entraînement.
// Paramètres :
//   - diametre : diamètre de l'arbre (en mm)
//   - meplat : largeur du méplat (en mm)
//   - longueur : longueur de l'arbre (en mm)
module arbre(diametre, meplat, longueur) {
  difference() {
    // Cylindre de base (légèrement étendu pour éviter les artefacts)
    translate([0, 0, -0.1]) 
      cylinder(d = diametre, h = longueur);
    
    // Soustrait un cube pour créer le méplat
    // Positionné pour créer une face plane sur le cylindre
    translate([-diametre/2, diametre/2 - (md - mm), -0.2]) 
      cube([diametre, diametre, 2*longueur]);
  }
}


// ----------------------------
// MODULE : FENTE (RADIALE)
// ----------------------------
// Crée une fente radiale (encoche dans la roue).
// Paramètres :
//   - largeur : largeur de la fente (en mm)
//   - longueur : longueur de la fente (en mm)
module fente(largeur, longueur) {
  hull() {
    // Cylindre de départ
    cylinder(d = largeur, h = 10);
    // Cylindre déplacé pour créer l'effet de fente
    translate([longueur - largeur, 0, 0]) 
      cylinder(d = largeur, h = 10);
  }
}


// ============================================
// CONSTRUCTION PRINCIPALE
// ============================================

difference() {
  // ----------------------------
  // Union des éléments de base
  // ----------------------------
  union() {
    // Base de la roue (disque supérieur)
    cylinder(d = D, h = 2);
    
    // Corps de la vis (cylindre central)
    cylinder(d = d, h = 5);
    
    // Axe central (cylindre traversant toute la longueur)
    cylinder(d = md * 2, h = L + ve/2);
    
    // Filetage hélicoïdal sur le corps de la vis
    vis(d, 1, vp, L);
    
    // Roulement (cylindre central pour le guidage)
    cylinder(d = rd, h = L + rl);
  }
  
  // ----------------------------
  // Évidements : fentes radiales
  // ----------------------------
  // Crée n fentes réparties uniformément sur 360°
  // Chaque fente est tournée de (360/n)*i degrés
  for (i = [0:n]) {
    rotate([0, 0, (360/n)*i]) 
      translate([D/2 - 5, 0, -1]) 
        fente(2, 5);
  }
  
  // ----------------------------
  // Évidement : arbre avec méplat
  // ----------------------------
  // Soustrait l'arbre avec méplat au centre
  arbre(md, mm, ml);
  
  // ----------------------------
  // CODE COMMENTÉ (anciens tests)
  // ----------------------------
  // translate([0,0,50+5])cube([100,100,100], center=true);
  // translate([5,-50,-1])cube([100,100,100]);
  // rotate([0,0,90])translate([5,-50,-1])cube([100,100,100]);
  // rotate([0,0,180])translate([5,-50,-1])cube([100,100,100]);
  // rotate([0,0,270])translate([5,-50,-1])cube([100,100,100]);
}
