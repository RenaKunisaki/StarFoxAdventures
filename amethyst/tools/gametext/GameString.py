from __future__ import annotations
import struct
import enum
import io
from .util import printf, readStruct, writeStruct
from .FontTextureBuilder import FontEnum

class GameString:
    """A string from the gametext files."""

    class ControlCode(enum.IntEnum):
        SeqId      = 0xE000 # sequence ID (param: ID) (no idea what this is actually used for)
        SeqTime    = 0xE018 # sequence timing (params: time, ?, ?)
        Hint       = 0xE020 # comes before hint text title
        Scale      = 0xF8F4 # set scale (param: scale; 256 = 100%)
        Font       = 0xF8F7 # select font (param: font ID)
        JustLeft   = 0xF8F8 # left justify
        JustRight  = 0xF8F9 # right justify
        JustCenter = 0xF8FA # center justify
        JustFull   = 0xF8FB # full justify
        Color      = 0xF8FF # set color (params: text rg, text ba, shadow rg, shadow ba)

    ControlCodeChars = {
        'seq':   ControlCode.SeqId,
        'time':  ControlCode.SeqTime,
        'scale': ControlCode.SeqTime,
        'hint':  ControlCode.Hint,
        'font':  ControlCode.Font,
        'ljust': ControlCode.JustLeft,
        'rjust': ControlCode.JustRight,
        'cjust': ControlCode.JustCenter,
        'fjust': ControlCode.JustFull,
        'color': ControlCode.Color,
    }

    controlCharLength = { # Unicode code point => number of u16 params to follow
        # some of these are unused? at least, I don't see any code handling them.
        # these are all Private Use characters (E000-F8FF).
        # the order is the same as in the game code.
        0xF8F2:                 2,
        0xF8F3:                 0,
        ControlCode.Scale:      1,
        0xF8F5:                 1,
        0xF8F6:                 1,
        ControlCode.Font:       1,
        ControlCode.JustLeft:   0,
        ControlCode.JustRight:  0,
        ControlCode.JustCenter: 0,
        ControlCode.JustFull:   0,
        0xF8FC:                 0,
        0xF8FD:                 0,
        0xF8FE:                 0,
        ControlCode.Color:      4,
        ControlCode.SeqId:      1,
        ControlCode.SeqTime:    3,
        ControlCode.Hint:       1,
    }

    escapeChars = {
        '\\': '\\',
        't':  '\t',
        'n':  '\n',
        'r':  '\r',
        '{':  '{', # we use { to start a control code
    }

    def __init__(self, value:str = None):
        self.str = value

    def __str__(self):
        if self.str is None: return ""
        return self.str

    def __repr__(self):
        if self.str is None: return 'GameString()'
        return 'GameString("%s")' % self.str

    @staticmethod
    def read(file:io.IOBase, offset:int=None, maxLen:int=50000) -> GameString:
        """Read string from binary file.

        file: File to read from.
        offset: Offset to seek to before reading.

        Reads a UTF-8 string, including the game's control codes which can
        include embedded nulls. (These are converted to a markup format I've made up.)
        Carriage return, line feed, tab, and unrecognized control codes
        are converted to escapes.
        """
        if offset is not None: file.seek(offset, 0)
        res = ''
        C = GameString.ControlCode

        while len(res) < maxLen:
            b = file.read(1)[0]
            if b == 0: break
            if b >= 0x80: # UTF-8 more bytes
                if   b >= 0xF0: n, b = 3, b & 0x07
                elif b >= 0xE0: n, b = 2, b & 0x0F
                else: n, b = 1, 0x1F
                for _ in range(n):
                    b = (b << 6) | (file.read(1)[0] & 0x3F)

            params = []
            try:
                code = GameString.ControlCode(b)
                if code in GameString.controlCharLength:
                    cl = GameString.controlCharLength[code]
                    if cl == 1: params.append(readStruct(file, '>H'))
                    else: params += readStruct(file, '>%dH' % cl)
            except ValueError: code = 0

            if   b == 0x0009:       res += '\\t'
            elif b == 0x000A:       res += '\\n'
            elif b == 0x000D:       res += '\\r'
            elif b == 0x005C:       res += '\\\\'
            elif b == 0x007B:       res += '\\{'
            elif b == C.SeqId:      res += '{seq %d}' % params[0]
            elif b == C.SeqTime:    res += '{time %d %d %d}' % (params[0], params[1], params[2])
            elif b == C.Hint:       res += '{hint %d}' % params[0] # no idea what param is...
            elif b == C.Scale:      res += '{scale %d}' % params[0]
            elif b == C.Font:       res += '{font %d}' % params[0]
            elif b == C.JustLeft:   res += '{ljust}'
            elif b == C.JustRight:  res += '{rjust}'
            elif b == C.JustCenter: res += '{cjust}'
            elif b == C.JustFull:   res += '{fjust}'
            elif b == C.Color:
                c = (params[0], params[1], params[2], params[3])
                if c[0] > 0xFF or c[1] > 0xFF or c[2] > 0xFF or c[3] > 0xFF:
                    print("Color:", c)
                res += '{color %02X%02X%02X%02X}' % c
            elif (b >= 0xE000 and b <= 0xF8FF) or b < 0x20:
                res += '\\u%04X' % b
                for p in params: res += '\\u%04X' % p
            else: res += chr(b)
        return GameString(res)

    def _parseControlCode(self, string:str) -> bytes:
        res = b''
        end = string.find('}')
        params = string[0:end].split(' ')
        cmd = params.pop(0)
        try: cmdId = GameString.ControlCodeChars[cmd]
        except KeyError: raise ValueError("Unrecognized control code: "+cmd)

        paramData = []
        if cmd == 'color': # hex params
            # color is 4 params (r, g, b, a) with high byte ignored.
            # XXX but in some case it's also rrgg bbaa (text) rrgg bbaa (shadow)?
            # is that actually used somewhere?
            paramData.append(int(params[0][0:2], 16))
            paramData.append(int(params[0][2:4], 16))
            paramData.append(int(params[0][4:6], 16))
            paramData.append(int(params[0][6:8], 16))
        else:
            for p in params:
                paramData.append(int(p, 0))
        res += bytes(chr(cmdId), 'utf-8') + struct.pack('>%dH' % len(paramData), *paramData)
        return res, end

    def _parseEscape(self, string:str) -> (str,int):
        # expects string to be the substring following the backslash
        c = string[0]
        if c in GameString.escapeChars: return GameString.escapeChars[c], 1
        elif c == 'x': return chr(int(string[1:3], 16)), 3
        elif c == 'u': return chr(int(string[1:5], 16)), 5
        else: return '\\' + c, 1

    def toBytes(self) -> bytes:
        """Encode to binary string."""
        res, i = b'', 0
        while i < len(self.str):
            c = self.str[i]
            if c == '\\':
                s, l = self._parseEscape(self.str[i+1:])
                res += bytes(s, 'utf-8')
                i += l
            elif c == '{': # control code
                i += 1
                rs, ln = self._parseControlCode(self.str[i:])
                res, i = res+rs, i+ln
            else: res += bytes(c, 'utf-8')
            i += 1
        return res

    def getUsedChars(self) -> set:
        """Return set of characters used in this string.

        The entries are (font, character).
        """
        font = FontEnum.English
        res, i = set(), 0
        while i < len(self.str):
            c = self.str[i]
            if c == '\\':
                s, l = self._parseEscape(self.str[i+1:])
                for ch in s: res.add((font, ch))
                i += l
            elif c == '{': # control code
                i += 1
                s = self.str[i:]
                end = s.find('}')
                code = s[0:end].split(' ')
                if code[0] == 'font': font = FontEnum(int(code[1], 0))
                i += end
            else: res.add((font,c))
            i += 1
        return res
