
// Positionnement des trous du pcb
trou_dia=2.5;
trou_dis_g=25;
trou_dis=56;
trou_dis_d=19;
trou_dis_b=5.5;

trou_fixation=3;

bord=2;
fond=1.5;

h=16.5;
pcb_l=100.5;
pcb_L=100.5;
pcb_e=1.5;

languette=10;

support_l=10;
support_L=18;
support_trou=2.5;

difference(){
    union(){
      translate([-bord,-bord,0])cube([pcb_L+2*bord,pcb_L+2*bord,h+pcb_e]);
    }
    
    // evidement PCB
    translate([0,0,h])cube([pcb_L,pcb_L,2*h]);
    
    // evidement interieur
    translate([bord,bord,fond])cube([pcb_L-2*bord,pcb_L-2*bord,2*h]);
    translate([10*bord,10*bord,-1])cube([pcb_L-20*bord,pcb_L-20*bord,2*h]);
    
    // Evidement languette
    translate([10,pcb_L-5*bord,3*fond])cube([languette,10*bord,2*h]);
    
    // Evidement relais
    translate([10+languette+5,pcb_L-12,-1])cube([pcb_l,10*bord,2*h]);
    
    // Evidement rtc
    translate([-2*bord,-2*bord,-1])cube([24.5,15,2*h]);
    
    // Evidement secteur
    translate([pcb_l-10+2*bord,22,-1])cube([10,55,2*h]);
    
    // Evidement nano
    translate([-2*bord,30,3*fond])cube([10,40,2*h]);
    
    // trous de fixation
    translate([12,25,-fond])cylinder(d=trou_fixation, h=3*fond,$fn=20);
    translate([pcb_L-8,12,-fond])cylinder(d=trou_fixation, h=3*fond,$fn=20);
    translate([12,pcb_L-8,-fond])cylinder(d=trou_fixation, h=3*fond,$fn=20);
    translate([pcb_L-12,pcb_L-25,-fond])cylinder(d=trou_fixation, h=3*fond,$fn=20);
    
}

// Renford carré au fond
difference(){
      translate([9*bord,9*bord,0])cube([pcb_L-18*bord,pcb_L-18*bord,3*fond]);
    translate([10*bord,10*bord,-1])cube([pcb_L-20*bord,pcb_L-20*bord,2*h]);
}

// Renfords transverse
translate([0,8*bord,0]) cube([pcb_l,2*bord,3*fond]);
translate([0,10*bord+pcb_L-20*bord,0]) cube([pcb_l,2*bord,3*fond]);

// Support a visser
translate([trou_dis_g-5+0.5,0,0]) difference(){
  // Bloc sous les vis  
  union(){
    cube([support_l,support_L,h]);
    translate([trou_dis,0,0])cube([support_l,support_L,h]);
  }
  // T vis  
  translate([support_l/2,trou_dis_b,5])cylinder(d=support_trou,h=h,$fn=20);
  translate([trou_dis+support_l/2,trou_dis_b,5])cylinder(d=support_trou,h=h,$fn=20);
}

// Languette
translate([11,pcb_L,0])cube([languette-2,bord,pcb_e+h+0.3]);
hull(){
  translate([11,pcb_L-bord,pcb_e+h+0.3])cube([languette-2,2*bord,0.1]);
  translate([11,pcb_L,pcb_e+h+0.3+h])cube([languette-2,bord,0.1]);
}





