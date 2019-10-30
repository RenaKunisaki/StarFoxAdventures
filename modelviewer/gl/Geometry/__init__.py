"""support for geometric computation


"""

try:
    __import__('pkg_resources').declare_namespace(__name__)
except:
    from pkgutil import extend_path
    __path__ = extend_path(__path__, __name__)

__author__ = '\n'.join(["Erik O'Shaughnessy",
                        'erik.oshaughnessy@gmail.com',
                        'https://github.com/JnyJny/Geometry'])

__version__ = "0.0.23"

from .exceptions import (ZeroSlope, InfiniteSlope, CollinearPoints,
                         InfiniteLength, ParallelLines, CollinearLines)
from .point import Point, PointSequence, MutablePointSequence
from .line import Line, Segment, Ray
from .ellipse import Ellipse, Circle
from .polygon import Polygon, MutablePolygon
from .triangle2 import Triangle
from .rectangle import Rectangle
from .graph import Node, Edge, Graph


__all__ = ['Point', 'PointSequence', 'MutablePointSequence',
           'Polygon','MutablePolygon',
           'Ellipse', 'Circle',
           'Line', 'Segment', 'Ray',
           'Triangle', 'Rectangle',
           'Graph', 'Node', 'Edge',
           'ZeroSlope', 'InfiniteSlope', 'CollinearPoints',
           'InfiniteLength', 'ParallelLines', 'CollinearLines',
           '__author__', '__version__']
