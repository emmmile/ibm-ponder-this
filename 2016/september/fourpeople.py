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
    # return count
    # count = 0
    # for a, b in pairwise(seats):
    #     for ai, bi in zip(a, b):
    #         if ai is bi:
    #             count += 1
    # return count

def seats():
    return ["".join(s) for s in list(itertools.permutations("abcd"))]

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
    while True:
        new_candidates = {}
        for s in candidates.keys():
            for another in seats():
                if another not in s:
                    newone = s[-2:] + (another,)
                    new_candidates[newone] = cache[newone] + candidates[s]
        candidates = new_candidates
        print(len(new_candidates))



    # seats = possible_seats()
    # print(seats)
    # best = 24 * 4
    # while True:
    #     random.shuffle(seats)
    #     candidate = [seats[0]]
    #     while len(candidate) < 24:
    #         minimum = 4
    #         another = None
    #         for s in seats:
    #             if s not in candidate and violations([candidate[-1], s]) < minimum:
    #                 another = s
    #         candidate.append(another)
    #
    #     if violations(candidate) <= best:
    #         best = violations(candidate)
    #         print(violations(candidate), candidate)
