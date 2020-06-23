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
from .MapObject import MapObject

class Map:
    """SFA map."""
    def __init__(self, program):
        self.program = program
        self.objects = []


    def load(self, name):
        """Load map by name."""
        # XXX eventually embed the name table and load other assets.
        # for now, use name as romlist path.
        self.loadRomList(name+'.romlist.zlb')
        return self


    def loadRomList(self, path):
        """Load romlist.zlb file."""
        file = BinaryFile(self.program.discRoot+path)
        data = Zlb(file).decompress()
        offs = 0
        while offs < len(data):
            typ, length = struct.unpack_from('>hB', data, offs)
            if length == 0: break
            entry = data[offs:offs+(length*4)]
            obj   = MapObject(self, entry)
            self.objects.append(obj)
            offs += length * 4
