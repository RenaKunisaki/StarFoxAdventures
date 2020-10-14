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
import asyncio
import aiohttp
from aiohttp import web
import datetime
import subprocess
import zlib


class ZlibView(aiohttp.web.View):
    async def get(self):
        """Handle GET request."""
        path = self.request.match_info['path']
        while path.startswith('/'): path = path[1:]
        path = path.replace('..', '%2E%2E')
        log.info("ZLB: path=%r Q=%r", path, self.request.query)
        try:
            with open(path, 'rb') as file:
                file.seek(int(self.request.query.get('offset', 0)))
                data = file.read(int(self.request.query.get('length', 0)))
                try:
                    data = zlib.decompress(data)
                except Exception as ex:
                    return await self.request.app.sendError(400, self.request, str(ex))
                return await self.request.app.sendData(data, self.request,
                    content_type="application/octet-stream")
        except FileNotFoundError as ex:
            return await self.request.app.sendError(404, self.request, str(ex))
        except Exception as ex:
            return await self.request.app.sendError(500, self.request, str(ex))


class WebServer(web.Application):
    extensions={
        '.css':	     'text/css',
        '.glsl':     'text/plain',
        '.html':     'text/html',
        '.jpg':      'image/jpg',
        '.js':	     'application/x-javascript',
        '.manifest': 'text/cache-manifest',
        '.png':      'image/png',
        '.svg':	     'image/svg+xml',
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
