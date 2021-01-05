#!/usr/bin/env python3
"""This is a much more complex server that needs to exist solely to do ZLIB
decompression because we can't do it client-side because lol.
"""
import logger
logger.setup("WebServer", logfile=None)
log = logger.logging.getLogger(__name__)

import io
import os
import os.path
import sys
import time
import json
import asyncio
import aiohttp
from aiohttp import web
import datetime
import subprocess
import zlib
import struct
from texture import ImageFormat, BITS_PER_PIXEL, decode_image, encode_image


class ZlibView(aiohttp.web.View):
    """A view that decompresses ZLB.

    Given a request path like: /zlb/path?offset=x&length=y
    attempts to decompress the ZLB data at offset `x` with length `y`
    from the specified file.
    If decompression fails, returns error 500.
    If the file doesn't exist, returns error 404.
    Otherwise, returns the uncompressed data.
    """
    async def get(self):
        """Handle GET request."""
        path = self.request.match_info['path']
        while path.startswith('/'): path = path[1:]
        path = path.replace('..', '%2E%2E')
        log.info("ZLB: path=%r Q=%r", path, self.request.query)
        try:
            useHeader = self.request.query.get('header', '0')
            with open(path, 'rb') as file:
                file.seek(0, 2)
                fileSize = file.tell()
                offs = int(self.request.query.get('offset', 0))
                file.seek(offs)

                if useHeader == '1':
                    sig = file.read(4)
                    log.debug("Header @ 0x%X: %s", offs, sig.hex())
                    if sig == b'\xFA\xCE\xFE\xED':
                        decLen, zlbOffs, compLen = struct.unpack('>3I', file.read(12))
                        file.read(zlbOffs * 4) # skip

                    elif sig == b'DIR\0':
                        ver, decLen, compLen = struct.unpack('>3I', file.read(12))
                        data = file.read(compLen)
                        return await self.request.app.sendData(data, self.request,
                            content_type="application/octet-stream")

                    elif sig == b'ZLB\0':
                        ver, decLen, compLen = struct.unpack('>3I', file.read(12))
                        if ver != 1:
                            return await self.request.app.sendError(400, self.request,
                                "Unsupported version 0x%X" % ver)

                    else: return await self.request.app.sendError(400, self.request,
                        "Not ZLB (0x%02X%02X%02X%02X)" % (sig[0], sig[1], sig[2], sig[3]))

                    data = file.read(compLen)
                else:
                    length = int(self.request.query.get('length', fileSize-offs))
                    log.debug("FileSize=0x%X offset=0x%X len=0x%X", fileSize, offs, length)
                    data = file.read()
                try:
                    data = zlib.decompress(data)
                except Exception as ex:
                    log.exception("ZLB decompression failed")
                    return await self.request.app.sendError(400, self.request, str(ex))
                return await self.request.app.sendData(data, self.request,
                    content_type="application/octet-stream")
        except FileNotFoundError as ex:
            return await self.request.app.sendError(404, self.request, str(ex))
        except Exception as ex:
            return await self.request.app.sendError(500, self.request, str(ex))


