#!/usr/bin/env python3
"""Program for viewing a binary file as raw pixel data."""
import gi
gi.require_version('Gtk', '3.0')
from gi.repository import GObject, Gtk, Gdk, GLib, GdkPixbuf
import sys
import math
import struct
from PIL import Image, ImageDraw, ImageFont
from itertools import islice

from Canvas import Canvas
from ImageFormat import imageFormats
from ComboBox import ComboBox


class MainWindow(Gtk.Window):
    def __init__(self, file):
        super().__init__()
        self.connect('delete-event', Gtk.main_quit) # exit when window closed
        self.connect('size-allocate', self._on_resize)
        self.connect('key-press-event', self._on_key)
        self.file   = file
        self.width  = 256
        self.scale  = 2
        self.offset = 0
        self.format = imageFormats[0]

        file.seek(0, 2)
        self.fileSize = file.tell()
        file.seek(0)


        self.set_default_size(1024, 768)

        mainBox = Gtk.Box(orientation='horizontal')
        self.add(mainBox)

        self.canvas = Canvas(self)
        mainBox.pack_start(self.canvas, True, True, 0)

        rightBox = Gtk.Box(orientation='vertical')
        mainBox.pack_start(rightBox, False, False, 0)

        # format box
        box = Gtk.Box(orientation='horizontal')
        rightBox.pack_start(box, False, False, 0)
        box.pack_start(Gtk.Label(label="Format:"), False, False, 0)

        fmts = [fmt.name for fmt in imageFormats]
        self.lstFormat = ComboBox(fmts, onChange=lambda: self.redraw())
        box.pack_start(self.lstFormat, False, False, 0)


        # offset box
        box = Gtk.Box(orientation='horizontal')
        rightBox.pack_start(box, False, False, 0)
        box.pack_start(Gtk.Label(label="Offset: 0x"), False, False, 0)
        self.txtOffs = Gtk.Entry(
            input_purpose  = Gtk.InputPurpose.NUMBER,
            overwrite_mode = True)
        self.txtOffs.set_text('000000')
        self.txtOffs.connect('changed', lambda widget: self.redraw())
        box.pack_start(self.txtOffs, False, False, 0)

        # size box
        box = Gtk.Box(orientation='horizontal')
        rightBox.pack_start(box, False, False, 0)
        box.pack_start(Gtk.Label(label="Size: 0x"), False, False, 0)
        self.txtFileSize = Gtk.Entry(editable = False)
        self.txtFileSize.set_text('%06X' % self.fileSize)
        box.pack_start(self.txtFileSize, False, False, 0)

        # width box
        box = Gtk.Box(orientation='horizontal')
        rightBox.pack_start(box, False, False, 0)
        box.pack_start(Gtk.Label(label="Width:"), False, False, 0)
        self.txtWidth = Gtk.Entry(
            input_purpose  = Gtk.InputPurpose.NUMBER,
            overwrite_mode = True)
        self.txtWidth.set_text(str(self.width))
        self.txtWidth.connect('changed', lambda widget: self.redraw())
        box.pack_start(self.txtWidth, False, False, 0)

        # scale box
        box = Gtk.Box(orientation='horizontal')
        rightBox.pack_start(box, False, False, 0)
        box.pack_start(Gtk.Label(label="Scale:"), False, False, 0)
        self.txtScale = Gtk.Entry(
            input_purpose  = Gtk.InputPurpose.NUMBER,
            overwrite_mode = True)
        self.txtScale.set_text(str(self.scale))
        self.txtScale.connect('changed', lambda widget: self.redraw())
        box.pack_start(self.txtScale, False, False, 0)


    def redraw(self):
        try:
            self.offset = int(  self.txtOffs .get_text(), 16)
            self.width  = int(  self.txtWidth.get_text())
            self.scale  = float(self.txtScale.get_text())
            self.format = imageFormats[self.lstFormat.selected]
        except: # one of the fields is invalid
            return
        self.canvas.reread()
        self.queue_draw()


    def _on_resize(self, widget, size):
        self.redraw()

    def _on_key(self, widget, event):
        lineSize = self.width * self.format.bits * 8
        if event.keyval == Gdk.KEY_Up:
            offs = self.offset - lineSize
            if offs < 0: offs = 0
            self.txtOffs.set_text('%06X' % offs)
        elif event.keyval == Gdk.KEY_Down:
            offs = self.offset + lineSize
            self.txtOffs.set_text('%06X' % offs)
        elif event.keyval == Gdk.KEY_Left:
            size = self.width - 1
            if size < 1: size = 1
            self.txtWidth.set_text(str(size))
        elif event.keyval == Gdk.KEY_Right:
            size = self.width + 1
            self.txtWidth.set_text(str(size))
        elif event.keyval == Gdk.KEY_Page_Up:
            offs = self.offset - (lineSize * 256)
            if offs < 0: offs = 0
            self.txtOffs.set_text('%06X' % offs)
        elif event.keyval == Gdk.KEY_Page_Down:
            offs = self.offset + (lineSize * 256)
            self.txtOffs.set_text('%06X' % offs)
        else: return False # don't eat event
        return True # eat event



def main():
    with open(sys.argv[1], 'rb') as file:
        win = MainWindow(file)
        win.show_all()
        Gtk.main()


if __name__ == '__main__':
    main()
