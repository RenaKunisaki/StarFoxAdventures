from enum import Enum, IntEnum

class FilterMode(IntEnum):
    """Texture filter modes."""
    NEAR          = 0x00
    LINEAR        = 0x01
    NEAR_MIP_NEAR = 0x02
    LIN_MIP_NEAR  = 0x03
    NEAR_MIP_LIN  = 0x04
    LIN_MIP_LIN   = 0x05
    # XXX where are these ones used?
    #NEAR              = 0x00
    #NEAR_MIP_NEAR     = 0x01
    #NEAR_MIP_LINEAR   = 0x02
    ## 0x03 is unused/invalid
    #LINEAR            = 0x04
    #LINEAR_MIP_NEAR   = 0x05
    #LINEAR_MIP_LINEAR = 0x06
    ## 0x07 is unused/invalid

class OldFilterMode(IntEnum):
    """Only used by some prototype hardware.
    Probably not used by any game except posibly
    some very old prototypes?"""
    NEAR              = 0x00
    LINEAR            = 0x01
    NEAR_MIP_NEAR     = 0x02
    LINEAR_MIP_NEAR   = 0x03
    NEAR_MIP_LINEAR   = 0x04
    LINEAR_MIP_LINEAR = 0x05

class WrapMode(IntEnum):
    """Texture wrap modes."""
    CLAMP  = 0x00
    REPEAT = 0x01
    MIRROR = 0x02

class ImageFormat(IntEnum): # aka GXTexFmt
    """GX image formats."""
    I4     = 0x00
    I8     = 0x01
    IA4    = 0x02
    IA8    = 0x03
    RGB565 = 0x04
    RGB5A3 = 0x05
    RGBA8  = 0x06 # aka RGBA32
    C4     = 0x08
    C8     = 0x09
    C14X2  = 0x0A
    CMPR   = 0x0E # aka BC1

    # following are defined by GX but not used for image files
    ZTF    = 0x10 # Z-texture-format
    CTF    = 0x20 # copy-texture-format only

    R4     = 0x00 | CTF # For copying 4 bits from red.
    RA4    = 0x02 | CTF # For copying 4 bits from red, 4 bits from alpha.
    RA8    = 0x03 | CTF # For copying 8 bits from red, 8 bits from alpha.
    YUVA8  = 0x06 | CTF
    A8     = 0x07 | CTF # For copying 8 bits from alpha.
    R8     = 0x08 | CTF # For copying 8 bits from red.
    G8     = 0x09 | CTF # For copying 8 bits from green.
    B8     = 0x0A | CTF # For copying 8 bits from blue.
    RG8    = 0x0B | CTF # For copying 8 bits from red, 8 bits from green.
    GB8    = 0x0C | CTF # For copying 8 bits from green, 8 bits from blue.

    Z8     = 0x01 | ZTF # For texture copy, specifies upper 8 bits of Z.
    Z16    = 0x03 | ZTF # For texture copy, specifies upper 16 bits of Z.
    Z24X8  = 0x06 | ZTF # For texture copy, copies 24 Z bits and 0xff.

    Z4     = 0x00 | ZTF | CTF # For copying 4 upper bits from Z.
    Z8M    = 0x09 | ZTF | CTF # For copying the middle 8 bits of Z.
    Z8L    = 0x0A | ZTF | CTF # For copying the lower 8 bits of Z.
    Z16L   = 0x0C | ZTF | CTF # For copying the lower 16 bits of Z.

class PaletteFormat(Enum):
    IA8    = 0
    RGB565 = 1
    RGB5A3 = 2
