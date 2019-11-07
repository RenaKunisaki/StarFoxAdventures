import logging; log = logging.getLogger(__name__)
import struct
import gl

class DlistParser:
    """Parse GameCube display lists, at least enough for SFA models."""
    drawModes = ('quads', 'quads2', 'tris', 'triStrip',
        'triFan', 'lines', 'lineStrip', 'points')

    opNames = {
        0x00: 'NOP',
        0x08: 'LOAD CP',
        0x10: 'LOAD XF',
        0x20: 'LOAD IDX A',
        0x28: 'LOAD IDX B',
        0x30: 'LOAD IDX C',
        0x38: 'LOAD IDX D',
        0x40: 'CALL',
        0x48: 'INVAL CACHE',
        0x61: 'LOAD BP',
        0x80: 'DRAW QUADS',
        0x88: 'DRAW QUADS2',
        0x90: 'DRAW TRIS',
        0x98: 'DRAW TRISTRIPS',
        0xA0: 'DRAW TRIFANS',
        0xA8: 'DRAW LINES',
        0xB0: 'DRAW LINESTRIPS',
        0xB8: 'DRAW POINTS',
    }

    # each vtx has (if they're enabled) the fields in order:
    #PNMTXIDX - Position/Normal Matrix Index
    #TEX0MTXIDX - Texture 0 Matrix Index
    #TEX1MTXIDX - Texture 1 Matrix Index
    #TEX2MTXIDX - Texture 2 Matrix Index
    #TEX3MTXIDX - Texture 3 Matrix Index
    #TEX4MTXIDX - Texture 4 Matrix Index
    #TEX5MTXIDX - Texture 5 Matrix Index
    #TEX6MTXIDX - Texture 6 Matrix Index
    #TEX7MTXIDX - Texture 7 Matrix Index
    #POS - Position Vector
    #NRM - Normal or NBT - Binormal vector (T, B)
    #CLR0 - Color0 (Diffused)
    #CLR1 - Color1 (Specular)
    #TEX0 - Texture 0 data
    #TEX1 - Texture 1 data
    #TEX2 - Texture 2 data
    #TEX3 - Texture 3 data
    #TEX4 - Texture 4 data
    #TEX5 - Texture 5 data
    #TEX6 - Texture 6 data
    #TEX7 - Texture 7 data
    # The VCD tells wether data for a component exists (and if yes, if it is direct or indexed) and the VAT tells the actual format of the respective component.
    # maybe it's enough to just check what they're set to...
    # CP50 = 00001F03 (set by object 112 in Fifo log)
    # CP60 = 00000003
    # PMIDX (direct),
    # T0MIDX (direct),
    # T7MIDX (direct),
    # POS (u16),
    # NRM (u16),
    # TEX0 (u16),
    # => 9 bytes/vtx
    # this seems to get it to not fail to parse
    # no idea if it's correct data

    vatFields = { # name => (reg, idx, size)
        'PNMTXIDX': (0x50,  0, 1),
        'T0MIDX':   (0x50,  1, 1),
        'T1MIDX':   (0x50,  2, 1),
        'T2MIDX':   (0x50,  3, 1),
        'T3MIDX':   (0x50,  4, 1),
        'T4MIDX':   (0x50,  5, 1),
        'T5MIDX':   (0x50,  6, 1),
        'T6MIDX':   (0x50,  7, 1),
        'T7MIDX':   (0x50,  8, 1),
        'POS':      (0x50,  9, 2),
        'NRM':      (0x50, 11, 2),
        'COL0':     (0x50, 13, 2),
        'COL1':     (0x50, 15, 2),
        'TEX0':     (0x60,  0, 2),
        'TEX1':     (0x60,  2, 2),
        'TEX2':     (0x60,  4, 2),
        'TEX3':     (0x60,  6, 2),
        'TEX4':     (0x60,  8, 2),
        'TEX5':     (0x60, 10, 2),
        'TEX6':     (0x60, 12, 2),
        'TEX7':     (0x60, 14, 2),
    }
    vatFieldOrder = (
        'PNMTXIDX', 'T0MIDX', 'T1MIDX', 'T2MIDX', 'T3MIDX', 'T4MIDX',
        'T5MIDX', 'T6MIDX', 'T7MIDX', 'POS', 'NRM', 'COL0', 'COL1',
        'TEX0', 'TEX1', 'TEX2', 'TEX3', 'TEX4', 'TEX5', 'TEX6',
        'TEX7')

    def __init__(self, model, idx):
        self.model   = model
        self.listIdx = idx
        self.list    = self.model.dlistPtrs[idx]
        self.offset  = self.list.offset
        log.debug("Parsing dlist %d (0x%X) size %d (0x%X) offs=0x%X", idx,
            idx, self.list.size, self.list.size, self.list.offset)
        self.model.file.seek(self.list.offset)
        self.bytes    = self.model.file.read(self.list.size)
        self._offset  = 0
        self.material = None
        self.mtxLut   = {}
        self.polys    = []
        self._cpRegs  = {
            0x30: 0x3C03C780, # MATIDX_REG_A
            0x50: 0x00001F03, # VCD_LO[0]
            0x51: 0x00001F03, # VCD_LO[1] XXX probably wrong
            0x52: 0x00001F03, # VCD_LO[2]
            0x53: 0x00001F03, # VCD_LO[3]
            0x54: 0x00001F03, # VCD_LO[4]
            0x55: 0x00001F03, # VCD_LO[5]
            0x56: 0x00001F03, # VCD_LO[6]
            0x57: 0x00001F03, # VCD_LO[7]
            0x60: 0x00000003, # VCD_HI[0]
            0x61: 0x00000003, # VCD_HI[1] XXX probably wrong
            0x62: 0x00000003, # VCD_HI[2]
            0x63: 0x00000003, # VCD_HI[3]
            0x64: 0x00000003, # VCD_HI[4]
            0x65: 0x00000003, # VCD_HI[5]
            0x66: 0x00000003, # VCD_HI[6]
            0x67: 0x00000003, # VCD_HI[7]
            # XXX get 70-77, 80-87, 90-97 (CP_VAT_REG_A/B/C)
            0xA0: 0x009F8400, # ARRAY_BASE = Model.vtxs[0]
            0xA1: 0x00F51E60,
            0xA2: 0x00000000,
            # A3 is missing
            0xA4: 0x00F539E0,
            0xA5: 0x00F539E0,
            0xB0: 0x00000006, # ARRAY_STRIDE
            0xB1: 0x00000003,
            0xB2: 0x00000002,
            # B3 is missing
            0xB4: 0x00000004,
            0xB5: 0x00000004,
        }
        self._bpRegs = { # mostly set up by obj41 in fifolog
            0x00: 0x000010, # GEN_MODE
            0x49: 0x000000, # EFB source rectangle top left
            0x4A: 0x007C1F, # EFB source rectangle width, height-1
            0x4B: 0x0BE544, # XFB target address
            0x4D: 0x000010, # stride ?
            0x52: 0x010063, # pe copy execute?
        }
        self._xfRegs = {
            0x1009: 0x00000001, # NumColors - NUMCOLORS - (=0x00)
            0x100A: 0x0A141A00, # Ambient0 - chan Ambient color 0 (=0x00)
            0x100E: 0x0000070E, # COLOR0CNTRL (=0x0401)
            0x100F: 0x00000400, # COLOR1CNTRL (=0x0401)
            0x1010: 0x00000400, # ALPHA0CNTRL (=0x0401)
            0x1011: 0x00000400, # ALPHA1CNTRL (=0x0401)
        }
        # the model has 2218 vtxs and there are (2218 * 6) bytes between
        # vtxs[0] and vtxs[1] so that suggests the stride is indeed 6,
        # as CP 0xB0 suggests
        # so what format? must be just vec3s
        # but that's the format of the raw vtx data,
        # which we already knew,
        # not the idxs given in the dlist.


    def nextByte(self):
        self._offset += 1
        return self.bytes[self._offset - 1]

    def nextU16(self):
        self._offset += 2
        return struct.unpack_from('>H', self.bytes[self._offset - 2:])[0]

    def nextU32(self):
        self._offset += 4
        return struct.unpack_from('>I', self.bytes[self._offset - 4:])[0]

    def nextVtxField(self, fmt):
        if fmt == 0: return False, None
        if fmt == 1: return False, self.nextByte() # XXX verify
        if fmt == 2: return True,  self.nextByte() # is index
        if fmt == 3: return True,  self.nextU16()

    def _resolveIndex(self, name, idx):
        """Given attribute name and index, return the referenced value."""
        if   name == 'POS': return self.model.vtxs[idx]
        elif name == 'NRM': return self.model.normals[idx]
        elif name.startswith('TEX'): return self.model.texCoords[idx]
        elif name.endswith('IDX'): return self.mtxLut[idx//3]
        else:
            raise NotImplementedError("Not implemented indexed %s" % name)

    def _convertVtx(self, vtx):
        """Apply matrix transforms and convert from
        Vec[23][fs] to tuple for each attribute.
        """
        #log.debug("Vtx: %s", vtx)
        # convert types and apply mtxs
        p, n = vtx['POS'], vtx['NRM']
        px, py, pz = p.x, p.y, p.z
        nx, ny, nz = n.x, n.y, n.z
        if vtx['PNMTXIDX'] is not None:
            # we only store the translation vector, not whole mtx
            tx, ty, tz = vtx['PNMTXIDX']
            px, py, pz = px+tx, py+ty, pz+tz
            nx, ny, nz = nx+tx, ny+ty, nz+tz
        vtx['POS'] = (px, py, pz)
        vtx['NRM'] = (nx, ny, nz)

        for i in range(8):
            p = vtx['TEX%d' % i]
            if p is not None:
                x, y = p.x, p.y
                # XXX
                #if vtx['T%dMIDX' % i] is not None:
                #    tx, ty = self.mtxLut[vtx['T%dMIDX' % i]]
                #    x, y = x+tx, y+ty
                vtx['TEX%d' % i] = (x, y)
        return vtx

    def nextVertex(self, vat):
        """Read vertex data from current offset."""
        fmtLo = self._cpRegs[vat + 0x50]
        fmtHi = self._cpRegs[vat + 0x60]
        start = self._offset
        vtx = {'offs':start}
        for name in self.vatFieldOrder:
            reg, idx, size = self.vatFields[name]
            fmt = self._cpRegs[reg + vat]
            mask = (1 << size) - 1

            try: isIdx, val = self.nextVtxField((fmt >> idx) & mask)
            except (IndexError, struct.error):
                log.exception("Index out of range for vtx field %s at 0x%X fmt %d vat %d (len=0x%X)",
                    name, start, (fmt >> idx) & mask, vat,
                    len(self.bytes))
                raise

            if (isIdx or name == 'PNMTXIDX') and (val is not None):
                vtx[name+'_idx'] = val
                try:
                    val = self._resolveIndex(name, val)
                except KeyError:
                    log.warning("Vtx in list %d attr %s idx is invalid (%s / %s)",
                        self.listIdx, name, val,
                        ', '.join(map(str, self.mtxLut.keys())))
            vtx[name] = val

        #log.debug("VAT %d fmt %X, %X => %d bytes", vat, fmtLo, fmtHi, self._offset - start)
        return self._convertVtx(vtx)


    def describeVat(self, vat): # for debug
        fmtNames = ('-', 'D', '1', '2')
        fmtSizes = (0, 1, 1, 2) # XXX verify
        # pretty sure "direct" fmt is defined by VAT regs

        res, size = [], 0
        for name in self.vatFieldOrder:
            reg, idx, nbits = self.vatFields[name]
            mask = (1 << nbits) - 1
            fmt = (self._cpRegs[reg + vat] >> idx) & mask
            if fmt != 0: res.append('%s=%s' % (name, fmtNames[fmt]))
            size += fmtSizes[fmt]

        return '(%X, %X) %s (%d bytes)' % (
            self._cpRegs[0x50+vat],
            self._cpRegs[0x60+vat],
            ' '.join(res), size)


    def setVtxFmt(self, vat, **vals):
        """Set one or more VAT fields.

        Used when following render instructions to implement "Set vtx format".
        """
        for name, val in vals.items():
            reg, idx, size = self.vatFields[name]
            curVal = self._cpRegs[reg + vat]
            mask = (1 << size) - 1
            set  = val & mask
            curVal = (curVal & ~(mask << idx)) | (set << idx)
            self._cpRegs[reg + vat] = curVal


    def setMaterial(self, mat):
        """Set the material to use."""
        self.material = mat


    def setMtxLut(self, lut):
        """Set the matrix lookup table to use."""
        self.mtxLut = lut


    def parse(self):
        #print(gl.Util.Data.dumpHex(self.bytes))
        try:
            while self._offset < len(self.bytes):
                opcode  = self.nextByte()
                drawCmd = opcode & 0xF8
                opName  = self.opNames.get(opcode, self.opNames.get(drawCmd, '???'))
                #log.debug("%04X %02X %s", self._offset-1, opcode, opName)

                if   opcode == 0x00: pass # NOP
                elif opcode == 0x08: self.op_loadCpReg()
                elif opcode == 0x10: self.op_loadXfReg()

                elif opcode == 0x20: # Load Indx A (position mtxs)
                    self.op_loadIndex(0xC)

                elif opcode == 0x28: # Load Indx B (normal mtxs)
                    self.op_loadIndex(0xD)

                elif opcode == 0x30: # Load Indx C (postmtxs)
                    self.op_loadIndex(0xE)

                elif opcode == 0x38: # Load Indx D (lights)
                    self.op_loadIndex(0xF)

                elif opcode == 0x40: # Call Dlist
                    addr = self.nextU32()
                    size = self.nextU32()
                    log.debug("CALL %08X[%08X]", addr, size)

                elif opcode == 0x48: pass # Invalidate Vtx Cache
                elif opcode == 0x61: self.op_loadBpReg()
                elif opcode >= 0x80 and opcode < 0xC0: self.op_draw(opcode)
                else:
                    log.error("DlistParser: unknown opcode 0x%02X at 0x%04X", opcode, self._offset - 1)
                    return self
            log.debug("Parsed OK, end=0x%04X", self._offset)
        except IndexError:
            log.exception("Failed parsing dlist %d at 0x%04X",
                self.listIdx, self._offset)
        return self


    def op_loadIndex(self, which):
        reg = self.nextU32()
        log.debug("IX[%02X] = %08X", which, reg)

    def op_loadCpReg(self):
        reg = self.nextByte()
        val = self.nextU32()
        log.debug("CP[%02X] = %08X", reg, val)
        self._cpRegs[reg] = val

    def op_loadXfReg(self):
        offs = self.nextU32()
        size = self.nextU32()
        log.debug("XF[%08X] = %08X", offs, size)

    def op_loadBpReg(self):
        val = self.nextU32()
        log.debug("BP[%02X] = %06X", val >> 24, val & 0xFFFFFF)
        self._bpRegs[val >> 24] = val & 0xFFFFFF

    def op_draw(self, op):
        offs  = self._offset - 1
        mode  = (op & 0x78) >> 3
        vat   = op &  7
        count = self.nextU16()
        vtxs  = []
        try: name = self.drawModes[mode]
        except IndexError: name = 'unk%02X' % mode

        if self.listIdx in range(12, 13):
            log.debug("%04X %2d Draw(#%3d: %d %s, VAT %d): %s",
                self._offset - 3, self.listIdx,
                len(self.polys), count, name, vat, self.describeVat(vat))

        for i in range(count):
            vtx = self.nextVertex(vat)
            #self._printVtx(i, vtx, vat, offs)
            vtxs.append(vtx)

        #log.debug(" -> P %s", list(map(lambda v: v['POS'], vtxs)))
        #log.debug(" -> M %s", list(map(lambda v: v['PNMTXIDX'], vtxs)))
        #if (self.listIdx == 25 and
        #((len(self.polys) >=  42 and len(self.polys) <  52) or
        #( len(self.polys) >= 163 and len(self.polys) < 164))):
        #    vtxs = []
        self.polys.append({
            'vat':  vat,
            'mode': mode,
            'vtxs': vtxs,
            'list': self.listIdx,
            'offs': offs,
            'idx':  len(self.polys),
            'material': self.material,
        })


    def _printVtx(self, i, vtx, vat, offs):
        # XXX this won't work anymore...
        p = vtx['POS']
        m = vtx['PNMTXIDX']
        t = vtx['TEX0']
        n = vtx['NRM']
        m = "%s%4d" % ("*" if m[0] else " ", m[1])
        t = "%s%4d" % ("*" if t[0] else " ", t[1])
        n = "%s%4d" % ("*" if n[0] else " ", n[1])

        #p = "%s%4d" % ("*" if p[0] else " ", p[1])
        if p[0]: # is index
            try: v = self.model.vtxs[p[1]]
            except IndexError:
                log.error("Vtx %d (0x%X) out of range (offs 0x%X idx %d): vat %d: %s",
                    p[1], p[1], offs, i, vat,
                    self.describeVat(vat))
                for name, val in vtx.items():
                    log.error("  %s = %s", name, val)
                return
            p = "*%4d(%+6.1f, %+6.1f, %+6.1f)" % (p[1], v.x, v.y, v.z)

        log.debug("  v[%2d] P=%s M=%s T=%s N=%s", i, p, m, t, n)
