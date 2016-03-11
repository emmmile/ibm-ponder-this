import math

def naive(start):
    modulus = 10
    while start >= 0:
        if start > modulus:
            modulus *= 10
        if check(start, start**2, modulus):
            print(start, start**2)
        start += 1

def check(x, y, modulus):
    return y % modulus == reverse(x)
    #return str(y).endswith(str(x)[::-1])

def reverse(n):
    result = 0
    while n > 0:
        result *= 10
        result += n % 10
        n = math.floor(n/10)

    return result

if __name__ == "__main__":
    print(reverse(1231239128))
    naive(0)