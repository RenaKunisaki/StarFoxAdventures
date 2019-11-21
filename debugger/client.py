import socket
import struct


class Connection:
    """Abstract base class for connection to server."""

    def isConnected(self) -> bool:
        """Check if connection is still open."""
        raise NotImplementedError

    def send(self, data:bytes):
        """Send data to server."""
        raise NotImplementedError

    def recv(self, size:int) -> bytes:
        """Receive data from server."""
        raise NotImplementedError


class TcpConnection(Connection):
    def __init__(self, host="localhost", port=55020):
        self.host, self.port = host, port
        self.socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        self.socket.connect((host, port))

    def isConnected(self) -> bool:
        return True # XXX

    def send(self, data:bytes):
        """Send data to server."""
        #print("SEND", data)
        self.socket.send(data)

    def recv(self, size:int) -> bytes:
        """Receive data from server."""
        r = self.socket.recv(size)
        #if len(r) > 0: print("RECV", r)
        return r


class Client:
    """Base class for debugger client."""
    def write(self, address, data:bytes):
        """Write to memory."""
        raise NotImplementedError

    def read(self, address, size:(int, str)):
        """Read from memory."""
        raise NotImplementedError

    def readStr(self, address):
        """Read null-terminated string from memory."""
        result = []
        while True:
            b = self.read(address, 'b')
            if b == 0: break
            result.append(chr(b))
        return ''.join(result)

    def pause(self):
        """Pause the game."""
        raise NotImplementedError

    def resume(self):
        """Unpause the game."""
        raise NotImplementedError

    def frameAdvance(self):
        """Advance one frame while paused."""
        raise NotImplementedError

    def callFunc(self, addr, *args):
        """Call a function in the game."""
        raise NotImplementedError


class FileClient(Client):
    """Uses static memory dump."""
    def __init__(self, path):
        self.path = path
        self.file = open(path, 'rb')

    def read(self, address, size:(int, str)):
        """Read from memory."""
        self.file.seek(address - 0x80000000)
        if type(size) is str:
            fmt = size
            size = struct.calcsize(fmt)
            data = self.file.read(size)
            r = struct.unpack(fmt, data)
            if len(r) == 1: return r[0] # grumble
            return r
        else:
            return self.file.read(size)

    def pause(self):
        """Pause the game."""
        pass

    def resume(self):
        """Unpause the game."""
        pass

    def frameAdvance(self):
        """Advance one frame while paused."""
        pass




class GeckoClient(Client):
    """Connects to remote Gecko handler."""
    def __init__(self, connection):
        self.conn = connection

    def _packAddress(self, address):
        return struct.pack(">I", address)

    def _packCmd(self, cmd, address, data=b''):
        return struct.pack(">BI", cmd, address) + data

    def _recv(self, size):
        r = b''
        while len(r) < size: r += self.conn.recv(size-len(r))
        return r

    def _recvBytes(self, size):
        r = b''
        while True:
            b = self.conn.recv(1)
            if b != b"\xAA":
                print("Expected 0xAA but got", b)
                return r
            else: break
        while len(r) < size:
            d = self.conn.recv(size-len(r))
            r += d
            #self.conn.send(b"\xAA") # ACK; BB=retry, CC=cancel
        return r

    def write(self, address, data:bytes):
        """Write to memory."""
        if len(data) == 1:
            self.conn.send(self._packCmd(1, address, b"\x00\x00\x00"+data))
        elif len(data) == 2:
            self.conn.send(self._packCmd(2, address, b"\x00\x00"+data))
        elif len(data) == 3:
            self.write(address, data[0:1])
            self.write(address+1, data[1:2])
            self.write(address+2, data[2:3])
        elif len(data) == 4:
            self.conn.send(self._packCmd(3, address, data))
        else:
            while len(data) >= 4:
                self.write(address, data[0:4])
                address += 4
                data = data[4:]
            if len(data) > 0: self.write(address, data)

    def read(self, address, size:(int, str)):
        """Read from memory."""
        if type(size) is str:
            fmt = size
            size = struct.calcsize(fmt)
            endAddr = address + size
            self.conn.send(b"\x04" +
                self._packAddress(address) + self._packAddress(endAddr))
            r = struct.unpack(fmt, self._recvBytes(size))
            if len(r) == 1: return r[0] # grumble
            return r
        else:
            endAddr = address + size
            self.conn.send(b"\x04" +
                self._packAddress(address) + self._packAddress(endAddr))
            return self._recvBytes(size)

    def pause(self):
        """Pause the game."""
        self.conn.send(b"\x06")

    def resume(self):
        """Unpause the game."""
        self.conn.send(b"\x07")

    def frameAdvance(self):
        """Advance one frame while paused."""
        self.conn.send(b"\x08")

    def callFunc(self, addr, *args):
        """Call a function in the game."""
        data = struct.pack('>BBI', 0xA0, len(args)+1, addr)
        for arg in args: data += struct.pack('>I', arg)
        self.conn.send(data)
