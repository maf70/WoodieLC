


trou_dia=4.2;
trou_dis=56;

// Screw 7mm
ecrou_d=8.6;
ecrou_e=3;

base_l=10;
base_Lh=7;
base_D_Lb=27;
base_G_Lb=15;

h=25;

dessus_e=5;

ecran_l=28;
ecran_h=28.3;
ecran_e=3;
ecran_s=4;

vis_d=1.5;


$fn=50;

difference(){
  union(){
    // Jambe Gauche
    cube([base_Lh+base_G_Lb,base_l,h]);
    // Jambe Droite
    translate([0,trou_dis,0])cube([base_Lh+base_D_Lb,base_l,h]);
    // Jambe milieu
    translate([base_Lh+base_D_Lb-base_l/2,base_l*1.5,0])cube([base_l/2,base_l,h]);
    // Plateau
    translate([0,0,h-dessus_e])cube([base_Lh+base_D_Lb,base_l+trou_dis,dessus_e]);  
  }
  
  // Passage de vis et emplacement ecrou Droite
  translate([base_Lh,base_l/2,-0.1])cylinder(d=trou_dia,h=h+5);
  translate([base_Lh,base_l/2,h-ecrou_e])cylinder(d=ecrou_d,h=h,$fn=6);
  
  // Passage de vis et emplacement ecrou Droite
  translate([0,trou_dis,0])translate([base_Lh,base_l/2,-0.1])cylinder(d=trou_dia,h=h+5);
  translate([0,trou_dis,0])translate([base_Lh,base_l/2,h-ecrou_e])cylinder(d=ecrou_d,h=h,$fn=6);
  
  // empladcement ecran
  translate([(base_Lh+base_D_Lb-ecran_h)/2,(base_l+trou_dis-ecran_l)/2+7,h-ecran_e])cube([ecran_h,ecran_l,ecran_e * 2]);
  // evidement sous ecran
  translate([(base_Lh+base_D_Lb-ecran_h)/2,(base_l+trou_dis-ecran_l)/2+7+ecran_s,])cube([ecran_h-ecran_s,ecran_l-2*ecran_s,h * 2]);
  
  // Trous de vis pour fixation ecran
  translate([(base_Lh+base_D_Lb-ecran_h)/2+2,(base_l+trou_dis-ecran_l)/2+7+2,0])union(){
    cylinder(d=vis_d, h=2*h);
    translate([ecran_h-4,0,0])cylinder(d=vis_d, h=2*h);
    translate([0,ecran_l-4,0])cylinder(d=vis_d, h=2*h);
    translate([ecran_h-4,ecran_l-4,0])cylinder(d=vis_d, h=2*h);
  }
  
  // evidement pour acces connecteur sonde thermo
  translate([base_Lh-trou_dia/2+2-9,base_l/2+trou_dia/2+0.5,-1])cube([9,17.5,2*h]);
  
  // Test impression gabari
  //translate([-1,-1,-dessus_e-0.1])cube([trou_dis*2,trou_dis*2,h]);
  //translate([-1,-1,h-dessus_e+0.1])cube([trou_dis*2,trou_dis*2,h]);
  // Test impression reservation ecrou
  //translate([-1,-1,-dessus_e])cube([trou_dis*2,trou_dis*2,h]);
  //translate([13,-1,0])cube([trou_dis*2,trou_dis*2,2*h]);
  //translate([-1,11,0])cube([trou_dis*2,trou_dis*2,2*h]);

}


