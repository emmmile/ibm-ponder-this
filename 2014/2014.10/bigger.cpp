// g++ october.cpp -o october -O3 -mtune=native -Wall -lgmp -lgmpxx
// http://stackoverflow.com/questions/21367824/how-to-evalute-an-exponential-tower-modulo-a-prime/21368784#21368784

#include <iostream>
#include <cmath>
#include <map>
#include <gmp.h>
#include <gmpxx.h>
#include <sstream>
using namespace std;

typedef unsigned long long ull;

mpz_class ull2mpz (ull a) {
  stringstream str;
  str << a;
  return mpz_class (str.str());
}


ull ipow(ull base, ull exp, ull mod) {
    mpz_class result;
    mpz_class b = ull2mpz(base);
    mpz_class m = ull2mpz(mod);
    mpz_powm_ui(result.get_mpz_t(), b.get_mpz_t(), exp, m.get_mpz_t());
    return result.get_ui();
}

ull ipow2(ull base, ull exp, ull mod) {
	ull result = 1;
    while (exp)
    {
        if (exp & 1)
            result = (result * base) % mod;
        exp >>= 1;
        base = (base * base) % mod;
    }

    return result;
}

// compute the cycle lenght of a^k mod m
pair<ull, ull> cycle ( ull a, ull m ) {
	map<ull, ull> results;

	ull k = 0;
	while ( true ) {
		ull current = ipow(a, k, m);
		if ( results.count(current) != 0 )
			return pair<ull, ull>(k - results[current], results[current]);

		//if ( k && k % 1000000 == 0 ) cout << "Computed " << k << " iterations..." << endl;
		if ( k < 100 ) results[current] = k;
		++k;
	}
}


int main ( ) {
	cout << "Cycle length for " << 2 << "^k and m = " << 10 << " is " << cycle(2, 10).first << endl;
	cout << "Cycle length for " << 2 << "^k and m = " << 100 << " is " << cycle(2, 100).first << endl;
	cout << 5 << "^" << 1000 << " mod 156250 is " << ipow2(5,100,156250) << endl;
	cout << 5 << "^" << 1001 << " mod 156250 is " << ipow2(5,100,156250) << endl;
	cout << 5 << "^" << 1002 << " mod 156250 is " << ipow2(5,100,156250) << endl;

	ull m = 10000000000L;
	for ( ull n = 2; n != 9; ++n ) {
		pair<ull,ull> res = cycle(n, m);
		cout << n << "^k mod " << m << " has cycle length of " << res.first << " starting from " << n << "^" << res.second << endl;
		m = res.first;
	}

	return 0;
}