import logging; log = logging.getLogger(__name__)

class RenderInstrParser:
    """Parse bit-packed render instructions specific to SFA models."""

    def __init__(self, model, start=0):
        self.model = model
        self._bitOffs = start
        self._nBits = model.header.nRenderInstrs * 8
        self.model.file.seek(self.model.header.renderInstrs)
        # read 3 extra bytes since we always fetch 3 at a time.
        self.bytes = self.model.file.read(self.model.header.nRenderInstrs+3)
        self.curMaterial = None


    def getU24(self):
        # the game reads 3 bytes ahead
        byteOffs = self._bitOffs >> 3
        shift = self._bitOffs & 7
        b = self.bytes[byteOffs: byteOffs+3]
        return ((b[2] << 16) | (b[1] << 8) | b[0]) >> shift


    def getBits(self, n):
        val = self.getU24() & ((1 << n)-1)
        self._bitOffs += n
        return val


    def parse(self):
        #log.debug("nBits = %d (0x%X); %d (0x%X) bytes",
        #    self._nBits, self._nBits, self._nBits >> 3, self._nBits >> 3)
        opCount = {0:0, 1:0, 2:0, 3:0, 4:0, 5:0}
        self.result = []

        while True:
            op = self.getBits(4)
            if op not in opCount: opCount[op] = 0
            opCount[op] += 1
            #log.debug("Bit %04X (byte %04X) op %X",
            #    (self._bitOffs-4), (self._bitOffs-4) >> 3, op)
            if   op == 1: self.op_selectTexture()
            elif op == 2: self.op_callDlist()
            elif op == 3: self.op_setVtxFmt()
            elif op == 4: self.op_initMtx()
            elif op == 5: # end
                #log.debug("End at bit %d (byte %d)",
                #    self._bitOffs, self._bitOffs >> 3)
                break
            else:
                log.error("Invalid opcode %d at bit %d (byte %d)", op,
                    self._bitOffs, self._bitOffs >> 3)
                break
        #log.debug("Opcode counts: 0=%d, 1=%d, 2=%d, 3=%d, 4=%d",
        #    opCount[0], opCount[1], opCount[2], opCount[3], opCount[4])
        return self.result


    def op_selectTexture(self): # opcode 1: select texture and material
        idx = self.getBits(6) # idx of both texture and material
        ok  = (idx < self.model.header.nTextures)
        log.debug("%04X Select tex/mtrl %d (%s)", self._bitOffs-6,
            idx, "OK" if ok else "ERR")
        self.curMaterial = idx
        self.result.append(('TEX', idx))


    def op_callDlist(self): # opcode 2: call display list
        offs = self._bitOffs
        idx  = self.getBits(8)
        ok   = (idx < self.model.header.nDlists)
        log.debug("%04X Call dlist %d (%s)", offs, idx,
            "OK" if ok else "ERR")
        self.result.append(('CALL', idx))


    def op_setVtxFmt(self): # opcode 3: set vtx format
        if self.curMaterial is None:
            flags = 0
        else:
            mat = self.model.materials[self.curMaterial]
            flags = mat.flags_40
            
        if (flags & 2) != 0:
            fmt = self.getBits(4)
            posSize = 2 if (fmt & 1) == 0 else 3 # VAT format (2=8bit 3=16bit)
            nrmSize = 2 if (fmt & 2) == 0 else 3
            colSize = 2 if (fmt & 4) == 0 else 3
            texSize = 2 if (fmt & 8) == 0 else 3 # for all 8 texture slots
        else:
            fmt = self.getBits(3)
            posSize = 2 if (fmt & 1) == 0 else 3
            nrmSize = 2 if (fmt & 2) == 0 else 3
            colSize = 0
            texSize = 2 if (fmt & 4) == 0 else 3 # for all 8 texture slots
        log.debug("VtxFmt: pos=%d nrm=%d col=%d tex=%d", posSize,
            nrmSize, colSize, texSize)
        self.result.append(('VFMT', posSize, nrmSize, colSize, texSize))


    def op_initMtx(self): # opcode 4: init matrix
        # it looks like opcode 0 would be the same as this but isn't used
        nMtxs = self.getBits(4)
        # here the game asserts that nMtxs < 20, but this is impossible...
        #log.debug("Init %d mtxs (bit %d)", nMtxs, self._bitOffs)
        mtxs = []
        for i in range(nMtxs):
            idx = self.getBits(8)
            ok  = idx < self.model.header.nVtxGroups + self.model.header.nBones
            #log.debug("  Init mtx %d (%s)", idx, "OK" if ok else "ERR")
            mtxs.append(idx)
        self.result.append(('MTX', mtxs))
