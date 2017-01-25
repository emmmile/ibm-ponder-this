#! /usr/bin/env python

import numpy as np
import itertools as it
from fractions import Fraction

# generate all rational numbers as fractions in [start, end) with steps of
# 1/precision
def rationals(start, end, precision):
    end = precision * (end - start)
    for i in range(start, end):
        simplified = Fraction(i, precision)
        yield simplified.numerator, simplified.denominator

# generate all good digit configurations
def goodx(prefix):
    strings = [str(prefix) + '.' + ''.join(x) for x in it.permutations('123')]
    for x in strings:
        simplified = Fraction(x)
        yield simplified.numerator, simplified.denominator

# test if the two fractions x = a/b and y = c/d satisfy x^y = y^x
def test(a, b, c, d):
    left  = a ** (b * c) * d ** (a * d)
    right = b ** (b * c) * c ** (a * d)
    return left == right

def main():
    precision = 100
    limit = 5
    for x in goodx(1):
        for y in rationals(0, limit, precision):
            a, b = x
            c, d = y
            if x != y and test(a, b, c, d):
                print(x, y)


if __name__ == '__main__':
    main()
