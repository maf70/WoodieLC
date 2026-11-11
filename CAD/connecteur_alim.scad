
d=8;
t=2.8;

e=2;
l=15;
h=15;

$fn=50;


difference(){
  cube([l+2*e,2*l+2*e,h+d]);
  translate([e,l+e,e])cube([l,2*l,h+d]);
  translate([e,-l,e])cube([l,2*l,h+d]);
    
  translate([e+l/2,0,h])rotate([-90,0,0])cylinder(d=d, h=100);
  translate([e/2+l/2,0,h])cube([e,100,h]);
    
    
  translate([e+l/2,6,-h])rotate([0,0,0])cylinder(d=t, h=100);
  translate([e+l/2,26,-h])rotate([0,0,0])cylinder(d=t, h=100);
  
  translate([-e,0,e]) rotate([57,0,0]) cube([2*l,2*l,h+d]);
}
