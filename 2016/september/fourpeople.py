#!/usr/bin/python3

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
    for a, b in itertools.product(seats[:-1], [seats[-1]]):
        for ai, bi in zip(a, b):
            if ai is bi:
                count += 1
    return count

def seats():
    return ['abcd', 'abdc', 'acbd', 'acdb', 'adbc', 'adcb', 'bacd', 'badc',
            'bcad', 'bcda', 'bdac', 'bdca', 'cabd', 'cadb', 'cbad', 'cbda',
            'cdab', 'cdba', 'dabc', 'dacb', 'dbac', 'dbca', 'dcab', 'dcba']

def pairs():
    return [s for s in itertools.permutations(seats(), 2)]

def triples():
    return [s for s in itertools.permutations(seats(), 3)]

if __name__ == "__main__":
    # given a pair like ["abcd", "dcba"] or a triple,, how many violations
    # add the last element?
    cache = {}

    for i in pairs() + triples():
        cache[i] = violations(i)

    candidates = {(s,): 0 for s in seats()}
    length = 1
    while length < 24:
        new_candidates = {}
        for s in candidates.keys():
            for another in seats():
                if another not in s:
                    newone = s[-2:] + (another,)
                    if cache[newone] + candidates[s] < 2:
                        prefix = s[:-2]
                        new_candidates[prefix + newone] = cache[newone] + candidates[s]
        candidates = new_candidates
        length += 1
        print(length, len(new_candidates))

    for s in candidates.keys():
        print(candidates[s], s)

# ('adcb', 'dcba', 'badc', 'cdab', 'abcd', 'dabc', 'bcda', 'cbad', 'adbc', 'bacd', 'dcab', 'abdc', 'cdba', 'bcad', 'dacb', 'cbda', 'acbd', 'bdac', 'dbca', 'acdb', 'cabd', 'dbac', 'bdca', 'cadb')
