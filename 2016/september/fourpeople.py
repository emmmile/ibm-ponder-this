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

def seats():
    return [''.join(s) for s in itertools.permutations('ABCD')]

def pairs():
    return [s for s in itertools.permutations(seats(), 2)]

def triples():
    return [s for s in itertools.permutations(seats(), 3)]

def loop(candidate, suffixes, cost):
    if len(candidate) == 24:
        yield cost, candidate
    else:
        for s in seats():
            if s not in candidate:
                newone = candidate[-2:] + (s,)
                newcost = suffixes[newone] + cost
                if newcost < 1:
                    for i in loop(candidate + (s,), suffixes, newcost):
                        yield i

if __name__ == "__main__":
    # given a pair like ['abcd', 'dcba'] or a triple, how many violations
    # the last element contributes to?
    suffixes = {}
    for i in pairs() + triples():
        suffixes[i] = violations(i)

    # chain the generators from every starting seat
    generator = itertools.chain(*[loop((s,), suffixes, 0) for s in seats()])

    # print the solutions
    for cost, seat in generator:
        print(cost, seat)
