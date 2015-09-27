
#include <algorithm>
#include <cassert>
#include <iostream>
#include <set>
#include <vector>
using namespace std;


#define M 8

typedef vector<int> integer_partition;
vector<vector<integer_partition>> all;
set<integer_partition> memo;

void print ( integer_partition& p ) {
   for ( auto i : p ) {
      if( i != 1 ) cout << i << " ";
   }
   cout << endl;
}

int size ( integer_partition& p ) {
   return p.size() - count(p.begin(), p.end(), 1);
}

void add (integer_partition& v, int level){
   int groups = 0;
   for (int i=0;i<=level;i++)
      if (v[i] > 1) ++groups;
   if ( groups >= M-1 ) return;

   // cout << level+1 << ": ";
   all[level].emplace_back(v.begin(), v.begin() + level + 1);
}

// http://stackoverflow.com/questions/10716324/partition-function-in-c
void part(int n, integer_partition& v, int level){
   int first; /* first is before last */

   if(n<1) return ;
   v[level]=n;
   add(v, level);

   first=(level==0) ? 1 : v[level-1];

   for(int i=first;i<=n/2;i++){
      v[level]=i; /* replace last */
      part(n-i, v, level+1);
   }
}

void valid(int N, int s, integer_partition groups) {
   if ( s == N || size(groups) > M ) {
      if ( size(groups) <= M ) {
         print(groups);
         exit(0);
      }

      return;
   }

   if ( memo.count(groups) ) {
      return;
   }

   // for each partition of size = s
   for ( auto i : all[s-1] ) {
      integer_partition updated_groups;
      set_union(groups.begin(), groups.end(), i.begin(), i.end(), std::back_inserter(updated_groups));

      valid(N, s+1, updated_groups);
   }

   memo.insert(groups);
}

int main(int argc, char** argv){
   int N = std::stoi(argv[1]);
   all.resize(N);
   integer_partition v(N);
   part(N, v, 0);

   int total = 0;
   int current = 1;
   for ( auto i : all ) {
      // cout << i.size() << " partition of N with " << current << " parts." << endl;
      current++;
      total += i.size();
   }
   cout << "N = " << N << ". Generated in total " << total << " partitions." << endl;

   valid(N, 1, integer_partition{2,3,5,N});
}