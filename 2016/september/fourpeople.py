#!/usr/bin/python3

import itertools

def violations(seats):
    count = 0
    for a, b in itertools.product(seats[:-1], [seats[-1]]):
        for ai, bi in zip(a, b):
            if ai is bi:
                count += 1
    return count

def singles():
    return [''.join(s) for s in itertools.permutations('ABCD')]

def pairs():
    return [s for s in itertools.permutations(singles(), 2)]

def triples():
    return [s for s in itertools.permutations(singles(), 3)]

def loop(candidate, suffixes, cost):
    if len(candidate) == 24:
        yield cost, candidate
    else:
        for s in singles():
            if s not in candidate:
                suffix = candidate[-2:] + (s,)
                newcost = suffixes[suffix] + cost
                if newcost < 2:
                    newone = candidate + (s,)
                    for i in loop(newone, suffixes, newcost):
                        yield i

if __name__ == "__main__":
    suffixes = {}
    for i in pairs() + triples():
        suffixes[i] = violations(i)

    generators = [loop((s,), suffixes, 0) for s in singles()]
    solutions = itertools.chain(*generators)
    print(next(solutions))
