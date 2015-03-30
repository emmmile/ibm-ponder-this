#include <iostream>
using namespace std;

vector<uint> cache;


template<unsigned int A = 10, unsigned int B = 1>
uint optimal ( uint n ) {
    if ( n == 1 ) return 0;
    if ( cache[n] != 0 ) return cache[n];
    
    uint besttotalcost = std::numeric_limits<uint>::max();
    uint bestsplit = 0;

    for ( uint i = 0; i < n - 1; ++i ) {
        uint total = optimal(i+1) + optimal(n-i-1) + A * (i+1) + B * (n-i-1);

        if ( total < besttotalcost ) {
            besttotalcost = total; // I need where the split was, i.e. i?
            bestsplit = i;
        }
    }

    printf( "%d %d %d", n, besttotalcost, bestsplit );
    getchar();
    cache[n] = besttotalcost;
    return besttotalcost;
}

int main ( ) {
    cache.resize( 200 );

    for ( int i = 1; i < 100; ++i ) optimal(i);

    uint best = optimal(100);


    printf( "best is %.2f\n", best / float(100) );

    return 0;
}