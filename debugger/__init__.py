#!/usr/bin/env python3
import re
import sys
import time
import inspect
from util import printf, hexdump
from client import GeckoClient, FileClient, TcpConnection
from game import Game

class App:
    def __init__(self, *args):
        self.client = None
        self.game   = None

    def help(self):
        """Display help text."""
        methods = [
            (func, getattr(self, func))
            for func in dir(self)
            if callable(getattr(self, func))
            and not func.startswith('_')
        ]
        for name, method in sorted(methods, key = lambda it: it[0]):
            doc = method.__doc__.strip()
            doc = re.sub(r'\n\n+\s*', '\n  ', doc)
            printf("%s: %s\n", name, doc)

    def connect(self, where):
        """Connect to Gecko server.

        where: eg "tcp:localhost:55020" or "file:ramdump.bin"
        """
        proto, addr = where.split(':', maxsplit=1)
        if proto == 'tcp':
            if ':' in addr: host, port = addr.split(':', maxsplit=1)
            else: host, port = addr, 55020
            conn = TcpConnection(host, int(port))
            self.client = GeckoClient(conn)

        elif proto == 'file':
            self.client = FileClient(addr)

        else:
            print("Unrecognized protocol:", proto)
            self.help()
            return
        self.game = Game(self.client)

    def _checkConnected(self):
        if self.client is None: raise RuntimeError("Missing 'connect' argument.")

    def listObjects(self):
        """List loaded objects."""
        self._checkConnected()
        self.client.pause()
        self.game.listLoadedObjects()
        self.client.resume()

    def dumpObject(self, addr, destPath):
        """Dump object to binary file."""
        self._checkConnected()
        addr = int(addr, 16)
        self.client.pause()
        obj = self.game.readObject(addr)
        self.client.resume()
        with open(destPath, 'wb') as outFile:
            outFile.write(obj)

    def showObject(self, addr):
        """Display details about object."""
        self._checkConnected()
        addr = int(addr, 16)
        self.game.showObject(addr)
        self.client.conn.send(b"\xAA")

    def listObjModels(self, addr):
        """Display list of object's models."""
        self._checkConnected()
        addr = int(addr, 16)
        self.game.listObjModels(addr)
        self.client.conn.send(b"\xAA")

    def listFiles(self):
        """Display the RAM address of each loaded file."""
        self._checkConnected()
        self.game.listFiles()
        self.client.conn.send(b"\xAA")

    def showHeap(self):
        """Display the heap map."""
        self._checkConnected()
        self.game.showHeap()
        self.client.conn.send(b"\xAA")

    def heapStats(self):
        """Display basic heap info."""
        self._checkConnected()
        self.game.heapStats()
        self.client.conn.send(b"\xAA")

    def watchHeap(self):
        """Display heap stats in real time."""
        self._checkConnected()
        print("\x1B[2J\x1B[H", end='') # clear; cursor to 1,1
        try:
            while True:
                print("\x1B[H", end='') # cursor to 1,1
                self.game.heapStats()
                print("\x1B[7H", end='') # cursor to 1,7, so that
                    # stopping it doesn't erase the info
                self.client.conn.send(b"\xAA") # ACK; BB=retry, CC=cancel
                time.sleep(0.05)
        except KeyboardInterrupt:
            self.client.conn.send(b"\xAA")

    def pause(self):
        """Freeze the game."""
        self._checkConnected()
        self.client.pause()

    def resume(self):
        """Unfreeze the game."""
        self._checkConnected()
        self.client.resume()

    def step(self):
        """Advance one frame while paused."""
        self._checkConnected()
        self.client.frameAdvance()

    def write(self, addr, data):
        """Write data to RAM."""
        self._checkConnected()
        addr = int(addr, 16)
        bts  = []
        for i in range(0, len(data), 2):
            bts.append(int(data[i:i+2], 16))
        bts = bytes(bts)
        print("write %X (%X): %s" % (addr, len(bts), bts))
        self.client.write(addr, bts)

    def read(self, addr, length):
        """Read data from RAM."""
        self._checkConnected()
        addr   = int(addr,   16)
        length = int(length, 16)
        data = self.client.read(addr, length)
        self.client.conn.send(b"\xAA") # ACK
        hexdump(data, addr)

    def fill(self, addr, data, count):
        """Fill RAM region with a pattern."""
        self._checkConnected()
        addr  = int(addr,  16)
        count = int(count, 16)
        bts   = []
        for i in range(0, len(data), 2):
            bts.append(int(data[i:i+2], 16))
        bts = bytes(bts) * count
        print("write %X (%X): %s" % (addr, len(bts), bts))
        self.client.write(addr, bts)

    def watch(self, addr, length):
        """Read data from RAM until stopped."""
        self._checkConnected()
        addr   = int(addr,   16)
        length = int(length, 16)
        try:
            while True:
                data = self.client.read(addr, length)
                print("\x1B[2J\x1B[H", end='') # clear; cursor to 1,1
                hexdump(data, addr)
                self.client.conn.send(b"\xAA") # ACK; BB=retry, CC=cancel
                time.sleep(0.01)
        except KeyboardInterrupt:
            self.client.conn.send(b"\xAA")


def main(*args):
    app = App()

    if len(args) < 1:
        app.help()
        return 0

    while len(args) > 0:
        method = args[0]
        if method.startswith('--'): method = method[2:]

        func = getattr(app, method, None)
        if func is None or not callable(func):
            print("Unknown operation:",method)
            return 1

        sig   = inspect.signature(func)
        nArg  = len(sig.parameters)
        fArgs = args[1:nArg+1]
        args  = args[nArg+1:]

        if len(fArgs) < nArg:
            msg = [method]
            for name in sig.parameters:
                msg.append(name)
            print("Usage:", ' '.join(msg))
            return 1

        func(*fArgs)
    return 0

if __name__ == '__main__':
    main(*sys.argv[1:])
