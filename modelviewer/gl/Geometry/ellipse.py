'''a pythonic Ellipse.

Super awesome.
'''

import math
from . import Point
from .line import Line, Segment


class Ellipse(object):
    '''
    Implements an ellipse in the XY plane with the supplied radii.

    Returns a unit ellipse centered on the origin in the XY by default.

    Usage:
    >>> from Geometry import Ellipse
    >>> e = Ellipse()
    >>> e.isCircle
    True
    >>> type(e)
    <class 'Geometry.ellipse.Ellipse'>
    >>> e.radius.x *= 2
    >>> e.isCircle,e.isEllipse
    (False,True)

    '''

    def __init__(self, center=None, x_radius=1, y_radius=1, z_radius=0):
        '''
        :center:    - optional Point class initializer
        :x_radius:  - optional float
        :y_radius:  - optional float
        :z_radius:  - optional float

        Defaults to a unit ellipse centered on the origin.
        '''

        # XXX what does it mean to have non-zero radii in
        #     all three dimensions?

        self.center = center
        self.radius = x_radius, y_radius, z_radius

    @property
    def center(self):
        '''
        Center point of the ellipse, equidistant from foci, Point class.\n
        Defaults to the origin.
        '''
        try:
            return self._center
        except AttributeError:
            pass
        self._center = Point()
        return self._center

    @center.setter
    def center(self, newValue):
        self.center.xyz = newValue

    @property
    def radius(self):
        '''
        Radius of the ellipse, Point class.
        '''
        try:
            return self._radius
        except AttributeError:
            pass
        self._radius = Point(1, 1, 0)
        return self._radius

    @radius.setter
    def radius(self, newValue):
        self.radius.xyz = newValue

    def __str__(self):
        '''
        '''
        return 'center={p.center!r}, radius={p.radius!r}'.format(p=self)

    def __repr__(self):
        '''
        '''
        return '{p.__class__.__name__}({p!s})'.format(p=self)

    def __hash__(self):
        '''
        '''
        return hash(self.center) + hash(self.radius)

    @property
    def majorRadius(self):
        '''
        The longest radius of the ellipse, float.

        '''
        return max(self.radius.x, self.radius.y)

    @property
    def minorRadius(self):
        '''
        The shortest radius of the ellipse, float.

        '''
        return min(self.radius.x, self.radius.y)

    @property
    def xAxisIsMajor(self):
        '''
        Returns True if the major axis is parallel to the X axis, boolean.
        '''
        return max(self.radius.x, self.radius.y) == self.radius.x

    @property
    def xAxisIsMinor(self):
        '''
        Returns True if the minor axis is parallel to the X axis, boolean.
        '''
        return min(self.radius.x, self.radius.y) == self.radius.x

    @property
    def yAxisIsMajor(self):
        '''
        Returns True if the major axis is parallel to the Y axis, boolean.
        '''
        return max(self.radius.x, self.radius.y) == self.radius.y

    @property
    def yAxisIsMinor(self):
        '''
        Returns True if the minor axis is parallel to the Y axis, boolean.
        '''
        return min(self.radius.x, self.radius.y) == self.radius.y

    @property
    def eccentricity(self):
        '''
        The ratio of the distance between the two foci to the length
        of the major axis, float.

        0 <= e <= 1

        An eccentricity of zero indicates the ellipse is a circle.

        As e tends towards 1, the ellipse elongates.  It tends to the
        shape of a line segment if the foci remain a finite distance
        apart and a parabola if one focus is kept fixed as the other
        is allowed to move arbitrarily far away.

        '''
        return math.sqrt(1 - ((self.minorRadius / self.majorRadius) ** 2))

    @property
    def e(self):
        '''
        Shorthand notation for eccentricity, float.

        '''
        return self.eccentricity

    @property
    def linearEccentricity(self):
        '''
        Distance between the center of the ellipse and a focus, float.

        '''
        return math.sqrt((self.majorRadius ** 2) - (self.minorRadius ** 2))

    @property
    def f(self):
        '''
        Shorthand notation for linearEccentricity, float.

        '''
        return self.linearEccentricity

    @property
    def a(self):
        '''
        Positive antipodal point on the major axis, Point class.

        '''
        a = Point(self.center)

        if self.xAxisIsMajor:
            a.x += self.majorRadius
        else:
            a.y += self.majorRadius
        return a

    @property
    def a_neg(self):
        '''
        Negative antipodal point on the major axis, Point class.

        '''
        na = Point(self.center)

        if self.xAxisIsMajor:
            na.x -= self.majorRadius
        else:
            na.y -= self.majorRadius
        return na

    @property
    def b(self):
        '''
        Positive antipodal point on the minor axis, Point class.

        '''
        b = Point(self.center)

        if self.xAxisIsMinor:
            b.x += self.minorRadius
        else:
            b.y += self.minorRadius
        return b

    @property
    def b_neg(self):
        '''
        Negative antipodal point on the minor axis, Point class.
        '''
        nb = Point(self.center)

        if self.xAxisIsMinor:
            nb.x -= self.minorRadius
        else:
            nb.y -= self.minorRadius
        return nb

    @property
    def vertices(self):
        '''
        A dictionary of four points where the axes intersect the ellipse, dict.
        '''
        return {'a': self.a, 'a_neg': self.a_neg,
                'b': self.b, 'b_neg': self.b_neg}

    @property
    def focus0(self):
        '''
        First focus of the ellipse, Point class.

        '''
        f = Point(self.center)

        if self.xAxisIsMajor:
            f.x -= self.linearEccentricity
        else:
            f.y -= self.linearEccentricity
        return f

    @property
    def f0(self):
        '''
        Shorthand notation for focus0, Point class
        '''
        return self.focus0

    @property
    def focus1(self):
        '''
        Second focus of the ellipse, Point class.
        '''
        f = Point(self.center)

        if self.xAxisIsMajor:
            f.x += self.linearEccentricity
        else:
            f.y += self.linearEccentricity
        return f

    @property
    def f1(self):
        '''
        Shorthand notation for focus1, Point class
        '''
        return self.focus1

    @property
    def foci(self):
        '''
        A list containing the ellipse's foci, list.

        '''
        return [self.focus0, self.focus1]

    @property
    def majorAxis(self):
        '''
        A line coincident with the ellipse's major axis, Segment class.
        The major axis is the largest distance across an ellipse.

        '''
        return Segment(self.a_neg, self.a)

    @property
    def minorAxis(self):
        '''
        A line coincident with the ellipse' minor axis, Segment class.
        The minor axis is the smallest distance across an ellipse.

        '''
        return Segment(self.b_neg, self.b)

    @property
    def isCircle(self):
        '''
        Is true if the major and minor axes are equal, boolean.

        '''
        return self.radius.x == self.radius.y

    @property
    def isEllipse(self):
        '''
        Is true if the major and minor axes are not equal, boolean.

        '''
        return self.radius.x != self.radius.y

    def __eq__(self, other):
        '''
        a == b iff:
            a.center   == b.center
            a.radius.x == b.radius.x
            a.radius.y == b.radius.y
        '''
        if self.center != other.center:
            return False

        if self.radius.x != other.radius.x:
            return False

        if self.radius.y != other.radius.y:
            return False

        return True

    def __contains__(self, other):
        '''
        x in y

        Is true iff x is a point on or inside the ellipse y.

        '''

        otherType = type(other)

        if issubclass(otherType, Point):
            d = sum([other.distance(f) for f in self.foci])
            # d < majorAxis.length interior point
            # d == majorAxis.length on perimeter of ellipse
            # d > majorAxis.length exterior point
            return d <= self.majorAxis.length

        if issubclass(otherType, Segment):
            return (other.A in self) and (other.B in self)

        if issubclass(otherType, Ellipse):
            return (other.majorAxis in self) and (other.minorAxis in self)

        raise TypeError("unknown type '{t}'".format(t=otherType))

    # XXX do math operations on ellipses make sense?


