import logging; log = logging.getLogger(__name__)
import struct
import math
import sys
import os
import os.path
from common.BinaryFile import BinaryFile
from common.sfa.zlb import Zlb
from common.math import Vec2s, Vec3s, Vec3f, Quaternion, Mtx43, Mtx44
from ..ModelRenderer import ModelRenderer

class MapObject:
    """An object on an SFA map."""

    def __init__(self, parentMap, romListEntry):
        self.map = parentMap
        entry = struct.unpack_from('>hBBBBBBfffI', romListEntry, 0)
        objTyp = entry[0]
        self.origType = objTyp

        if objTyp >= 0: # use OBJINDEX.bin to translate ID
            if objTyp < len(parentMap.game.objIndex):
                objTyp = parentMap.game.objIndex[objTyp]
            else: log.warning("Unknown object ID 0x%X", objTyp)
        else: objTyp = -objTyp # do not translate ID
        self.type = objTyp

        # entry[2] is correct here; these are not byte offsets.
        self.unk03, self.unk04, self.unk05, self.unk06 = entry[2:6]

        self.pos       = Vec3f(romListEntry[0x08:0x14])
        self.uniqueId  = entry[10]
        self.seqData   = romListEntry[0x18:]
        try:
            self.objDef = parentMap.game.objDefs[self.type]
        except IndexError:
            log.warning("Missing objdef 0x%04X", self.type)
            self.objDef = parentMap.game.objDefs[0]

        self.name    = self.objDef.name
        self.modelId = self.objDef.modelIds[0]


    def render(self):
        """Render this object."""
        renderer = self.map.game.modelRenderers[self.modelId]
        renderer.addInstance((self.pos.x, self.pos.y, self.pos.z))
