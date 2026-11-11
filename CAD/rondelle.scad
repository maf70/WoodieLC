

// Diametre ext
dx=9;

// Diametre trou
dt=4.2;

// Epaisseur
de=1;



$fn=100;


difference(){
    cylinder(d=dx,h=de);
    translate([0,0,-de])cylinder(d=dt,h=3*de);
}
