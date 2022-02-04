#!/usr/bin/env python3

import http.server
import socketserver

PORT = 8000
Handler = http.server.SimpleHTTPRequestHandler
Handler.extensions_map={
    '.css':	     'text/css',
    '.glsl':     'text/plain',
    '.html':     'text/html',
    '.jpg':      'image/jpg',
    '.js':	     'application/x-javascript',
    '.manifest': 'text/cache-manifest',
    '.png':      'image/png',
    '.svg':	     'image/svg+xml',
    '':          'application/octet-stream', # Default
}

httpd = socketserver.TCPServer(("", PORT), Handler)

print("serving at port", PORT)
try: httpd.serve_forever()
except KeyboardInterrupt: pass
