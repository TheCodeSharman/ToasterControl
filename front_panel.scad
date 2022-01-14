knob_hole=40.32;
fit_tolerance=0.1;
panel_thickness=1.2;
snap_fit_width=0.6;
prong_gap=knob_hole/2.5;

union() {
    difference() {
        union() {
            cylinder(h=panel_thickness+fit_tolerance,d=knob_hole-fit_tolerance);
        
            translate([0,0,0])
               cylinder(h=snap_fit_width,d1=knob_hole+snap_fit_width,
                    d2=knob_hole);
        }
        translate([0,0,-2])
            cylinder(h=6,d=knob_hole-fit_tolerance-2);
        translate([0,0,1.5])
        cube(size=[knob_hole+10,prong_gap,6],center=true);
         translate([0,0,1.5]) rotate([0,0,90])
        cube(size=[knob_hole+10,prong_gap,6],center=true);
        
    }
    
    translate([0,0,+panel_thickness])
        cylinder(h=1,d=knob_hole+20);
    

}