class TextureView(aiohttp.web.View):
    """A view that decodes textures.

    Given a request path like: /texture/path?offset=x
    attempts to decode a texture from the given path at the given offset
    and return it as a PNG image.
    If decoding fails, returns error 500.
    If the file doesn't exist, returns error 404.
    Otherwise, returns the image.
    """
    _metadata = {} # texID => info
    async def get(self):
        """Handle GET request."""
        path = self.request.match_info['path']

        # ugly hack to pass the actual texture info to the client.
        # we can't use HTTP headers because it's a regular image
        # download and there's no way I know of to catch that.
        # instead of having the client download the entire texture
        # again just to decompress it and read the header, we provide
        # this endpoint to read back the header info of a previously
        # loaded texture.
        tid = int(self.request.query.get('id', 0))
        if self.request.query.get('get', None) == 'info':
            if tid not in self._metadata:
                return await self.request.app.sendError(404, self.request)
            return await self.request.app.sendData(
                json.dumps(self._metadata[tid]), self.request,
                content_type="application/json")

        while path.startswith('/'): path = path[1:]
        path = path.replace('..', '%2E%2E')
        try:
            with open(path, 'rb') as file:
                offset = int(self.request.query.get('offset', 0))
                file.seek(offset)
                try:
                    zlbHeader = file.read(16)
                    sig, ver, compLen, rawLen = struct.unpack_from('>4I', zlbHeader)
                    if sig in (0x44495200, 0x4449526E): # 'DIR\0', 'DIRn'
                        data = file.read(compLen)
                    else:
                        if sig != 0x5A4C4200: # 'ZLB\0'
                            log.error("Not ZLB: 0x%08X @ 0x%06X in %s",
                                sig, offset, path)
                            return await self.request.app.sendError(500, self.request, "Not ZLB")
                        data = zlib.decompress(file.read(compLen))
                    width, height = struct.unpack_from('>HH', data, 0x0A)
                    nFrames = struct.unpack_from('>B', data, 0x19)[0] # grumble
                    fmtId = struct.unpack_from('>B', data, 0x16)[0] # grumble
                    format = ImageFormat(fmtId)
                    bpp = BITS_PER_PIXEL[format]
                    dataLen = width * height * bpp // 8
                    log.debug("Texture %s: size=%dx%d frames=%d fmt=0x%02X",
                        path, width, height, nFrames, fmtId)
                    image = decode_image(data[0x60:],
                        None, # palette_data
                        format, # image_format
                        None, # palette_format
                        0, # num_colors (for palettes)
                        width, height)

                    # convert to PNG
                    data = io.BytesIO()
                    image.save(data, 'PNG')

                except Exception as ex:
                    #return await self.request.app.sendError(400, self.request, str(ex))
                    log.error("Texture decode fail: %r", ex)
                    return await self.request.app.sendFile('blank.png', self.request,
                        content_type="image/png")

                self._metadata[tid] = {
                    'format':       hex(fmtId),
                    'offset':       hex(offset),
                    'nFrames':      str(nFrames),
                    'packedLength': hex(compLen),
                    'rawLength':    hex(rawLen),
                }

                return await self.request.app.sendData(data.getvalue(), self.request,
                    content_type="image/png")

        except FileNotFoundError as ex:
            return await self.request.app.sendError(404, self.request, str(ex))
        except Exception as ex:
            #return await self.request.app.sendError(500, self.request, str(ex))
            log.error("Texture decode fail: %r", ex)
            return await self.request.app.sendFile('blank.png', self.request,
                content_type="image/png")


