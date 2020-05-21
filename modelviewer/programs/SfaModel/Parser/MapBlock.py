import logging; log = logging.getLogger(__name__)
import struct
from .structs import Bone, DisplayListPtr, Shader, HitboxStruct, ModelStruct, ModelVtxGroup, Mtx43, Mtx44, Quaternion, Vec2s, Vec3f, Vec3s
from .DlistParser import DlistParser
from .RenderInstrParser import RenderInstrParser
from .Model import Model

class MapBlock(Model):
    """A map block model extracted from SFA.

    This is similar to a Model, but with some differences in
    structure and contents.
    """

    def __init__(self, file):
        self.file   = file
        self.offset = file.tell()
        self.header = ModelStruct.readFromFile(file)
        self.xlates = []
        # xlates are mtxs in the game code, combining a camera matrix
        # and a translation. since we don't need the camera matrix,
        # we'll store only the translation vector.

        for name, field in ModelStruct._fields.items():
            val = getattr(self.header, name)
            if type(val) is int: print(name, val, hex(val))
            else: print(name, val)
        self._readBones()
        self._readVtxGroups()
        self._readVtxs()
        self._readNormals()
        self._readTexCoords()
        self._readHitBoxes()
        self._readTextures()
        self._readShaders()
        self._readRenderInstrs()
        self._readDlistPtrs()
