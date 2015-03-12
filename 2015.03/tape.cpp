
#include <array>
#include <cassert>
#include <iomanip>
#include <set>
#include <iostream>
#include <iterator>
#include <bitset>
#include <vector>
#include <unordered_map>
#include <map>
#include <algorithm>
#include <powerset.h>
using namespace std;

size_t level = 0;


/* X is the number of bits
 * N is the number of symbols
 * K is number of bits out of X to use initially
 */

template<size_t X, size_t N = 26 size_t K = 2>
class tape {
	typedef unsigned short int conf_t;

	static const size_t M = 1 << X;
	static const powerset<> powers;

	inline bool greater ( const conf_t& a, const conf_t& b ) const {
		return ( (a & b) == a || (a & b) == b ) && 				// the 1 bits must be in the same position
				count(a) >= count(b);	// count the bits
	}

	inline size_t count ( const conf_t& n ) const {
		return bitset<X>(n).count();
	}

	inline size_t used ( ) const {
		return M - std::count(taken.begin(), taken.end(), 0);
	}

	inline size_t different ( ) const {
		return (*max_element(taken.begin(), taken.end()) - 'a') + 1;
	}

public:
	array<char, M> taken;
	array<conf_t, N> lower; // these are indexed by the char

	tape ( ) {
		fill(taken.begin(), taken.end(), 0);

		init();
		if ( generate_greather() ) {
			//assert(check());
			cerr << *this << endl;
			getchar();
		}
	}

	bool check ( ) {
		bitset<N> letters;

		// I must find an entry that is smaller than any other, for every letter
		for ( conf_t ientry : lower ) {
			bitset<N> current;

			assert(taken[ientry] != 0);
			for ( size_t jentry = 0; jentry < M; ++jentry ) {
				if ( taken[jentry] != 0 && greater(jentry, ientry) ) {
					//cout << "It's possible to write " << taken[jentry] << " over " << taken[ientry] << endl;
					current.set(taken[jentry] - 'a');
				}
			}

			if( current.count() == different()) {
				letters.set(taken[ientry] - 'a');
			}
		}
		
		//cout << letters << endl;
		return letters.count() == different();
	}

	// loop over the encodings that are greater than current
	conf_t find_greater( const conf_t& current ) {
		bitset<X> bits(current);
		size_t o = bits.count();
		size_t z = X - o;

		conf_t mask = M - 1;
		size_t iterations = z ? (1 << z) : 0;
		//cout << bits << " " << o << " " << z << " " << iterations << endl;
		for ( int i = iterations - 1; i >= 0; --i ) {
			conf_t newone = powers[z][i];
			for ( size_t p = 0; p < X; ++p ) {
				if ( bits[p] ) {
					// move the bits in the right place
					newone = (newone & ~(mask << p)) | (newone & (mask << p)) << 1;
				}
			}

			newone = newone | current;
			//cout << bitset<X>(newone) << endl;
			//getchar();
			if ( taken[newone] == 0 ) return newone;
		}

		return M;
	}

	conf_t find_greater_inverse( const conf_t& current ) {
		bitset<X> bits(current);
		size_t o = bits.count();
		size_t z = X - o;

		conf_t mask = M - 1;
		size_t iterations = z ? (1 << z) : 0;
		//cout << bits << " " << o << " " << z << " " << iterations << endl;
		for ( size_t i = 0; i < iterations; ++i ) {
			conf_t newone = powers[z][i];
			for ( size_t p = 0; p < X; ++p ) {
				if ( bits[p] ) {
					// move the bits in the right place
					newone = (newone & ~(mask << p)) | (newone & (mask << p)) << 1;
				}
			}

			newone = newone | current;
			//cout << bitset<X>(newone) << endl;
			//getchar();
			if ( taken[newone] == 0 ) return newone;
		}

		return M;
	}

