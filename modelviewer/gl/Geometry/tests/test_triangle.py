
import unittest
import math

from .. import Point, Triangle, Segment, Circle
from ..exceptions import *


class TriangleTestCase(unittest.TestCase):

    def assertAlmostEqual(self, test_value, known_value):
        self.assertTrue(
            round(
                float(test_value),
                10) == round(
                float(known_value),
                10))

    def assertAlmostNotEqual(self, test_value, known_value):
        self.assertTrue(
            round(
                float(test_value),
                10) != round(
                float(known_value),
                10))

    def assertIsTriangle(self, obj):
        self.assertIsInstance(obj, Triangle)
        self.assertEqual(len(obj), 3)
        self.assertEqual(len(obj.vertices), 3)
        self.assertEqual(len(obj.sides), 3)
        self.assertEqual(len(set(obj.vertices)), 3)

    def assertTriangleVerticesEqual(self, triangle, vertices):
        for a, b in zip(triangle.vertices, vertices):
            self.assertEqual(a, b)

    def testTriangleCreationWithoutArguments(self):
        '''
        '''
        t = Triangle()
        self.assertIsTriangle(t)
        self.assertTriangleVerticesEqual(t, [[0, 0, 0],
                                             [1, 0, 0],
                                             [0, 1, 0]])

    def testTriangleClassmethodUnit(self):
        '''
        '''
        t = Triangle.unit()
        self.assertIsTriangle(t)
        self.assertTriangleVerticesEqual(t, [[1, 0, 0],
                                             [0, 1, 0],
                                             [0, 0, 1]])

    def testTriangleCreationWithArguments(self):
        '''
        '''
        i, j, k = Point.units()

        t = Triangle(i, j, k)
        self.assertTriangleVerticesEqual(t, (i, j, k))

        t = Triangle([i, j, k])
        self.assertTriangleVerticesEqual(t, (i, j, k))

        t = Triangle(A=i, B=j, C=k)
        self.assertTriangleVerticesEqual(t, (i, j, k))

        t = Triangle(B=j, A=i, C=k)
        self.assertTriangleVerticesEqual(t, (i, j, k))
        
        t = Triangle(C=k, B=j, A=i)
        self.assertTriangleVerticesEqual(t, (i, j, k))        

#        t = Triangle(i, j, C=k)
#        self.assertTriangleVerticesEqual(t, (i, j, k))
#
#        t = Triangle(i, B=j, C=k)
#        self.assertTriangleVerticesEqual(t, (i, j, k))
#
#        t = Triangle(A=i, B=j, C=k)
#        self.assertTriangleVerticesEqual(t, (i, j, k))
#
#        t = Triangle(None, A=i, B=j, C=k)
#        self.assertTriangleVerticesEqual(t, (i, j, k))
#
#        t = Triangle(None, None, A=i, B=j, C=k)
#        self.assertTriangleVerticesEqual(t, (i, j, k))
#
#        t = Triangle(None, None, None, A=i, B=j, C=k)
#        self.assertTriangleVerticesEqual(t, (i, j, k))
#
#        t = Triangle([None, None, None], None, A=i, B=j, C=k)
#        self.assertTriangleVerticesEqual(t, (i, j, k))


    def testTriangleProperty_Getters(self):

        i, j, k = Point.units()

        t = Triangle(i, j, k)

        self.assertEqual(t.A, i)
        self.assertEqual(t.B, j)
        self.assertEqual(t.C, k)

        self.assertSequenceEqual(t.AB, (i, j))
        self.assertSequenceEqual(t.BA, (j, i))

        self.assertSequenceEqual(t.BC, (j, k))
        self.assertSequenceEqual(t.CB, (k, j))

        self.assertSequenceEqual(t.AC, (i, k))
        self.assertSequenceEqual(t.CA, (k, i))

        self.assertSequenceEqual(t.ABC, (i, j, k))
        self.assertSequenceEqual(t.vertices, (i, j, k))

    def testTrianglePropertySetters(self):
        i, j, k = Point.units(5)

        t = Triangle()

        self.assertNotEqual(t.A, i)
        t.A = i
        self.assertEqual(t.A, i)

        self.assertNotEqual(t.B, j)
        t.B = j
        self.assertEqual(t.B, j)

        self.assertNotEqual(t.C, k)
        t.C = k
        self.assertEqual(t.C, k)

    def testTriangleProperty_MultiSetters(self):

        i, j, k = Point.units(5)

        t = Triangle()

        self.assertNotEqual(t.A, i)
        self.assertNotEqual(t.B, j)
        t.AB = i, j
        self.assertSequenceEqual(t.AB, (i, j))
