#!/usr/bin/env python3
import sys
from PIL import Image

inPath  = sys.argv[1]
outPath = sys.argv[2]
width   = int(sys.argv[3])
height  = int(sys.argv[4])
fmt     = sys.argv[5]
offset  = int(sys.argv[6])

def clamp(v, vMin, vMax):
    if v < vMin: v = vMin
    if v > vMax: v = vMax
    return int(v)

def yuv2rgb(y, u, v):
    b = 1.164 * (y - 16)                     + 2.018 * (u - 128)
    g = 1.164 * (y - 16) - 0.813 * (v - 128) - 0.391 * (u - 128)
    r = 1.164 * (y - 16) + 1.596 * (v - 128)
    return (
        clamp(r, 0, 255),
        clamp(g, 0, 255),
        clamp(b, 0, 255),
    )

def extractPixel(px):
    if 'R' in px:
        r = px['R']
        g = px.get('G', 0)
        b = px.get('B', 0)
        a = px.get('A', 255)
    elif 'Y' in px:
        y = px['Y']
        u = px.get('U', 0)
        v = px.get('V', 0)
        a = px.get('A', 255)
        r, g, b = yuv2rgb(y, u, v)

    return (
        clamp(r, 0, 255),
        clamp(g, 0, 255),
        clamp(b, 0, 255),
        clamp(a, 0, 255),
    )

def setPixel(img, px, x, y):
    img[x,y] = extractPixel(px)


def read_yuyv(file, width, height):
    img = Image.new('RGBA', (width, height))
    pixels = img.load()

    for y in range(height):
        for x in range(0, width, 2):
            b = file.read(4)
            y1, y2 = b[0], b[2]
            u, v = b[1], b[3]

            r1, g1, b1 = yuv2rgb(y1, u, v)
            r2, g2, b2 = yuv2rgb(y2, u, v)
            pixels[x,  y] = (r1, g1, b1, 255)
            pixels[x+1,y] = (r2, g2, b2, 255)
    return img


def read_img(file, width, height):
    img = Image.new('RGBA', (width, height))
    pixels = img.load()

    fields = []
    field, size = None, ''
    while len(fmt) > 0:
        c = fmt[0]
        if c in 'RGBAYUVX':
            if field is not None:
                fields.append((field, int(size)))
                size = ''
            field = c
        elif c in '0123456789': size += c
        else: pass
        fmt = fmt[1:]
    if field is not None:
        fields.append((field, int(size)))
    totalSize = sum(map(lambda f: f[1], fields))
    pad = totalSize & 7
    if pad > 0: totalSize += (8-pad)
    sizeBytes = totalSize // 8

    print("fields:", ', '.join(map(str, fields)))
    print("total size", totalSize, sizeBytes)

    x, y = 0, 0
    while True:
        data = inFile.read(sizeBytes)
        if len(data) < sizeBytes: break

        v = 0
        for i in range(sizeBytes):
            #v = (v << 8) | data[i]
            v = (v << 8) | data[len(data)-(i+1)]

        b = totalSize - 1
        p = {}
        for field, size in fields:
            mask  = (1 << size)-1
            shift = 8 - size
            val   = (v >> b) & mask
            p[field] = val << shift #val * ((1 << shift)-1)
            b -= size
        setPixel(pixels, p, x, y)
        x += 1
        if x >= width:
            x = 0
            y += 1
            if y >= height: break


with open(inPath, 'rb') as inFile:
    inFile.seek(offset)
    img = read_yuyv(inFile, width, height)
    img.save(outPath)
