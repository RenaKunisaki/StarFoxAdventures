
import unittest
import math
import sys

from .. import Line, Segment, Ray, Point
from ..exceptions import *


class LineTestCase(unittest.TestCase):

    def assertIsLine(self, l, msg=None):
        self.assertIsInstance(l, Line)

    def assertLinesEqual(self, p, q, msg=None):
        pass

    def testLineCreation(self):
        i, j, k = Point.units()
        o = Point.origin()

        self.assertIsLine(Line())
        self.assertIsLine(Line(None, None))
        self.assertIsLine(Line(o, i))


class SegmentTestCase(unittest.TestCase):
    pass


class RayTestCase(unittest.TestCase):
    pass
