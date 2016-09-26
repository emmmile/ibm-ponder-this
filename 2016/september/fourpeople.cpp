#include <iostream>
#include <vector>
#include <algorithm>
#include <string>
#include <random>
#include <array>
using namespace std;

// g++ fourpeople.cpp -o fourpeople -O3 -std=c++11 -ffast-math -lgmpxx -lgmp -mtune=native -Wall

string seats[] = {"abcd", "abdc", "acbd", "acdb", "adbc", "adcb",
                  "bacd", "badc", "bcad", "bcda", "bdac", "bdca",
                  "cabd", "cadb", "cbad", "cbda", "cdab", "cdba",
                  "dabc", "dacb", "dbac", "dbca", "dcab", "dcba"};

array<int, 24 * 24> violations;

int difference(int i, int j) {
  int count = 0;

  for (int z = 0; z < 4; ++z) {
    if (seats[i][z] == seats[j][z]) {
      count++;
    }
  }

  return count;
}

void initialize() {
  for (int i = 0; i < 24; ++i) {
    for (int j = 0; j < 24; ++j) {
      violations[i * 24 + j] = difference(i, j);
    }
  }
}

int main ( int argc, char** argv ) {
  initialize();

  array<int, 24> shuffled = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23};
  random_shuffle(shuffled.begin(), shuffled.end());
  array<int, 24> candidate;


  while (true) {
    int currentviolations = 0;
    candidate[0] = random() % 24;
    for (int i = 1; i < 24; ++i) {
      int minimum = 4;
      int selected = -1;
      for (int j : shuffled) {
        bool present = find(candidate.begin(), candidate.begin() + i, j) != candidate.begin() + i;
        if (!present && violations[candidate[i - 1] * 24 + j] < minimum) {
          selected = j;
          minimum = violations[candidate[i - 1] * 24 + j];
        }
      }
      candidate[i] = selected;
      currentviolations += minimum;
    }

    for (auto s : candidate) {
      cout << seats[s] << ", ";
    }
    cout << endl;

    cout << currentviolations << endl;
  }
}
