#! /usr/bin/env python3

import bigfloat
import itertools
import sys

# this is not so elegant but it's not easy to get the nth digit of an irrational number
# so I made an iterator tha get the nth digit *after* having computed the whole number
def digits(n):
    string = str(n).replace('.', '')
    for i in string:
        yield i

# move a n-digit window over the number
def slider(n, iterable):
    group = ''
    index = 0
    for i in iterable:
        group += i
        index += 1
        if len(group) == n:
            yield group, index
            # discard front digit
            group = group[1:]

def alldigits(n, iterable):
    pairs = sorted([(v, i) for v, i in slider(n, digits(iterable))])
    position = dict()
    for v, i in pairs:
        if v not in position:
            position[v] = i
    return position

if __name__ == "__main__":
    context = bigfloat.Context(precision=int(sys.argv[1]))
    bignums = [
        bigfloat.const_pi(context),
        bigfloat.exp(1, context),
        bigfloat.sqrt(2, context)
    ]

    # compute a map digits -> position for each of the big numbers, where position
    # is the first one
    n = 5
    alldigits = [alldigits(n, x) for x in bignums]
    # get the intersection of the printed digits
    intersection = set.intersection(*[set(m.keys()) for m in alldigits])
    print("The intersection of the {}-digits strings is made of {} items".format(n, len(intersection)))
    # after how many seconds each string in the intersection is printed
    seconds = [(sum([d[i] for d in alldigits]), i) for i in intersection]
    for s, n in sorted(seconds)[:10]:
        print("{} is printed after {} seconds".format(n, s))
