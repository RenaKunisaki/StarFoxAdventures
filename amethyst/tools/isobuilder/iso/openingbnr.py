import struct
from .binaryfile import BinaryFile
from .isofile import IsoFile
from PIL import Image
import xml.etree.ElementTree as ET

OPENING_BNR_SIZE = 0x1960
BNR_MAGIC1 = 0x424E5231 # "BNR1" (US/JP)
BNR_MAGIC2 = 0x424E5232 # "BNR2" (EU)
BNR_WIDTH  = 96
BNR_HEIGHT = 32

# note: In the filesystem of european Games with multi-lingual text there may be several .bnr\
# files (opening.bnr, openingUS.bnr, openingEU.bnr, openingJP.bnr). The opening.bnr is a BNR2
# file, it is just like a regular BNR file, except that the metadata at the end repeats several
# times in different languages. 0x1820 through 0x1960 are the first, and it continues in blocks of
# 0x0140.

lut5to8 = (
    0x00, 0x08, 0x10, 0x18, 0x20, 0x29, 0x31, 0x39, 0x41, 0x4A, 0x52,
    0x5A, 0x62, 0x6A, 0x73, 0x7B, 0x83, 0x8B, 0x94, 0x9C, 0xA4, 0xAC,
    0xB4, 0xBD, 0xC5, 0xCD, 0xD5, 0xDE, 0xE6, 0xEE, 0xF6, 0xFF)
lut4to8 = (
    0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77,
    0x88, 0x99, 0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF)
lut3to8 = (0x00, 0x24, 0x48, 0x6D, 0x91, 0xB6, 0xDA, 0xFF)

def decode5A3(val): # cribbed from Dolphin
    if val & 0x8000:
        r = lut5to8[(val >> 10) & 0x1F]
        g = lut5to8[(val >>  5) & 0x1F]
        b = lut5to8[ val        & 0x1F]
        a = 0xFF
    else:
        a = lut3to8[(val >> 12) & 0x7]
        r = lut4to8[(val >>  8) & 0xF]
        g = lut4to8[(val >>  4) & 0xF]
        b = lut4to8[ val        & 0xF]
    return r, g, b, a

def encode5A3(r, g, b, a):
    if a < 224: # 0aaarrrr ggggbbbb
        r, g, b, a = r >> 4, g >> 4, b >> 4, a >> 5
        return (a << 12) | (r << 8) | (g << 4) | b
    else: # 1rrrrrgg gggbbbbb
        r, g, b = r >> 3, g >> 3, b >> 3
        return 0x8000 | (r << 10) | (g << 5) | b

