
#include <algorithm>
#include <iostream>
#include <map>
#include <set>
#include <unordered_set>
#include <queue>
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

    bool operator < ( const state& another ) const {
        return lexicographical_compare(c, c + 3, another.c, another.c + 3);
    }

    bool operator == ( const state& another ) const {
        return equal(c, c + 3, another.c);
    }

    friend ostream& operator << ( ostream& o, const state& c ) {
        return o << "(" << c.c[0] << ", " << c.c[1] << ", " << c.c[2] << ")";
    }
};

int main ( ) {
    typedef pair<state, state> edge;
    vector<state> initial;
    set<edge> edges;
    set<state> visited;
    queue<state> all;

    for ( int x = 1; x <= 255; ++x )
        for ( int y = x + 1; y <= 255; ++y )
            for ( int z = y + 1; z <= 255; ++z ) {
                initial.push_back(state(x, y, z));
                all.push(state(x, y, z));
            }

    while ( all.size() ) {
        state c = all.front();
        all.pop();

        if ( ! c.valid() ) continue;
        if ( visited.count(c) ) continue;

        state x(c[0], c[1] * 2, c[2] - c[1]);
        state y(c[0] * 2, c[1], c[2] - c[0]);
        state z(c[0] * 2, c[1] - c[0], c[2]);
        all.push(x);
        all.push(y);
        all.push(z);
        edges.insert({{x, c}, {y, c}, {z, c}});
        //edges.insert({{c,x}, {c,y}, {c,z}});
        visited.insert(c);
    }

    for ( auto e : edges ) cout << e.first << " -> " << e.second << endl;
    cout << edges.size() << " total edges." << endl;
}
