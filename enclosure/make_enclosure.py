"""
Creates an enclosure for the clearprep
"""
from py2scad import *
from clearprep_enclosure import Clearprep_Enclosure

INCH2MM = 25.4

create_dxf = True 

# Inside dimensions
#x,y,z = 2.61*INCH2MM, 3.0*INCH2MM, 3.0*INCH2MM
x,y,z = 2.61*INCH2MM, 3.0*INCH2MM, 2.5*INCH2MM  
wall_thickness = 3.0 


# Create enclosure parameters
params = {
        'inner_dimensions'        : (x,y,z), 
        'wall_thickness'          : wall_thickness, 
        'lid_radius'              : 0.25*INCH2MM,  
        'top_x_overhang'          : 0.16*INCH2MM,
        'top_y_overhang'          : 0.16*INCH2MM,
        'bottom_x_overhang'       : 0.16*INCH2MM,
        'bottom_y_overhang'       : 0.16*INCH2MM, 
        'lid2front_tabs'          : (0.25,0.75),
        'lid2side_tabs'           : (0.25, 0.75),
        'side2side_tabs'          : (0.5,),
        'inner_panel_side_tabs'   : (0.5,),
        'inner_panel_bottom_tabs' : (0.25, 0.75),
        'lid2front_tab_width'     : 0.5*INCH2MM,
        'lid2side_tab_width'      : 0.5*INCH2MM, 
        'side2side_tab_width'     : 0.5*INCH2MM,
        'tab_depth_adjust'        : -0.25,
        'inner_panel_tab_width'   : 0.3*INCH2MM, 
        'standoff_diameter'       : 0.25*INCH2MM,
        'standoff_offset'         : 0.05*INCH2MM,
        'standoff_hole_diameter'  : 0.116*INCH2MM, 
        'led_cable_hole_position' : (-1.0*INCH2MM, -0.5*z + 0.4*INCH2MM),
        'led_cable_hole_size'     : 3.85,
        'slot_size'               : (5.0,2.0),
        #'slot_size'               : (3.0,5.0),
        #'slot_size'               : (3.87,3.87),
        #'slot_position'           : (0.0, -0.5*z + 19.3),
        'slot_position'           : (0.0, -0.5*z + 19.3 + 0.5),
        #'pcb_position'            : (0, -0.5*z + 19.3),
        'pcb_position'            : (0, -0.5*z + 19.3 + 0.5),
        'pcb_hole_spacing'        : (1.6*INCH2MM, 0.8*INCH2MM),
        'pcb_mount_hole_diam'     : 0.12* INCH2MM,
        'shrouded_hole_position'  : (0.475*INCH2MM, -0.5*z + 19.935),
        'shrouded_hole_size'      : (9.5, 20.5, 1),
        'holder_size'             : (28.0,60.0),
        'holder_standoff_height'  : 1.00*INCH2MM,
        'holder_standoff_inset'   : 2.0,
        'hole_list'               : [],
        'inner_panel_vert_tol'    : 0.5,
        'tube_diameter_top'       : 16.9, 
        'tube_diameter_mid'       : 16.6, 
        'tube_diameter_low'       : 14.65, 
        'tube_hole_offset'        : -0.12*INCH2MM,
        }

enclosure = Clearprep_Enclosure(params)
enclosure.make()

part_assembly = enclosure.get_assembly(
        explode=(0,0,0),
        show_top=True,
        show_second_top=True,
        show_bottom=True,
        show_left=False,
        show_right=False,
        show_front=False,
        show_back=True,
        show_holder=True,
        show_standoffs=True,
        show_holder_standoffs=True,
        show_inner_panel=True,
         )

part_projection = enclosure.get_projection(project=True)

prog_assembly = SCAD_Prog()
prog_assembly.fn = 50
prog_assembly.add(part_assembly)
prog_assembly.write('clearprep_assembly.scad')

enclosure.write_projections(create_dxf=create_dxf)

