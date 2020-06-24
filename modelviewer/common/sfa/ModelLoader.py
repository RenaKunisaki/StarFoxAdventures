import logging; log = logging.getLogger(__name__)
import struct
import math
import sys
import gl
import numpy as np
import zlib
import tempfile
from .Model import Model
from common.gamecube.DlistParser import DlistParser
from common.BinaryFile import BinaryFile


class ModelLoader:
    """Load a model extracted from SFA."""

    def __init__(self):
        self.model       = None
        self._vtxFmt     = {'pos':0, 'nrm':0, 'col':0, 'tex':0}
        self.curShader   = None
        self.mtxLut      = {}
        self.dlists      = []


    def loadFromFile(self, file:BinaryFile):
        curOffs = file.tell()
        header =  file.readu32()
        if header == 0xFACEFEED:
            self._loadFaceFeed(file)

        elif header == 0x5A4C4200: # "ZLB\0"
            self._loadZlb(file)

        elif header == 0xE0E0E0E0: # uncompressed
            # following is size, data offset
            # but unsure how to interpret offset
            raise NotImplementedError("Not implemented 0xE0E0E0E0 header")

        else: # uncompressed?
            file.seek(curOffs) # go back
            self.model = Model(file)

        #self._setupVtxDescrs()
        self._doRenderInstrs()
        return self.model


    def _loadFaceFeed(self, file):
        """Decompress model with FACEFEED header.
        Expects `file` to be seeked to just past the 0xFACEFEED word.
        """
        offs        = file.tell()
        header      = file.readStruct('8I')
        decLen      = header[0]
        zlbDataOffs = header[1]
        compLen     = header[2] # minus 0x10
        # other fields unknown
        #log.debug("FACEFEED header: %s", header)

        # XXX the game doesn't actually do these checks.
        # it's entirely possible for there to not be a ZLB header here.
        h2 = file.readStruct('3sb3I')
        #log.debug("ZLB header: %s", h2)
        assert h2[0] == b'ZLB' and h2[1] == 0, \
            "Corrupt ZLB header following FACEFEED"
        assert h2[2] == 1, "Unsupported ZLB version"
        assert h2[3] == decLen, "ZLB/FACEFEED headers disagree on decLen"
        #assert h2[4] == compLen+0x10, \
        #    "ZLB/FACEFEED headers disagree on compLen"

        #log.debug("ZLB data at 0x%X (0x%X; now 0x%X)",
        #    zlbDataOffs + offs, zlbDataOffs, offs)
        #file.seek(zlbDataOffs + offs) # no idea
        self._decompress(file, decLen, compLen + 0x10)


    def _loadZlb(self, file):
        """Decompress model with ZLB header.
        Expects `file` to be seeked to just past the "ZLB\0" word.
        """
        version, decLen, compLen  = file.readStruct('3I')
        if version == 0x44495200: # "DIR\0"
            # is this ever used in the game?
            raise NotImplementedError("ZLB DIR not supported")
        elif version != 1:
            raise NotImplementedError("Unsupported ZLB version %d" % version)

        self._decompress(file, decLen, compLen)


    def _decompress(self, file, decLen, compLen):
        """Decompress ZLB data from file, with given decompressed length
        and compressed length.
        """
        #log.debug("Decompress from offset 0x%X, dec=0x%X comp=0x%X",
        #    file.tell(), decLen, compLen)
        compData = file.read(compLen)
        assert len(compData) == compLen, "Compressed file is truncated"
        result   = zlib.decompress(compData)
        assert len(result) == decLen, "Decompression failed"
        tmp = tempfile.TemporaryFile()
        tmp.write(result)
        tmp.seek(0)
        self.model = Model(tmp)

        # debug
        #with open('decodedmodel.bin', 'wb') as out:
        #    out.write(result)


    def _setupVtxDescrs(self):
        """Setup vertex descriptors."""
        NONE   = 0
        DIRECT = 1
        IDX8   = 2
        IDX16  = 3
        #self._vtxFmt     = {'pos':0, 'nrm':0, 'col':0, 'tex':0}
        if len(self.model.bones) < 2:
            # gxSetTexMtxIdx(0);
            pass
        else:
            self._vtxFmt['pos'] = DIRECT
            # XXX this isn't quite right. where is ShaderDef set from?
            # and there might be different settings per texture mtx 0-7
            if self.curShader is not None and self.curShader.textureId[0] != 0:
                self._vtxFmt['tex'] = DIRECT
                if self.curShader.flags_40 & 0x01: # UseNormals
                    self._vtxFmt['nrm'] = DIRECT



    def _doRenderInstrs(self):
        """Follow previously parsed render instructions for loaded model."""
        for instr in self.model.renderInstrs:
            op = instr[0]
            if op == 'TEX':
                #log.debug("Set shader %d", instr[1])
                self.curShader = self.model.shaders[instr[1]]

            elif op == 'CALL':
                self._callDlist(instr[1])

            elif op == 'VFMT':
                self._vtxFmt['pos'] = instr[1]
                if self.curShader is not None:
                    if self.curShader.flags_40 & 0x01: # UseNormals
                        self._vtxFmt['nrm'] = instr[2]
                    if self.curShader.flags_40 & 0x02: # UseColors
                        self._vtxFmt['col'] = instr[3]
                    if self.curShader.nTextures > 0:
                        self._vtxFmt['tex'] = instr[4]
                log.debug("Set vtx fmt: %s", self._vtxFmt)

            elif op == 'MTX':
                # not sure why the game does this
                tbl = (0,  1,  2,  3,  4,  5,  6,  7,  8,  9, 0, 0,
                      10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 0, 0)
                mtxs = instr[1]
                for i, mtx in enumerate(mtxs):
                    n = tbl[i]
                    try:
                        self.mtxLut[n] = self.model.xlates[mtx]
                    except IndexError:
                        log.error("Model parsing error: reference to uninitialized matrix %d", mtx)
                        self.mtxLut[n] = (0, 0, 0)

            else:
                log.error("BUG: RenderInstrParser generated unknown opcode %s", op)
                raise RuntimeError("RenderInstrParser failure")


    def _callDlist(self, idx):
        parser = DlistParser(self.model, idx)
        #self._setupVtxDescrs()

        attrs = {
            # XXX col and tex apply to all enabled attrs
            'POS':  self._vtxFmt['pos'],
            'NRM':  self._vtxFmt['nrm'],
            'COL0': self._vtxFmt['col'],
            #'COL1' : self._vtxFmt['col'],
            'TEX0': self._vtxFmt['tex'],
            # XXX is this correct?
            'PNMTXIDX': self._vtxFmt['pos'],
            'T0MIDX': self._vtxFmt['tex'],
            'T7MIDX': self._vtxFmt['tex'],
        }

        #if self.curShader:
        #    log.debug("Shader Texture ID is %d, %d", *self.curShader.textureId)
        #else:
        #    log.debug("No shader")
        if self.curShader and self.curShader.textureId[0] >= 0:
            attrs['TEX1'] = self._vtxFmt['tex']

        #if idx == 24: # HACK - Krystal eyes
        #    # probably related to shader, they're the only ones
        #    # using shader 3
        #    attrs['PNMTXIDX'] = 1
        #    attrs['T0MIDX'] = 1
        #    attrs['COL0'] = 1
        #    attrs['COL1'] = 1

        parser.setVtxFmt(6, **attrs) # XXX always 6?
        parser.setShader(self.curShader)
        parser.setMtxLut(self.mtxLut)
        #log.debug("Parse dlist %d with attrs: %r", idx, attrs)
        dlist = parser.parse()
        #log.debug("Dlist %d has %d polys", idx, len(parser.polys))
        self.dlists.append(dlist)
