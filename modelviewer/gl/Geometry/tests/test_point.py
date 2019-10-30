
import unittest
import sys
import math

from .. import Point
from ..exceptions import *
from ..constants import nearly_eq


class PointTestCase(unittest.TestCase):

    def testObjects(self, value=0, *args):
        '''
        '''
        test_objs = [Point([value] * 3),
                     {'x': value, 'y': value, 'z': value},
                     [value] * 3]
        if len(args):
            test_objs.extend(args)
        return test_objs

    def assertIsPoint(self, p):
        self.assertIsInstance(p, Point)

    def assertCoordinatesEqual(self, p, values, msg=None):
        '''
        '''
        self.assertSequenceEqual(p.xyz, values[:3], msg)

    def assertIsOrigin(self, p, msg=None):
        '''
        '''
        self.assertIsInstance(p, Point)
        self.assertCoordinatesEqual(p, [0] * 3, msg)

    def assertPointsEqual(self, a, b):
        '''
        '''
        self.assertIsInstance(a, Point)
        self.assertIsInstance(b, Point)
        self.assertFalse(a is b)
        self.assertSequenceEqual(a.xyz, b.xyz)

    def assertEpsilonEqual(self, test_value, known_value):
        '''

        '''
        return nearly_eq(test_value, known_value)

    def testOriginPointCreation(self):
        '''
        '''
        self.assertIsOrigin(Point.origin())
        self.assertIsOrigin(Point())
        self.assertIsOrigin(Point(None))

    def testPointCreation(self):
        '''
        '''

        # intialized with positional arguments
        self.assertCoordinatesEqual(Point(1), (1, 0, 0))
        self.assertCoordinatesEqual(Point(1, 2), (1, 2, 0))
        self.assertCoordinatesEqual(Point(1, 2, 3), (1, 2, 3))

        # initialized with keyword arguments
        self.assertCoordinatesEqual(Point(x=1), (1, 0, 0))
        self.assertCoordinatesEqual(Point(y=1), (0, 1, 0))
        self.assertCoordinatesEqual(Point(z=1), (0, 0, 1))
        self.assertCoordinatesEqual(Point(x=1, y=1), (1, 1, 0))
        self.assertCoordinatesEqual(Point(x=1, z=1), (1, 0, 1))
        self.assertCoordinatesEqual(Point(y=1, z=1), (0, 1, 1))
        self.assertCoordinatesEqual(Point(x=1, y=1, z=1), (1, 1, 1))

        # initialized with a sequence (list in this cas)
        self.assertCoordinatesEqual(Point([1] * 1), (1, 0, 0))
        self.assertCoordinatesEqual(Point([1] * 2), (1, 1, 0))
        self.assertCoordinatesEqual(Point([1] * 3), (1, 1, 1))

        # initialized with a mapping
        self.assertCoordinatesEqual(Point({'x': 1}), (1, 0, 0))
        self.assertCoordinatesEqual(Point({'y': 1}), (0, 1, 0))
        self.assertCoordinatesEqual(Point({'z': 1}), (0, 0, 1))
        self.assertCoordinatesEqual(Point({'x': 1, 'y': 1}), (1, 1, 0))
        self.assertCoordinatesEqual(Point({'x': 1, 'z': 1}), (1, 0, 1))
        self.assertCoordinatesEqual(Point({'y': 1, 'z': 1}), (0, 1, 1))
        self.assertCoordinatesEqual(Point({'x': 1, 'y': 1, 'z': 1}), (1, 1, 1))

        with self.assertRaises(ValueError, msg="Point('nope')"):
            Point('nope')

    def testPointPropertyGetters(self):
        '''
        '''

        p = Point(1, 2, 3)

        # property style
        self.assertEqual(p.x, 1)
        self.assertEqual(p.y, 2)
        self.assertEqual(p.z, 3)
        self.assertEqual(p.w, 1)

        # mapping style
        self.assertEqual(p['x'], 1)
        self.assertEqual(p['y'], 2)
        self.assertEqual(p['z'], 3)
        self.assertEqual(p['w'], 1)

        # indexed style
        self.assertEqual(p[0], 1)
        self.assertEqual(p[1], 2)
        self.assertEqual(p[2], 3)
        self.assertEqual(p[3], 1)

        # bad index keys
        for bad_key in [-1, 9, 'f', ]:
            with self.assertRaises(TypeError,
                                   msg='p[{}] = 1'.format(bad_key)):
                p[bad_key] = 1

    def testPointMultiplePropertyGetters(self):
        '''
        '''

        p = Point(1, 2, 3)

        # property style
        self.assertSequenceEqual(p.xy, (1, 2))
        self.assertSequenceEqual(p.xz, (1, 3))
        self.assertSequenceEqual(p.yz, (2, 3))
        self.assertSequenceEqual(p.xyz, (1, 2, 3))
        self.assertSequenceEqual(p.xyzw, (1, 2, 3, 1))

        # mapping, string style

        self.assertSequenceEqual(p['xy'], (1, 2))
        self.assertSequenceEqual(p['yz'], (2, 3))
        self.assertSequenceEqual(p['xz'], (1, 3))
        self.assertSequenceEqual(p['xyz'], (1, 2, 3))
        self.assertSequenceEqual(p['xyzw'], (1, 2, 3, 1))

        # mapping, index style

        self.assertSequenceEqual(p[4], (1, 2))
        self.assertSequenceEqual(p[5], (2, 3))
        self.assertSequenceEqual(p[6], (1, 3))
        self.assertSequenceEqual(p[7], (1, 2, 3))
        self.assertSequenceEqual(p[8], (1, 2, 3, 1))

        for bad_key in [-1, 'zyx', 'zx', 9]:
            with self.assertRaises(TypeError,
                                   msg='p[{}] = 1'.format(bad_key)):
                p[bad_key] = 1

    def testPointPropertySettersSimple(self):
        '''
        '''

        p = Point()

        p.x = 1
        self.assertEqual(p.x, 1)
        p[0] = 0
        self.assertEqual(p.x, 0)
        p['x'] = 1
        self.assertEqual(p.x, 1)
        p.x = None
        self.assertEqual(p.x, 0)

        p.y = 1
        self.assertEqual(p.y, 1)
        p[1] = 0
        self.assertEqual(p.y, 0)
        p['y'] = 1
        self.assertEqual(p.y, 1)
        p.y = None
        self.assertEqual(p.y, 0)

        p.z = 1
        self.assertEqual(p.z, 1)
        p[2] = 0
        self.assertEqual(p.z, 0)
        p['z'] = 1
        self.assertEqual(p.z, 1)
        p.z = None
        self.assertEqual(p.y, 0)

        with self.assertRaises(AttributeError, msg='p.w = 2'):
            p.w = 2
        with self.assertRaises(TypeError, msg='p[3] = 2'):
            p[3] = 2
        with self.assertRaises(TypeError, msg="p['w'] = 2"):
            p['w'] = 2

    def testPointMultiplePropertySettersSimple(self):
        '''
        '''

        p = Point()

        p.xy = (1, 1)
        self.assertSequenceEqual(p.xyz, (1, 1, 0))
        p.xy = None
        self.assertIsOrigin(p)

        p.xz = (2, 2)
        self.assertSequenceEqual(p.xyz, (2, 0, 2))
        p.xz = None
        self.assertIsOrigin(p)

        p.yz = (3, 3)
        self.assertSequenceEqual(p.xyz, (0, 3, 3))
        p.yz = None
        self.assertIsOrigin(p)

        p.xyz = (4, 4, 4)
        self.assertSequenceEqual(p.xyz, (4, 4, 4))
        p.xyz = None
        self.assertIsOrigin(p)

        p.xyzw = (5, 5, 5, 5)
        self.assertSequenceEqual(p.xyzw, (5, 5, 5, 1))
        p.xyzw = None
        self.assertIsOrigin(p)

        p['xy'] = (6, 6)
        self.assertSequenceEqual(p.xyz, (6, 6, 0))
        p['xy'] = None
        self.assertIsOrigin(p)

        p['yz'] = (7, 7)
        self.assertSequenceEqual(p.xyz, (0, 7, 7))
        p['yz'] = None
        self.assertIsOrigin(p)

        p['xz'] = (8, 8)
        self.assertSequenceEqual(p.xyz, (8, 0, 8))
        p['xz'] = None
        self.assertIsOrigin(p)

        p['xyz'] = (9, 9, 9)
        self.assertSequenceEqual(p.xyz, (9, 9, 9))
        p['xyz'] = None
        self.assertIsOrigin(p)

        p['xyzw'] = (10, 10, 10, 10)
        self.assertSequenceEqual(p.xyzw, (10, 10, 10, 1))
        p['xyzw'] = None
        self.assertIsOrigin(p)

    def testPointPropertySettersComplex(self):
        '''
        '''

        mapping = {'x': 1, 'y': 2, 'z': 3}
        iterable = [4, 5, 6]

        p = Point()
        p.x = mapping
        self.assertCoordinatesEqual(p, (1, 0, 0))
        p.x = iterable
        self.assertCoordinatesEqual(p, (4, 0, 0))

        p = Point()
        p.y = mapping
        self.assertCoordinatesEqual(p, (0, 2, 0))
        p.y = iterable
        self.assertCoordinatesEqual(p, (0, 4, 0))

        p = Point()
        p.z = mapping
        self.assertCoordinatesEqual(p, (0, 0, 3))
        p.z = iterable
        self.assertCoordinatesEqual(p, (0, 0, 4))

    def testPointMultiplePropertySettersComplex(self):
        '''
        '''

        mapping = {'x': 1, 'y': 2, 'z': 3}
        iterable = [4, 5, 6]

        p = Point()
        p.xy = mapping
        self.assertCoordinatesEqual(p, (1, 2, 0))
        p.xy = iterable
        self.assertCoordinatesEqual(p, (4, 5, 0))

        p = Point()
        p.xz = mapping
        self.assertCoordinatesEqual(p, (1, 0, 3))
        p.xz = iterable
        self.assertCoordinatesEqual(p, (4, 0, 5))

        p = Point()
        p.yz = mapping
        self.assertCoordinatesEqual(p, (0, 2, 3))
        p.yz = iterable
        self.assertCoordinatesEqual(p, (0, 4, 5))

        p = Point()
        p.xyz = mapping
        self.assertCoordinatesEqual(p, (1, 2, 3))
        p.xyz = iterable
        self.assertCoordinatesEqual(p, (4, 5, 6))

        p = Point()
        p.xyzw = mapping
        self.assertCoordinatesEqual(p, (1, 2, 3))
        p.xyzw = iterable
        self.assertCoordinatesEqual(p, (4, 5, 6))

    def testPointClassmethod_unit(self):
        '''
        '''

        with self.assertRaises(TypeError, msg='Point.unit(None,None)'):
            Point.unit(None, None)

        o = Point.origin()

        self.assertCoordinatesEqual(Point.unit(o, Point(3, 0, 0)), [1, 0, 0])
        self.assertCoordinatesEqual(Point.unit(o, Point(0, 3, 0)), [0, 1, 0])
        self.assertCoordinatesEqual(Point.unit(o, Point(0, 0, 3)), [0, 0, 1])

        for x in range(0, 100):
            self.assertEpsilonEqual(
                o.distance(
                    Point.unit(
                        o,
                        Point.random())),
                1.0)

    def testPointClassmethod_units(self):
        '''
        '''
        v = Point.units()
        self.assertEqual(len(v), 3)
        self.assertCoordinatesEqual(v[0], [1, 0, 0])
        self.assertCoordinatesEqual(v[1], [0, 1, 0])
        self.assertCoordinatesEqual(v[2], [0, 0, 1])

        for scale in [2, 3, 4]:
            v = Point.units(scale)
            self.assertEqual(len(v), 3)
            self.assertCoordinatesEqual(v[0], [scale, 0, 0])
            self.assertCoordinatesEqual(v[1], [0, scale, 0])
            self.assertCoordinatesEqual(v[2], [0, 0, scale])

    def testPointClassmethod_gaussian(self):
        '''
        '''

        self.assertIsPoint(Point.gaussian())

    def testPointClassmethod_random(self):
        '''
        '''

        self.assertIsPoint(Point.random())
        for _ in range(100):
            self.assertTrue(Point.random().distance() <= 1.0)

        o = Point(10, 10)
        for _ in range(100):
            self.assertTrue(Point.random(o, 2).distance(o) <= 2)

    def testPointMethod_hash(self):
        '''
        '''

        # MAGIC NUMBER: hash value for a Point(0,0,0) object
        #               will fail if the hash calculation changes.
        #               need a better test method.

        v = 1553416657114974281

        self.assertTrue(hash(Point.origin()) == v)
        self.assertFalse(hash(Point(1, 1, 1)) == v)

    def testPointMethod_len(self):
        '''
        '''

        self.assertEqual(len(Point()), 3)

    def testPointMethod_iter(self):
        '''
        '''
        p = Point()
        i = iter(p)
        self.assertTrue(p != i)
        self.assertFalse(p is i)

    def testPointMethod_next(self):
        '''
        '''
        i = iter(Point())

        self.assertTrue(next(i) == 'x')
        self.assertTrue(next(i) == 'y')
        self.assertTrue(next(i) == 'z')

        with self.assertRaises(StopIteration, msg='next({!r})'.format(i)):
            next(i)

    def testPointMethod_eq(self):
        '''
        '''

        self.assertTrue(Point() == Point())
        self.assertTrue(Point() != Point(1))

        self.assertTrue(None == Point.origin())
        self.assertFalse(Point.origin() is None)
        self.assertTrue(None != Point(1))
        self.assertTrue(Point(1) is not None)

        o = Point.origin()
        for x in range(1, 3):
            zeros = [0] * x
            ones = [1] * x
            self.assertTrue(zeros == o)
            self.assertTrue(o == zeros)
            self.assertTrue(ones != o)
            self.assertTrue(o != ones)

    def testPointMethod_addition(self):
        '''
        '''
        for b in self.testObjects(1):
            a = Point()
            c = a + b
            self.assertFalse(c is a)
            self.assertFalse(c is b)
            self.assertCoordinatesEqual(c, [1, 1, 1])
            c = b + a
            self.assertFalse(c is a)
            self.assertFalse(c is b)
            self.assertCoordinatesEqual(c, [1, 1, 1])
            a += b
            self.assertCoordinatesEqual(a, [1, 1, 1])

    def testPointMethod_subtraction(self):
        '''
        '''
        for b in self.testObjects(1):
            a = Point()
            c = a - b
            self.assertFalse(c is a)
            self.assertFalse(c is b)
            self.assertCoordinatesEqual(c, [-1, -1, -1])
            c = b - a
            self.assertFalse(c is a)
            self.assertFalse(c is b)
            self.assertCoordinatesEqual(c, [1, 1, 1])
            a -= b
            self.assertCoordinatesEqual(a, [-1, -1, -1])

    def testPointMethod_multiplication(self):
        '''
        '''
        for b in self.testObjects(2):
            a = Point(2, 2, 2)
            c = a * b
            self.assertFalse(c is a)
            self.assertFalse(c is b)
            self.assertCoordinatesEqual(c, [4, 4, 4])
            c = b * a
            self.assertFalse(c is a)
            self.assertFalse(c is b)
            self.assertCoordinatesEqual(c, [4, 4, 4])
            a *= b
            self.assertCoordinatesEqual(a, [4, 4, 4])

    def testPointMethod_floordivision(self):
        '''
        '''
        for b in self.testObjects(2):
            a = Point(3, 3, 3)
            c = a // b
            self.assertFalse(c is a)
            self.assertFalse(c is b)
            self.assertCoordinatesEqual(c, [1, 1, 1])
            c = b // a
            self.assertFalse(c is a)
            self.assertFalse(c is b)
            self.assertCoordinatesEqual(c, [0, 0, 0])
            a //= b
            self.assertCoordinatesEqual(a, [1, 1, 1])

        a = Point(1, 1, 1)
        for b in self.testObjects(0, None):
            with self.assertRaises(ZeroDivisionError,
                                   msg='{!r} // {!r}'.format(a, b)):
                c = a // b
            with self.assertRaises(ZeroDivisionError,
                                   msg='{!r} //= {!r}'.format(a, b)):
                a //= b

    def testPointMethod_truedivision(self):
        '''
        '''
        for b in self.testObjects(2):
            a = Point(3, 3, 3)
            c = a / b
            self.assertFalse(c is a)
            self.assertFalse(c is b)
            self.assertCoordinatesEqual(c, [1.5] * 3)
            c = b / a
            self.assertFalse(c is a)
            self.assertFalse(c is b)
            self.assertCoordinatesEqual(c, [2 / 3] * 3)
            a /= b
            self.assertCoordinatesEqual(a, [1.5] * 3)

        a = Point(1, 1, 1)
        for b in self.testObjects(0, None):
            with self.assertRaises(ZeroDivisionError,
                                   msg='{!r} / {!r}'.format(a, b)):
                c = a / b
            with self.assertRaises(ZeroDivisionError,
                                   msg='{!r} /= {!r}'.format(a, b)):
                a /= b

    def testPointMethod_modulus(self):
        '''
        '''
        for b in self.testObjects(2):
            a = Point(1, 1, 1)
            c = a % b
            self.assertFalse(c is a)
            self.assertFalse(c is b)
            self.assertCoordinatesEqual(c, [1, 1, 1])
            c = b % a
            self.assertFalse(c is a)
            self.assertFalse(c is b)
            self.assertCoordinatesEqual(c, [0, 0, 0])
            a %= b
            self.assertCoordinatesEqual(a, [1, 1, 1])

        a = Point(1, 1, 1)
        for b in self.testObjects(0, None):
            with self.assertRaises(ZeroDivisionError,
                                   msg='{!r} % {!r}'.format(a, b)):
                c = a % b
            with self.assertRaises(ZeroDivisionError,
                                   msg='{!r} %= {!r}'.format(a, b)):
                a %= b

    def testPointMethod_power(self):
        '''
        '''
        self.assertCoordinatesEqual(Point() ** 1, [0, 0, 0])
        self.assertCoordinatesEqual(Point() ** 0, [1, 1, 1])

        for b in self.testObjects(2):
            a = Point(2, 2, 2)
            c = a ** b
            self.assertFalse(c is a)
            self.assertFalse(c is b)
            self.assertCoordinatesEqual(c, [4, 4, 4])
            c = b ** a
            self.assertFalse(c is a)
            self.assertFalse(c is b)
            self.assertCoordinatesEqual(c, [4, 4, 4])
            a **= b
            self.assertCoordinatesEqual(a, [4, 4, 4])

    def testPointMethod_rightshift(self):
        '''
        '''
        for b in self.testObjects(1):
            a = Point(2, 2, 2)
            c = a >> b
            self.assertFalse(c is a)
            self.assertFalse(c is b)
            self.assertCoordinatesEqual(c, [1, 1, 1])
            c = b >> a
            self.assertFalse(c is a)
            self.assertFalse(c is b)
            self.assertCoordinatesEqual(c, [0, 0, 0])
            a >>= b
            self.assertCoordinatesEqual(a, [1, 1, 1])

    def testPointMethod_leftshift(self):
        '''
        '''
        for b in self.testObjects(1):
            a = Point(2, 2, 2)
            c = a << b
            self.assertFalse(c is a)
            self.assertFalse(c is b)
            self.assertCoordinatesEqual(c, [4, 4, 4])
            c = b << a
            self.assertFalse(c is a)
            self.assertFalse(c is b)
            self.assertCoordinatesEqual(c, [4, 4, 4])
            a <<= b
            self.assertCoordinatesEqual(a, [4, 4, 4])

    def testPointMethod_bitwise_and(self):
        '''
        '''
        for b in self.testObjects(1):
            a = Point()
            c = a & b
            self.assertFalse(c is a)
            self.assertFalse(c is b)
            self.assertCoordinatesEqual(c, [0, 0, 0])

            c = b & a
            self.assertFalse(c is a)
            self.assertFalse(c is b)
            self.assertCoordinatesEqual(c, [0, 0, 0])

            a &= b
            self.assertCoordinatesEqual(a, [0, 0, 0])

    def testPointMethod_bitwise_or(self):
        '''
        '''
        for b in self.testObjects(1):
            a = Point()
            c = a | b
            self.assertFalse(c is a)
            self.assertFalse(c is b)
            self.assertCoordinatesEqual(c, [1, 1, 1])

            c = b | a
            self.assertFalse(c is a)
            self.assertFalse(c is b)
            self.assertCoordinatesEqual(c, [1, 1, 1])

            a |= b
            self.assertCoordinatesEqual(a, [1, 1, 1])

    def testPointMethod_bitwise_xor(self):
        '''
        '''

        # XOR Truth Table:
        # 0 ^ 0 -> 0
        # 0 ^ 1 -> 1
        # 1 ^ 0 -> 1
        # 1 ^ 1 -> 0

        for b in self.testObjects(1):
            a = Point()
            c = a ^ b
            self.assertFalse(c is a)
            self.assertFalse(c is b)
            self.assertCoordinatesEqual(c, [1, 1, 1])
            a ^= b
            self.assertCoordinatesEqual(a, [1, 1, 1])

            a = Point(1, 1, 1)
            c = a ^ b
            self.assertFalse(c is a)
            self.assertFalse(c is b)
            self.assertCoordinatesEqual(c, [0, 0, 0])
            a ^= b
            self.assertCoordinatesEqual(a, [0, 0, 0])

        for b in self.testObjects(0):
            a = Point(1, 1, 1)
            c = a ^ b
            self.assertFalse(c is a)
            self.assertFalse(c is b)
            self.assertCoordinatesEqual(c, [1, 1, 1])
            a ^= b
            self.assertCoordinatesEqual(a, [1, 1, 1])
            a = Point()
            c = a ^ b
            self.assertFalse(c is a)
            self.assertFalse(c is b)
            self.assertCoordinatesEqual(c, [0, 0, 0])
            a ^= b
            self.assertCoordinatesEqual(a, [0, 0, 0])

    # unary methods

    def testPointMethod_positive(self):
        '''
        '''
        for v in [1, -1]:
            p = Point([v] * 3)
            self.assertTrue(+p is p)
            self.assertCoordinatesEqual(+p, [v] * 3)

    def testPointMethod_negative(self):
        '''
        '''
        for v in [1, -1]:
            p = Point([v] * 3)
            self.assertFalse(-p is p)
            self.assertCoordinatesEqual(-p, [-v] * 3)

    def testPointMethod_absolute(self):
        '''
        '''
        for v in [1, -1]:
            p = Point([v] * 3)
            self.assertTrue(abs(p) is p)
            self.assertCoordinatesEqual(p, [abs(v)] * 3)

    def testPointMethod_invert(self):
        '''
        '''
        p = Point()

        self.assertTrue(~p is p)
        self.assertCoordinatesEqual(p, [-1] * 3)
        ~p
        self.assertCoordinatesEqual(p, [0] * 3)

    def testPointMethod_round(self):
        '''
        '''
        V = 1.234

        p = Point([V] * 3)
        q = round(p)
        self.assertCoordinatesEqual(q, [round(V)] * 3)

        for n in range(1, 4):
            p = Point([V] * 3)
            q = round(p, n)
            self.assertFalse(q is p)
            self.assertCoordinatesEqual(
                q, [round(V, n)] * 3, 'round({!r},{})'.format(p, n))

    def testPointMethod_floor(self):
        '''
        '''
        for v in range(1, 10):
            f = math.floor(1 / v)
            p = Point([1 / v] * 3)
            q = math.floor(p)
            self.assertFalse(q is p)
            self.assertCoordinatesEqual(
                q, [f] * 3, 'floor {!r} for 1/{}'.format(p, v))

    def testPointMethod_ceiling(self):
        '''
        '''
        for v in range(1, 10):
            f = math.ceil(1 / v)
            p = Point([1 / v] * 3)
            q = math.ceil(p)
            self.assertFalse(q is p)
            self.assertCoordinatesEqual(
                q, [f] * 3, 'ceil {!r} for 1/{}'.format(p, v))

    # class specific methods
    def testPointMethod_dot_product(self):
        '''
        '''
        for scale in [1, -1]:
            i, j, k = Point.units(scale)
            self.assertEqual(i.dot(i), 1)
            self.assertEqual(i.dot(j), 0)
            self.assertEqual(i.dot(k), 0)

            self.assertEqual(j.dot(j), 1)
            self.assertEqual(j.dot(i), 0)
            self.assertEqual(j.dot(k), 0)

            self.assertEqual(k.dot(k), 1)
            self.assertEqual(k.dot(i), 0)
            self.assertEqual(k.dot(j), 0)

    def testPointMethod_cross_product(self):
        '''
        '''
        for scale in [1, -1]:
            i, j, k = Point.units(scale)
            self.assertEqual(i.cross(i), 0)
            self.assertEqual(i.cross(j), 1)
            self.assertEqual(i.cross(k), -1)

            self.assertEqual(j.cross(j), 0)
            self.assertEqual(j.cross(k), 1)
            self.assertEqual(j.cross(i), -1)

            self.assertEqual(k.cross(k), 0)
            self.assertEqual(k.cross(i), 1)
            self.assertEqual(k.cross(j), -1)

    def testPointMethod_midpoint(self):
        '''
        '''
        p = Point(2, 2, 2)
        q = Point.origin()
        m = p.midpoint(q)
        self.assertFalse(m is p)
        self.assertFalse(m is q)
        self.assertCoordinatesEqual(m, [1, 1, 1])
        self.assertEqual(m.distance(p), m.distance(q))

    def testPointMethod_isBetween(self):
        '''
        '''
        o = Point.origin()
        p = Point(1, 1, 1)
        u, v, w = Point.units(scale=2)

        self.assertFalse(p.isBetween(o, u))
        self.assertFalse(p.isBetween(o, v))
        self.assertFalse(p.isBetween(o, w))

        self.assertTrue(p.isBetween(o, u + v + w))

        self.assertTrue(p.isBetween(o, u, axes='x'))
        self.assertTrue(p.isBetween(o, v, axes='y'))
        self.assertTrue(p.isBetween(o, w, axes='z'))

        self.assertFalse(p.isBetween(o, u, axes='y'))
        self.assertFalse(p.isBetween(o, u, axes='z'))
        self.assertFalse(p.isBetween(o, u, axes='yz'))

        self.assertFalse(p.isBetween(o, v, axes='x'))
        self.assertFalse(p.isBetween(o, v, axes='z'))
        self.assertFalse(p.isBetween(o, v, axes='xz'))

        self.assertFalse(p.isBetween(o, w, axes='x'))
        self.assertFalse(p.isBetween(o, w, axes='y'))
        self.assertFalse(p.isBetween(o, w, axes='xy'))

    def testPointMethod_distance(self):
        '''
        '''
        i, j, k = Point.units()

        o = Point.origin()

        self.assertEqual(i.distance(i), 0)
        self.assertEqual(j.distance(j), 0)
        self.assertEqual(k.distance(k), 0)

        self.assertEqual(o.distance(i), 1)
        self.assertEqual(o.distance(j), 1)
        self.assertEqual(o.distance(k), 1)

        self.assertEqual(i.distance(j), j.distance(i))
        self.assertEqual(i.distance(k), k.distance(i))
        self.assertEqual(j.distance(k), k.distance(j))

        self.assertEqual(i.distance(), i.distance(o))

    def testPointMethod_distanceSquared(self):
        '''
        '''
        i, j, k = Point.units()
        o = Point.origin()

        self.assertEqual(i.distanceSquared(i), 0)
        self.assertEqual(j.distanceSquared(j), 0)
        self.assertEqual(k.distanceSquared(k), 0)

        self.assertEqual(i.distanceSquared(o), 1)
        self.assertEqual(j.distanceSquared(o), 1)
        self.assertEqual(k.distanceSquared(o), 1)

        self.assertEqual(i.distanceSquared(j), 2)
        self.assertEqual(i.distanceSquared(k), 2)
        self.assertEqual(j.distanceSquared(i), 2)
        self.assertEqual(j.distanceSquared(k), 2)
        self.assertEqual(k.distanceSquared(i), 2)
        self.assertEqual(k.distanceSquared(j), 2)

        self.assertEqual(i.distanceSquared(), i.distanceSquared(o))

    def testPointMethod_ccw(self):
        '''
        '''
        i, j, k = Point.units()
        o = Point.origin()

        self.assertEqual(o.ccw(i, j, axis='x'), 0)
        self.assertEqual(o.ccw(i, j, axis='y'), 0)
        self.assertEqual(o.ccw(i, j, axis='z'), 1)

        self.assertEqual(o.ccw(j, i, axis='x'), 0)
        self.assertEqual(o.ccw(j, i, axis='y'), 0)
        self.assertEqual(o.ccw(j, i, axis='z'), -1)

        self.assertEqual(o.ccw(i, k, axis='x'), 0)
        self.assertEqual(o.ccw(i, k, axis='y'), 1)
        self.assertEqual(o.ccw(i, k, axis='z'), 0)

        self.assertEqual(o.ccw(k, i, axis='x'), 0)
        self.assertEqual(o.ccw(k, i, axis='y'), -1)
        self.assertEqual(o.ccw(k, i, axis='z'), 0)

        self.assertEqual(o.ccw(j, k, axis='x'), 1)
        self.assertEqual(o.ccw(j, k, axis='y'), 0)
        self.assertEqual(o.ccw(j, k, axis='z'), 0)

        self.assertEqual(o.ccw(k, j, axis='x'), -1)
        self.assertEqual(o.ccw(k, j, axis='y'), 0)
        self.assertEqual(o.ccw(k, j, axis='z'), 0)

        for junk in [-1, 'foo', {}, 13.0, None]:
            with self.assertRaises(ValueError,
                                   msg='junk is {}'.format(junk)):
                o.ccw(i, j, axis=junk)

    def testPointMethod_isCCW(self):
        '''
        '''
        i, j, k = Point.units()
        o = Point.origin()

        self.assertTrue(o.isCCW(i, j, axis='z'))
        self.assertFalse(o.isCCW(j, i, axis='z'))

        self.assertTrue(o.isCCW(j, k, axis='x'))
        self.assertFalse(o.isCCW(k, j, axis='x'))

        self.assertTrue(o.isCCW(i, k, axis='y'))
        self.assertFalse(o.isCCW(k, i, axis='y'))

        with self.assertRaises(CollinearPoints):
            o.isCCW(i, j, axis='x')
        with self.assertRaises(CollinearPoints):
            o.isCCW(i, j, axis='y')

        with self.assertRaises(CollinearPoints):
            o.isCCW(j, k, axis='y')
        with self.assertRaises(CollinearPoints):
            o.isCCW(j, k, axis='z')

        with self.assertRaises(CollinearPoints):
            o.isCCW(i, k, axis='x')
        with self.assertRaises(CollinearPoints):
            o.isCCW(i, k, axis='z')

        for junk in [-1, 'foo', {}, 13.0, None]:
            with self.assertRaises(ValueError,
                                   msg='junk is {}'.format(junk)):
                o.isCCW(i, j, axis=junk)

    def testPointMethod_isCollinear(self):
        '''
        '''
        i, j, k = Point.units()
        u, v, w = Point.units(2)
        o = Point.origin()

        self.assertTrue(o.isCollinear(i, u))
        self.assertTrue(o.isCollinear(u, i))
        self.assertTrue(i.isCollinear(o, u))
        self.assertTrue(i.isCollinear(u, o))
        self.assertTrue(u.isCollinear(o, i))
        self.assertTrue(u.isCollinear(i, o))

        self.assertTrue(o.isCollinear(j, v))
        self.assertTrue(o.isCollinear(v, j))
        self.assertTrue(j.isCollinear(o, v))
        self.assertTrue(j.isCollinear(v, o))
        self.assertTrue(v.isCollinear(o, j))
        self.assertTrue(v.isCollinear(j, o))

        self.assertTrue(o.isCollinear(k, w))
        self.assertTrue(o.isCollinear(w, k))
        self.assertTrue(k.isCollinear(o, w))
        self.assertTrue(k.isCollinear(w, o))
        self.assertTrue(w.isCollinear(o, k))
        self.assertTrue(w.isCollinear(k, o))

        self.assertFalse(o.isCollinear(i, v))
        self.assertFalse(o.isCollinear(i, w))

        self.assertFalse(o.isCollinear(j, u))
        self.assertFalse(o.isCollinear(j, w))

        self.assertFalse(o.isCollinear(k, u))
        self.assertFalse(o.isCollinear(k, v))

    def testPointMethod_rotate2d(self):
        '''
        '''

    def testPointMethod_irotate2d(self):
        '''
        '''

    def rotate(self, theta_x, theta_y, theta_z, origin=None, radians=False):
        '''
        '''
