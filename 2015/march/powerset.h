#ifndef POWERSET_H
#define POWERSET_H

#include <bitset>
#include <vector>
using namespace std;



/* I want to scan the powerset in order of the number of bits in the representation.
 * Therefore I compute it here once for all.
 */
template<size_t Y = 10>
class powerset {
	array<vector<size_t>, Y> order;
public:
	powerset ( ) {
		for ( size_t i = 0; i < Y; ++i ) {
			order[i] = vector<size_t>(1u << i);

			size_t n = 0;
    		generate(order[i].begin(), order[i].end(), [&]{ return n++; });

			// an easily readable lambda
			sort(order[i].begin(),  order[i].end(), [](const size_t& a, const size_t& b) {
				return __builtin_popcount(a) == __builtin_popcount(b) ? a > b : 
					   __builtin_popcount(a) > __builtin_popcount(b);
			});
		}
	}

	const vector<size_t>& operator [] ( size_t index ) const {
		return order[index];
	}

	powerset inverse ( ) const {
		powerset newone = *this;

		for ( size_t i = 0; i < Y; ++i )
			reverse(newone.order[i].begin(), newone.order[i].end());

		return newone;
	}
};

#endif