class Circle(Ellipse):
    '''
    Implements a circle in the XY plane with the supplied
    center point and radius.

    Example usage:
    >>> from Geometry import Circle,Point
    >>> u = Circle()
    >>> u
    Circle((0.0,0.0,0.0),1.00)
    >>> import math
    >>> u.area == math.pi
    True
    >>> u.circumfrence == 2 * math.pi
    True
    >>> p = Point.gaussian()
    >>> p in u
    False
    >>> p.xyz = None
    >>> p in u
    True
    >>> p
    Point(0.0,0.0,0.0)

    '''

    @classmethod
    def inscribedInRectangle(cls, rectangle):
        raise NotImplementedError('inscribedInRectangle')

    @classmethod
    def inscribedInTriangle(cls, triangle):
        raise NotImplementedError('inscribedInTriangle')
        pass

    @classmethod
    def circumscribingRectangle(cls, rectangle):
        raise NotImplementedError('circumscribingRectangle')

    @classmethod
    def circumscribingTriangle(cls, triangle):
        raise NotImplementedError('circumscribingTriangle')

    @classmethod
    def circumcircleForTriangle(cls, triangle):
        '''
        :param: triangle - Triangle class
        :return: Circle class

        Returns the circle where every vertex in the input triangle is
        on the radius of that circle.

        '''

        if triangle.isRight:
            # circumcircle origin is the midpoint of the hypotenues
            o = triangle.hypotenuse.midpoint
            r = o.distance(triangle.A)
            return cls(o, r)

        # otherwise
        # 1. find the normals to two sides
        # 2. translate them to the midpoints of those two sides
        # 3. intersect those lines for center of circumcircle
        # 4. radius is distance from center to any vertex in the triangle

        abn = triangle.AB.normal
        abn += triangle.AB.midpoint

        acn = triangle.AC.normal
        acn += triangle.AC.midpoint

        o = abn.intersection(acn)
        r = o.distance(triangle.A)
        return cls(o, r)

    def __init__(self, center=None, radius=1.0):
        '''
        :param: center - optional Point class initializer
        :param: radius - optional float
        Defaults to a unit circle centered on the origin.
        '''
        self.center = center
        self.radius = radius

    @property
    def radius(self):
        '''
        The circle's radius, float.
        '''
        try:
            return self._radius
        except AttributeError:
            pass
        self._radius = 1.0
        return self._radius

    @radius.setter
    def radius(self, newValue):
        self._radius = float(newValue)

    @property
    def diameter(self):
        '''
        The circle's diameter, float.
        '''
        return self.radius * 2

    @property
    def circumfrence(self):
        '''
        The circle's circumfrence, float.
        '''
        return 2 * math.pi * self.radius

    @property
    def area(self):
        '''
        The circle's area, float.
        '''
        return math.pi * (self.radius ** 2)

    @property
    def a(self):
        return Point(self.radius, self.center.y)

    @property
    def a_neg(self):
        return Point(-self.radius, self.center.y)

    @property
    def b(self):
        return Point(self.center.x, self.radius)

    @property
    def b_neg(self):
        return Point(self.center.x, -self.radius)

    def __contains__(self, other):
        '''
        :param: Point | Segment | Ellipse class
        :return: boolean

        Returns True if the distance from the center to the point
        is less than or equal to the radius.
        '''
        otherType = type(other)

        if issubclass(otherType, Point):
            return other.distance(self.center) <= self.radius

        if issubclass(otherType, Segment):
            return (other.A in self) and (other.B in self)

        if issubclass(otherType, Circle):
            m = self.center.distance(other.center) / 2
            return (m <= self.radius) and (m <= other.radius)

        if issubclass(otherType, Ellipse):
            return (other.majorAxis in self) and (other.minorAxis in self)

        raise TypeError("unknown type '{t}'".format(t=otherType))

    def doesIntersect(self, other):
        '''
        :param: other - Circle class

        Returns True iff:
          self.center.distance(other.center) <= self.radius+other.radius
        '''

        otherType = type(other)

        if issubclass(otherType, Ellipse):
            distance = self.center.distance(other.center)
            radiisum = self.radius + other.radius
            return distance <= radiisum

        if issubclass(otherType, Line):
            raise NotImplementedError('doesIntersect,other is Line class')

        raise TypeError("unknown type '{t}'".format(t=otherType))


class Sphere(Circle):

    @property
    def volume(self):
        '''
        The spherical volume bounded by this circle, float.
        '''
        return (4. / 3.) * math.pi * (self.radius ** 3)
