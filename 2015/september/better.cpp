#include <algorithm>
#include <iostream>
#include <vector>
using namespace std;

typedef vector<int> integer_partition;

// verify that is possible to select n numbers with sum N
bool verify ( const integer_partition& p, int n ) {
  int N = p.back();
  int M = p.size();
  vector<bool> v(M);

  // selecting n-k ones means selecting k numbers in p
  for ( int k = 0; k <= min(M,n); ++k ) {
    fill(v.begin(), v.begin() + k, false);
    fill(v.begin() + k, v.end(), true);

    do {
      int sum = 0;
      for ( int i = 0; i < M; ++i ) {
        if ( !v[i] ) {
          sum += p[i];
        }
      }

      if ( sum + n - k == N ) {
        return true;
      }
    } while (next_permutation(v.begin(), v.end()));
  }

  return false;
}

bool verify ( const integer_partition& p ) {
  bool result = true;

  for ( int i = 1; i <= p.back() && result; ++i ) {
    result = result && verify(p, i);
  }

  return result;
}

void print ( const integer_partition& p ) {
  for ( auto i : p ) {
    cout << i << " ";
  }

  cout << endl;
}

int main ( ) {
	cout << verify(integer_partition{2,3,5,8,16}) << endl;

  integer_partition last = {2, 3};
  int M = last.size();

  while ( true ) {
    print(last);
    integer_partition p = last;
    integer_partition next;
    p.resize(M + 2, p.back() + 1);

    while ( verify(p) ) {
      next = p;
      p[M]++;
      p[M+1] = p[M] + p[M-1];
    }

    last = next;
    M += 2;
  }

  return 0;
}


// (1,2,3)
// (1,2,3,5,8)
// (1,2,3,5,8,16,24)
// (1,2,3,5,8,16,24,54,78)
// (1,2,3,5,8,16,24,54,78,184,262)
// (1,2,3,5,8,16,24,54,78,184,262,628,890)
// (1,2,3,5,8,16,24,54,78,184,262,628,890,2144,3034)

// (1,2,3,5)
// (1,2,3,5,9,14)
// (1,2,3,5,9,14,30,44)
// (1,2,3,5,9,14,30,44,102,146)
// (1,2,3,5,9,14,30,44,102,146,348,494)
// (1,2,3,5,9,14,30,44,102,146,348,494,1188,1682)