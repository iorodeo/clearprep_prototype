from py2scad import *
import subprocess

class Clearprep_Enclosure(Basic_Enclosure):

    def __init__(self,params):
        super(Clearprep_Enclosure,self).__init__(params) 

    def make(self):
        super(Clearprep_Enclosure,self).make()

        # Make inner panel, custom cutouts, holder and second top
        self.make_inner_panel()
        self.make_inner_panel_tab_holes()
        self.make_slot()
        self.make_pcb_holes()
        self.make_shrouded_header_hole()
        self.make_top_hole()
        self.make_led_cable_hole()
        self.make_holder()
        self.make_holder_standoffs()
        self.make_second_top()
        self.make_second_top_hole()

    def get_inner_panel_offset(self):
        value = 0.5*self.params['tube_diameter_top'] 
        value += 0.5*self.params['wall_thickness']
        value += self.params['tube_hole_offset']
        return value

    def make_inner_panel(self):
        inner_x, inner_y, inner_z = self.params['inner_dimensions']
        wall_thickness = self.params['wall_thickness']
        inner_panel_tab_width = self.params['inner_panel_tab_width']
        try:
            depth_adjust = self.params['tab_depth_adjust']
        except KeyError:
            depth_adjust = 0.0
        tab_depth = wall_thickness + depth_adjust 

        # Create tab data top and bottom tabs
        xz_pos = []
        xz_neg = []
        for loc in self.params['inner_panel_bottom_tabs']:
            tab_data = (loc, inner_panel_tab_width, tab_depth, '+')
            xz_neg.append(tab_data)

        # Create tab data for front and back tabs 
        yz_pos = []
        yz_neg = []
        for loc in self.params['inner_panel_side_tabs']:
            tab_data = (loc, inner_panel_tab_width, tab_depth, '+')
            yz_pos.append(tab_data)
            yz_neg.append(tab_data)

        # Pack panel data into parameters structure
        panel_x = inner_y+0*wall_thickness
        panel_y = inner_z
        panel_z = wall_thickness
        params = {
                'size' : (panel_x, panel_y, panel_z),
                'xz+'  : xz_pos,
                'xz-'  : xz_neg,
                'yz+'  : yz_pos,
                'yz-'  : yz_neg,
                }

        plate_maker = Plate_W_Tabs(params)
        self.inner_panel= plate_maker.make()


    def make_inner_panel_tab_holes(self):
        inner_x, inner_y, inner_z = self.params['inner_dimensions']
        wall_thickness = self.params['wall_thickness']
        inner_panel_tab_width = self.params['inner_panel_tab_width']
        inner_panel_offset = self.get_inner_panel_offset()
        hole_list = []

        # create top and bottom holes
        for loc in self.params['inner_panel_bottom_tabs']:
            for panel in ('bottom',):
                x = inner_panel_offset
                y = -0.5*inner_y + inner_y*loc
                hole = {
                        'panel'    : panel,
                        'type'     : 'square',
                        'location' : (x, y),
                        'size'     : (wall_thickness,inner_panel_tab_width),
                        }
                hole_list.append(hole)

        # create front and back holes
        for loc in self.params['inner_panel_side_tabs']:
            for panel in ('front', 'back'):
                x = inner_panel_offset
                z = -0.5*inner_z + inner_z*loc
                hole = {
                        'panel'    : panel,
                        'type'     : 'square',
                        'location' : (x,z),
                        'size'     : (wall_thickness, inner_panel_tab_width),
                        }
                hole_list.append(hole)

        self.add_holes(hole_list)

    def make_slot(self):
        hole = {
                'panel' : 'inner_panel',
                'type'  : 'square',
                'location' : self.params['slot_position'],
                'size'     : self.params['slot_size'],
                }
        self.add_holes([hole])

    def make_pcb_holes(self): 
        pcb_center_x, pcb_center_y = self.params['pcb_position']
        pcb_hole_spacing_x, pcb_hole_spacing_y = self.params['pcb_hole_spacing']
        pcb_mount_hole_diam = self.params['pcb_mount_hole_diam']
        hole_list = []
        # Create PCB holes
        for i in (-1,1):
            for j in (-1,1):
                for panel in ('left', 'right'):
                    hole_x = pcb_center_x + i*0.5*pcb_hole_spacing_x
                    hole_z = pcb_center_y + j*0.5*pcb_hole_spacing_y
                    hole = { 
                            'panel'     : panel,
                            'type'      : 'round',
                            'location'  : (hole_x, hole_z),
                            'size'      : pcb_mount_hole_diam, 
                            }
                    hole_list.append(hole)
        self.add_holes(hole_list)

    def make_shrouded_header_hole(self):
        shrouded_hole_x,shrouded_hole_z = self.params['shrouded_hole_position']
        shrouded_hole_size = self.params['shrouded_hole_size']
        hole = {
                'panel'    : 'right',
                'type'     : 'rounded_square',
                'location' : (shrouded_hole_x,shrouded_hole_z),
                'size'     : shrouded_hole_size,
                }
        self.add_holes([hole])

    def make_top_hole(self):
        tube_hole_size = self.params['tube_diameter_top']
        tube_hole_offset = self.params['tube_hole_offset']
        hole_top = {
                'panel'    : 'top',
                'type'     : 'round',
                'location' : (tube_hole_offset,0),
                'size'     : tube_hole_size,
                }
        
        self.add_holes([hole_top])

    def make_led_cable_hole(self):
        led_cable_hole_position = self.params['led_cable_hole_position']
        led_cable_hole_size = self.params['led_cable_hole_size']
        hole = {
                'panel'    : 'inner_panel',
                'type'     : 'round',
                'location' : led_cable_hole_position,
                'size'     : led_cable_hole_size,
                }
        self.add_holes([hole])

    def make_holder(self):
        holder_size = self.params['holder_size']
        holder_x, holder_y = holder_size
        thickness = self.params['wall_thickness']

        tube_diam_top = self.params['tube_diameter_top']
        tube_diam_mid = self.params['tube_diameter_mid']
        tube_diam_low = self.params['tube_diameter_low']
        tube_hole_offset = self.params['tube_hole_offset']

        standoff_hole_diam = self.params['standoff_hole_diameter']
        standoff_diam = self.params['standoff_diameter']
        holder_standoff_inset = self.params['holder_standoff_inset']
        inner_panel_offset = self.get_inner_panel_offset()

        self.holder_mid = Cube(size=(holder_x, holder_y,thickness))
        self.holder_low = Cube(size=(holder_x, holder_y,thickness))

        hole_list = []

        tube_hole_mid_0 = {
                'panel': 'holder_mid',
                'type' : 'round',
                'location': (0.5*holder_x-0.5*tube_diam_top,0),
                'size' : tube_diam_mid,
                }
        tube_hole_mid_1 = {
                'panel': 'holder_mid',
                'type': 'square',
                'location': (0.5*holder_x,0),
                'size': (0.6*tube_diam_mid, 0.6*tube_diam_mid),
                }
        tube_hole_low_0 = {
                'panel': 'holder_low',
                'type' : 'round',
                'location': (0.5*holder_x-0.5*tube_diam_top,0),
                'size' : tube_diam_low,
                }
        tube_hole_low_1 = {
                'panel': 'holder_low',
                'type': 'square',
                'location': (0.5*holder_x,0),
                'size': (0.6*tube_diam_low, 0.6*tube_diam_low),
                }

        self.holder_standoff_hole_xy = []

        for i in (-1,1):
            hole_x = 0
            hole_y = i*(0.5*holder_y - 0.5*standoff_diam - holder_standoff_inset)
            standoff_hole_mid = {
                    'panel': 'holder_mid',
                    'type': 'round',
                    'location': (hole_x, hole_y),
                    'size': standoff_hole_diam,
                    }
            standoff_hole_low = {
                    'panel': 'holder_low',
                    'type': 'round',
                    'location': (hole_x, hole_y),
                    'size': standoff_hole_diam,
                    }
            hole_x = -0.5*holder_x + inner_panel_offset - 0.5*thickness  
            standoff_floor_hole = {
                    'panel': 'bottom',
                    'type': 'round',
                    'location': (hole_x, hole_y),
                    'size': standoff_hole_diam,
                    }

            self.holder_standoff_hole_xy.append((hole_x, hole_y))
            hole_list.append(standoff_hole_mid)
            hole_list.append(standoff_hole_low)
            hole_list.append(standoff_floor_hole)

        hole_list.append(tube_hole_mid_0)
        hole_list.append(tube_hole_mid_1)
        hole_list.append(tube_hole_low_0)
        hole_list.append(tube_hole_low_1)
        self.add_holes(hole_list)

    def make_holder_standoffs(self):
        standoff_diam = self.params['standoff_diameter']
        r = 0.5*standoff_diam
        h = self.params['holder_standoff_height']
        standoff = Cylinder(h=h,r1=r,r2=r)
        self.holder_standoffs = [standoff, standoff]

    def make_second_top(self):
        lid_radius = self.params['lid_radius']
        thickness = self.params['wall_thickness']
        standoff_hole_diam = self.params['standoff_hole_diameter']
        hole_list = []
        for x,y in self.standoff_xy_pos:
            hole = (x,y,standoff_hole_diam)
            hole_list.append(hole)
        self.second_top = plate_w_holes(self.top_x, self.top_y, thickness, hole_list, radius = lid_radius)
    
    def make_second_top_hole(self):
        tube_hole_size = self.params['tube_diameter_top']
        tube_hole_offset = self.params['tube_hole_offset']
        second_top_hole = {
                'panel'    : 'second_top',
                'type'     : 'round',
                'location' : (tube_hole_offset,0),
                'size'     : tube_hole_size,
                }
        self.add_holes([second_top_hole])

        
    def get_assembly(self,**kwargs):
        try:
            show_inner_panel = kwargs.pop('show_inner_panel')
        except KeyError:
            show_inner_panel = True
        try:
            show_holder = kwargs.pop('show_holder')
        except KeyError:
            show_holder = True
        try:
            show_holder_standoffs = kwargs.pop('show_holder_standoffs')
        except KeyError:
            show_holder_standoffs = True
        try:
            show_second_top = kwargs.pop('show_second_top')
        except KeyError:
            show_second_top = True
        

        part_list = super(Clearprep_Enclosure,self).get_assembly(**kwargs)

        inner_x, inner_y, inner_z = self.params['inner_dimensions']
        wall_thickness = self.params['wall_thickness']
        explode_x, explode_y, explode_z = kwargs['explode']

        # Position inner panel
        x_shift = self.get_inner_panel_offset()
        inner_panel = Rotate(self.inner_panel, a=90, v=(0,0,1))
        inner_panel = Rotate(inner_panel, a=90, v=(0,1,0))
        inner_panel = Translate(inner_panel,v=(x_shift,0,0))
        if show_inner_panel:
            part_list.append(inner_panel)

        # Position holders
        holder_x, holder_y = self.params['holder_size']
        holder_standoff_height = self.params['holder_standoff_height']
        inner_panel_offset = self.get_inner_panel_offset()
        x_shift = -0.5*holder_x + 0.5*self.params['tube_diameter_top']  + self.params['tube_hole_offset']
        z_shift = 0.5*wall_thickness - 0.5*inner_z + holder_standoff_height
        holder_low = Translate(self.holder_low,v=(x_shift,0,z_shift))
        z_shift += wall_thickness + holder_standoff_height
        holder_mid = Translate(self.holder_mid,v=(x_shift,0,z_shift))

        # Position holder standoffs
        for val in ('low', 'mid'):
            if val == 'low':
                z_shift = 0.5*holder_standoff_height - 0.5*inner_z 
            else:
                z_shift = 1.5*holder_standoff_height - 0.5*inner_z + wall_thickness 
            for pos, standoff in zip(self.holder_standoff_hole_xy, self.holder_standoffs):
                x_shift, y_shift = pos
                standoff = Translate(standoff,v=(x_shift, y_shift,z_shift))
                if show_holder_standoffs:
                    part_list.append(standoff)

        if show_holder:
            part_list.append(holder_low)
            part_list.append(holder_mid)

        # Position second top
        z_shift = 1.5*wall_thickness + 0.5*inner_z + 2*explode_z
        second_top = Translate(self.second_top, v=(0,0,z_shift)) ## Fix this 
        #second_top = Translate(self.second_top, v= (0,0,75)) ## Fix this 
        if show_second_top:
            part_list.append(second_top)
        return part_list

    def write_projections(self,prefix='projection', create_dxf=False):
        self.ref_cube = Cube(size=[INCH2MM,INCH2MM,INCH2MM])
        parts_list = [
                'top',
                'second_top',
                'bottom',
                'left',
                'right',
                'front',
                'back',
                'inner_panel',
                'holder_mid',
                'holder_low',
                'ref_cube',
                ]
        for part_name in parts_list:
            part = getattr(self,part_name)
            filename_base = '{0}_{1}'.format(prefix,part_name)
            filename_scad = '{0}.scad'.format(filename_base) 
            print 'writing: {0}'.format(filename_scad)

            # Create openscad projections
            prog = SCAD_Prog()
            prog.fn = 50
            part_projection = Projection(part)
            prog.add(part_projection)
            prog.write(filename_scad)
            del prog

            # Write dxf files
            if create_dxf:
                filename_dxf = '{0}.dxf'.format(filename_base)
                print 'writing: {0}'.format(filename_dxf)
                subprocess.call(['openscad', '-x', filename_dxf, filename_scad])



