from __future__ import print_function
import os 
import sys
from py2gcode import gcode_cmd
from py2gcode import cnc_dxf

fileName = sys.argv[1] 

feedrate = 120.0
depth = 0.16
startZ = 0.0
safeZ = 0.5
toolDiam = 0.125 
direction = 'ccw'
maxCutDepth = 0.1
startDwell = 2.0
startCond = 'minX'
maxArcLen = 1.0e-2 
ptEquivTol=  1.0e-5


prog = gcode_cmd.GCodeProg()
prog.add(gcode_cmd.GenericStart())
prog.add(gcode_cmd.Space())
prog.add(gcode_cmd.FeedRate(feedrate))

# Cut circular pocket
layerNames = ['tube_circle']
param = {
        'fileName'     : fileName,
        'layers'       : layerNames,
        'depth'        : depth,
        'startZ'       : startZ,
        'safeZ'        : safeZ,
        'toolDiam'     : toolDiam,
        'toolOffset'   : 'inside',
        'direction'    : direction,
        'maxCutDepth'  : maxCutDepth,
        'startDwell'   : startDwell,
        }
boundary = cnc_dxf.DxfCircBoundary(param)
prog.add(boundary)

# Cut cutout
layerNames = ['tube_cutout']
param = {
        'fileName'    : fileName,
        'layers'      : layerNames,
        'depth'       : depth,
        'startZ'      : startZ, 
        'safeZ'       : safeZ, 
        'toolDiam'    : toolDiam,
        'direction'   : direction, 
        'cutterComp'  : 'inside',
        'maxCutDepth' : maxCutDepth,
        'startDwell'  : startDwell,
        'startCond'   : startCond,
        'maxArcLen'   : maxArcLen, 
        'ptEquivTol'  : ptEquivTol, 
        }
cutout = cnc_dxf.DxfBoundary(param)
prog.add(cutout)


# Cut boundary
layerNames = ['boundary']
param = {
        'fileName'    : fileName,
        'layers'      : layerNames,
        'depth'       : depth,
        'startZ'      : startZ, 
        'safeZ'       : safeZ, 
        'toolDiam'    : toolDiam,
        'direction'   : direction, 
        'cutterComp'  : 'outside',
        'maxCutDepth' : maxCutDepth,
        'startDwell'  : startDwell,
        'startCond'   : startCond,
        'maxArcLen'   : maxArcLen, 
        'ptEquivTol'  : ptEquivTol, 
        }
boundary = cnc_dxf.DxfBoundary(param)
prog.add(boundary)

prog.add(gcode_cmd.Space())
prog.add(gcode_cmd.End(),comment=True)
baseName, dummy = os.path.splitext(__file__)
fileName = '{0}.ngc'.format(baseName)
print('generating: {0}'.format(fileName))
prog.write(fileName)
