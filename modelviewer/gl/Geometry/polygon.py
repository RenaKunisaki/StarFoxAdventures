''' python Polygon

'''

from . import Point, PointSequence, MutablePointSequence, Segment
from . import CollinearPoints

class Polygon(PointSequence):
    '''
    A plane figure that is bound by a finite chain of straight
    line segments closing in a loop to form a closed chain or
    circuit. The segments are called edges and the points where
    the two edges meet are the polgon's vertices.

    '''

    def __contains__(self, point):
        '''
        True iff point is on the perimeter or enclosed by the polygon.

        '''
        try:
            r = [a.isCCW(b,point) for a,b in self.pairs()]
        except CollinearPoints:
            return True

        return not (any(r) and not all(r))
        
    def edges(self):
        '''
        A list of Segments.
        '''
        return [Segment(a,b) for a,b in self.pairs()]

    def sides(self):
        '''
        Segment lengths, list of floats.
        '''
        return [s.length for s in self.segments()]

    @property
    def perimeter(self):
        '''
        Sum of the length of all sides, float.
        '''
        return sum([a.distance(b) for a, b in self.pairs()])

    @property
    def semiperimeter(self):
        '''
        Half of the perimeter, float.
        '''
        return self.perimeter / 2

    @property
    def centroid(self):
        '''
        '''
        return sum(self.vertices) / len(self.vertices)

    def incenter(self):
        '''
        '''
        return sum([s*p for s,p in zip(self.sides, self.vertices)]) / len(self.vertices)

    @property
    def midpoints(self):
        '''
        The midpoints of each edge, list of Points.

        '''
        return [s.midpoint for s in self.edges]    


class MutablePolygon(Polygon, MutablePointSequence):
    pass


