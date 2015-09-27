// g++ september.cpp -o september -O3 -lgmp -lgmpxx -lmpfr

#include <iostream>
#include <gmpxx.h>
#include <mpfr.h>
#include <cmath>
using namespace std;


int main ( ) {
	mpfr_t c;
	mpfr_t e;
	mpfr_t r;


	mpz_class big;
	mpz_ui_pow_ui (big.get_mpz_t(), 3, 1024);

	mpfr_init2 (c, 2000);
	mpfr_init2 (e, 2000);
	mpfr_init2 (r, 2000);
	mpfr_set_d (c, 1.0, MPFR_RNDD);
	mpfr_set_z (r, big.get_mpz_t(), MPFR_RNDD);


	mpfr_exp(e, c, MPFR_RNDZ);
	//mpfr_out_str (stdout, 10, 0, e, MPFR_RNDD);
	//cout << endl;

	mpfr_log2(c, e, MPFR_RNDZ);
	//mpfr_out_str (stdout, 10, 0, c, MPFR_RNDD);
	//cout << endl;

	mpfr_div (r, r, c, MPFR_RNDZ);
	mpfr_frac(r, r, MPFR_RNDZ);
	mpfr_exp(r, r, MPFR_RNDZ);
	mpfr_printf ("%.20Rf", r);
	//mpfr_out_str (stdout, 10, 0, r, MPFR_RNDD);
	cout << endl;


	/*mpz_class a = 1234;
	mpf_class b(1.0, 100000);

	mpz_ui_pow_ui (a.get_mpz_t(), 3, 1024);
	b = a / (double) log2l(M_El);

	cout << b << endl;
	gmp_printf("%.100Ff\n", b.get_mpf_t());  */
	return 0;
}