#        with self.assertRaises(ValueError):
#            t.AB = i, j, k

        t.BA = i, j
        self.assertSequenceEqual(t.BA, (i, j))
#        with self.assertRaises(ValueError):
#            t.BA = i, j, k

        t.AC = i, j
        self.assertSequenceEqual(t.AC, (i, j))
#        with self.assertRaises(ValueError):
#            t.AC = i, j, k

        t.CA = i, j
        self.assertSequenceEqual(t.CA, (i, j))
#        with self.assertRaises(ValueError):
#            t.CA = i, j, k

        t.BC = i, j
        self.assertSequenceEqual(t.BC, (i, j))
#        with self.assertRaises(ValueError):
#            t.BC = i, j, k

        t.CB = i, j
        self.assertSequenceEqual(t.CB, (i, j))
#        with self.assertRaises(ValueError):
#            t.CB = i, j, k

    def testTriangleProperty_edges(self):

        t = Triangle()

        sAB = Segment(t.AB)
        sBC = Segment(t.BC)
        sAC = Segment(t.AC)

        self.assertSequenceEqual(t.edges(), [sAB, sBC, sAC])

    def testTriangleProperty_Hypotenuse(self):

        t = Triangle()

        h = max(t.edges(),key=lambda s:s.length)

        self.assertEqual(t.hypotenuse.length, h.length)

        with self.assertRaises(AttributeError):
            t.hypotenuse = Segment([14, 17], [22, 44])

    def testTriangleProperty_Circumcenter(self):

        t = Triangle()

        cc = t.circumcenter

        dA = t.A.distance(cc)
        dB = t.B.distance(cc)
        dC = t.C.distance(cc)

        self.assertTrue(dA == dB == dC)

        with self.assertRaises(AttributeError):
            t.circumcenter = Point()

    def testTriangleProperty_Orthocenter(self):

        t = Triangle()
        
        with self.assertRaises(NotImplementedError):
            oc = t.orthocenter

        with self.assertRaises(AttributeError):
            t.orthocenter = [0,0]
            
            
    def testTriangleProperty_Circumradius(self):

        t = Triangle()

        cc = t.circumcenter
        cr = t.circumradius

        for p in t.ABC:
            self.assertEqual(p.distance(cc), cr)

        with self.assertRaises(AttributeError):
            t.circumradius = 12

    def testTriangleProperty_Circumcircle(self):

        t = Triangle()
        c = t.circumcircle

#        for p in t.ABC:
#            self.assertEqual(p.distance(c.center), c.radius)

        self.assertTrue(all([p.distance(c.center) == c.radius for p in t.ABC]))

        with self.assertRaises(AttributeError):
            t.circumcircle = Circle()

    def testTriangleProperties_AlphaBetaGamma(self):

        t = Triangle()

        self.assertEqual(sum([t.alpha, t.beta, t.gamma]), math.pi)
        self.assertEqual(sum(t.angles), math.pi)

        with self.assertRaises(AttributeError):
            t.alpha = 11

        with self.assertRaises(AttributeError):
            t.beta = 11

        with self.assertRaises(AttributeError):
            t.gamma = 11

    def testTriangleProperties_abc(self):

        t = Triangle()

        self.assertEqual(t.a, t.hypotenuse.length)
        self.assertEqual(t.b, 1)
        self.assertEqual(t.c, 1)

        with self.assertRaises(AttributeError):
            t.a = 11

        with self.assertRaises(AttributeError):
            t.b = 11

        with self.assertRaises(AttributeError):
            t.c = 11

    def testTriangleProperties_sides(self):

        t = Triangle()

        self.assertEqual(len(t.sides), 3)
        self.assertEqual(t.sides[0], Segment(t.BC).length)
        self.assertEqual(t.sides[1], Segment(t.AC).length)
        self.assertEqual(t.sides[2], Segment(t.AB).length)

        with self.assertRaises(AttributeError):
            t.sides = [5, 6, 7]

    def testTriangleProperty_isCCW_flip_ccw_area(self):

        t = Triangle()

        for edge in ['AB', 'BC', 'AC']:
            self.assertTrue(t.isCCW)
            self.assertTrue(t.ccw > 0)
