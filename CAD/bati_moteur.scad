
// Moteur 
mD=12;
mE=10;
mL=12;

// Bati
bH=30;
bL=2;
bT=3;

// Socle
sL=30;
sE=2;
sT=2.8;


$fn=100;

module moteur(d, l, e){
  difference(){
    rotate([0,90,0]) cylinder(d=d, h=l, center=true);

    translate([0,d/2+e/2,0]) cube([2*l,d,d], center=true);
    translate([0,-d/2-e/2,0]) cube([2*l,d,d], center=true);
  }
}

difference(){
  union(){
    translate([0,0,sE/2]) cube([mL,sL,sE], center=true);
    translate([0,0,bH/2+mD/2]) cube([mL,mE+2*bL,bH+mD], center=true);
  }
  
  translate([0,0,bH]) moteur(mD, mL+2, mE);
  translate([0,0,bH+sE]) cube([mL+2,1,2*bH], center=true);
  
  translate([0,mE/2+bL+(sL/2-mE/2-bL)/2,0]) cylinder(d=sT, h= 4*sE, center=true);
  translate([0,-(mE/2+bL+(sL/2-mE/2-bL)/2),0]) cylinder(d=sT, h= 4*sE, center=true);
  
  translate([0,0,bH+mD/2+3]) rotate([90,0,0]) cylinder(d=bT, h=100, center=true);
  translate([0,0,bH-mD/2-3]) rotate([90,0,0]) cylinder(d=bT, h=100, center=true);
  
}