	// find all upper representations of a letter
	conf_t find_upper ( char letter ) {
		assert(letter >= 'a');
		assert(letter <= char('a' + N));

		//for ( auto it = lower.begin(); it != lower.end(); it++ ) cout << bitset<X>(*it) << " is the lower representation of letter " << char('a' + (it - lower.begin())) << endl;

		size_t rsize = 0;
		size_t isize = 0;
		conf_t representations [M];
		conf_t incomplete 	   [M];
		if ( level ) cout << "Letter " << letter << ":" << endl;

		// search for representations of letter
		for ( size_t i = 0; i < M; ++i ) {
			if ( taken[i] == letter ) {
				representations[rsize++] = i;
				if ( level ) cout << "  Represented with " << bitset<X>(i) << (lower[letter - 'a'] == i ? " #" : "") << endl;
			}
		}

		// search for lower representation that cannot be overwritten by letter (looking at the previous representations)
		for ( size_t i = 0; i < N; ++i ) {
			bool ok = char(i + 'a') == letter; // if the letter is the same I don't need to write it again
			for ( auto it = representations; it != representations + rsize && !ok; ++it ) {
				if ( greater(*it, lower[i]) ) ok = true;
			}

			if ( ! ok ) {
				if ( level ) cout << "  Cannot be written over " << char('a' + i) << " " << bitset<X>(lower[i]) << endl;
				incomplete[isize++] = lower[i];
			}
		}

		return greedy_greater(incomplete, incomplete + isize);
		//getchar();
	}

	conf_t greedy_greater( conf_t* beg, conf_t* end ) {
		if ( beg == end ) return 0;
		random_shuffle(beg, end);
		size_t elements = end - beg;
		if ( elements >= 10 ) elements = 9; // hard limit to the first 2^9 configurations

		for ( size_t i = 0; i < (1u << elements) - 1; ++i ) {
			auto dio = powers[elements];
			size_t chosen = powers[elements][i];
			//cout << "    Considering " << bitset<X>(chosen) << endl;

			conf_t c = 0;
			for ( conf_t* i = beg; i != beg + elements; ++i )
				if ( bitset<X>(chosen)[i-beg] ) c |= *i;
		
			conf_t candidate = find_greater(c);
			if ( candidate == M ) {
				if ( level ) cout << "    All greater configurations of " << bitset<X>(c) << " are taken." << endl;
			} else {
				if ( level ) cout << "    Found configuration greater than " << bitset<X>(c) << " (" << bitset<X>(candidate) << ")" << endl;
				return candidate;
			}
		}

		return find_greater_inverse(0);
	}

	bool generate_greather() {
		size_t aux [N];
		for ( size_t i = 0; i < N; ++i ) aux[i] = i;
		random_shuffle(aux, aux + N);

		for ( size_t i = 0; i < N; ) {
			conf_t ecurrent = lower[i];
			char current = 'a' + i;
			//cout << bitset<X>(ecurrent) << " is " << current <<  endl;
			conf_t hint = find_upper(current);

			if ( hint == M ) {
				if ( level ) cerr << "Stopped at " << i+1 << " letters." << endl;
				return false;
			}
			if ( hint == 0 ) ++i ;
			if ( hint != 0 ) taken[hint] = current;
		}

		return true;
	}

	void insert_with_complement( conf_t e, char c) {
		assert(taken[e] == 0);
		taken[e] = c;
		bitset<X> bits(e);
		conf_t complement = bits.flip().to_ulong();
		taken[complement] = c;
		lower[c - 'a'] = e;  // assume they are contiguous
	}

	bool init() {
		char current = 'a';
		insert_with_complement(0, current++);
		insert_with_complement(1, current++);
		insert_with_complement(2, current++);
		insert_with_complement(4, current++);
		insert_with_complement(8, current++);
		insert_with_complement(16, current++);
		insert_with_complement(32, current++);
		insert_with_complement(64, current++);

		// remaining letters
		while ( unsigned(current) < 'a' + N ) {
			conf_t candidate = 0;
			for ( size_t k = 0; k < K; ++k) candidate |= (1 << random() % X);

			if ( taken[candidate] == 0 ) {
				insert_with_complement(candidate, current);
				current++;
			}
		}

		//cout << *this << endl;

		return true;
	}

	friend ostream& operator<< ( ostream& out, tape& p ) {
		for ( size_t k = 0; k < M; ++k ) {
			if ( p.taken[k] == 0 ) {
				out << bitset<X>(k) << " " << "*" << endl;
			} else {
				bool lower = find(p.lower.begin(), p.lower.end(), k) != p.lower.end();
				out << bitset<X>(k) << " " << char(p.taken[k]) << (lower ? " #" : "") << endl;
			}
		}

		out << "used:  " << p.used() << endl;
		out << "valid: " << p.check() << endl;
		return out;
	}
};

template<>
const powerset<> tape<7,25>::powers = powerset<>();


int main () {
	srandom(std::random_device()());
	//tape<7>::print_graph();

	while(true) {
		tape<7, 25> t;
	}

	return 0;
}