class OpeningBnr(IsoFile):
    """opening.bnr file"""

    def __init__(self, offset:int=0, file:(str,BinaryFile)=None):
        super().__init__("bi2.bin", isDir=False, offset=offset,
            size=OPENING_BNR_SIZE, file=file, fileOffs=offset)

        self.fileName       = "opening.bnr"
        self.magic          = BNR_MAGIC1
        self.graphic        = bytes(0x1800)
        self.shortTitle     = "No Name"
        self.shortDeveloper = "No Developer"
        self.title          = "The Game With No Name"
        self.developer      = "No Developers Inc"
        self.description    = "No description"

        if file is not None:
            self.readFile(file, offset)

    def readFile(self, file:(str,BinaryFile)="boot.bin", offset:int=0):
        """Read boot.bin file."""
        if type(file) is str:
            file = BinaryFile(file, 'rb', offset=offset)
        else: file.seek(offset)
        self.fileName       = file.file.name.split('/')[-1]
        self.magic          = file.readu32()
        self.graphic        = file.readBytes (offset=0x0020, count=0x1800) #RGB5A1 96x32
        self.shortTitle     = file.readString(offset=0x1820, maxLen=0x20)
        self.shortDeveloper = file.readString(offset=0x1840, maxLen=0x20)
        self.title          = file.readString(offset=0x1860, maxLen=0x40)
        self.developer      = file.readString(offset=0x18A0, maxLen=0x40)
        self.description    = file.readString(offset=0x18E0, maxLen=0x80)

        if self.magic not in (BNR_MAGIC1, BNR_MAGIC2):
            raise ValueError("Incorrect/corrupt opening.bnr file")

    def writeToFile(self, file:(str,BinaryFile), chunkSize:int=4096):
        """Write this file's content to disk."""
        if type(file) is str: file = BinaryFile(file, 'wb')
        file.writeu32(self.magic)
        file.padUntil(0x20)
        file.writeBytes(self.graphic)
        file.writeString(self.shortTitle,     length=0x20)
        file.writeString(self.shortDeveloper, length=0x20)
        file.writeString(self.title,          length=0x40)
        file.writeString(self.developer,      length=0x40)
        file.writeString(self.description,    length=0x80)

    def dump(self):
        """Dump to console."""
        print("%s @ 0x%08X:" % (self.fileName, self.offset))
        print(" Magic:                0x%08X (%s)" % (self.magic,
            "OK" if self.magic in (BNR_MAGIC1, BNR_MAGIC2) else "FAIL"))
        print(" Short Title:          \"%s\"" % self.shortTitle)
        print(" Short Developer:      \"%s\"" % self.shortDeveloper)
        print(" Title:                \"%s\"" % self.title)
        print(" Developer:            \"%s\"" % self.developer)
        print(" Description:          \"%s\"" % self.description)

    def toImage(self):
        """Convert banner graphic to image object."""
        data = bytearray(BNR_WIDTH * BNR_HEIGHT * 4)
        rows, cols = BNR_HEIGHT//4, BNR_WIDTH//4
        iIn = 0
        for i in range(rows*cols):
            for y in range(4):
                for x in range(4):
                    row, col = i // cols, i % cols
                    pIdx = ((row * 4) + y) * BNR_WIDTH + (col * 4) + x
                    p = struct.unpack_from('>H', self.graphic, iIn*2)[0] # grumble
                    data[(pIdx*4) : ((pIdx*4)+4)] = decode5A3(p)
                    iIn += 1
        return Image.frombytes('RGBA', (BNR_WIDTH, BNR_HEIGHT), bytes(data), 'raw',
            'RGBA', 0, 1)

    def fromImage(self, img:(Image,str)):
        """Replace banner graphic from image object."""
        data = bytearray(BNR_WIDTH * BNR_HEIGHT * 2)
        if type(img) is str: img = Image.open(img)
        if img.size[0] != BNR_WIDTH or img.size[1] != BNR_HEIGHT:
            raise ValueError("Wrong size for banner image")
        img = img.convert('RGBA')
        pixels = img.load()

        data = bytearray(BNR_WIDTH * BNR_HEIGHT * 2)
        rows, cols = BNR_HEIGHT//4, BNR_WIDTH//4
        iOut = 0
        for i in range(rows*cols):
            for y in range(4):
                for x in range(4):
                    row, col = i // cols, i % cols
                    px, py = (col*4)+x, (row*4)+y
                    data[(iOut*2) : ((iOut*2)+2)] = struct.pack('>H', encode5A3(*pixels[px,py]))
                    iOut += 1
        self.graphic = data

    def toXml(self, path:str):
        """Write text to XML file."""
        root = ET.Element('banner', {'version': '2' if self.magic == BNR_MAGIC2 else '1'})
        e = ET.SubElement(root, 'shortTitle');     e.text = self.shortTitle
        e = ET.SubElement(root, 'shortDeveloper'); e.text = self.shortDeveloper
        e = ET.SubElement(root, 'title');          e.text = self.title
        e = ET.SubElement(root, 'developer');      e.text = self.developer
        e = ET.SubElement(root, 'description');    e.text = self.description
        ET.ElementTree(root).write(path, encoding='utf-8', xml_declaration=True)

    def fromXml(self, path:str):
        """Read text from XML file."""
        with open(path, 'rt') as file: # for some reason this is necessary for encoding.
            root = ET.fromstring(file.read())

        def C(text):
            # this is extremely dumb. XXX probably breaks for Japanese.
            # we can't just export the XML with this encoding because Reasons.
            return bytes(text, 'iso-8859-1')
        self.magic = BNR_MAGIC2 if root.get('version') == '2' else BNR_MAGIC1
        self.shortTitle     = C(root.find('shortTitle').text)
        self.shortDeveloper = C(root.find('shortDeveloper').text)
        self.title          = C(root.find('title').text)
        self.developer      = C(root.find('developer').text)
        self.description    = C(root.find('description').text)
        self.dump()
