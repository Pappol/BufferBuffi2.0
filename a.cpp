//#include "./swrace.h"
#include <bits/stdc++.h>
#include <fstream>
#include <set>
#define VUOTO 0
#define BIANCO 1
#define NERO 2

using namespace std;
// Pair: row and column
typedef pair<ushort, ushort> rc;
// distance and row, column
typedef pair<int, rc> distRC;

// Cell data structure
typedef struct {
  bool U = true;  // Can go up from here
  bool D = true;  // Can go down from here
  bool L = true;  // Can go left from here
  bool R = true;  // Can go right from here
  short type = 0; // 0=vuota,1=bianca, 2=nera
  bool vis = false;
} cell;
vector<vector<cell>> matrix;
ushort n, m;
vector<rc> blacks, whites;

pair<char, rc> findNeroBelo(){

}

pair<char, rc> beroBeloAndDir(){

}
