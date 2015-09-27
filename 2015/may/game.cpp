#include <iostream>
#include <algorithm>
using namespace std;

struct state {
    uint16_t c[3];

    state( uint16_t x, uint16_t y, uint16_t z ) {
        c[0] = x;
        c[1] = y;
        c[2] = z;
        sort(c, c + 3);
    }

    bool valid ( ) const {
        return c[0] != 0 && ( c[0] != c[1] && c[1] != c[2]);
    }

    uint16_t operator [] ( const int i ) const { 
        return c[i]; 
    }

    bool operator == ( const state& another ) const {
        return equal(c, c + 3, another.c);
    }

    friend ostream& operator << ( ostream& o, const state& c ) {
        return o << c.c[0] << " " << c.c[1] << " " << c.c[2];
    }
};

int simulate ( state c, int minutes, int limit ) {
    if ( ! c.valid() ) return minutes;
    if ( minutes == limit ) return limit;

    int x = simulate( state(c[0], c[1] * 2, c[2] - c[1]), minutes + 5, limit );
    int y = simulate( state(c[0] * 2, c[1], c[2] - c[0]), minutes + 5, limit );
    int z = simulate( state(c[0] * 2, c[1] - c[0], c[2]), minutes + 5, limit );

    return min(x, min(y, z));
}

int main ( ) {
    vector<int> results;
    vector<state> all;

    for ( int x = 1; x <= 255; ++x )
        for ( int y = x + 1; y <= 255; ++y )
            for ( int z = y + 1; z <= 255; ++z )
                all.push_back(state(x, y, z));

    for ( int limit = 10; limit < 60; limit += 5) {
        results.clear();

        for ( auto i : all )
            results.push_back( simulate(state(i), 0, limit) );

        for ( uint i = 0; i < results.size(); ++i )
            if ( results[i] < limit )
                all[i] = state(0, 0, 0);

        all.erase(remove(all.begin(), all.end(), state(0, 0, 0)), all.end());
        cout << limit << " " << all.size() << endl;
    }

    for ( auto i : all ) cout << state(i) << endl;
}
