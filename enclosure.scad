$fa = 1;
$fs = 0.5;


board_pcb_thickness = 1.55;
probe_conn_height=19.0;

board_length = 87.8; // length including terminal connections
board_width = 51.2;
board_height = board_pcb_thickness+probe_conn_height; // height including tallest component
board_clearance_h = 5;
board_clearance_w = 15;

board_pcb_length=81.5;

probe_conn_width=15.2;

probe_conn_length=19.8;
probe_conn_offset_y=34;
probe_conn_offset_x=9.5;

ssr_conn_width=8.5;
ssr_conn_height=12.0;
ssr_conn_length=11.6;
ssr_conn_offset_y=30.8;
ssr_conn_offset_x=-8.5;

ssr_plug_width=15.1;
ssr_plug_height=8.9;
ssr_plug_length=11.4;
ssr_plug_offset_x=-3; // TODO: measure

micro_height=16.9;
micro_length=50.2;
micro_width=20.8;
micro_offset_y=-15.6;

wall_thickness = 2;
lid_thickness = 2;
fit_tolerance = 0.5;
pcb_mount_screw_offset = 34;
lid_screw_offset = 4.7; 

usb_offset_h = 16; // relative to the bottom edge of the PCB board
usb_offset_w = 0; // relative to the centre of the PCB board
usb_width = 12;
usb_height = 6;

front_screw_offset_h= usb_offset_h - 11;
front_screw_offset_w=0;
front_screw_gap=45;

screw1 = 2.6;
screw1_length=10;
nut1_width=5.5;
nut1_depth=1.7;

mount_width=nut1_width + fit_tolerance + 2.0;
mount_height=mount_width-0.65 - fit_tolerance;
mount_length=screw1_length+2;

// make overall outside dimension enclose pcb assembly
height = board_height + board_clearance_h 
        + wall_thickness + lid_thickness + fit_tolerance;
width = 67.5 - fit_tolerance*2;
length = board_length + wall_thickness*2 + fit_tolerance*2;

module pcb_board() {
    // pcb board
    color("white")
        translate([0,-(board_length - board_pcb_length)/2,0])
       cube([board_width,board_pcb_length,board_pcb_thickness],true);
}

module micro_board() {
     // micro proccesor daughter board
    color("cornflowerblue")
        translate([0,micro_offset_y,micro_height/2+board_pcb_thickness])
            cube([micro_width,micro_length,micro_height],true);
}

module probe_conn() {
    color("yellow")
        translate([ probe_conn_offset_x,
                    probe_conn_offset_y,
                    probe_conn_height/2+board_pcb_thickness ])
        cube([probe_conn_width,probe_conn_length,probe_conn_height],true);
}

module ssr_conn() {
    color("green")
        translate([ ssr_conn_offset_x,
                    ssr_conn_offset_y,
                    ssr_conn_height/2+board_pcb_thickness])
            cube([ssr_conn_width,ssr_conn_length,ssr_conn_height],true);
}

module ssr_plug() {
    translate([ ssr_conn_offset_x+ssr_plug_offset_x,
                ssr_conn_offset_y,
                ssr_plug_height/2+ssr_conn_height+board_pcb_thickness ])
        union() {
            cable_angle=18;
            color("green")
                cube([ssr_plug_width,ssr_plug_length,ssr_plug_height],true);
            color("red")
                translate([0,-2,0]) rotate([0,-90 + cable_angle,0]) cylinder(100,r=1.4/2);
            color("black")
                translate([0,2,0]) rotate([0,-90 + cable_angle,0]) cylinder(100,r=1.4/2);
            color([0,0,0,0.2])
                rotate([0,-90+cable_angle,0]) translate([0,0,48]) cube([4,6,90],true);
            
        }
}

module pcb_board_assembly() {
    translate([0,board_length/2,board_pcb_thickness/2])
    union() {
        translate([0,0,board_height/2])
        *cube([board_width,board_length,board_height],true);
        
        pcb_board();
        micro_board();
        probe_conn();
        ssr_conn();
        ssr_plug();
    }
}

