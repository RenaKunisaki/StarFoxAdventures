''' a pythonic Triangle
'''

import math
import collections
import itertools
from . import Point, Segment, Circle
from .constants import Epsilon, Half_Pi, nearly_eq, Sqrt_3
from .exceptions import *


class Triangle(object):
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
    >>> e = Triangle(A=p0,B=p1,C=p2)

    '''
    vertexNames = 'ABC'

    @classmethod
    def random(cls, origin=None, radius=1):
        '''
        :origin: - optional Point subclass
        :radius: - optional float
        :return: Triangle

        Creates a triangle with random coordinates in the circle
        described by (origin,radius).  If origin is unspecified, (0,0)
        is assumed. If the radius is unspecified, 1.0 is assumed.

        '''
        # XXX no collinearity checks, possible to generate a
        #     line (not likely, just possible).
        #
        pts = set()
        while len(pts) < 3:
            p = Point.random(origin, radius)
            pts.add(p)
        return cls(pts)

    @classmethod
    def equilateral(cls, origin=None, side=1):
        '''
        :origin: optional Point
        :side: optional float describing triangle side length
        :return: Triangle initialized with points comprising a
                 equilateral triangle.

        XXX equilateral triangle definition

        '''
        o = Point(origin)

        base = o.x + side
        h = 0.5 * Sqrt_3 * side + o.y
        
        return cls(o, [base, o.y], [base / 2, h])
        #return cls.withAngles(alpha=60,beta=60,gamma=60,base=side,inDegrees=True)

    @classmethod
    def isosceles(cls, origin=None, base=1, alpha=90):
        '''
        :origin: optional Point
        :base: optional float describing triangle base length
        :return: Triangle initialized with points comprising a
                 isosceles triangle.

        XXX isoceles triangle definition

        '''
        o = Point(origin)
        base = o.x + base

        return cls(o, [base, o.y], [base / 2, o.y + base])


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
    def unit(cls):
        '''
        Returns a unit triangle.

        '''
        return cls(Point.units())

    def __init__(self, *args, **kwds):

        self(*args, **kwds)

    def __call__(self, *args, **kwds):
        '''

        Arguments are:
        - positional arguments corresponding to A, B, and C
        - sequence of Point or point equivalents
        - keyword mappings: A, B and C
        '''

        # XXX not happy with this yet

        if len(args) == 0 and len(kwds) == 0:
            return

        if len(args) == 1:
            if issubclass(type(args[0]), Point):
                self.A = args[0]
            else:
                if isinstance(args[0], collections.Iterable):
                    if len(args[0]) == 1:
                        self.A = args[0]
                    if len(args[0]) == 2:
                        self.AB = args[0]
                    if len(args[0]) == 3:
                        self.ABC = args[0]

        if len(args) == 2:
            self.AB = args

        if len(args) >= 3:
            self.ABC = args[:3]

        for name in self.vertexNames:
            try:
                setattr(self, name, kwds[name])
            except:
                pass

    @property
    def A(self):
        '''
        First vertex of triangle, Point subclass.

        '''
        try:
            return self._A
        except AttributeError:
            pass
        self._A = Point()
        return self._A

    @A.setter
    def A(self, newValue):
        self.A.xyz = newValue

    @property
    def B(self):
        '''
        Second vertex of triangle, Point subclass.

        '''
        try:
            return self._B
        except AttributeError:
            pass
        self._B = Point(1, 0)
        return self._B

    @B.setter
    def B(self, newValue):
        self.B.xyz = newValue

    @property
    def C(self):
        '''
        Third vertex of triangle, Point subclass.

        '''
        try:
            return self._C
        except AttributeError:
            pass
        self._C = Point(0, 1)
        return self._C

    @C.setter
    def C(self, newValue):
        self.C.xyz = newValue

    @property
    def ABC(self):
        '''
        A list of the triangle's vertices, list.

        '''
        try:
            return self._ABC
        except AttributeError:
            pass
        self._ABC = [self.A, self.B, self.C]
        return self._ABC

    @ABC.setter
    def ABC(self, iterable):
        self.A, self.B, self.C = iterable

    @property
    def AB(self):
        '''
        Vertices A and B, list.

        '''
        try:
            return self._AB
        except AttributeError:
            pass
        self._AB = [self.A, self.B]
        return self._AB

    @AB.setter
    def AB(self, iterable):
        self.A, self.B = iterable

    @property
    def BA(self):
        '''
        Vertices B and A, list.

        '''
        try:
            return self._BA
        except AttributeError:
            pass
        self._BA = [self.B, self.A]
        return self._BA

    @BA.setter
    def BA(self, iterable):
        self.B, self.A = iterable

    @property
    def AC(self):
        '''
        Vertices A and C, list.

        '''
        try:
            return self._AC
        except AttributeError:
            pass
        self._AC = [self.A, self.C]
        return self._AC

    @AC.setter
    def AC(self, iterable):
        self.A, self.C = iterable

    @property
    def CA(self):
        '''
        Vertices C and A, list.

        '''
        try:
            return self._CA
        except AttributeError:
            pass
        self._CA = [self.C, self.A]
        return self._CA

    @CA.setter
    def CA(self, iterable):
        self.C, self.A = iterable

    @property
    def BC(self):
        '''
        Vertices B and C, list.

        '''
        try:
            return self._BC
        except AttributeError:
            pass
        self._BC = [self.B, self.C]
        return self._BC

    @BC.setter
    def BC(self, iterable):
        self.B, self.C = iterable

    @property
    def CB(self):
        '''
        Vertices C and B, list.

        '''
        try:
            return self._CB
        except AttributeError:
            pass
        self._CB = [self.C, self.B]
        return self._CB

    @CB.setter
    def CB(self, iterable):
        self.C, self.B = iterable

    @property
    def vertices(self):
        '''
        Alias for property "ABC", list.

        '''
        return self.ABC

    @vertices.setter
    def vertices(self, iterable):
        self.ABC = iterable

    @property
    def segments(self):
        '''
        A list of the Triangle's line segments [AB, BC, AC], list.

        '''
        return [Segment(self.AB),
                Segment(self.BC),
                Segment(self.AC)]

    @property
    def hypotenuse(self):
        '''
        The longest side of the triangle, Segment.

        If the triangle is an equilateral, the first Segment
        in the segments property is returned.

        '''
        return max(self.segments,key=lambda s:s.length)

    @property
    def centroid(self):
        '''
        Intersection of the medians, Point.

        '''
        return sum(self.vertices) / 3

    @property
    def incenter(self):
        '''
        The intersection of angle bisectors, Point.

        '''
        return sum([s * p for s, p in zip(self.sides, self.vertices)]) / 3

    @property
    def inradius(self):
        '''
        The radius of the triangle's incircle, float.

        '''
        ## XXX need to know the points on the triangle
        ##     where the incircle touches to verify the
        ##     inradius computation.  Could verify by
        ##     intersecting each segment with the circle
        ##     and checking to make sure the segment is
        ##     a tangent.
        return (self.area * 2) / self.perimeter

    @property
    def incircle(self):
        '''
        The circle inscribed in the triangle whose center
        is at 'incenter' with radius 'inradius', Circle.

        '''
        return Circle(self.incenter, self.inradius)

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
            # translate triangle to origin
            t = Triangle(self.A - self.A, self.B - self.A, self.C - self.A)
            # XXX translation would be easier by defining add and sub for points
            # t = self - self.A  

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
        vertices in the triangle, Circle.

        '''
        return Circle(self.circumcenter, self.circumradius)

    @property
    def orthocenter(self):
        '''
        The intersection of the altitudes of the triangle, Point.

        '''
        raise NotImplementedError('orthocenter')

    @property
    def midpoints(self):
        '''
        The midpoints for segments AB, BC and AC, list of Points.

        '''
        return [s.midpoint for s in self.segements]

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
        List of angles [alpha, beta, gamma], list of floats.

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
        A list of side lengths [a, b, c], list of floats.
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
        a = self.area * 2

        return [a / self.a, a / self.b, a / self.c]

    @property
    def isCCW(self):
        '''
        True if ABC has a counter-clockwise rotation, boolean.

        '''
        return self.A.isCCW(self.B, self.C)

    @property
    def ccw(self):
        '''
        Result of ccw(A,B,C), float.

        '''
        return self.A.ccw(self.B, self.C)

    @property
    def area(self):
        '''
        Area of the triangle, float.

        Performance note: computed via Point.ccw (subtractions and
        multiplications).

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
    def perimeter(self):
        '''
        Perimeter of the triangle: |AB|+|BC|+|AC|, float.

        '''
        return sum(self.sides)

    @property
    def semiperimeter(self):
        '''
        Semiperimeter of the triangle: (|AB|+|BC|+|AC|) / 2, float.

        '''
        return self.perimeter / 2

    @property
    def isEquilateral(self):
        '''
        True if all sides of the triangle are the same length.

        All equilateral triangles are also isosceles.
        All equilateral triangles are also acute.

        '''
        if not nearly_eq(self.a, self.b):
            return False

        if not nearly_eq(self.b, self.c):
            return False

        return nearly_eq(self.a, self.c)

    @property
    def isIsosceles(self):
        '''
        True if two sides of the triangle are the same length.

        '''
        if self.a == self.b:
            return True

        if self.a == self.c:
            return True

        return self.b == self.c

    @property
    def isScalene(self):
        '''
        True if all sides are unequal in length.

        '''
        return self.a != self.b != self.c

    @property
    def isRight(self):
        '''
        True if one angle is equal to Pi/2 radians.

        '''
        return any([nearly_eq(Half_Pi, a) for a in self.angles])

    @property
    def isObtuse(self):
        '''
        True if one angle is greater than Pi/2 radians.

        '''
        return any([a > Half_Pi for a in self.angles])

    @property
    def isAcute(self):
        '''
        True if all angles are less than 90 degrees ( Pi/2 radians).

        '''
        return all([a < Half_Pi for a in self.angles])

    def __str__(self):
        # if the mapping behavior stays in
        # candidate for pointcollection
        # return ', '.join(['{}=({})'.format(l,p) for l,p in self])
        return 'A={t.A!r}, B={t.B!r}, C={t.C!r}'.format(t=self)

    def __repr__(self):
        return '{o.__class__.__name__}({o!s})'.format(o=self)

    #
    # XXX treating a triangle like a mapping, not sure about this.
    #

    def __len__(self):
        return 3

    def __iter__(self):
        return zip(self.vertexNames, self.vertices)

    def __getitem__(self, key):
        '''
        '''
        if key in ['A', 0]:
            return self.A

        if key in ['B', 1]:
            return self.B

        if key in ['C', 2]:
            return self.C

        raise KeyError(key)

    def __setitem__(self, key, newValue):

        if key in ['A', 0]:
            return self.A

        if key in ['B', 1]:
            return self.B

        if key in ['C', 2]:
            return self.C

        raise KeyError(key)
    ##
    # end of mapping gorp
    ##

    def __eq__(self, other):
        '''
        x == y

        True iff len(set(x.vertices).difference(set(y.vertices))) == 0

        The ordering of points in either Triangle is not considered.

        '''
        a = set(self.vertices)
        b = set(other.vertices)
        return len(a.difference(b)) == 0

    def congruent(self, other):
        '''
        True iff all self and other angles are equal AND
                 all self and other sides are equal.

        '''

        a = set(self.angles)
        b = set(other.angles)
        if len(a) != len(b) or len(a.difference(b)) != 0:
            return False

        a = set(self.sides)
        b = set(other.sides)
        return len(a) == len(b) and len(a.difference(b)) == 0


    def __contains__(self, point):
        '''
        :param: point - Point subclass
        :return: boolean

        True if point is inside the triangle or on any of it's sides.

        '''
        
        try:
            r = [self.A.isCCW(self.B, point),
                 self.B.isCCW(self.C, point),
                 self.C.isCCW(self.A, point)]
        except CollinearPoints:
            # point is on the lines AB, BC, or CA and that counts.
            return True

        return not (any(r) and not all(r))

    def swap(self, side='AB', inplace=False):
        '''
        :side: - optional string
        :inplace: - optional boolean
        :return: Triangle with flipped side.

        The optional side paramater should have one of three values:
        AB, BC, or AC.

        Changes the order of the triangle's points, swapping the
        specified points. Doing so will change the results of isCCW
        and ccw.

        '''
        try:
            flipset = {'AB': (self.B.xyz, self.A.xyz, self.C.xyz),
                       'BC': (self.A.xyz, self.C.xyz, self.B.xyz),
                       'AC': (self.C.xyz, self.B.xyz, self.A.xyz)}[side]
        except KeyError as e:
            raise KeyError(str(e))

        if inplace:
            self.ABC = flipset
            return self

        return Triangle(flipset)

    def doesIntersect(self, other):
        '''
        :param: other - Triangle or Line subclass
        :return: boolean

        Returns True iff:
           Any segment in self intersects any segment in other.

        '''
        otherType = type(other)

        if issubclass(otherType, Triangle):
            for s in self.segments.values():
                for q in other.segments.values():
                    if s.doesIntersect(q):
                        return True
            return False

        if issubclass(otherType, Line):
            for s in self.segments.values():
                if s.doesIntersect(other):
                    return True
            return False

        msg = "expecting Line or Triangle subclasses, got '{}'"

        raise TypeError(msg.format(otherType))

    # arithmetic methods
    # s = scalar
    # t = Triangle()
    # u = Triangle()
    # p = Point()
    #
    # t op s -> apply op to vertices using scalar
    # t op p -> apply op to vertices using point
    # t op u -> apply op to corresponding vertices (?)
    #
    # should be moved out to a parent ngon class
    # maybe pointcollection?
