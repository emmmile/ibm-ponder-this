import math

def check(x, y, digits):
    return y % (10 ** digits) == reverse(x, digits)

def reverse(n, digits):
    result = 0
    while digits:
        result *= 10
        result += n % 10
        n = math.floor(n/10)
        digits -= 1

    return result

if __name__ == "__main__":
    digits = 7

    for i in range(1, 10 ** digits):
        j = i * i % (10 ** digits)
        j = reverse(j, digits)

        c = i + j * (10 ** digits)
        if ( check(c, c * c, 2 * digits)):
            print(c)

        c = i + (j - j % 10) * (10 ** (digits - 1))
        if ( check(c, c * c, 2 * digits - 1)):
            print(c)
