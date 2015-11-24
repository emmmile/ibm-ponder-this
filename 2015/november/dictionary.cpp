#include <algorithm>
#include <fstream>
#include <iostream>
#include <set>
#include <vector>
using namespace std;

bool simple ( const string& word ) {
	if ( word.size() > 6 ) {
		return false;
	}

	for ( auto c : word ) {
		if ( c < 97 || c > 122 ) {
			return false;
		}
	}


	string all = word + "watson";
	sort( all.begin(), all.end() );
	auto last = unique( all.begin(), all.end() );
	all.erase( last, all.end() );

	if ( all.size() > 6 ) {
		return false;
	}


	return true;
}

int main ( ) {
	std::ifstream infile("simple.english");
	vector<string> words;
	string word;

	while (infile >> word) {
		if ( simple(word) ) {
			words.emplace_back(word);
		}
	}

	sort( words.begin(), words.end() );
	auto last = unique( words.begin(), words.end() );
	words.erase( last, words.end() );

	for ( auto i : words ) {
		cout << i << endl;
	}
}
