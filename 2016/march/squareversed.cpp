#include <bitset>
#include <array>
#include <iostream>
#include <vector>
using namespace std;

#define DIGITS 16


long long reverse(long long n) {
  long long result = 0;
  do {
    result *= 10;
    result += n % 10;
    n /= 10;
  } while( n );

  return result;
}


bool check( long long x, long long y, long long modulus) {
    return y % modulus == reverse(x);
}

void naive( long long start) {
    long long modulus = 10;

    while(start >= 0) {
        if (start > modulus)
            modulus *= 10;

        if ( check(start, start * start, modulus) )
            cout << start << " " << start * start << endl;

        start += 1;
    }
}

int main ( ) {
  cout << reverse(123918231298301230L) << endl;

  naive(0);
}
