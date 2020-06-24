import logging; log = logging.getLogger(__name__)
import struct
from common.math import Vec2s, Vec3s, Vec3f, Quaternion, Mtx43, Mtx44
from .structs import Bone, DisplayListPtr, Shader, HitboxStruct, ModelStruct, ModelVtxGroup
from common.gamecube.DlistParser import DlistParser
from .RenderInstrParser import RenderInstrParser
#from gl.Util import Matrix

class Model:
    """A model extracted from SFA."""

    def __init__(self, file):
        self.file   = file
        self.offset = file.tell()
        self.header = ModelStruct.readFromFile(file)
        self.xlates = []
        # xlates are mtxs in the game code, combining a camera matrix
        # and a translation. since we don't need the camera matrix,
        # we'll store only the translation vector.

        #for name, field in ModelStruct._fields.items():
        #    val = getattr(self.header, name)
        #    name = name.ljust(20)
        #    if type(val) is int: print(name, val, hex(val))
        #    else: print(name, val)
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


    def _readVtxGroups(self):
        #log.debug("nVtxGroups=%d from 0x%X", self.header.nVtxGroups,
        #    self.header.vtxGroups)
        offs = self.header.vtxGroups + self.offset
        self.vtxGroups = ModelVtxGroup.readFromFile(self.file, offs, self.header.nVtxGroups)

        for grp in self.vtxGroups:
            bone0 = self.bones[grp.bone0]
            bone1 = self.bones[grp.bone1]
            # the weight isn't scaled by 255 like you might expect.
            wgt0 = grp.weight / 4
            wgt1 = 1.0 - wgt0
            head0, tail0 = self.calcBonePos(bone0, True)
            head1, tail1 = self.calcBonePos(bone1, True)
            # we do have to scale these because we scale the vtxs
            # but we don't scale the bone transforms because they're
            # getting the vertex groups into place
            x = (((tail0[0] * wgt0) + (tail1[0] * wgt1))) / 256
            y = (((tail0[1] * wgt0) + (tail1[1] * wgt1))) / 256
            z = (((tail0[2] * wgt0) + (tail1[2] * wgt1))) / 256
            self.xlates.append((x, y, z))


    def _readBones(self):
        #log.debug("nBones=%d from 0x%X", self.header.nBones,
        #    self.header.bones)
        offs = self.header.bones + self.offset
        self.bones = Bone.readFromFile(self.file, offs, self.header.nBones)

        for i, bone in enumerate(self.bones):
            # don't use relative transform here because we're in model
            # space, not bone space, or something idk, it works
            head, tail = self.calcBonePos(bone, False)
            self.xlates.append(tail)
            #log.debug("%3d|%3d|%02X %02X %02X|%+6.2f %+6.2f %+6.2f|%+6.2f %+6.2f %+6.2f|%+6.2f %+6.2f %+6.2f|%+6.2f %+6.2f %+6.2f",
            #    i, bone.parent, bone.unk01, bone.unk02, bone.unk03,
            #    bone.head.x, bone.head.y, bone.head.z,
            #    bone.tail.x, bone.tail.y, bone.tail.z,
            #    head[0], head[1], head[2],
            #    tail[0], tail[1], tail[2])

    def _readVtxs(self):
        #log.debug("nVtxs=%d from 0x%X", self.header.nVtxs,
        #    self.header.vtxs)
        offs = self.header.vtxs + self.offset
        self.vtxs = Vec3s.readFromFile(self.file, offs, self.header.nVtxs)
        for vtx in self.vtxs:
            vtx.x /= 256.0
            vtx.y /= 256.0
            vtx.z /= 256.0

    def _readNormals(self):
        #log.debug("nNormals=%d from 0x%X", self.header.nNormals,
        #    self.header.normals)
        offs = self.header.normals + self.offset
        self.normals = Vec3s.readFromFile(self.file, offs, self.header.nNormals)
        for vtx in self.normals:
            vtx.x /= 256.0
            vtx.y /= 256.0
            vtx.z /= 256.0

    def _readTexCoords(self):
        #log.debug("nTexCoords=%d from 0x%X", self.header.nTexCoords,
        #    self.header.texCoords)
        offs = self.header.texCoords + self.offset
        self.texCoords = Vec2s.readFromFile(self.file, offs, self.header.nTexCoords)
        for vtx in self.texCoords:
            vtx.x /= 1024.0
            vtx.y /= 1024.0

    def _readShaders(self):
        #log.debug("nShaders=%d from 0x%X", self.header.nShaders,
        #    self.header.shaders)
        offs = self.header.shaders + self.offset
        self.shaders = Shader.readFromFile(self.file, offs, self.header.nShaders)

    def _readHitBoxes(self):
        #log.debug("nHitBoxes=%d from 0x%X", self.header.nHitBoxes,
        #    self.header.hitboxes)
        offs = self.header.hitboxes + self.offset
        self.hitboxes = HitboxStruct.readFromFile(self.file, offs, self.header.nHitBoxes)

    def _readRenderInstrs(self):
        #log.debug("nRenderInstrs=%d from 0x%X", self.header.nRenderInstrs,
        #    self.header.renderInstrs)
        self.renderInstrs = RenderInstrParser(self).parse()

    def _readDlistPtrs(self):
        #log.debug("nDlists=%d from 0x%X", self.header.nDlists,
        #    self.header.dlists)
        offs = self.header.dlists + self.offset
        self.dlistPtrs = DisplayListPtr.readFromFile(self.file, offs, self.header.nDlists)

    def _readTextures(self):
        #log.debug("nTextures=%d from 0x%X", self.header.nTextures,
        #    self.header.textures)
        self.file.seek(self.header.textures + self.offset)
        data = self.file.read(4*self.header.nTextures)
        self.textureIds = struct.unpack('>%dI' % self.header.nTextures, data)


    def calcBonePos(self, bone, relative, _depth=0):
        """Compute bone head/tail position relative to ancestors."""
        assert _depth < 10
        head = [bone.head.x, bone.head.y, bone.head.z]
        tail = [bone.tail.x, bone.tail.y, bone.tail.z]
        if bone.parent != 0xFF:
            if relative: # this makes the bones display correctly but the polygons not...
                tail[0] -= head[0]
                tail[1] -= head[1]
                tail[2] -= head[2]
            parent = self.bones[bone.parent]
            pHead, pTail = self.calcBonePos(parent, relative, _depth+1)
            head[0] += pHead[0]
            head[1] += pHead[1]
            head[2] += pHead[2]
            #tail[0] += pTail[0]
            #tail[1] += pTail[1]
            #tail[2] += pTail[2]
        return (head, tail)
