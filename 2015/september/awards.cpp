#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;


#define M 8

vector<vector<int>> all;

void print (vector<int>& v, int level){
   int groups = 0;
   for (int i=0;i<=level;i++)
      if (v[i] > 1) ++groups;
   if ( groups > M ) return;

   // cout << level+1 << ": ";
   all.emplace_back(v.begin(), v.begin() + level + 1);

   // for(int i=0;i<=level;i++)
   //    cout << v[i] << " ";
   // cout << endl;
}

void part(int n, vector<int>& v, int level){
   int first; /* first is before last */

   if(n<1) return ;
   v[level]=n;
   print(v, level);

   first=(level==0) ? 1 : v[level-1];

   for(int i=first;i<=n/2;i++){
      v[level]=i; /* replace last */
      part(n-i, v, level+1);
   }
}

int main(int argc, char** argv){
   int N = std::stoi(argv[1]);

   vector<int> v(N);

   part(N, v, 0);

   sort(all.begin(), all.end(), [](const vector<int>& a, const vector<int>& b) -> bool {return a.size() < b.size();});
   for ( auto i : all ) {
      for ( auto j : i ) {
         cout << j << " ";
      }
      cout << endl;
   }
}