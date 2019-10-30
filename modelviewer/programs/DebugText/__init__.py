import logging; log = logging.getLogger(__name__)
from importlib import resources
from programs import Common
import struct
import sys
import gl
import numpy as np


class DprintHandler(logging.Handler):
    def __init__(self, debugText):
        super().__init__()
        self._debugText = debugText

    def emit(self, record):
        text = self.format(record)
        #print("DPRINTHANDLER", text)
        self._debugText.printf("%s\n", text)


class DebugText(gl.Program):
    """Handles debug text display."""
    DEBUG_LEVELV_NUM = 9 # logger debug level
    MAX_CHARS = 4096 # buffer size
    CHAR_FMT  = 'iii' # x, y, chr
    scale     = 1.5
    gridSize  = (16, 6) # characters in font
    charSize  = (8, 8) # XXX derive from texture
    chrDataSize = struct.calcsize(CHAR_FMT)
    bufSize   = MAX_CHARS * chrDataSize
    palette   = ( # ANSI colors
        # r    g    b
        (0.00, 0.00, 0.00), # black
        (0.67, 0.00, 0.00), # red
        (0.00, 0.67, 0.00), # green
        (0.67, 0.33, 0.00), # brown/yellow
        (0.00, 0.00, 0.67), # blue
        (0.67, 0.00, 0.67), # magenta
        (0.00, 0.67, 0.67), # cyan
        (0.67, 0.67, 0.67), # gray
        (0.33, 0.33, 0.33), # dark gray
        (1.00, 0.33, 0.33), # red
        (0.33, 1.00, 0.33), # green
        (1.00, 1.00, 0.33), # yellow
        (0.33, 0.33, 1.00), # blue
        (1.00, 0.33, 1.00), # magenta
        (0.33, 1.00, 1.00), # cyan
        (1.00, 1.00, 1.00), # white
    )

    def __init__(self, ctx):
        super().__init__(ctx,
            vertex_shader   = resources.read_text(__package__, 'debugtext.vert'),
            geometry_shader = resources.read_text(__package__, 'debugtext.geom'),
            fragment_shader = resources.read_text(Common, 'common.frag'),
        )
        self.region   = (8, 8, -16, -16) # x, y, w, h
        self.tabSize  = 4 # chars
        self.bufText  = self.ctx.Buffer(self.bufSize, True)
        self._data = bytearray(self.bufSize)

        self.attribs['character'].bindBuffer(
            self.bufText, self.ctx.GL_INT, 3)

        self.setUniforms(
            matModelview  = gl.Util.Matrix.IDENTIY_4x4,
            charBase      = 0x20,
            gridSize      = self.gridSize,
            enableTexture = True,
            textZpos      = -1.0,
            minAlpha      =  0.5,
            modColor      = np.array([1.0, 1.0, 1.0, 1.0], dtype=np.float),
        )

        self.texture = self.ctx.Texture("./data/debugtext.png")
        self.reset()
        self._setupLogger()


    def _setupLogger(self):
        """Install custom logging level to print on scr/een."""
        logging.addLevelName(self.DEBUG_LEVELV_NUM, "DPRINT")
        def dprint(lg, message, *args, **kws):
            if lg.isEnabledFor(self.DEBUG_LEVELV_NUM):
                # Yes, logger takes its '*args' as 'args'.
                lg._log(self.DEBUG_LEVELV_NUM, message, args, **kws)
        logging.Logger.dprint = dprint

        handler = DprintHandler(self)
        handler.setLevel(self.DEBUG_LEVELV_NUM)
        formatter = logging.Formatter('%(message)s')
        handler.setFormatter(formatter)
        logging.getLogger('').addHandler(handler)



    def reset(self):
        """Reset state for new frame."""
        self.charIdx = 0 # position in buffer
        self.cursor  = [0, 0] # x, y position to draw at
        self.color   = (1.0, 1.0, 1.0) # RGB

        view = self.ctx.viewport
        self._maxX = ((view[2] + self.region[2]) - self.region[0]) / self.scale
        self._maxY = ((view[3] + self.region[3]) - self.region[1]) / self.scale


    def run(self):
        if self.charIdx == 0: return # nothing to draw.
        with self: # activate the program
            self.texture.bind()

            #self.attribs['character'].bindBuffer(
            #    self.bufText, self.ctx.GL_INT, 3)

            # write the text buffer
            self.bufText.write(self._data,
                length=self.charIdx * self.chrDataSize)
            #self.ctx.glFlush()

            # set the viewport
            x, y, width, height = self.ctx.viewport
            width  /= self.scale
            height /= self.scale
            self.uniforms['matProjection'].value = \
                gl.Util.Matrix.ortho(0, width, height, 0, 1, 100)

            # draw the text
            self.vao.render(self.ctx.GL_POINTS, count=self.charIdx)

    @property
    def memUsedCpu(self):
        """Estimated amount of CPU-side memory used."""
        return self.MAX_CHARS * self.chrDataSize

    @property
    def memUsedGpu(self):
        """Estimated amount of GPU-side memory used."""
        return self.MAX_CHARS * self.chrDataSize

    def printf(self, fmt, *args):
        text = fmt % args
        i    = 0
        while i < len(text):
            c = text[0]
            text = text[1:]
            if c == '\n':
                self.cursor[0] = 0
                self.cursor[1] += self.charSize[1]
            elif c == '\r':
                self.cursor[0] = 0
            elif c == '\t':
                tab = self.tabSize * self.charSize[0]
                add = tab - (self.cursor[0] % tab)
                if add == 0: add = tab
                self.cursor[0] += add
            elif c == '\b':
                self.cursor[0] = max(0,
                    self.cursor[0] - self.charSize[0])
            elif c == '\x1B':
                text = self._handleEscape(text)
            else:
                self.putchr(c)


    def putchr(self, chr, attr=None):
        if self.charIdx >= self.MAX_CHARS: return
        if attr is None:
            r = int(self.color[0] * 3)
            g = int(self.color[1] * 3)
            b = int(self.color[2] * 3)
            attr = (r << 12) | (g << 10) | (b << 8)

        offs = self.charIdx * self.chrDataSize
        self._data[offs:offs+self.chrDataSize] = struct.pack(self.CHAR_FMT,
            self.cursor[0] + self.region[0],
            self.cursor[1] + self.region[1],
            ord(chr) | attr)
        self.charIdx += 1

        if self.cursor[0] >= self._maxX:
            self.cursor[0] = 0
            self.cursor[1] += self.charSize[1]
        else:
            self.cursor[0] += self.charSize[0]


    def _handleEscape(self, text):
        # we only handle sequences beginning with [
        if text[0] != '[': return text
        text = text[1:]
        args = []
        argv = 0
        haveArg = False

        # read args, separated by , or ;
        while len(text) > 0 and not text[0].isalpha():
            c = text[0]
            if c.isdigit():
                argv = (argv * 10) + int(c)
                haveArg = True
            elif c in ',;':
                args.append(argv)
                argv, haveArg = 0, False
            text = text[1:]
        if haveArg: # save last arg
            args.append(argv)

        # parse command
        c = text[0]
        if   c == 'A': # cursor up
            self.cursor[1] -= args[0] if len(args) > 0 else 1
        elif c == 'B': # cursor down
            self.cursor[1] += args[0] if len(args) > 0 else 1
        elif c == 'C': # cursor right
            self.cursor[0] += args[0] if len(args) > 0 else 1
        elif c == 'D': # cursor right
            self.cursor[0] -= args[0] if len(args) > 0 else 1
        elif c == 'E': # cursor next line
            self.cursor[1] += args[0] if len(args) > 0 else 1
            self.cursor[0] = 0
        elif c == 'F': # cursor previous line
            self.cursor[1] -= args[0] if len(args) > 0 else 1
            self.cursor[0] = 0
        elif c == 'G': # cursor horizontal absolute
            self.cursor[0] = args[0]
        elif c == 'H' or c == 'f': # cursor position: row;col
            self.cursor[1] = args[0] if len(args) > 0 else 0
            self.cursor[0] = args[1] if len(args) > 1 else 0
        elif c == 'm': # select graphic rendition
            self._handleSGR(args)
        # XXX else, warn
        return text[1:]


    def _handleSGR(self, args):
        bright = False
        i = 0
        while i < len(args):
            cmd = args[i]
            i += 1
            if cmd == 0: # reset
                self.color = (1.0, 1.0, 1.0)
                bright     = False

            elif cmd == 1: # bold/bright
                bright = not bright

            # XXX others

            elif cmd >= 30 and cmd <= 37: # set FG color
                self.color = self.palette[
                    (cmd - 30) + (8 if bright else 0)]

            elif cmd == 38: # extended text color
                arg = args[i]
                i += 1
                if arg == 2: # 2;r;g;b
                    r, g, b = args[i : i+3]
                    self.color = (r/255, g/255, b/255)
                    i += 3
                elif arg == 5: # 5;idx
                    idx = args[i]
                    i += 1
                    if idx < 0 or idx > len(self.palette): idx = 0
                    self.color = self.palette[idx]

            elif cmd == 39: # default text color
                self.color = (1.0, 1.0, 1.0)

            # XXX else warn
