
// Longueur totale
L=70;

// Diametres roue / vis
D=50;
d=20;

// Nombre de fente
n=13;

// Axe moteur
md=3.4;
mm=2.8;
ml=10;

// Vis
vp=30;
ve=1;

// Roulement
rd=3;
rl=10;

$fn=100;

module vis(diametre,filet,pas, longueur){
  a=360/(pas/0.1);
  ni=(longueur-filet/2-0.1)/0.1;
  for (i = [0:ni]) {
    hull() {
      translate([0,0,filet/2+(i+1)/10])rotate([0,0,a*(i+1)])cube([diametre,filet,filet], center=true);
      translate([0,0,filet/2+i/10])rotate([0,0,a*i])cube([diametre,filet,filet], center=true);
    }
  }
}

module arbre (diametre, meplat, longueur) {
  difference(){
    translate([0,0,-0.1]) cylinder(d=diametre, h=longueur);
    translate([-diametre/2,diametre/2-(md-mm),-0.2]) cube([diametre,diametre,2*longueur]);  
  } 
}

module fente (largeur, longueur){
  hull(){
    cylinder(d=largeur, h=10);
    translate([longueur-largeur,0,0]) cylinder(d=largeur, h=10);
  }
}

difference(){

  union() {
    cylinder(d=D, h=2);
    cylinder(d=d, h=5);
    cylinder(d=md*2, h=L+ve/2);
      
    vis ( d, 1, vp, L);
      
    cylinder(d=rd, h=L+rl);
    
  }
  
  for (i = [0:n]) {
    rotate([0,0,(360/n)*i])translate([D/2-5,0,-1])fente (2, 5);
  }
  
  arbre (md, mm, ml);
  
  //translate([0,0,50+5])cube([100,100,100], center=true);
  //translate([5,-50,-1])cube([100,100,100]);
  //rotate([0,0,90])translate([5,-50,-1])cube([100,100,100]);
  //rotate([0,0,180])translate([5,-50,-1])cube([100,100,100]);
  //rotate([0,0,270])translate([5,-50,-1])cube([100,100,100]);
  
}
