
#include <array>
#include <cassert>
#include <iomanip>
#include <set>
#include <iostream>
#include <iterator>
#include <vector>
#include <algorithm>
using namespace std;


template<int N, int M>
struct integer_partition {
	typedef array<int, M> part_t;
	typedef array<int, M> part_sentinel_t;
	vector<part_t> parts;

	// taken from knuth
	integer_partition ( ) {
		part_sentinel_t a;
		static_assert(M >= 1, "number of parts must be greater than 0");
		static_assert(N >= M, "integer has to be greater or equal to the numeber of parts");

		if ( M == 1 ) {
			parts.push_back({N});
			return;
		}

		// init
		a[0] = N - M + 1;
		a[M] = -1;
		fill(a.begin() + 1, a.begin() + M, 1);

		while ( true ) {
			// H2 visit
			while ( true ) {
				parts.push_back(a);
				if ( a[1] >= a[0] - 1 ) break;

				// H3
				a[0] -= 1;
				a[1] += 1;
			}

			// H4
			int j = 3;
			int s = a[0] + a[1] - 1;
			while ( a[j - 1] >= a[0] - 1 ) {
				s += a[j - 1];
				++j;
			}


			// H5
			if ( j > M ) break;
			int x = a[j - 1] + 1;
			a[j - 1] = x;
			--j;

			// H6
			while ( j > 1 ) {
				a[j - 1] = x;
				s -= x;
				--j;
			}

			a[0] = s;
		}
	}

	integer_partition& different ( ) {
		auto it = remove_if( parts.begin(), parts.end(), 
			[](const part_t& x)->bool { 
				part_t p = x; 
				sort(p.begin(), p.end());
				return unique(p.begin(), p.end()) != p.end();
			}
		);

		parts.resize(distance(parts.begin(), it));

  		return *this;
	}

	// careful, this increases the number of parts exactly by M!
	integer_partition& with_permutations ( ) {
		size_t original = parts.size();

		for ( size_t i = 0; i < original; ++i ) {
			part_t s = parts[i];
			sort(s.begin(), s.end());
			parts[i] = s;

		    while(next_permutation(s.begin(), s.end())) {
		    	parts.push_back(s);
		    }
		}

		sort(parts.begin(), parts.end());

  		return *this;
	}

	size_t size () {
		return parts.size();
	}

	const part_t& operator[] ( const size_t i ) const {
		return parts[i];
	}

	friend ostream& operator<< ( ostream& out, integer_partition& p ) {
		for ( auto i : p.parts ) {
			out << "[";
			for ( size_t j = 0; j < i.size() - 1; ++j ) out << i[j] << ",";
			out << i[i.size()-1] << "] ";
		}

		return out;
	}
};

int main () {
	integer_partition<8, 3> p;
	cout << p.size() << endl;
	cout << p << endl;

	cout << p.different().size() << endl;
	cout << p << endl;

	cout << p.with_permutations().size() << endl;
	cout << p << endl;
	return 0;
}