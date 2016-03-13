#include <iostream>
#include <vector>
#include <gmpxx.h>
#include <sstream>
#include <sys/types.h>
using namespace std;

// g++ squareversed.cpp -o squarereversed -O3 -std=c++11 -ffast-math -lgmpxx -lgmp -mtune=native -Wall

typedef __uint128_t utype;
vector<utype> pows;

ostream& operator<< (ostream& stream, const __uint128_t& x) {
    stringstream s;
    __uint128_t n = x;

    while (n != 0) {
        s << (int) (n % 10);
        n /= 10;
    }

    string str = s.str();
    reverse(str.begin(), str.end());

    return stream << str;
}


utype reverse(utype n, int digits ) {
    utype result = 0;
    do {
        result *= 10;
        result += n % 10;
        n /= 10;
        digits--;
    } while( digits );

    return result;
}

utype ipow ( utype b, utype e ) {
    utype result = 1;
    while ( e ) {
        result *= b;
        e--;
    }

    return result;
}

bool check( utype x, utype y, int digits) {
    return y % pows[digits] == reverse(x, digits);
}

int main ( int argc, char** argv ) {
    cout << "Can solve the problem up to " << (int) log10(pow(2, 64) - 1) << " digits." << endl;

    for ( int i = 0; i < log10(pow(2, 128) - 1); ++i ) {
        pows.emplace_back(ipow(10, i));
    }

    int digits = stoi(argv[1]);
    utype h = pows[digits];
    for (utype i = h - 1; i > 0; --i ) {
        utype j = reverse((i * i) % h, digits);

        utype c = i + j * h;
        if ( c > 1 && check(c, c * c, 2 * digits) )
            cout << i << " " << c << endl;

        j /= 10;
        c = i + j * h;
        if ( c > 1 && check(c, c * c, 2 * digits - 1) )
            cout << i << " " << c << endl;
    }
}

// 15: 182921919071841
// 15: 655785969669834
// 18: 650700037578750084
// 19: nothing
// 20: nothing
// 21: 125631041500927357539