#            t.swap(edge, inplace=True)
#            self.assertFalse(t.isCCW)
#            self.assertTrue(t.ccw < 0)
#            t.swap(edge, inplace=True)
#            self.assertTrue(t.isCCW)
            self.assertAlmostEqual(t.ccw / 2, t.heronsArea)
            self.assertAlmostEqual(t.area, t.heronsArea)

        with self.assertRaises(AttributeError):
            t.ccw = 55

        with self.assertRaises(AttributeError):
            t.isCCW = False

    def testTriangleProperty_perimeter_semiperimeter(self):

        t = Triangle()

        p = t.A.distance(t.B) + t.B.distance(t.C) + t.C.distance(t.A)

        self.assertEqual(t.perimeter, p)

        self.assertEqual(t.semiperimeter, p / 2)

        with self.assertRaises(AttributeError):
            t.perimeter = 11

        with self.assertRaises(AttributeError):
            t.semiperimeter = 11

    def testTriangleProperty_isEquilateral_isIsoceles_isAcute(self):

        with self.assertRaises(NotImplementedError):
            t = Triangle.withAngles(alpha=60,beta=60,gamma=60,inDegrees=True)
            self.assertTrue(t.isEquilateral)
            self.assertTrue(t.isIsosceles)
            self.assertTrue(t.isAcute)
            self.assertAlmostEqual(t.a, t.b)
            self.assertAlmostEqual(t.b, t.c)
            self.assertAlmostEqual(t.a, t.c)
            self.assertAlmostEqual(t.alpha, t.beta)
            self.assertAlmostEqual(t.beta, t.gamma)
            self.assertAlmostEqual(t.alpha, t.gamma)
            self.assertTrue(all([a < (math.pi/2) for a in t.angles]))

    def testTriangleProperty_isIsosceles(self):
        
        with self.assertRaises(NotImplementedError):
            t = Triangle.withAngles(alpha=40,beta=40,gamma=100,inDegrees=True)
            self.assertTrue(t.isIsosceles)
            self.assertFalse(t.isEquilateral)
            self.assertTrue(t.isAcute)

    def testTriangleProperty_isScalene(self):

        with self.assertRaises(NotImplementedError):
            t = Triangle.withSides(a=1,b=2,c=3)
            self.assertTrue(t.isScalene)

    def testTriangleProperty_isRight(self):

        with self.assertRaises(NotImplementedError):
            t = Triangle.withAngles(alpha=90,beta=45,gamma=45,inDegrees=True)
            self.assertTrue(t.isRight)
            self.assertTrue(t.circumcenter == t.hypotenuse.midpoint)

    def testTriangleProperty_isObtuse(self):
        
        with self.assertRaises(NotImplementedError):
            t = Triangle.withAngles(alpha=100,beta=40,gamma=40,inDegrees=True)
            self.assertTrue(t.isObtuse)


    def testTriangleOperation__eq__(self):

        t = Triangle()
        u = Triangle()

        self.assertFalse(t is u)
        self.assertTrue(t == u)

    def testTriangleMethod_congruent(self):

        t = Triangle()

        p = Point(10, 10)

        u = Triangle(t.A + p, t.B + p, t.C + p)

        self.assertTrue(t.congruent(u))
        self.assertTrue(u.congruent(t))

    def testTriangleOperation__contains__(self):

        t = Triangle()

        self.assertTrue(t.circumcenter in t)
        self.assertFalse(t.A + Point(-10, -10) in t)

    def testTriangleMethod_doesIntersect(self):

        t = Triangle()
        u = Triangle()
