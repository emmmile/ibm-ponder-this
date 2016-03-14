#!/usr/bin/python

import math
import sys

def reverse(n, digits = 0):
    result = 0
    while digits > 0 or n > 0:
        result = result * 10 + n % 10
        n = n // 10
        digits -= 1

    return result

def check(x, y, digits):
    return y % (10 ** digits) == reverse(x)

if __name__ == "__main__":
    digits = int(sys.argv[1])

    for x in range(1, 10 ** digits):
        j = reverse(x * x % (10 ** digits), digits)

        c = j * (10 ** digits) + x
        if ( check(c, c * c, 2 * digits)):
            print(c, c * c)

        c = j // 10 * (10 ** digits) + x
        if ( check(c, c * c, 2 * digits - 1)):
            print(c, c * c)
