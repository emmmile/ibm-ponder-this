import math

def naive(start):
    while start >= 0:
        if check(start, start**2):
            print(start, start**2)
        start += 1

def check(x, y):
    return str(y).endswith(str(x)[::-1])
