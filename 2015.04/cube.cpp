#include <array>
#include <random>
#include <cassert>
#include <vector>
#include <cstring>
#include <set>
#include <bitset>
#include <iostream>
#include <algorithm>    // std::random_shuffle
#include "vect.hpp"
using namespace std;

#define X 7
#define P 172

template<size_t N>
struct cube {
    // 0 is peter pan
    // 1 is wendy
	bitset<N * N * N> cells;
    array<int, N * N * N> unsolved;
    static_assert(N % 2, "N must be odd.");
    static const int D = N / 2;
    static const vector<vect> hooks;
    static const vector<vect> directions;

    
    static vector<vect> generate_surface () {
        vector<vect> surface;
        int s = D + 1;
        for ( int x = -s; x <= +s; ++x ) {
            for ( int y = -s; y <= +s; ++y ) {
                for ( int z = -s; z <= +s; ++z ) {
                    if ( max(abs(x), max(abs(y), abs(z))) == s )
                        surface.push_back(vect{x, y, z});
                }
            }
        }

        //cout << "Generated surface of a " << 2 * s - 1 << " cube." << endl;
        return surface;
    }


    bool valid ( ) {
        check();
        auto maximum = max_element(unsolved.begin(), unsolved.end());
        return *maximum == 0;
    }

    // for any index in [0, N * N * N) return the number of unsolved paths
    // if the array = 0 the solution is valid, i.e. captain hook is losing
    void check ( ) {
        std::fill(unsolved.begin(), unsolved.end(), 0);

        // loop over hooks and generate the paths using directions
        for ( auto hook : hooks ) {
            for ( vect d : directions ) {
                check_path(hook, d);
            }
        }
    }

    // starts from a hook and walks in a given direction, inside the cube
    // if it founds a wendy, exits
    // otherwise mark the path as unsolved
    void check_path( const vect& hook, const vect& direction ) {
        vect current = hook;
        while( inside(current + direction) ) {
            current = current + direction;
            //cout << current << endl;
            if ( cells[index(current)] ) return;
        }

        // if reaches here it means it is empty or unsolved
        current = hook;
        while( inside(current + direction) ) {
            current = current + direction;
            unsolved[index(current)]++;
        }
    }

    size_t index ( const vect& v ) const {
        // XXX any reasonable mapping shold be OK, you can rotate the cube as you want
        vect t = v + vect{D, D, D};
        int index = t[0] + N * t[2] + N * N * t[1];

        assert(index >= 0);
        assert(index < int(N * N * N));
        return index;
    }

    bool inside ( const vect& v ) const {
        return abs(v[0]) <= D && abs(v[1]) <= D && abs(v[2]) <= D;
    }

    cube ( ) : cube(
        "WWWWWWWWWWWWWWWPPPPPWWPPPPPWWWWWWWWWPPPPPWWWWWWWW"
        "WPPPPPWWPPPPPWWPPPPPWWPPPPPWWPPPPPWWPPPPPWWPPPPPW"
        "WPPPPPWWPPPPPWWPPPPPWWPPPPPPPPPPPPWWPPPPPWWPPPPPW"
        "WPPPPPWWPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPWWPPPPPW"
        "WPPPPPWWPPPPPWWPPPPPWWPPPPPPPPPPPPWWPPPPPWWPPPPPW"
        "WPPPPPWWPPPPPWWPPPPPWWPPPPPWWPPPPPWWPPPPPWWPPPPPW"
        "WWWWWWWWPPPPPWWWWWWWWWPPPPPWWPPPPPWWWWWWWWWWWWWWW") {
        fill();

        //assert(valid());
    }

    cube ( const char* s ) {
        assert( strlen(s) == cells.size() );
        for ( size_t i = 0; i < cells.size(); ++i ) {
            assert ( s[i] == 'W' || s[i] == 'P' );
            if ( s[i] == 'W' ) cells[i] = true;
        }
    }

    void fill ( ) {
        random_device rd;
        mt19937 gen(rd());
        uniform_real_distribution<> dis;

        do {
            check();
            auto maximum = max_element(unsolved.begin(), unsolved.end());
            if ( *maximum == 0 ) break;

            // select at random one of these cells, knuth method
            int count = 1;
            size_t selected;
            for ( size_t i = 0; i < unsolved.size(); ++i ) {
                if ( unsolved[i] == *maximum && dis(gen) < 1.0 / (double) count ) {
                    count++;
                    selected = i;
                }
            }

            cells[selected] = true;
        } while ( true );
    }

    size_t wendys ( ) const {
        return cells.count();
    }

    size_t peters ( ) const {
        return cells.size() - wendys();
    }

	friend ostream& operator << ( ostream& stream, cube& c ) {
        for ( unsigned int i = 0; i < N * N * N; ++i ) {
            if ( i && i % (N * N) == 0 ) stream << endl;
            stream << (c.cells[i] ? "W" : "P");
        }

		stream << endl;
        stream << c.wendys() << " wendys" << endl;
        stream << c.peters() << " peter pans" << endl;
        stream << (c.valid() ? "valid" : "not valid") << endl;
		return stream;
	}
};

template<>
const vector<vect> cube<X>::hooks = cube<X>::generate_surface();

template<>
const vector<vect> cube<X>::directions = cube<1>::generate_surface();


int main ( ) {
    assert(cube<X>::directions.size() == 26);
    long long visited = 0;
    cube<X> c;

    while ( c.peters() < P ) {
        ++visited;
        c = cube<X>();
    }

    cout << c << endl;
    cout << visited << " visited" << endl;
}


