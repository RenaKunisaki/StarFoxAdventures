'''
'''

import collections
import hashlib
import random
import math
from .constants import Half_Pi, Two_Pi
from .exceptions import CollinearPoints
from .propgen import FloatProperty, FloatMultiProperty


class Point(collections.Mapping):
    '''
    A three dimensional Point initialized with x,y and z values
    supplied or the origin if no ordinates are given.

    Point initialization is very flexible! It can be initialized with
    zero to three positional parameters; 'x', 'y', 'z'.

    e.g:

    >> repr(Point())
    'Point(x=0.0, y=0.0, z=0.0)'
    >> repr(Point(1))
    'Point(x=1.0, y=0.0, z=0.0)'
    >> repr(Point(1,2))
    'Point(x=1.0, y=2.0, z=0.0)'
    >> repr(Point(1,2,3))
    'Point(x=1.0, y=2.0, z=3.0)'

    Point can also be initialized with keyword arguments: x, y and z.

    >> repr(Point(z=1)
    'Point(x=0.0, y=0.0, z=1.0)'
    >> repr(Point(z=1,y=2,x=3)
    'Point(x=3.0, y=2.0, z=1.0)'

    Finally, Points can be initialized by mappings or sequences:

    >> repr(Point([4]*3))
    'Point(x=4.0, y=4.0, z=4.0)'
    >> repr(Point({'x':1,'y':2,'z':3,'foo':'whatevers'}))
    'Point(x=1.0, y=2.0, z=3.0)'

    Accessing the values of Point are also very flexible. Each
    coordinate can be set/get individually via the properties 'x',
    'y' and 'z'. Coordinates can also be set/get in groups using
    the properties 'xy', 'xz', 'yz', 'xyz' and 'xyzw'.  Arguments
    to the setters can be mappings, sequences, or scalars.

    Operations
    ==========

    +, -, *, /, //, %, **, +=, -=, *=, /=, //=, %=, **=, ==, !=

    Operands can be mappings, sequences or scalars.

    Methods
    =======
                ccw: counter-clockwise function, also known as:
                     "The beating red heart of computational geometry."
                dot: dot product of two points
              cross: cross product of two points
           midpoint: point between two other points
           distance: Euclidean distance between points
    distanceSquared: Squared Euclidean distance
          isBetween: bounds checking a point between two other points
              isCCW: is angle ABC a counter-clockwise rotation?
        isCollinear: are points ABC collinear?

    Class Methods
    =============

      origin: a Point initialized to 0,0,0
        unit: converts vector AB to a unit vector
       units: list of three unit vectors on each axis
    gaussian: random Point with coordinates chosen from gaussian distribution.
      random: random Point within a circle

    '''

    x = FloatProperty('x', docs='X axis coordinate, float.')
    y = FloatProperty('y', docs='Y axis coordinate, float.')
    z = FloatProperty('z', docs='Z axis coordinate, float.')
    w = FloatProperty('w',
                      docs='W coordinate, read-only float. '
                      'Used to form square matrices. ',
                      default=1.0, readonly=True)
    xyzw = FloatMultiProperty('xyzw',
                              docs='A list of all coordinates including W.',
                              readonly_keys='w')
    xyz = FloatMultiProperty('xyz',
                             docs='A list of all coordinates excluding W.')

    xy = FloatMultiProperty('xy', docs='A list of X and Y coordinates.')
    yx = FloatMultiProperty('yx', docs='A list of Y and X coordinates.')

    yz = FloatMultiProperty('yz', docs='A list of Y and Z coordinates.')
    zy = FloatMultiProperty('zy', docs='A list of Z and Y coordinates.')

    xz = FloatMultiProperty('xz', docs='A list of X and Z coordinates.')
    zx = FloatMultiProperty('zx', docs='A list of Z and X coordinates.')

    _keys = 'xyz'

    @classmethod
    def origin(cls):
        '''
        Returns a Point whose coordinates are [0,0,0]
        '''
        return cls(0, 0, 0)

    @classmethod
    def _convert(cls, other, ignoreScalars=False):
        '''
        :other: Point or point equivalent
        :ignorescalars: optional boolean
        :return: Point

        Class private method for converting 'other' into a Point
        subclasss. If 'other' already is a Point subclass, nothing
        is done. If ignoreScalars is True and other is a float or int
        type, a TypeError exception is raised.
        '''
        if ignoreScalars:
            if isinstance(other, (int, float)):
                msg = "unable to convert {} to {}".format(other, cls.__name__)
                raise TypeError(msg)

        return cls(other) if not issubclass(type(other), cls) else other

    @classmethod
    def unit(cls, A, B):
        '''
        :A:      Point subclass
        :B:      Point subclass
        :return: Point subclass

        Translates the vector AB to the origin and scales the length
        of the vector to one. Returns a Point() whose distance from
        the origin is 1.
        '''

        return (B - A) / A.distance(B)

    @classmethod
    def units(cls, scale=1):
        '''
        :scale: optional integer scaling factor
        :return: list of three Point subclass

        Returns three points whose coordinates are the head of a
        unit vector from the origin ( conventionally i, j and k).

        '''
        return [cls(x=scale), cls(y=scale), cls(z=scale)]

    @classmethod
    def gaussian(cls, mu=0, sigma=1):
        '''
        :mu:     mean
        :sigma:  standard deviation
        :return: Point subclass

        Returns a point whose coordinates are picked from a Gaussian
        distribution with mean 'mu' and standard deviation 'sigma'.
        See random.gauss for further explanation of those parameters.
        '''
        return cls(random.gauss(mu, sigma),
                   random.gauss(mu, sigma),
                   random.gauss(mu, sigma))

    @classmethod
    def random(cls, origin=None, radius=1):
        '''
        :origin: optional Point or point equivalent
        :radius: optional float, radius around origin
        :return: Point subclass

        Returns a point with random x, y and z coordinates bounded by
        the sphere defined by (origin,radius).

        If a sphere is not supplied, a unit sphere at the origin is
        used by default.
        '''

        p = cls(origin)

        r = random.uniform(0, radius)
        u = random.uniform(0, Two_Pi)
        v = random.uniform(-Half_Pi, Half_Pi)

        r_cosv = r * math.cos(v)

        p.x += r_cosv * math.cos(u)
        p.y += r_cosv * math.sin(u)
        p.z += radius * math.sin(v)

        return p

