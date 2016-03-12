#include <bitset>
#include <array>
#include <iostream>
#include <vector>
#include <gmpxx.h>
#include <limits>       // std::numeric_limits
using namespace std;

template<class T>
T reverse(T n, int digits ) {
  T result = 0;
  do {
    result *= 10;
    result += n % 10;
    n /= 10;
    digits--;
  } while( digits );

  return result;
}


bool check( mpz_class x, mpz_class y, int digits) {
    mpz_class h;
    mpz_ui_pow_ui (h.get_mpz_t(), 10, digits);
    return y % h == reverse(x, digits);
}

int main ( ) {
    cout << numeric_limits<uint128_t>::max() << endl;
    cout << numeric_limits<unsigned int>::max() << endl;
    int digits = 11;
    mpz_class h, l;
    mpz_ui_pow_ui (h.get_mpz_t(), 10, digits);
    mpz_ui_pow_ui (l.get_mpz_t(), 10, digits - 1);

    for (mpz_class i = 0; i < h; ++i ) {
        mpz_class j = reverse((i * i) % h, digits);

        mpz_class c = i + j * h;
        // cout << i << " " << j << " " << c << endl;
        if ( c > 1 && check(c, c * c, 2 * digits) )
           cout << i << " " << c << endl;

        c = i + (j - j % 10) * l;
        if ( c > 1 && check(c, c * c, 2 * digits - 1) )
           cout << i << " " << c << endl;
    }
}

// 15: 182921919071841
// 15: 655785969669834
// 18: 650700037578750084
// 21: 125631041500927357539
