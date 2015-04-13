#include <array>
#include <random>
#include <cassert>
#include <vector>
#include <set>
#include <bitset>
#include <iostream>
#include <algorithm>    // std::random_shuffle
#include <valarray>
#include "vect.hpp"
using namespace std;

#define X 13

template<size_t N>
struct cube {
    // 0 is peter pan
    // 1 is wendy
	bitset<N * N * N> cells;
    static_assert(N % 2, "N must be odd.");
    static const int D = N / 2;
    static const set<vect> hooks;
    static const set<vect> directions;
        
    static set<vect> generate_surface () {
        set<vect> surface;
        int s = D + 1;
        for ( int x = -s; x <= +s; ++x ) {
            for ( int y = -s; y <= +s; ++y ) {
                for ( int z = -s; z <= +s; ++z ) {
                    if ( max(abs(x), max(abs(y), abs(z))) == s )
                        surface.insert(vect{x, y, z});
                }
            }
        }

        return surface;
    }

    // for any index in [0, N * N * N) return the number of unsolved paths
    // if the array = 0 the solution is valid, i.e. captain hook is losing
    array<int, N * N * N> check ( ) {
        array<int, N * N * N> unsolved;
        fill(unsolved.begin(), unsolved.end(), 0);

        // loop over hooks and generate the paths using directions
        for ( auto hook : hooks ) {
            for ( vect d : directions ) {
                check_path(hook, d, unsolved);
            }
        }

        return unsolved;
    }

    // starts from a hook and walks in a given direction, inside the cube
    // if it founds a wendy, exits
    // otherwise mark the path as unsolved
    void check_path( const vect& hook, const vect& direction, array<int, N * N * N>& unsolved ) {
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

    size_t index ( const vect& v ) {
        // XXX any reasonable mapping shold be OK, you can rotate the cube as you want
        vect t = v + vect{D, D, D};
        int index = t[0] + N * t[2] + N * N * t[1];

        assert(index >= 0);
        assert(index < int(N * N * N));
        return index;
    }

    bool inside ( const vect& v ) {
        return abs(v[0]) <= D && abs(v[1]) <= D && abs(v[2]) <= D;
    }

    cube ( ) {
        // TODO until not solved
        check();
    }

	friend ostream& operator << ( ostream& stream, cube& c ) {
        for ( unsigned int i = 0; i < N * N * N; ++i )
            stream << (c.cells[i] ? "W" : "P");
		
		return stream;
	}
};

template<>
const set<vect> cube<X>::hooks = cube<X>::generate_surface();

template<>
const set<vect> cube<X>::directions = cube<1>::generate_surface();

int main ( ) {
	cube<X> c;

    cout << c << endl;
}
