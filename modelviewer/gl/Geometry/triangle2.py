''' a Triangle
'''


import math
import collections
import itertools
from . import Polygon, Point, Segment, Circle
from .constants import Epsilon, Half_Pi, nearly_eq, Sqrt_3
from .exceptions import *


class Triangle(Polygon):

    '''a pythonic Triangle

    Implements a Triangle object in the XY plane having three
    non-coincident vertices and three intersecting edges.

    Vertices are labeled;  'A', 'B' and 'C'.

    Edges are labeled; 'AB', 'BC' and 'AC'.

    The length of edges opposite each vertex are labeled:

       'a' for the side opposite vertex A.
       'b' for the side opposite vertex B.
       'c' for the side opposite vertex C.

    Interior angles in radians are labeled:

      'alpha' for CAB
      'beta'  for ABC
      'gamma' for BCA

    Usage:

    >>> a = Triangle()
    >>> b = Triangle(A,B,C)    # A,B,C are Points or Point equivalents
    >>> c = Triangle([p,q,r])  # p,q,r are Points or Point equivalents
    >>> d = Triangle([x,y,z],[x,y,z],[x,y,z])
    '''

    @classmethod
    def withAngles(cls, origin=None, base=1, alpha=None,
                   beta=None, gamma=None, inDegrees=False):
        '''
        :origin: optional Point
        :alpha: optional float describing length of the side opposite A
        :beta: optional float describing length of the side opposite B
        :gamma: optional float describing length of the side opposite C
        :return: Triangle initialized with points comprising the triangle
                 with the specified angles.
        '''
        raise NotImplementedError("withAngles")

    @classmethod
    def withSides(cls, origin=None, a=1, b=1, c=1):
        '''
        :origin: optional Point
        :a: optional float describing length of the side opposite A
        :b: optional float describing length of the side opposite B
        :c: optional float describing length of the side opposite C
        :return: Triangle initialized with points comprising the triangle
                 with the specified side lengths.

        If only 'a' is specified, an equilateral triangle is returned.

        '''
        raise NotImplementedError("withSides")

    @classmethod
    def unit(cls,scale=1):
        return cls(Point.units(scale))
        

    def __init__(self, *args, **kwds):
        '''
        :args: iterable of Point or Point equivalents
        :kwds: named Points where recognized names are 'A', 'B' and 'C'.

        If A is an iterable containing Point or Point equivalent objects
        it will be used to initialize up to three points in the triangle.

        '''

        kwds['defaults'] = Point(),Point(1,0),Point(0,1)

        super().__init__(*args,**kwds)
        
        if len(self) != 3:
            raise ValueError(len(self))


    @property
    def AB(self):
        return self.pairs('AB')

    @AB.setter
    def AB(self, iterable):
        self.A, self.B = iterable

    @property
    def BA(self):
        return self.pairs('BA')


    @BA.setter
    def BA(self, iterable):
        self.B, self.A = iterable

    @property
    def BC(self):
        return self.pairs('BC')

    @BC.setter
    def BC(self, iterable):
        self.B, self.C = iterable

    @property
    def CB(self):
        return self.pairs('CB')

    @CB.setter
    def CB(self, iterable):
        self.C, self.B = iterable

    @property
    def AC(self):
        return self.pairs('AC')

    @AC.setter
    def AC(self, iterable):
        self.A, self.C = iterable

    @property
    def CA(self):
        return self.pairs('CA')

    @CA.setter
    def CA(self, iterable):
        self.C, self.A = iterable

    @property
    def ABC(self):
        return [self.A, self.B, self.C]

    @ABC.setter
    def ABC(self, iterable):
        self.A, self.B, self.C = iterable

    @property
    def ccw(self):
        '''
        Result of A.ccw(B,C), float.

        See Point.ccw

        '''
        return self.A.ccw(self.B, self.C)

    @property
    def isCCW(self):
        '''
        True if ABC has a counter-clockwise rotation, boolean.

        '''
        return self.A.isCCW(self.B,self.C)

    @property
    def area(self):
        '''
        Area of the triangle, float.

        Performance note: computed via Triangle.ccw (subtractions and
        multiplications and a divison).

        '''
        return abs(self.ccw) / 2

    @property
    def heronsArea(self):
        '''
        Heron's forumla for computing the area of a triangle, float.

        Performance note: contains a square root.

        '''
        s = self.semiperimeter

        return math.sqrt(s * ((s - self.a) * (s - self.b) * (s - self.c)))

    @property
    def inradius(self):
        '''
        The radius of the triangle's incircle, float.
        '''

        return (self.area * 2) / self.perimeter

    @property
    def circumcenter(self):
        '''
        The intersection of the median perpendicular bisectors, Point.

        The center of the circumscribed circle, which is the circle that
        passes through all vertices of the triangle.

        https://en.wikipedia.org/wiki/Circumscribed_circle#Cartesian_coordinates_2

        BUG: only finds the circumcenter in the XY plane
        '''

        if self.isRight:
            return self.hypotenuse.midpoint

        if self.A.isOrigin:
            t = self
        else:
            t = Triangle(self.A - self.A, self.B - self.A, self.C - self.A)

        if not t.A.isOrigin:
            raise ValueError('failed to translate {} to origin'.format(t))
        
        BmulC = t.B * t.C.yx

        d = 2 * (BmulC.x - BmulC.y)

        bSqSum = sum((t.B ** 2).xy)
        cSqSum = sum((t.C ** 2).xy)

        x = (((t.C.y * bSqSum) - (t.B.y * cSqSum)) / d) + self.A.x
        y = (((t.B.x * cSqSum) - (t.C.x * bSqSum)) / d) + self.A.y

        return Point(x, y)

    @property
    def circumradius(self):
        '''
        Distance from the circumcenter to all the verticies in
        the Triangle, float.

        '''
        return (self.a * self.b * self.c) / (self.area * 4)

    @property
    def circumcircle(self):
        '''
        A circle whose center is equidistant from all the
        vertices of the triangle, Circle.
        '''
        return Circle(self.circumcenter, self.circumradius)

    @property
    def orthocenter(self):
        '''
        The intersection of the altitudes of the triangle, Point.
        '''
        raise NotImplementedError('orthocenter')


    @property
    def hypotenuse(self):
        '''
        The longest edge of the triangle, Segment.
        '''
        return max(self.edges(),key=lambda s:s.length)


    @property
    def alpha(self):
        '''
        The angle described by angle CAB in radians, float.

        '''
        return Segment(self.CA).radiansBetween(Segment(self.BA))

    @property
    def beta(self):
        '''
        The angle described by angle ABC in radians, float.

        '''
        return Segment(self.AB).radiansBetween(Segment(self.CB))

    @property
    def gamma(self):
        '''
        The angle described by angle BCA in radians, float.

        '''
        return Segment(self.BC).radiansBetween(Segment(self.AC))

    @property
    def angles(self):
        '''
        A list of the interior angles of the triangle, list of floats.
        '''
        return [self.alpha, self.beta, self.gamma]

    @property
    def a(self):
        '''
        The length of line segment BC, opposite vertex A, float.

        '''
        return abs(self.B.distance(self.C))

    @property
    def b(self):
        '''
        The length of line segment AC, opposite vertex B, float.

        '''
        return abs(self.A.distance(self.C))

    @property
    def c(self):
        '''
        The length of line segment AB, opposite vertex C, float.

        '''
        return abs(self.A.distance(self.B))

    @property
    def sides(self):
        '''
        A list of edge lengths [a, b, c], list of floats.

        '''
        return [self.a, self.b, self.c]

    @property
    def altitudes(self):
        '''
        A list of the altitudes of each vertex [AltA, AltB, AltC], list of
        floats.

        An altitude is the shortest distance from a vertex to the side
        opposite of it.

        '''
        A = self.area * 2

        return [A / self.a, A / self.b, A / self.c]

    @property
    def isEquilateral(self):
        '''
        True iff all side lengths are equal, boolean.
        '''
        return self.a == self.b == self.c

    @property
    def isIsosceles(self):
        '''
        True iff two side lengths are equal, boolean.
        '''
        return (self.a == self.b) or (self.a == self.c) or (self.b == self.c)

    @property
    def isScalene(self):
        '''
        True iff all side lengths are unequal, boolean.
        '''
        return self.a != self.b != self.c

    @property
    def isRight(self):
        '''
        True if one angle measures 90 degrees (Pi/2 radians), float. 
        '''
        return any([nearly_eq(v,Half_Pi) for v in self.angles])

    @property
    def isObtuse(self):
        '''
        True if one angle measures greater than 90 degrees (Pi/2 radians),
        float.

        '''
        return any([v > Half_Pi for v in self.angles])

    @property
    def isAcute(self):
        '''
        True iff all angles measure less than 90 degrees (Pi/2 radians),
        float.

        '''
        return all([v < Half_Pi for v in self.angles])

    def congruent(self, other):
        '''
        A congruent B

        True iff all angles of 'A' equal angles in 'B' and
        all side lengths of 'A' equal all side lengths of 'B', boolean.

        '''

        a = set(self.angles)
        b = set(other.angles)

        if len(a) != len(b) or len(a.difference(b)) != 0:
            return False

        a = set(self.sides)
        b = set(other.sides)

        return len(a) == len(b) and len(a.difference(b)) == 0

    

        
