import logging; log = logging.getLogger(__name__)
import struct
import math
import sys
import os
import os.path
from common.sfa.ModelLoader import ModelLoader
from common.gamecube.DlistParser import DlistParser
from common.BinaryFile import BinaryFile
from common.sfa.zlb import Zlb
from common.math import Vec2s, Vec3s, Vec3f, Quaternion, Mtx43, Mtx44

class MapObject:
    """An object on an SFA map."""

    def __init__(self, parentMap, romListEntry):
        self.map = parentMap
        entry = struct.unpack_from('>hBBBBBBfffI', romListEntry, 0)
        objTyp = entry[0]
        self.origType = objTyp

        if objTyp >= 0: # use OBJINDEX.bin to translate ID
            if objTyp < len(parentMap.program.objIndex):
                objTyp = parentMap.program.objIndex[objTyp]
            else: log.warning("Unknown object ID 0x%X", objTyp)
        else: objTyp = -objTyp # do not translate ID
        self.type = objTyp

        # entry[2] is correct here; these are not byte offsets.
        self.unk03, self.unk04, self.unk05, self.unk06 = entry[2:6]

        self.pos       = Vec3f(romListEntry[0x08:0x14])
        self.uniqueId  = entry[10]
        self.seqData   = romListEntry[0x18:]
        try: self.name = parentMap.program.objDefs[self.type]['name']
        except IndexError: self.name = '?'


    def render(self, program):
        """Render this object."""
        s = self.map.program.objDefs[self.type]['scale']
        pointA = (self.pos.x - s, self.pos.y - s, self.pos.z - s)
        pointB = (self.pos.x + s, self.pos.y + s, self.pos.z + s)
        T = self.type
        color  = (
            (( T       & 3) | (((T >>  6) & 7) << 2)) / 31,
            (((T >> 2) & 3) | (((T >>  9) & 7) << 2)) / 31,
            (((T >> 4) & 3) | (((T >> 12) & 7) << 2)) / 31,
            0.5)
        program.boxRenderer.addBox(pointA, pointB, color)