# EJO - This (property classmethod) was a good idea, however in
#       practice it had the following problems:
#
#       - used outside of an __init__ method of a class it declared
#         the properties as class entities and not instanance entities.
#         So all instances shared the property rather than having their
#         own unique properties. Crazy annoying bugs to debug.
#
#       - used in an __init__ method declared the properties as
#         instance variables but the documentation visibility of
#         the property was impacted: eg:
#
#         class Foo(object):
#             def __init__(self):
#                 baz = Point.property("baz","it's a baz!")
#
#         >>>help(Foo)
#         # no mention of baz
#         >>> foo = Foo()
#         >>> help(foo.baz)
#         it's a baz!
#
#       I'm leaving this commented out in the source because I
#       like the idea and I liked the resulting usage, it just
#       didn't work satisfactorily and I hope to fix it in the
#       future.
#
#
#    @classmethod
#    def property(cls,name,docs=None,readonly=False,default=None):
#        '''
#        :name: string - client class property name
#        :docs: optional string - document string for the property
#        :readonly: optional Boolean - True if property is readonly
#        :default: optional Point or equivalent
#
#        This class method returns a Property class that can be
#        used by client classes as shorthand for a property
#        constructor, e.g.:
#
#        class Foo(object):
#            A = Point.property('A',"It's an A",default=[1,1])
#
#        >>>foo = Foo()
#        >>>foo.A
#        Point(x=1,y=1,z=0)
#
#        '''
#
#        private_name = '_'+name
#
#        default = cls._convert(default)
#
#        def getf(self):
#            if not hasattr(self, private_name):
#                setattr(self, private_name, default)
#            return getattr(self, private_name)
#
#        def setf(self, newValue):
#            try:
#                o = getattr(self, private_name)
#            except AttributeError:
#                o = default
#                setattr(self, private_name, o) #? this may be redundant
#            o.xyz = newValue
#
#        if readonly:
#            setf = None
#
#        return property(getf,setf,None,docs)

    def __init__(self, *args, **kwds):
        '''
        Initialize with:
        - positional arguments corresponding to x, y and z
        - keyword arguments: x, y and z
        - mappings
        - sequences
        '''
        # see docstring of __call__
        self(*args, **kwds)

    def __call__(self, *args, **kwds):
        '''
        Point objects are 'callable' as an alternate way to
        change the values of 'x', 'y', and 'z' properties.

        Can be useful in lambda functions where assignments are
        forbidden but calling an object with arguments acceptable.

        Arguments are:
        - positional arguments corresponding to x, y and z
        - keyword arguments: x, y and z
        - mappings
        - sequences

        '''

        if len(args) == 1:
            self.xyz = args[0]

        if len(args) > 1:
            self.xyz = args

        self.xyz = kwds

    def __setattr__(self, attr, value):
        '''
        Side-effect: deletes cached computed hash value if
                     x, y, or z attributes change.
        '''
        super().__setattr__(attr, value)
        try:
            if attr[0] == '_' and attr[1] in self._keys:
                try:
                    del(self._hashvalue)
                except AttributeError:
                    pass
        except IndexError:
            pass

    def __str__(self):
        return 'x={p.x}, y={p.y}, z={p.z}'.format(p=self)

    def __repr__(self):
        return '{p.__class__.__name__}({p!s})'.format(p=self)

    def __hash__(self):
        '''
        Hash computed from the repr value. Re-computed if the
        object's repr string changes (x, y, or z properties
        change).
        '''
        try:
            return self._hashvalue
        except AttributeError:
            pass
        digest = hashlib.sha1(bytes(repr(self), 'utf-8')).hexdigest()
        self._hashvalue = int(digest, 16)
        return self._hashvalue

    def __delhash__(self):
        '''
        '''
        try:
            del(self._hashvalue)
        except AttributeError:
            pass

    def __len__(self):
        '''
        Number of coordinates defined in a Point: x, y and z
        '''
        try:
            return self._len
        except AttributeError:
            pass
        self._len = len(self._keys)
        return self._len

    def __bool__(self):
        '''
        Returns True iff all coordinates are non-zero.
        '''
        return all(self.values())

    def __iter__(self):
        '''
        :return: iterator

        Returns an iterator which steps through the key
        names of the Point.
        '''
        return iter(self._keys)

    def __getitem__(self, key):
        '''
        :key:    string or integer
        :return: float

        Recognizes keys:

        x, y, z, w, xy, yz, xz, xyz, xyzw, [0 .. 8]

        Raises TypeError for any other key.
        '''
        # XXX reverse dual accessors missing [ yx, zy, zx ]

        if key in ['x', 0]:
            return self.x
        if key in ['y', 1]:
            return self.y
        if key in ['z', 2]:
            return self.z
        if key in ['w', 3]:
            return self.w
        if key in ['xy', 4]:
            return (self.x, self.y)
        if key in ['yz', 5]:
            return (self.y, self.z)
        if key in ['xz', 6]:
            return (self.x, self.z)
        if key in ['xyz', 7]:
            return (self.x, self.y, self.z)
        if key in ['xyzw', 8]:
            return (self.x, self.y, self.z, self.w)
        raise TypeError(key)

    def __setitem__(self, key, newValue):
        '''
        :key:       string or integer
        :newValue:  Point or point equivalent

        Recognizes keys:

        x, y, z, xy, yz, xz, xyz, xyzw, [0-2,4-8]

        Raises TypeError for any other key.
        '''
        if key in ['x', 0]:
            self.x = newValue
            return

        if key in ['y', 1]:
            self.y = newValue
            return

        if key in ['z', 2]:
            self.z = newValue
            return

        # key ['w',3] is readonly, will raise TypeError

        if key in ['xy', 4]:
            self.xy = newValue
            return

        if key in ['yz', 5]:
            self.yz = newValue
            return

        if key in ['xz', 6]:
            self.xz = newValue
            return

        if key in ['xyz', 'xyzw', 7, 8]:
            self.xyz = newValue
            return

        raise TypeError(key)

    def __eq__(self, other):
        '''
        :other:  Point or point equivalent
        :return: boolean

        a == b
        '''
        try:
            b = self.__class__._convert(other)
        except:
            return False
        return all(u == v for u, v in zip(self.xyz, b.xyz))

    def _binary_(self, other, func, inplace=False):
        '''
        :other:   Point or point equivalent
        :func:    binary function to apply
        :inplace: optional boolean
        :return:  Point

        Implementation private method.

        All of the binary operations funnel thru this method to
        reduce cut-and-paste code and enforce consistent behavior
        of binary ops.

        Applies 'func' to 'self' and 'other' and returns the result.

        If 'inplace' is True the results of will be stored in 'self',
        otherwise the results will be stored in a new object.

        Returns a Point.

        '''

        dst = self if inplace else self.__class__(self)

        try:
            b = self.__class__._convert(other, ignoreScalars=True)
            dst.x = func(dst.x, b.x)
            dst.y = func(dst.y, b.y)
            dst.z = func(dst.z, b.z)
            return dst
        except TypeError:
            pass

        dst.x = func(dst.x, other)
        dst.y = func(dst.y, other)
        dst.z = func(dst.z, other)
        return dst

    def _unary_(self, func, inplace=False):
        '''
        :func: unary function to apply to each coordinate
        :inplace: optional boolean
        :return: Point

        Implementation private method.

        All of the unary operations funnel thru this method
        to reduce cut-and-paste code and enforce consistent
        behavior of unary ops.

        Applies 'func' to self and returns the result.

        The expected call signature of 'func' is f(a)

        If 'inplace' is True, the results are stored in 'self',
        otherwise the results will be stored in a new object.

        Returns a Point.

        '''
        dst = self if inplace else self.__class__(self)
        dst.x = func(dst.x)
        dst.y = func(dst.y)
        dst.z = func(dst.z)
        return dst

    def __add__(self, other):
        '''
        :other: Point or point equivalent
        :return: Point

        a.x + b.x || a.x + b[0] || a.x + b
        a.y + b.y || a.y + b[1] || a.y + b
        a.z + b.z || a.z + b[2] || a.z + b

        Returns new object.
        '''
        try:
            return self._binary_(other, lambda a, b: a + b)
        except TypeError as e:
            err = TypeError(str(e))
        raise err

    def __radd__(self, other):
        '''
        :other: Point or point equivalent
        :return: Point

        b.x + a.x || b[0] + a.x || b + a.x
        b.y + a.y || b[1] + a.y || b + a.y
        b.z + a.z || b[2] + a.z || b + a.z

        Returns new object.
        '''
        try:
            return self._binary_(other, lambda a, b: b + a)
        except TypeError as e:
            err = TypeError(str(e))
        raise err

    def __iadd__(self, other):
        '''
        :other: Point or point equivalent
        :return: Point

        a.x += b.x || a.x += b[0] || a.x += b
        a.y += b.y || a.y += b[1] || a.y += b
        a.z += b.z || a.z += b[2] || a.z += b

        Returns self.
        '''
        try:
            return self._binary_(other, lambda a, b: a + b, inplace=True)
        except TypeError as e:
            err = TypeError(str(e))
        raise err

    def __sub__(self, other):
        '''
        :other: Point or point equivalent
        :return: Point

        a.x - b.x || a.x - b[0] || a.x - b
        a.y - b.y || a.y - b[1] || a.y - b
        a.z - b.z || a.z - b[2] || a.z - b

        Returns new object.
        '''
        try:
            return self._binary_(other, lambda a, b: a - b)
        except TypeError as e:
            err = TypeError(str(e))
        raise err

    def __rsub__(self, other):
        '''
        :other: Point or point equivalent
        :return: Point

        b.x - a.x || b[0] - a.x || b - a.x
        b.y - a.y || b[1] - a.y || b - a.y
        b.z - a.z || b[2] - a.z || b - a.z

        Returns new object.
        '''
        try:
            return self._binary_(other, lambda a, b: b - a)
        except TypeError as e:
            err = TypeError(str(e))
        raise err

    def __isub__(self, other):
        '''
        :other: Point or point equivalent
        :return: Point

        a.x -= b.x || a.x -= b[0] || a.x -= b
        a.y -= b.y || a.y -= b[1] || a.y -= b
        a.z -= b.z || a.z -= b[2] || a.z -= b

        Returns self.
        '''
        try:
            return self._binary_(other, lambda a, b: a - b)
        except TypeError as e:
            err = TypeError(str(e))
        raise err

    def __mul__(self, other):
        '''
        :other: Point or point equivalent
        :return: Point

        a.x * b.x || a.x * b[0] || a.x * b
        a.y * b.y || a.y * b[1] || a.y * b
        a.z * b.z || a.z * b[2] || a.z * b

        Returns new object.
        '''
        try:
            return self._binary_(other, lambda a, b: a * b)
        except TypeError as e:
            err = TypeError(str(e))
        raise err

    def __rmul__(self, other):
        '''
        :other: Point or point equivalent
        :return: Point

        b.x * a.x || b[0] * a.x || b * a.x
        b.y * a.y || b[1] * a.y || b * a.y
        b.z * a.z || b[2] * a.z || b * a.z

        Returns new object.
        '''
        try:
            return self._binary_(other, lambda a, b: b * a)
        except TypeError as e:
            err = TypeError(str(e))
        raise err

    def __imul__(self, other):
        '''
        :other: Point or point equivalent
        :return: Point

        a.x *= b.x || a.x *= b[0] || a.x *= b
        a.y *= b.y || a.y *= b[1] || a.y *= b
        a.z *= b.z || a.z *= b[2] || a.z *= b

        Returns self.
        '''
        try:
            return self._binary_(other, lambda a, b: a * b, inplace=True)
        except TypeError as e:
            err = TypeError(str(e))
        raise err

    def __floordiv__(self, other):
        '''
        :other: Point or point equivalent
        :return: Point

        a.x // b.x || a.x // b[0] || a.x // b
        a.y // b.y || a.y // b[1] || a.y // b
        a.z // b.z || a.z // b[2] || a.z // b

        Returns new object.
        '''
        try:
            return self._binary_(other, lambda a, b: a // b)
        except TypeError as e:
            err = TypeError(str(e))
        except ZeroDivisionError as e:
            err = ZeroDivisionError(str(e))
        raise err

    def __rfloordiv__(self, other):
        '''
        :other: Point or point equivalent
        :return: Point

        b.x // a.x || b[0] // a.x || b // a.x
        b.y // a.y || b[1] // a.y || b // a.y
        b.z // a.z || b[2] // a.z || b // a.z

        Returns new object.
        '''
        try:
            return self._binary_(other, lambda a, b: b // a)
        except TypeError as e:
            err = TypeError(str(e))
        except ZeroDivisionError as e:
            err = ZeroDivisionError(str(e))
        raise err

    def __ifloordiv__(self, other):
        '''
        :other: Point or point equivalent
        :return: Point

        a.x //= b.x || a.x //= b[0] || a.x //= b
        a.y //= b.y || a.y //= b[1] || a.y //= b
        a.z //= b.z || a.z //= b[2] || a.z //= b

        Returns self.
        '''
        try:
            return self._binary_(other, lambda a, b: a // b, inplace=True)
        except TypeError as e:
            err = TypeError(str(e))
        except ZeroDivisionError as e:
            err = ZeroDivisionError(str(e))
        raise err

    def __truediv__(self, other):
        '''
        :other: Point or point equivalent
        :return: Point

        a.x / b.x || a.x / b[0] || a.x / b
        a.y / b.y || a.y / b[1] || a.y / b
        a.z / b.z || a.z / b[2] || a.z / b

        Returns new object.
        '''
        try:
            return self._binary_(other, lambda a, b: a / b)
        except TypeError as e:
            err = TypeError(str(e))
        except ZeroDivisionError as e:
            err = ZeroDivisionError(str(e))
        raise err

    def __rtruediv__(self, other):
        '''
        :other: Point or point equivalent
        :return: Point

        b.x / a.x || b[0] / a.x || b / a.x
        b.y / a.y || b[1] / a.y || b / a.y
        b.z / a.z || b[2] / a.z || b / a.z

        Returns new object.
        '''
        try:
            return self._binary_(other, lambda a, b: b / a)
        except TypeError as e:
            err = TypeError(str(e))
        except ZeroDivisionError as e:
            err = ZeroDivisionError(str(e))
        raise err

    def __itruediv__(self, other):
        '''
        :other: Point or point equivalent
        :return: Point

        a.x /= b.x || a.x /= b[0] || a.x /= b
        a.y /= b.y || a.y /= b[1] || a.y /= b
        a.z /= b.z || a.z /= b[2] || a.z /= b

        Returns self.
        '''
        try:
            return self._binary_(other, lambda a, b: a / b, inplace=True)
        except TypeError as e:
            err = TypeError(str(e))
        except ZeroDivisionError as e:
            err = ZeroDivisionError(str(e))
        raise err

    def __mod__(self, other):
        '''
        :other: Point or point equivalent
        :return: Point

        a.x % b.x || a.x % b[0] || a.x % b
        a.y % b.y || a.y % b[1] || a.y % b
        a.z % b.z || a.z % b[2] || a.z % b

        Returns new object.
        '''
        try:
            return self._binary_(other, lambda a, b: a % b)
        except TypeError as e:
            err = TypeError(str(e))
        except ZeroDivisionError as e:
            err = ZeroDivisionError(str(e))
        raise err

    def __rmod__(self, other):
        '''
        :other: Point or point equivalent
        :return: Point

        b.x % a.x || b[0] % a.x || b % a.x
        b.y % a.y || b[1] % a.y || b % a.y
        b.z % a.z || b[2] % a.z || b % a.z

        Returns new object.
        '''
        try:
            return self._binary_(other, lambda a, b: b % a)
        except TypeError as e:
            err = TypeError(str(e))
        except ZeroDivisionError as e:
            err = ZeroDivisionError(str(e))
        raise err

    def __imod__(self, other):
        '''
        :other: Point or point equivalent
        :return: Point

        a.x %= b.x || a.x %= b[0] || a.x %= b
        a.y %= b.y || a.y %= b[1] || a.y %= b
        a.z %= b.z || a.z %= b[2] || a.z %= b

        Returns self.
        '''
        try:
            return self._binary_(other, lambda a, b: a % b, inplace=True)
        except TypeError as e:
            err = TypeError(str(e))
        except ZeroDivisionError as e:
            err = ZeroDivisionError(str(e))
        raise err

    def __pow__(self, other, modulus=None):
        '''
        :other: Point or point equivalent
        :modulus: optional integer, see pow
        :return: Point

        a.x ** b.x || a.x ** b[0] || a.x ** b
        a.y ** b.y || a.y ** b[1] || a.y ** b
        a.z ** b.z || a.z ** b[2] || a.z ** b


        Returns new object.
        '''
        fn = pow if modulus is None else lambda a, b: pow(
            int(a), int(b), modulus)
        try:
            return self._binary_(other, fn)
        except TypeError as e:
            err = TypeError(str(e))
        raise err

    def __rpow__(self, other, modulus=None):
        '''
        :other: Point or point equivalent
        :modulus: optional integer, see pow
        :return: Point

        b.x ** a.x || b[0] ** a.x || b ** a.x
        b.y ** a.y || b[1] ** a.y || b ** a.y
        b.z ** a.z || b[2] ** a.z || b ** a.z

        Returns new object.
        '''
        fn = pow if modulus is None else lambda a, b: pow(
            int(b), int(a), modulus)
        try:
            return self._binary_(other, fn)
        except TypeError as e:
            err = TypeError(str(e))
        raise err

    def __ipow__(self, other, modulus=None):
        '''
        :other: Point or point equivalent
        :modulus: optional integer, see pow
        :return: Point

        a.x **= b.x || a.x **= b[0] || a.x **= b
        a.y **= b.y || a.y **= b[1] || a.y **= b
        a.z **= b.z || a.z **= b[2] || a.z **= b

        Returns self.
        '''
        fn = pow if modulus is None else lambda a, b: pow(
            int(a), int(b), modulus)
        try:
            return self._binary_(other, fn, inplace=True)
        except TypeError as e:
            err = TypeError(str(e))
        raise err

    def __rshift__(self, other):
        '''
        :other: Point or point equivalent
        :return: Point

        a.x >> b.x || a.x >> b[0] || a.x >> b
        a.y >> b.y || a.y >> b[1] || a.y >> b
        a.z >> b.z || a.z >> b[2] || a.z >> b

        Returns new object.
        '''
        try:
            return self._binary_(other, lambda a, b: int(a) >> int(b))
        except TypeError as e:
            err = TypeError(str(e))
        raise err

    def __rrshift__(self, other):
        '''
        :other: Point or point equivalent
        :return: Point

        b.x >> a.x || b[0] >> a.x|| b >> a.x
        b.y >> a.y || b[1] >> a.y|| b >> a.y
        b.z >> a.z || b[2] >> a.z|| b >> a.z

        Returns new object.
        '''
        try:
            return self._binary_(other, lambda a, b: int(b) >> int(a))
        except TypeError as e:
            err = TypeError(str(e))
        raise err

    def __irshift__(self, other):
        '''
        :other: Point or point equivalent
        :return: Point

        a.x >> b.x || a.x >> b[0] || a.x >> b
        a.y >> b.y || a.y >> b[1] || a.y >> b
        a.z >> b.z || a.z >> b[2] || a.z >> b

        Returns self.
        '''
        try:
            return self._binary_(other, lambda a, b: int(a)
                                 >> int(b), inplace=True)
        except TypeError as e:
            err = TypeError(str(e))
        raise err

    def __lshift__(self, other):
        '''
        :other: Point or point equivalent
        :return: Point

        a.x << b.x || a.x << b[0] || a.x << b
        a.y << b.y || a.y << b[1] || a.y << b
        a.z << b.z || a.z << b[2] || a.z << b

        Returns new object.
        '''
        try:
            return self._binary_(other, lambda a, b: int(a) << int(b))
        except TypeError as e:
            err = TypeError(str(e))
        raise err

    def __rlshift__(self, other):
        '''
        :other: Point or point equivalent
        :return: Point

        b.x << a.x || b[0] << a.x|| b << a.x
        b.y << a.y || b[1] << a.y|| b << a.y
        b.z << a.z || b[2] << a.z|| b << a.z

        Returns new object.
        '''
        try:
            return self._binary_(other, lambda a, b: int(b) << int(a))
        except TypeError as e:
            err = TypeError(str(e))
        raise err

    def __ilshift__(self, other):
        '''
        :other: Point or point equivalent
        :return: Point

        a.x << b.x || a.x << b[0] || a.x << b
        a.y << b.y || a.y << b[1] || a.y << b
        a.z << b.z || a.z << b[2] || a.z << b

        Returns self.
        '''
        try:
            return self._binary_(other, lambda a, b: int(a)
                                 << int(b), inplace=True)
        except TypeError as e:
            err = TypeError(str(e))
        raise err

    def __and__(self, other):
        '''
        :other: Point or point equivalent
        :return: Point

        a.x & b.x || a.x & b[0] || a.x & b
        a.y & b.y || a.y & b[1] || a.y & b
        a.z & b.z || a.z & b[2] || a.z & b

        Returns new object.
        '''
        try:
            return self._binary_(other, lambda a, b: int(a) & int(b))
        except TypeError as e:
            err = TypeError(str(e))
        raise err

    def __rand__(self, other):
        '''
        :other: Point or point equivalent
        :return: Point

        b.x & a.x || b[0] & a.x|| b & a.x
        b.y & a.y || b[1] & a.y|| b & a.y
        b.z & a.z || b[2] & a.z|| b & a.z

        Returns new object.
        '''
        try:
            return self._binary_(other, lambda a, b: int(b) & int(a))
        except TypeError as e:
            err = TypeError(str(e))
        raise err

    def __iand__(self, other):
        '''
        :other: Point or point equivalent
        :return: Point

        a.x &= b.x || a.x &= b[0] || a.x &= b
        a.y &= b.y || a.y &= b[1] || a.y &= b
        a.z &= b.z || a.z &= b[2] || a.z &= b

        Returns self.
        '''
        try:
            return self._binary_(other, lambda a, b: int(a)
                                 & int(b), inplace=True)
        except TypeError as e:
            err = TypeError(str(e))
        raise err

    def __or__(self, other):
        '''
        :other: Point or point equivalent
        :return: Point

        a.x | b.x || a.x | b[0] || a.x | b
        a.y | b.y || a.y | b[1] || a.y | b
        a.z | b.z || a.z | b[2] || a.z | b

        Returns new object.
        '''
        try:
            return self._binary_(other, lambda a, b: int(a) | int(b))
        except TypeError as e:
            err = TypeError(str(e))
        raise err

    def __ror__(self, other):
        '''
        :other: Point or point equivalent
        :return: Point

        b.x | a.x || b[0] | a.x|| b | a.x
        b.y | a.y || b[1] | a.y|| b | a.y
        b.z | a.z || b[2] | a.z|| b | a.z

        Returns new object.
        '''
        try:
            return self._binary_(other, lambda a, b: int(b) | int(a))
        except TypeError as e:
            err = TypeError(str(e))
        raise err

    def __ior__(self, other):
        '''
        :other: Point or point equivalent
        :return: Point

        a.x | b.x || a.x | b[0] || a.x | b
        a.y | b.y || a.y | b[1] || a.y | b
        a.z | b.z || a.z | b[2] || a.z | b

        Returns self.
        '''
        try:
            return self._binary_(other, lambda a, b: int(a)
                                 | int(b), inplace=True)
        except TypeError as e:
            err = TypeError(str(e))
        raise err

    def __xor__(self, other):
        '''
        :other: Point or point equivalent
        :return: Point

        a.x ^ b.x || a.x ^ b[0] || a.x ^ b
        a.y ^ b.y || a.y ^ b[1] || a.y ^ b
        a.z ^ b.z || a.z ^ b[2] || a.z ^ b

        Returns new object.
        '''
        try:
            return self._binary_(other, lambda a, b: int(a) ^ int(b))
        except TypeError as e:
            err = TypeError(str(e))
        raise err

    def __rxor__(self, other):
        '''
        :other: Point or point equivalent
        :return: Point

        b.x ^ a.x || b[0] ^ a.x|| b ^ a.x
        b.y ^ a.y || b[1] ^ a.y|| b ^ a.y
        b.z ^ a.z || b[2] ^ a.z|| b ^ a.z

        Returns new object.
        '''

        try:
            return self._binary_(other, lambda a, b: int(b) ^ int(a))
        except TypeError as e:
            err = TypeError(str(e))
        raise err

    def __ixor__(self, other):
        '''
        :other: Point or point equivalent
        :return: Point

        a.x ^ b.x || a.x ^ b[0] || a.x ^ b
        a.y ^ b.y || a.y ^ b[1] || a.y ^ b
        a.z ^ b.z || a.z ^ b[2] || a.z ^ b

        Returns self.
        '''
        try:
            return self._binary_(other, lambda a, b: int(a)
                                 ^ int(b), inplace=True)
        except TypeError as e:
            err = TypeError(str(e))
        raise err

    # unary functions

    def __pos__(self):
        '''
        +a

        Returns self.
        '''
        return self

    def __neg__(self):
        '''
        -a

        Returns a new object with it's members negated.
        '''
        return self * -1

    def __abs__(self):
        '''
        abs(a.x),abs(a.y),abs(a.z)

        Returns self.
        '''
        return self._unary_(abs, inplace=True)

    def __invert__(self):
        '''
        ~int(a.x),~int(a.y),~int(a.z)

        Returns self.
        '''
        return self._unary_(lambda v: ~int(v), inplace=True)

    def __round__(self, n=0):
        '''
        :n: optional integer

        round(a.x,n), round(a.y,n), round(a.z,n)

        Returns a new object.
        '''
        return self._unary_(lambda v: round(v, n))

    def __floor__(self):
        '''
        floor(a.x,n), floor(a.y,n), floor(a.z,n)

        Returns a new object.
        '''
        return self._unary_(math.floor)

    def __ceil__(self):
        '''
        ceil(a.x,n), ceil(a.y,n), ceil(a.z,n)

        Returns a new object.
        '''
        return self._unary_(math.ceil)

    def __trunc__(self):
        '''
        trunc(a.x), trunc(a.y), trunc(a.z)

        Returns new object.
        '''

        return self._unary_(math.trunc)

    def dot(self, other):
        '''
        :other: Point or point equivalent
        :return: float

        Dot product of self and other, computed:

        a.x*b.x + a.y*b.y + a.z*b.z

        Returns a float
        '''
        return sum((self * other).values())

    def cross(self, other):
        '''
        :other: Point or point equivalent
        :return: float

        Vector cross product of points U (self) and V (other), computed:

        U x V = (u1*i + u2*j + u3*k) x (v1*i + v2*j + v3*k)
        s1 = u2v3 - u3v2
        s2 = u3v1 - u1v3
        s3 = u1v2 - u2v1

        U x V = s1 + s2 + s3

        Returns a float.
        '''

        b = self.__class__._convert(other)

        return sum([(self.y * b.z) - (self.z * b.y),
                    (self.z * b.x) - (self.x * b.z),
                    (self.x * b.y) - (self.y * b.x)])

    def midpoint(self, other):
        '''
        :other: Point or point equivalent
        :return: Point

        The point midway between 'self' and 'other'.

        Returns a new object.
        '''

        return (self + self.__class__._convert(other)) / 2

    @property
    def isOrigin(self):
        '''
        True if this point's coordinates are (0,0,0), boolean.
        '''
        return sum(self.xyz) == 0

    def isBetween(self, a, b, axes='xyz'):
        '''
        :a: Point or point equivalent
        :b: Point or point equivalent
        :axis: optional string
        :return: float

        Checks the coordinates specified in 'axes' of 'self' to
        determine if they are bounded by 'a' and 'b'. The range
        is inclusive of end-points.

        Returns boolean.
        '''
        a = self.__class__._convert(a)
        b = self.__class__._convert(b)

        fn = lambda k: (self[k] >= min(a[k], b[k])) and (
            self[k] <= max(a[k], b[k]))

        return all(fn(axis) for axis in axes)

    def distance(self, other=None):
        '''
        :other:  optional Point or point equivalent
        :return: float

        The Euclidean distance from 'self' to 'other'.

        If 'other' is not specified, the origin is used.

        Returns a float.
        '''
        return math.sqrt(self.distanceSquared(other))

    def distanceSquared(self, other=None):
        '''
        :other:  optional Point or point equivalent
        :return: float

        Returns the squared Euclidean distance from 'self' to 'other'.

        If 'other' is not specified, the origin is used.

        Returns a float.

        Note: Use distanceSquared when ordering points by distance
              from an arbitrary point. Avoids a square root which can
              improve performance.
        '''
        return sum(((other - self) ** 2).values())

    def ccw(self, b, c, axis='z'):
        '''
        :b: Point or point equivalent
        :c: Point or point equivalent
        :axis: optional string or integer in set('x',0,'y',1,'z',2)
        :return: float

        CCW - Counter Clockwise

        Returns an integer signifying the direction of rotation around 'axis'
        described by the angle [b, self, c].

        > 0 : counter-clockwise
          0 : points are collinear
        < 0 : clockwise

        Returns an integer.

        Raises ValueError if axis is not in 'xyz'.
        '''
        bsuba = b - self
        csuba = c - self

        if axis in ['z', 2]:
            return (bsuba.x * csuba.y) - (bsuba.y * csuba.x)

        if axis in ['y', 1]:
            return (bsuba.x * csuba.z) - (bsuba.z * csuba.x)

        if axis in ['x', 0]:
            return (bsuba.y * csuba.z) - (bsuba.z * csuba.y)

        msg = "invalid axis '{!r}', must be one of {}".format(axis, self._keys)

        raise ValueError(msg)

    def isCCW(self, b, c, axis='z'):
        '''
        :b: Point or point equivalent
        :c: Point or point equivalent
        :axis: optional string or integer in set('x',0,'y',1,'z',2)
        :return: boolean

        True if the angle determined by a,self,b around 'axis'
        describes a counter-clockwise rotation, otherwise False.

        Raises CollinearPoints if self, b, c are collinear.
        '''

        result = self.ccw(b, c, axis)

        if result == 0:
            raise CollinearPoints(b, self, c)

        return result > 0

    def isCollinear(self, b, c):
        '''
        :b: Point or point equivalent
        :c: Point or point equivalent
        :return: boolean

        True if 'self' is collinear with 'b' and 'c', otherwise False.
        '''

        return all(self.ccw(b, c, axis) == 0 for axis in self._keys)

    def rotate2d(self, theta, origin=None, axis='z', radians=False):
        '''
        :theta: float radians to rotate self around origin
        :origin: optional Point, defaults to 0,0,0

        Returns a Point rotated by :theta: around :origin:.
        '''

        origin = Point._convert(origin)

        delta = self - origin

        p = Point(origin)

        if not radians:
            theta = math.radians(theta)

        cosT = math.cos(theta)
        sinT = math.sin(theta)

        if axis == 'z':
            p.x += (cosT * delta.x) - (sinT * delta.y)
            p.y += (sinT * delta.x) + (cosT * delta.y)
            return p

        if axis == 'y':
            p.z += (cosT * delta.z) - (sinT * delta.x)
            p.x += (sinT * delta.z) + (cosT * delta.x)
            return p

        if axis == 'x':
            p.y += (cosT * delta.y) - (sinT * delta.z)
            p.z += (sinT * delta.y) + (cosT * delta.z)
            return p

        raise KeyError('unknown axis {}, expecting x, y or z'.format(axis))

    def irotate2d(self, theta, origin=None, axis='z', radians=False):

        self.xyz = self.rotate2d(theta, origin, axis, radians).xyz

        return self

    def rotate(self, theta_x, theta_y, theta_z, origin=None, radians=False):

        origin = Point._convert(origin)

        thetas = [theta_x, theta_y, theta_z]

        if not radians:
            thetas = list(map(math.radians, thetas))

        cosX, cosY, cosZ = list(map(math.cos, thetas))
        sinX, sinY, sinZ = list(map(math.sin.thetas))

        raise NotImplementedError("working on it")

class PointSequence(collections.Sequence):
    '''
    A labeled sequence of points.

    '''

    _labels = 'ABCDEFGHIJKLMNOPQRSTUVWXYZ'

    def __init__(self,*args,**kwds):
        '''
        :args: list of points
        :base: optional base character for labeling points

        The first point is labeled with the base character and
        subsequent points are labeled with the character 
        calculated by:

        label = chr(ord(base)+index)
    
        '''
        self(*args, **kwds)

    def __call__(self, *args, **kwds):
        '''
        '''
        
        try:
            self._base = kwds['base']
        except KeyError:
            self._base = 'A'

        try:
            if kwds['clear']:
                self.vertices.clear()
        except KeyError:
            pass

        if len(args) == 1:
            self.vertices.extend(args[0])
        else:
            self.vertices.extend(args)        

        try:
            for i,p in enumerate(kwds['defaults']):
                try:
                    self[i]
                except IndexError:
                    self.vertices.insert(i,p)
        except KeyError:
            pass

        if len(args) == 0:
            for label in self._labels:
                try:
                    setattr(self,label,kwds[label])
                except KeyError:
                    pass
            
    @property
    def vertices(self):
        try:
            return self._vertices
        except AttributeError:
            pass
        self._vertices = []
        return self._vertices

    @property
    def labels(self):
        return self._labels[:len(self)]

    def __str__(self):
        s = []
        for label, vertex in zip(self.labels,self):
            s.append('{}={!r}'.format(label, vertex))
        return ','.join(s)

    def __repr__(self):
        return '{s.__class__.__name__}({s!s})'.format(s=self)

    def __hash__(self):
        '''
        Hash computed from the repr value.
        '''
        return int(hashlib.sha1(bytes(repr(self),'utf-8')).hexdigest(), 16)

    def __contains__(self, point):
        '''
        True iff point is in the PointSequence list.
        '''
        return point in self.vertices

    def _keyToIndex(self,key):
        '''
        '''
        try:
            return int(key)
        except ValueError:
            pass
        except TypeError:
            return key
        return ord(key) - ord(self._base)


    def _keyToLabel(self, key):
        '''
        '''
        try:
            return chr(ord(key)+ord(self._base))
        except TypeError:
            pass
        try:
            return chr(ord(self._base)+key)
        except TypeError:
            pass
        return key

    def __getitem__(self, key):
        '''
        '''
        return self.vertices[self._keyToIndex(key)]

    def __getattr__(self, attr):
        '''
        '''
        if attr in self._labels:
            try:
                return self[attr]
            except (KeyError, IndexError):
                pass
        try:
            return self.__dict__[attr]
        except KeyError:
            pass
        
        raise AttributeError("{} object has no attribute '{}'".format(self.__class__.__name__,attr))

    def __setattr__(self, attr, value):
        '''
        '''

        if issubclass(type(value),Point):
            try:
                self[attr[:1]].xyz = value
                return
            except (KeyError, IndexError):
                pass

        if all([x in self._labels for x in attr]):
            try:
                for label, point in zip(attr, value):
                    self[label].xyz = point
                return
            except KeyError:
                pass

        super().__setattr__(attr, value)

        
    def __delattr__(self, attr):
        '''

        '''
        try:
            del(self[attr])
            return
        except KeyError:
            pass
        except TypeError:
            pass
        super().__delattr__(attr)

    def __iter__(self):
        '''

        '''
        return iter(self.vertices)

    def __reversed__(self):
        '''

        '''
        return reversed(self.vertices)


    def __len__(self):
        '''

        '''
        return len(self.vertices)

    def count(self, point):
        '''

        '''
        return self.vertices.count(point)

    def index(self, point):
        '''

        '''
        return self.vertices.index(point)

    def pairs(self,xy=None):
        '''

        '''

        if isinstance(xy,collections.Iterable):
            return (self[xy[0]],self[xy[1]])
        
        return zip(self[0:],self[1:]+self[0:1])

    def __eq__(self,other):
        '''
        x == y iff x and y have:
                 1. an equal number of vertices
                 2. the same vertices
        '''

        return len(set(self.vertices).difference(set(other.vertices))) == 0
    
    
    
class MutablePointSequence(PointSequence,collections.MutableSequence):
    '''
    A labeled mutable sequence of points.  Work in Progress.

    '''

    def __delitem__(self, key):
        '''
        '''
        del(self.vertices[self._keyToIndex(key)])

    def __setitem__(self, key, value):
        '''

        '''
        self.vertices[self._keyToIndex(key)].xyz = value

    def append(self, point):
        '''

        '''
        point = Point._convert(point)
        if not issubclass(type(point),Point):
            raise TypeError('{!r} is not a subclass of Point'.format(point))
        self.vertices.append(point)

    def extend(self, iterable):
        '''
        '''
        [self.append(p) for p in iterable]

    def insert(self, index, point):
        '''

        '''
        point = Point._convert(point)
        self.vertices.insert(index,point)

    def clear(self):
        '''
        '''
        self.vertices.clear()

    def pop(self, index=-1):
        '''
        '''
        return self.vertices.pop(index)

    def remove(self, point):
        '''
        '''
        self.vertices.remove(point)

    def reverse(self):
        '''
        '''
        self.vertices.reverse()


    def __add__(self, other):
        '''
        x + y
        '''
        for p in self:
            p += other
        return self

    def __isub__(self, other):
        for p in self:
            p -= other
        return self            
        
    def __imul__(self, other):
        for p in self:
            p *= other
        return self             

    def __itruediv__(self, other):
        for p in self:
            p /= other
        return self
    
    def __ifloordiv__(self, other):
        for p in self:
            p //= other
        return self

        
        





