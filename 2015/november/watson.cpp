#include <algorithm>
#include <fstream>
#include <iostream>
#include <map>
#include <random>
#include <set>
#include <vector>
using namespace std;

typedef array<int, 26> encoding;
vector<encoding> encodings;

encoding getenc ( const string& all, const vector<int>& values ) {
	encoding enc;
	enc.fill(0);

	for ( unsigned int i = 0; i < all.size(); ++i ) {
		//cout << all << ": value of " << all[i] << " is " << values[i] << endl;
		enc[all[i] - 'a'] = values[i];
	}

	return enc;
}

vector<encoding> all ( ) {
	vector<int> values = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
	vector<encoding> encodings;

    do {
		encodings.push_back(getenc("watson", values));
    } while( next_permutation(values.begin(), values.end()) );

    sort(encodings.begin(), encodings.end());
	auto last = unique( encodings.begin(), encodings.end() );
	encodings.erase( last, encodings.end() );

	cout << "there are " << encodings.size() << " different encodings." << endl;
	return encodings;
}

int decimal ( const string& a, const encoding& enc ) {
	int sum = 0;
	int multiplier = 1;

	for ( int i = a.size() - 1; i >= 0; --i ) {
		sum += enc[a[i] - 'a'] * multiplier;
		multiplier *= 10;
	}

	if ( enc[a[0] - 'a'] == 0 ) {
		return 0;
	}

	// cout << "value of " << a << " is " << sum << endl;
	// getchar();

	return sum;
}

int search ( const string& start, map<string, int>& all, int value, int deepness ) {
	if ( deepness == 5 ) {
		return;
	}

	int current = all[start];
	all.erase(start);

	for ( string another : all ) {
		search(another, all, value + all[another], deepness + 1);
	}

	all[start] = current;
}

map<int, string> allwords ( const vector<string>& words, const encoding& e ) {
	map<int, string> output;

	for ( string w : words ) {
		int value;

		if ( (value = decimal(w, e)) ) {
			output[value] = w;
			// cout << w << " " << value << endl;
		}
	}

	cout << "there are " << output.size() << " words for this encoding." << endl;
	// getchar();
	return output;
}

bool check ( const string& a, const string& b, const encoding& enc ) {
	int da = decimal(a, enc);
	int db = decimal(b, enc);
	int watson = decimal("watson", enc);

	if ( da && db && watson ) {
		if ( da + db == watson || 
			 db - da == watson || 
			 db + da == watson ||
			 db * da == watson ) {
			return true;
		}
	}

	return false;
}

// amble + meats = watson
// 69085 + 95647 = 164732

bool check ( const string& a, const string& b ) {
	if ( a.size() < 2 || b.size() < 2 ) {
		return false;
	}

	cout << a << " " << b << endl;

	vector<int> values = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
	int count = 0;
	encoding theenc;

    do {
		encoding enc = getenc("watson", values);

    	if ( check(a, b, enc ) ) {
    		theenc = enc;
    		count++;
    	}
    } while( next_permutation(values.begin(), values.end()) && count < 2 );

    if ( count == 1 ) {
    	cout << a << " +-* " << b << " = watson" << endl;
    	cout << decimal(a, theenc) << " +-* " << decimal(b, theenc) << " = " << decimal("watson", theenc) << endl;
    	getchar();
    }

	return count == 1;
}

int main ( ) {
	std::ifstream infile("english.dict");
	vector<string> words;
	string word;

	while (infile >> word) {
		words.emplace_back(word);
	}

	cout << "using " << words.size() << " words." << endl;
	auto encodings = all();

	for ( encoding e : encodings ) {
		auto validwords = allwords(words, e);
	}

 //    std::random_device rd;
 //    std::mt19937 g(rd());
	// shuffle(words.begin(), words.end(), g);

	// for ( unsigned int i = 0; i < words.size(); ++i ) {
	// 	for ( unsigned int j = i + 1; j < words.size(); ++j ) {
	// 		check( words[i], words[j] );
	// 	}
	// }
}
