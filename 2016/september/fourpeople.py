#!/usr/bin/python3

import itertools

# how many violations does the last seat add?
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

# generates the solutions one by one
def loop(candidate, suffixes, cost):
    if len(candidate) == 24:
        yield cost, candidate
    else:
        for s in singles():
            if s not in candidate:
                newone = candidate[-2:] + (s,)
                newcost = suffixes[newone] + cost
                # go on with the ones that has no more than this violations
                if newcost < 2:
                    for i in loop(candidate + (s,), suffixes, newcost):
                        yield i

if __name__ == "__main__":
    suffixes = {}
    for i in pairs() + triples():
        suffixes[i] = violations(i)

    # chain the generators from any starting point
    solutions = itertools.chain(*[loop((s,), suffixes, 0) for s in singles()])

    for violations, seats in solutions:
        print(violations, seats)
