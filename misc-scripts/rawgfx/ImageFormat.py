import gi
gi.require_version('Gtk', '3.0')
from gi.repository import GObject, Gtk, Gdk, GLib, GdkPixbuf
import sys
import math
import struct
from itertools import islice


def pnm_to_pixbuf(header, data):
    # abuse PNM image format...
    image  = bytes(header, 'ascii') + data
    loader = GdkPixbuf.PixbufLoader.new_with_type('pnm')
    loader.write(image)
    loader.close() # XXX can we reuse the loader?
    return loader.get_pixbuf()
    # or we could just do...
    #image = cairo.ImageSurface.create_for_data(data,
    #    cairo.FORMAT_RGB24, width, height)
    #self.fill.pattern = image
    # except that function isn't implemented yet because lol.

def split(data, length):
    """Return `length` elements at a time from `data`."""
    it = iter(data)
    chunk = list(islice(it, length))
    while chunk:
        yield chunk
        chunk = list(islice(it, length))

def bytes2shortBE(data):
    return (data[0] << 8) | data[1]

def bytes2shortLE(data):
    return (data[1] << 8) | data[0]

def bytes2intBE(data):
    return (data[0] << 24) | (data[1] << 16) | (data[2] << 8) | data[3]

def bytes2intLE(data):
    return (data[3] << 24) | (data[2] << 16) | (data[1] << 8) | data[0]


class ImageFormat:
    """A pixel format for binary data."""

    @classmethod
    def fromData(cls, data):
        pixels = []
        for pixel in split(data, cls.bytes):
            pixels.append(cls._toPixel(pixel))
        return b''.join(pixels)

    @classmethod
    def pixbufFromData(cls, data, width, height):
        pixels = cls.fromData(data)
        header = "P6 %d %d 255 " % (width, height)
        return pnm_to_pixbuf(header, pixels)


class RGBA8888(ImageFormat):
    name  = "RGBA 8888"
    bytes = 4

    @classmethod
    def _toPixel(cls, data):
        return bytes(data[0:3])

class ARGB8888(ImageFormat):
    name  = "ARGB 8888"
    bytes = 4

    @classmethod
    def _toPixel(cls, data):
        return bytes(data[1:4])

class BGRA8888(ImageFormat):
    name  = "BGRA 8888"
    bytes = 4

    @classmethod
    def _toPixel(cls, data):
        return bytes([data[2], data[1], data[0]])

class RGB888(ImageFormat):
    name  = "RGB 888"
    bytes = 3

    @classmethod
    def _toPixel(cls, data):
        return bytes(data)

class BGR888(ImageFormat):
    name  = "BGR 888"
    bytes = 3

    @classmethod
    def _toPixel(cls, data):
        return bytes(data[::-1])

class RGBA5551BE(ImageFormat):
    name  = "RGBA 5551 BE"
    bytes = 2

    @classmethod
    def _toPixel(cls, data):
        val = bytes2shortBE(data)
        return bytes([
            int(((val >> 11) & 0x1F) * (255/31)),
            int(((val >>  6) & 0x1F) * (255/31)),
            int(((val >>  1) & 0x1F) * (255/31)),
        ])

class RGBA5551LE(ImageFormat):
    name  = "RGBA 5551 LE"
    bytes = 2

    @classmethod
    def _toPixel(cls, data):
        val = bytes2shortLE(data)
        return bytes([
            int(((val >> 11) & 0x1F) * (255/31)),
            int(((val >>  6) & 0x1F) * (255/31)),
            int(((val >>  1) & 0x1F) * (255/31)),
        ])

class RGB565BE(ImageFormat):
    name  = "RGB 565 BE"
    bytes = 2

    @classmethod
    def _toPixel(cls, data):
        val = bytes2shortBE(data)
        return bytes([
            int(((val >> 11) & 0x1F) * (255/31)),
            int(((val >>  6) & 0x3F) * (255/63)),
            int(( val        & 0x1F) * (255/31)),
        ])

class RGB565LE(ImageFormat):
    name  = "RGB 565 LE"
    bytes = 2

    @classmethod
    def _toPixel(cls, data):
        val = bytes2shortLE(data)
        return bytes([
            int(((val >> 11) & 0x1F) * (255/31)),
            int(((val >>  6) & 0x3F) * (255/63)),
            int(( val        & 0x1F) * (255/31)),
        ])

class I8(ImageFormat):
    name  = "I8"
    bytes = 1

    @classmethod
    def _toPixel(cls, data):
        return bytes([data[0], data[0], data[0] ])

imageFormats = (
    RGBA8888, ARGB8888, BGRA8888,
    RGB888, BGR888,
    RGBA5551BE, RGBA5551LE,
    RGB565BE, RGB565LE,
    I8,
)