class WebServer(web.Application):
    extensions={
        '.css':	     'text/css',
        '.glsl':     'text/plain',
        '.html':     'text/html',
        '.jpg':      'image/jpg',
        '.js':	     'application/x-javascript',
        '.json':     'application/json',
        '.manifest': 'text/cache-manifest',
        '.mp4':      'video/mp4',
        '.png':      'image/png',
        '.svg':	     'image/svg+xml',
        '.xml':	     'text/xml',
    }

    def __init__(self):
        loop = asyncio.get_event_loop()
        super().__init__(loop=loop)
        self.root = '.'
        self.port=8000
        self._setupRoutes()

    def addView(self, path, view):
        self.router.add_route('*', path, view)

    def _setupRoutes(self):
        self.addView('/zlb/{path:.*}', ZlibView)
        self.addView('/texture/{path:.*}', TextureView)

        def handle_page(request):
            try:
                ctype = None
                path = request.match_info['path']
                while path.startswith('/'): path = path[1:]
                while path.endswith('/'): path = path[0:-1]
                src  = os.path.join(self.root, path)
                file = os.path.basename(src)
                if file == '': src += '/index.html'
                elif '.' not in file: src += '.html'
                if not os.path.isfile(src):
                    log.warning("404: %s", src)
                    return self.sendError(404, request)
                #log.debug("Send page '%s'", src)
                if ctype is None:
                    name, ext = os.path.splitext(src)
                    ctype = self.extensions.get(ext, 'application/octet-stream')
                return self.sendFile(src, request, content_type=ctype)
            except:
                log.exception("Error handling request")
                raise
        self.router.add_get('/{path:.*}', handle_page)

    def run(self):
        async def _start():
            try:
                self.runner = web.AppRunner(self)
                await self.runner.setup()
                self.site = web.TCPSite(self.runner, port=self.port,
                    backlog=1024,
                    reuse_address=True,
                    reuse_port=True,
                )
                await self.site.start()
            except:
                log.exception("Error in start")

        log.info("Starting")
        try:
            start = time.time()
            while True:
                try:
                    self.loop.run_until_complete(_start())
                    break
                except OSError as ex:
                    if 'address already in use' not in str(ex): raise
                    print(".", end='')
                    time.sleep(1)
                    if time.now() - start >= 30:
                        log.error("Unable to open socket."+
                            "Is another instance running?")
                        return
        except:
            log.exception("Start failed")
            return

        try:
            log.info("Started.")
            self.loop.run_forever()
        except KeyboardInterrupt:
            print("Terminated by KeyboardInterrupt.")
        except:
            log.exception("Unhandled exception")


    async def sendFile(self, path, request, content_type=None,
    charset=None, headers={}):
        """Send a file as response.

        path: Path to file to send.
        request: The request that we're responding to.
        content_type: MIME type to send in header.
        charset: Charset to send in header.
        headers: Other HTTP headers to set.
        """
        #log.debug("sendFile(%s)", path)
        try:
            file = io.open(path, 'rb')
        except FileNotFoundError:
            log.warn("sendFile(%s): not found", path)
            raise
        except:
            log.exception("Failed opening: %s", path)
            return await self.sendError(500)


        if content_type is None:
            cmd = ('file', '-bi', path)
            try:
                typ = subprocess.check_output(cmd).decode('utf-8').strip()
                # strip the '; charset=binary'
                # since chrome chokes on it
                content_type = typ.split(';', maxsplit=1)[0]
            except:
                log.exception("Failed executing: %s", cmd)
                return await self.sendError(500)

        try:
            if content_type is None:
                log.error("ctype is None on %s", path)

            if content_type.startswith('text/') and charset is None: # what the shit
                charset = 'utf-8'
            if charset is not None:
                content_type += '; charset='+charset

            resp = aiohttp.web.StreamResponse(headers=headers)
            #log.debug("jfkhhsfkfdjghgfhdgjs %s %r", type(content_type).__name__, content_type)
            resp.content_type = content_type

            file.seek(0, 2) # end
            resp.content_length = file.tell()
            file.seek(0) # start

            await resp.prepare(request)
            while True:
                data = file.read(io.DEFAULT_BUFFER_SIZE)
                if data == b'': break
                await resp.write(data)
                await resp.drain()
            await resp.write_eof()
            return resp
        except:
            log.exception("sendFile(%s) failed", path)
            return await self.sendError(500)


    async def sendData(self, data:(str,bytes), request, content_type=None,
    charset=None, headers={}):
        """Send data as response.

        data: Data to send.
        request: The request that we're responding to.
        content_type: MIME type to send in header.
        charset: Charset to send in header.
        headers: Other HTTP headers to set.
        """
        try:
            if type(data) is str:
                if charset is None: charset = 'utf-8'
                data = bytes(data, charset)

            if content_type is not None and charset is not None:
                content_type += '; charset='+charset

            resp = aiohttp.web.StreamResponse(headers=headers)
            resp.content_type = content_type
            resp.content_length = len(data)
            await resp.prepare(request)
            await resp.write(data)
            await resp.drain()
            await resp.write_eof()
            return resp
        except:
            log.exception("sendData(%s) failed", data[0:64])
            return await self.sendError(500)


    async def sendError(self, code, request, content=None,
    content_type="text/plain;charset=utf-8"):
        """Send an error response.

        code: HTTP status code.
        request: the request that we're responding to.
        content: the body of the response.
        """
        try:
            log.debug("send error code=%r content=%r", code, content)
            if content is None: content = str(code)
            if type(content) is str: content = bytes(content, 'utf-8')
            resp = aiohttp.web.StreamResponse(status=code)
            resp.content_type = content_type
            resp.content_length = len(content)
            await resp.prepare(request)
            await resp.write(content)
            await resp.drain()
            await resp.write_eof()
            return resp
        except:
            log.exception("Failed sending error: code=%r content=%r type=%r", code, content, content_type)
            raise

app = WebServer()
app.run()

#PORT = 8000
#Handler = http.server.SimpleHTTPRequestHandler
#Handler.extensions_map={
#    '.css':	     'text/css',
#    '.glsl':     'text/plain',
#    '.html':     'text/html',
#    '.jpg':      'image/jpg',
#    '.js':	     'application/x-javascript',
#    '.manifest': 'text/cache-manifest',
#    '.png':      'image/png',
#    '.svg':	     'image/svg+xml',
#    '':          'application/octet-stream', # Default
#}
#
#httpd = socketserver.TCPServer(("", PORT), Handler)
#
#print("serving at port", PORT)
#httpd.serve_forever()