module offset_from_corners( offset, w, l ) {
    union() {
        translate([-w/2+offset,-l/2+offset,0]) 
            children();
        
        translate([w/2-offset,-l/2+offset,0]) 
            children();
        
        translate([-w/2+offset,l/2-offset,0]) 
            children();
        
        translate([w/2-offset,l/2-offset,0])
            children();
    }
}

module screw_hole(h,d) {
    difference() {
        cylinder(h,r=d);
        cylinder(h,r=d/2);
    }
}

module usb_connector() {
    translate([ 0,-wall_thickness/2-fit_tolerance,usb_offset_h ])
        cube([usb_width,wall_thickness,usb_height],true);
}

module front_mount_screws() {
   
    union() {
        translate([ -front_screw_gap/2 + screw1/2 + front_screw_offset_w,
                    -fit_tolerance, front_screw_offset_h ]) {
            rotate([90,0,0])
                cylinder(screw1_length,r=screw1/2);
        }
        
        translate([ front_screw_gap/2 - screw1/2 + front_screw_offset_w,
                    -fit_tolerance, front_screw_offset_h ]) {
            rotate([90,0,0])
                cylinder(screw1_length,r=screw1/2);
        }
    }
}

module front_mount_hut_holder() {
    difference() {
        cube([mount_width,mount_length,mount_height],true);
        cube([nut1_width+fit_tolerance,nut1_depth+fit_tolerance,mount_height],true);
        translate([0,2.5,-mount_height/2]) rotate([12,0,0]) translate([0,0,-1]) cube([mount_width*2,mount_length*2,4],true);
    }
}

module front_mount_nut_holders() {
    
    translate([0,mount_length/2-wall_thickness,0])
    difference() {
        union() {
                translate([ -front_screw_gap/2 + screw1/2 + front_screw_offset_w,
                            -fit_tolerance, front_screw_offset_h ]) 
                    front_mount_hut_holder();
                
                translate([ front_screw_gap/2 - screw1/2 + front_screw_offset_w,
                            -fit_tolerance, front_screw_offset_h ]) 
                    front_mount_hut_holder();
            }
        front_mount_screws();
    }
}

module body() {
    union() {
        difference() {
            translate([0,length/2-wall_thickness-fit_tolerance,
                       height/2 - wall_thickness - board_clearance_h]) 
                difference() {
                    cube([width,length,height],true);
                    translate([0,0,wall_thickness])
                        cube( [ width-wall_thickness*2,
                                length-wall_thickness*2,
                                height],true);    
                }
        
            usb_connector();
            front_mount_screws();
        }
        front_mount_nut_holders();
        pcb_mounts();
        lid_screws();
    }
}

module pcb_mounts() {
    translate([0,pcb_mount_screw_offset + board_length/2,
        -board_clearance_h])
            screw_hole(board_clearance_h,screw1);
    support_thickness=2;
    translate([0,support_thickness/2,
        -board_clearance_h/2])
            cube([width,support_thickness,board_clearance_h],true);
}

module lid_screws() {
    screw_height = height - wall_thickness - lid_thickness;
    translate([0,board_length/2,-board_clearance_h])
        offset_from_corners(lid_screw_offset,width,length)
            screw_hole(screw_height,screw1);
}

module lid(explode) {
    hole_offset = lid_screw_offset-wall_thickness;
    lid_width = width-wall_thickness*2-fit_tolerance*2;
    lid_length = length-wall_thickness*2-fit_tolerance*2;
    
    translate([0,0,explode]) 
        difference() {
            translate([0,lid_length/2,board_height+lid_thickness/2 + fit_tolerance]) {
                            
                difference() {
                    cube([ lid_width,
                           lid_length,
                           lid_thickness-fit_tolerance ],true);
                    
                    offset_from_corners(hole_offset,lid_width,lid_length)
                        translate([0,0,-lid_thickness/2])
                        cylinder(lid_thickness,r=screw1/2);
                }
            }
            translate([0,board_length/2,board_pcb_thickness/2])
                ssr_plug();
        }
}

union() {
    *pcb_board_assembly();
    body();
    *lid(0);
}
