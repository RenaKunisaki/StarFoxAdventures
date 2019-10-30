''' geometry exceptions

'''


class ZeroSlope(Exception):
    '''
    '''
    pass


class InfiniteSlope(Exception):
    '''
    '''
    pass


class CollinearPoints(Exception):
    '''
    '''
    pass


class CollinearLines(CollinearPoints):
    '''
    '''
    pass


class ParallelLines(Exception):
    '''
    '''
    pass


class InfiniteLength(Exception):
    '''
    '''
    pass


class ExceededEpsilonError(Exception):
    '''
    '''

    def __init(self, x, y, epsilon):
        self.x, self.y, self.epsilon = x, y, epsilon

    def __str__(self):
        fmt = 'x={} - y={} {} > epsilon {}'
        return fmt.format(self.x, self.y,
                          self.x - self.y,
                          self.epsilon)
