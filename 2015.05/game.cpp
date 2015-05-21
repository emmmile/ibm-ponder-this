#include <iostream>
#include <algorithm>
using namespace std;

union conf {
    uint16_t c[4];
    uint64_t aggregate;

    conf(uint64_t n) : aggregate(n) {
        sort(c, c + 3);
    }

    conf(uint16_t x, uint16_t y, uint16_t z) {
        c[0] = x;
        c[1] = y;
        c[2] = z;
        c[3] = 0;
        sort(c, c + 3);
    }

    bool valid ( ) const {
        return c[0] != 0 && ( c[0] != c[1] && c[1] != c[2]);
    }

    friend ostream& operator << ( ostream& o, const conf& c ) {
        return o << c.c[0] << " " << c.c[1] << " " << c.c[2];
    }
};

int simulate ( conf c, int minutes, int limit ) {
    if ( ! c.valid() ) return minutes;
    if ( minutes == limit ) return limit;

    int x = simulate( conf(c.c[0], c.c[1] * 2, c.c[2] - c.c[1]), minutes + 5, limit );
    int y = simulate( conf(c.c[0] * 2, c.c[1], c.c[2] - c.c[0]), minutes + 5, limit );
    int z = simulate( conf(c.c[0] * 2, c.c[1] - c.c[0], c.c[2]), minutes + 5, limit );

    return min(x, min(y, z));
}

int main ( ) {
    vector<int> results;
    vector<uint64_t> all;
    for ( int x = 175; x <= 255; ++x )
        for ( int y = x + 1; y <= 255; ++y )
            for ( int z = y + 1; z <= 255; ++z )
                all.push_back(conf(x,y,z).aggregate);

    for ( int limit = 25; limit < 60; limit += 5) {
        results.clear();

        for ( auto i : all )
            results.push_back( simulate(conf(i), 0, limit) );

        for ( uint i = 0; i < results.size(); ++i )
            if ( results[i] < limit )
                all[i] = 0;

        all.erase(remove(all.begin(), all.end(), 0), all.end());
    }

    for ( auto i : all ) cout << conf(i) << endl;

    return 0;
}