// 161 
// WWWWWWWWWPPWWWWWWPPWWWWPPWWWWWWWPPWWWPWWWWWWWWWWW
// WWWWWWWPPWPPPPWPPPPPPPPPWWPWPPPPPPWPPPPWPWWWWWPWW
// WWWWWWWWPPPPPPPPPPPWWWWPPPPPPPPPPWWWPPPPPPWWWPPWW
// WWWWWWWPPPPPPWWPWPPPPPPPPPPWPPPPPPWWPPPPPPWWWWPWW
// WWWWPPWPPPPPWWWPPPWPPPPPPPPWWWPPPPWPPPWPWWWWWWPWW
// WWPWPPWWPPPPPWWPPPPPWWPWPPPWWPPPWPWWPPPPPWWWWWWWW
// WWWWWWWWWWWWWWWPWWWWWWPWPPPWWPWPWPWWPWPPPWWWWWWWW

// 165 
// WWWWWWWWPPPWPWWPWPWPWWPPPWPWWWWWWPWWWWWWWWWWWWWWW
// WWWWWWWWPPPPPWWPWPPPWWPPPWPWWPPPPPWWPPPPPWWPPPPWW
// WWPWWWWWWPWPPPWPPPPWPWPPPPPPPPPPPPWWWPPPPPWPWWPWW
// WWPPWWWPPPPPPWWPPPPWPWPPPPPPPPPPWPWWPPPPPPWPWWWWW
// WWPPWPWPPPPPPWWPPPPPPWPPPPPWPPPPPPWWWPPPPWWWWWWWW
// WWPPWPWPPWPPPWWPPPPPPWWPPPWWWPPWPPPWPPPPPPWPPWWWW
// WWWWWWWWWWWWWWWWPWWWWWPWWPWWWPWWPWWWWWWPWWWWWWWWW

// 168 
// WWWWWWWWWWWWWWWPPPPPWWPPPPPWWWWWWWWWPPPPPWWWWWWWW
// WPPPPPWWPPPPPWWPPPPPWWPPPPPWWPPWPPWWPPPPPWWWWWWWW
// WWWWWWWWPPWPPWWPPPPPWWPPPPPWWPPPPPWWPPPPPWWPPPPPW
// WWWPWWWWPPPPPWWPPPPPWWPPWPPWWPPPPPWWPPPPPWWWWPWWW
// WPPPPPWWPPPPPWWPPPPPWWPPPPPWWPPPPPWWPPWPPWWWWWWWW
// WWWWWWWWPPPPPWWPPWPPWWWWPWWWWPPPPPWWPPPPPWWPPPPPW
// WWWWWWWWPPPPPWWWWWWWWWPPPPPWWPPPPPWWWWWWWWWWWWWWW

// 171
// WWWWWWWWWWWWWWWPPPPPWWPPPPPWWWWWWWWWPPPPPWWWWWWWW
// WPPPPPWWPPPPPWWPPPPPWWPWPWPWWPPPPPWWPPPPPWWWWWWWW
// WWWWWWWWPPPPPWWPPPPPWWPPPPPWWPPPPPWWPPPPPWWPPPPPW
// WWWWWWWWPPPPPWWPPPPPWPPPWPPPPPPPPPWWPPPPPWWWWWWWW
// WPPPPPWWPPPPPWWPPPPPWWWPPPWWWPPPPPWWWPPPPWWWWWWWW
// WWWWWWWWPPPPPWWPPPPPWWPPPPPWWPPPPPWWPPPPPWWPPPPPW
// WWWWWWWWPPPPPWWWWWWWWWPPPPPWWPPWPPWWWWWWWWWWWWWWW
// 172 wendys
// 171 peter pans
// valid

// 172
// WWWWWWWWWWWWWWWPPPPPWWPPPPPWWWWWWWWWPPPPPWWWWWWWW
// WPPPPPWWPPPPPWWPPPPPWWPWPPPWWPPPPPWWPPPPPWWWWWWWW
// WWWWWWWWPPPPPWWPPPPPWWPPPPWWWPPPPPWWPPPPPWWPPPPPW
// WWWWWWWWPPPPPWWPPPPPWPPPWPPPWPPPPPWWPPPPPWWWWWWWW
// WPPPPPWWPPPPPWWPPPPPWWWPPPPWWPPPPPWWPPPPPWWWWWWWW
// WWWWWWWWPPPPPWWPPPPPWWPPPWPWWPPPPPWWPPPPPWWPPPPPW
// WWWWWWWWPPPPPWWWWWWWWWPPPPPWWPPPPPWWWWWWWWWWWWWWW
// 171 wendys
// 172 peter pans
// valid

// 172
// WWWWWWWWWWWWWWWPPPPPWWPPPPPWWWWWWWWWPPPPPWWWWWWWW
// WPPPPPWWPPPPPWWPPPPPWWPPPPPWWPPPPPWWPPPPPWWWWWWWW
// WWWWWWWWPPPPPWWPPPPPWWWPPPWWWPPPPPWWPPPPPWWPPPPPW
// WWWWWWWWPPPPPWWPPPPPWPPPWPPPWPPPPPWWPPPPPWWWWWWWW
// WPPPPPWWPPPPPWWPPPPPWWPPPPPWWPPPPPWWPPPPPWWWWWWWW
// WWWWWWWWPPPPPWWPPPPPWWPWPWPWWPPPPPWWPPPPPWWPPPPPW
// WWWWWWWWPPPPPWWWWWWWWWPPPPPWWPPPPPWWWWWWWWWWWWWWW
// 171 wendys
// 172 peter pans
// valid
