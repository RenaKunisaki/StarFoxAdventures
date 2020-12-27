#!/usr/bin/env python3
"""This is a nice simple server that we could be using if there were any JS
ZLIB libraries that actually worked (or the browsers exposed the ones they
have built in).
"""
import http.server
from http.server import HTTPServer, BaseHTTPRequestHandler
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
httpd.serve_forever()
