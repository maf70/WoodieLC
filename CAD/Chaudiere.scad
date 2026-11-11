
// Corps de chaudiere
cl=50;
cL=60;
cH=90;
ce=2;
cf=10;

// Axe
ad=3.5;
ah=30;
aD=21;

// Sonde
sd=7;

// Foyer
fd=37;
fh=36;
fp=20;

// Tuyau
td=15;

// Charnieres
cht=2.5;
chh=5;

// Pieds
pil=10;
pit=2.8;

$fn=50;

module charniere_1 (_h) {
  
  _l=ce+cht;
  _t=cht;
    
  // Charniere
  translate([0,-_l/2,0]) difference() {
     union() {
      cylinder(d=_l,h=chh);
      translate([-_l/2,0,0])cube([_l,_l/2,chh]);
      translate([0,0,0]) cylinder(d=_t,h=2*chh-_t/2+_h);
      translate([0,0,2*chh-_t/2+_h]) sphere(d=_t);
      }   
    rotate([-20,0,0])translate([0,0,-chh/2])cube([2*_l,2*_l,chh+2], center=true);
    }
}


module charniere_2 (_a) {
  
  _l=ce+cht;
  _t=cht+0.5;

  // Charniere
  translate([0,-_l/2,chh]) rotate([0,0,_a])difference() {
     union() {
      cylinder(d=_l,h=chh);
      translate([-_l/2,0,0])cube([_l,2*_l,chh]);
      }   
      translate([0,0,0]) cylinder(d=_t,h=2*chh-_t/2);
    }
}

module pied (_l) {

  translate([0,0,0]) rotate([0,0,0])difference() {
     union() {
      //cylinder(d=_l,h=chh);
      translate([0,-_l,0])cube([_l,_l+cL/2,ce]);
      rotate([-8,0,0])translate([0,-_l*0.4,0])cube([_l,_l,_l*2]);
      }   
      translate([0,0,-20])cube([40,40,40], center=true);
      translate([_l/2,-_l/2- _l*0.2,-1]) cylinder(d=pit,h=_l);
    }
}


if (1) difference(){
  union(){
    difference(){
      union(){
        // Corps  
        cube([cl,cL,cH]);
        translate([cl/2,cL,cH-cl/3]) rotate([90,0,0]) cylinder(d=cl,h=cL);
        // Dessus  
        translate([0,0,cH])cube([cl,cL,ce]);
        // Tuyau de sortie
        translate([cl/2,2*cL/3,cH+ce]) cylinder(d=td+2*ce,h=10);
          
        pied(pil);
        translate([cl-pil,0,0]) pied(pil);
        translate([pil,cL,0])rotate([0,0,180])pied(pil);
        translate([cl,cL,0])rotate([0,0,180])pied(pil);
      }
 
      // Interieur
      translate([ce,ce,fp+ce+sd])cube([cl-2*ce,cL-2*ce,cH-fp-cl/2]);
      translate([cl/2,cL-ce,cH-cl/3]) rotate([90,0,0]) cylinder(d=cl-2*ce,h=cL-2*ce);
      // Foyer
      translate([cl/2,cL/2,ce+2*sd+2]) cylinder(d=fd,h=fh);
  
      // Tuyau de sortie
      translate([cl/2,2*cL/3,cH-ce]) cylinder(d=td,h=fh);
      
      // Sonde + alim
      translate([-ce,cL/2,ce+2*sd]) rotate([0,90,0]) cylinder(d=sd,h=fd/2+cl/2+ce);
      translate([3*cl/4,cL+ce,fp+2*ce+sd]) rotate([90,0,0]) cylinder(d=3,h=fd/2+cl/2+ce);
      
      // Porte
      translate([-fd/2+cl/2,-ce,ce+ah+aD/2])cube([fd,3*ce,fh]);
      
    
    }
    
    // Avant avec support vis  
    cube([cl,cf,ah+aD/2+ce]);
    
    translate([cht/2+ce/2 + fd + (cl-fd)/2 + ce/2,0,ce+ah+aD/2]) union(){
      charniere_1(0);
      translate([0,0,fh-2*chh])charniere_1(0);
    }
    
    translate([-cht/2-ce/2 + (cl-fd)/2 - ce/2,0,ce+ah+aD/2+fd/2-chh]) charniere_1(-ce);
  }
  
  // Axe vis
  translate([cl/2,cf+ce,ah]) rotate([90,0,0]) cylinder(d=ad,h=2*cf);
  translate([cl/2,2*ce,ah]) rotate([90,0,0]) cylinder(d=aD,h=2*cf);
      
  translate([cl/2,cL+ce,-cl/3-sd/2]) rotate([90,0,0]) cylinder(d=cl,h=cL+2*ce);
  translate([-ce,cL/2,-cl/3-sd/2]) rotate([0,90,0]) cylinder(d=cl,h=cL+2*ce);
  

}


if (1) color("green") union() {    
    translate([cht/2+ce/2 + fd + (cl-fd)/2 + ce/2,0,ce+ah+aD/2]) union(){
      charniere_2(90);
      translate([0,0,fh-2*chh])charniere_2(90);
    }
    
    translate([-cht/2-ce/2 + (cl-fd)/2 - ce/2,0,ce+ah+aD/2+fd/2-chh]) charniere_2(-90);
    
    // Porte
    translate([-fd/2+cl/2-ce/4,-ce,ce+ah+aD/2])cube([fd+ce/2,ce,fh]);
}
