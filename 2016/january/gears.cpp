#include <bitset>
#include <array>
#include <iostream>
#include <vector>
using namespace std;

#define MAX 12
#define M 6

bool isgear ( const array<int, M>& s, const array<int, M>& t, const int N ) {
  bitset<MAX * MAX + 1> result;
  result[0] = true;

  for ( int i : s ) {
    for ( int j : t ) {
      int index = i * j;

      if ( index > MAX * MAX ) {
        continue;
      }

      result[index] = result[index - 1] = result[index + 1] = true;
    }
  }

  for ( int i = 0; i <= N; ++i ) {
    if ( result[i] == false ) {
      return false;
    }
  }

  return true;
}

int maxnumber (const array<int, M>& s, const array<int, M>& t) {
  int maximum = 0;
  for ( int N = 1; N < 128; ++N ) {
    int current = isgear( s, t, N);
    if ( current > maximum ) {
      maximum = current;
    }
  }

  return maximum;
}

vector<array<int, M>> all;
array<int, M> combination;

void pretty_print(const array<int, 6>& v) {
  static int count = 0;
  cout << "[ ";
  for (int i = 0; i < v.size(); ++i) { cout << v[i] << " "; }
  cout << "] " << endl;
}

void go(const array<int, MAX>& base, int offset, int k) {
  if (k == 0) {
    pretty_print(combination);
    all.push_back(combination);
    return;
  }
  for (int i = offset; i <= base.size() - k; ++i) {
    combination[M - k] = base[i];
    go(base, i+1, k-1);
  }
}

void gears ( ) {
  array<int, MAX> base;

  for ( int i = 0; i < MAX; ++i ) {
    base[i] = i + 1;
  }

  go(base, 0, M);
}


int main ( ) {
  gears();
  int maximum = 0;

  for ( int i = 0; i < all.size(); ++i ) {
    for ( int j = i + 1; j < all.size(); ++j ) {
      //pretty_print(all[i]);
      int current = maxnumber(all[i], all[j]);
      if ( current > maximum ) {
        maximum = current;
        cout << maximum << endl;
      }
    }
  }

  cout << maximum << endl;
}
