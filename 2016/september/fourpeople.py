#!/usr/bin/python
# 4 ['cdba', 'abcd', 'bcda', 'cbad', 'adcb', 'bcad', 'badc', 'cdab', 'acbd', 'bdac', 'dcba', 'cadb', 'dbca', 'adbc', 'dacb', 'abdc', 'cabd', 'dcab', 'dabc', 'bacd', 'dbac', 'bdca', 'acdb', 'cbda']

import itertools
import math
import random
import sys

def pairwise(iterable):
    a, b = itertools.tee(iterable)
    next(b, None)
    return zip(a, b)

# given a list of seats give me the number of violations
# can be even a pair of seats, like ["abcd", "dcba"]
def violations(seats):
    count = 0
    for a, b in pairwise(seats):
        for ai, bi in zip(a, b):
            if ai is bi:
                count += 1
    return count

def possible_seats():
    return ["".join(s) for s in list(itertools.permutations("abcd"))]

if __name__ == "__main__":
    print(sorted(["cdba","abdc","bdca","cbad","adbc","cbda","bdac","acdb","bacd","cdab","bcda","dacb","acbd","dbac","cadb","abcd","dcab","badc","dcba","adcb","dabc","bcad","dbca","cabd"]))
    seats = possible_seats()
    print(seats)
    best = 24 * 4
    while True:
        random.shuffle(seats)
        candidate = [seats[0]]
        while len(candidate) < 24:
            minimum = 4
            another = None
            for s in seats:
                if s not in candidate and violations([candidate[-1], s]) < minimum:
                    another = s
            candidate.append(another)

        if violations(candidate) <= best:
            best = violations(candidate)
            print(violations(candidate), candidate)
