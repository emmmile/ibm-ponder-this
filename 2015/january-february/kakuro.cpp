
#include <array>
#include <cassert>
#include <iomanip>
#include <set>
#include <iostream>
#include <iterator>
#include <vector>
#include <algorithm>
using namespace std;

template<size_t N, size_t M>
struct kakuro {
	typedef size_t I;
	typedef array<I, M * N> board_t;
	typedef set<board_t> solutions_t;
	array<I, N> rdefs;
	array<I, M> cdefs;
	board_t board;
	const I EMPTY;

	kakuro& operator= (const kakuro& k) {
		this->rdefs = k.rdefs;
		this->cdefs = k.cdefs;
		this->board = k.board;
		return *this;
	}

	kakuro ( ) : EMPTY(0) {
		fill(board.begin(), board.end(), EMPTY);
		
		// Worst generation possible. The problem anyway is
		// to find a board that has UNIQUE solution (difficult)
		// while this instead should finish return very quickly.
		while ( true ) {
			size_t sum = 0;
			for ( size_t i = 0; i < N; ++i ) {
				rdefs[i] = M + random() % (8 * M);
				sum += rdefs[i];
			}

			size_t other_sum = 0;
			for ( size_t i = 0; i < M; ++i ) {
				cdefs[i] = N + random() % (8 * N);
				other_sum += cdefs[i];
			}
			if ( sum == other_sum ) break;
		}
	}

	I& cell ( size_t i, size_t j ) {
		assert(i < N);
		assert(j < M);
		return board[i * M + j];
	}

	size_t column ( size_t i ) {
		return i % M;
	}

	size_t row ( size_t i ) {
		return i / M;
	}






	bool check_different ( size_t i ) {
		return check_column_different(i) && check_row_different(i);
	}

	bool check_column_different ( size_t i ) {
		for ( size_t k = 0; k < N; ++k ) {
			size_t index = M * k + column(i);

			if ( index != i && board[index] == board[i] ) return false;
		}

		return true;
	}

	bool check_row_different ( size_t i ) {		
		for ( size_t k = 0; k < M; ++k ) {
			size_t index = k + M * row(i);

			if ( index != i && board[index] == board[i] ) return false;
		}

		return true;
	}





	bool check_sum ( size_t i ) {
		return check_sum_column(i) && check_sum_row(i);
	}

	bool check_sum_row ( size_t i ) {
		if ( rdefs[row(i)] == EMPTY ) return true;
		size_t sum = 0;

		for ( size_t k = 0; k < M; ++k ) {
			size_t index = k + M * row(i);
			sum += board[index];

			if ( board[index] == EMPTY ) return true;
		}

		//return (rdefs[row(i)] == EMPTY && sum < 10) || sum == rdefs[row(i)];		
		return sum == rdefs[row(i)];
	}

	bool check_sum_column ( size_t i ) {
		if ( cdefs[column(i)] == EMPTY ) return true;
		size_t sum = 0;

		for ( size_t k = 0; k < N; ++k ) {
			size_t index = M * k + column(i);
			sum += board[index];

			if ( board[index] == EMPTY ) return true;
		}

		//return (cdefs[column(i)] == EMPTY && sum < 10) || sum == cdefs[column(i)];		
		return sum == cdefs[column(i)];
	}





	solutions_t solve ( ) {
		solutions_t all;
		solve(0, all);

		/*if ( all.size() == 1 ) {
			board = *all.begin();
			cout << *this << endl;
		}*/

		return all;
	}

	bool is_start ( ) {
		return solve().size() == 1;
	}

	bool check_condition ( board_t& solution ) {
		size_t n = M + N;
		size_t k = 2;
		vector<bool> v(n);
   		fill(v.begin() + k, v.end(), true);


		kakuro original = *this;
   		size_t kdefs[2];
   		size_t j = 0;
		do {
			for (size_t i = 0; i < n; ++i) {
				if (!v[i]) {
					//cout << (i) << " ";
					kdefs[j++] = i;
				}
			}
			//std::cout << "\n";

			// for every possible definition pair, try to solve without the constraints
			// and check if the solutions are > 1
			remove_definitions(kdefs);
			solutions_t all = solve();


			if ( all.size() == 1 ) {
				cout << "There is just one solution again." << endl;
				return false;
			}

			/*if ( !check_solutions(solution, all) ) {
				cout << "The additional condition is not met." << endl;
				return false;
			}*/

			cout << "There are " << all.size() << " solutions. This is the first: " << endl;
			board = *all.begin();
			cout << *this << endl;

			j = 0;
			*this = original;
		} while (std::next_permutation(v.begin(), v.end()));

		return true;
	}

	bool check_solutions( board_t& original, solutions_t all ) {
		// for every cell X, check if exists one solution that differs at least in X
		for ( size_t x = 0; x < original.size(); ++x ) {
			I current = original[x];
			//cout << "Original cell is " << current << endl;

			bool found = false;
			for ( auto j : all ) {
				if ( j[x] != current ) {
					//cout << "Found solution with cell " << j[x] << endl;
					found = true;
					break;
				}
			}

			if ( !found ) return false;
		}

		return true;
	}

	void remove_definitions ( size_t kdefs [2] ) {
		//cout << kdefs[0] << " " << kdefs[1] << endl;
		for ( size_t i = 0; i < 2; ++i ) {
			if (kdefs[i] < N)
				rdefs[kdefs[i]] = EMPTY;
			else	cdefs[kdefs[i]-N] = EMPTY;
		}
		//cout << *this << endl;
		cout << "Removed definitions (" << kdefs[0] << ", " << kdefs[1] << "). ";
	}

	bool solve ( size_t i, solutions_t& all ) {
		if ( i == N * M )
			return true;

		if ( board[i] != EMPTY ) 
			return solve(i + 1, all);

		for ( size_t n = 1; n < 10; ++n ) {
			board[i] = n;

			if ( check_different(i) && check_sum(i) ) {
				if ( solve(i + 1, all) ) {
					//cout << endl << *this << endl;
					//return true;
					all.insert(board);
				}
			}

			board[i] = EMPTY;
		}

		return false;
	}

	ostream& print_cell( ostream& o, size_t value ) {
		if ( value != EMPTY ) 
			o << setw(2) << (int) value << " ";
		else
			o << "-- ";
		return o;
	}

	friend ostream& operator<< (ostream& o, kakuro& k) {
		o << "-- | ";
		for ( size_t i = 0; i < M; ++i )
			k.print_cell(o, k.cdefs[i]);
		o << endl;

		//cout << string(3 * (M+1), '-').insert(3, "+") << endl;
		for ( size_t i = 0; i < N; ++i ) {
			k.print_cell(o, k.rdefs[i]);
			o << "| ";

			for ( size_t j = 0; j < M; ++j )
				k.print_cell(o, k.cell(i, j));

			o << endl;
		}

		return o;
	}
};



int main () {

	while ( true ) {
		kakuro<4,3> kk;
		auto solutions = kk.solve();
		if ( solutions.size() != 1 ) continue;

		auto solution = *solutions.begin();

		
		cout << "==============================" << endl;
		cout << "This board has single solution: " << endl;
		cout << kk << endl;

		if ( kk.check_condition(solution) ) {
			cout << "This board meets the condition.";
			getchar();
		}
	}

	return 0;
}