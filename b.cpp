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
vector<vector<cell>> matrix;
ushort n, m;
vector<rc> blacks, whites;

// Cell data structure
typedef struct {
  bool U = true;  // Can go up from here
  bool D = true;  // Can go down from here
  bool L = true;  // Can go left from here
  bool R = true;  // Can go right from here
  short type = 0; // 0=vuota,1=bianca, 2=nera
  bool vis = false;
} cell;

bool white(short row, short col) {
  if (row < 0 || row >= n || col < 0 || col >= m) {
    return false;
  } else
    return matrix[row][col].type == BIANCO;
}
pair<char, rc> findNeroBelo() {
  for (rc black : blacks) { // nero con 2
    bool whiteTop, whiteBottom, whiteLeft, whiteRight;
    whiteTop = white(black.first--, black.second);
    whiteBottom = white(black.first++, black.second);
    whiteLeft = white(black.first++, black.second);
    whiteRight = white(black.first++, black.second);
    if (whiteTop && (whiteLeft || whiteRight))
      return pair<char, rc>('D', black);
    if (whiteBottom && (whiteLeft || whiteRight))
      return pair<char, rc>('U', black);
  }
  for (rc black : blacks) { // nero con 1
    bool whiteTop, whiteBottom, whiteLeft, whiteRight;
    whiteTop = white(black.first--, black.second);
    whiteBottom = white(black.first++, black.second);
    whiteLeft = white(black.first++, black.second);
    whiteRight = white(black.first++, black.second);
    if (whiteTop)
      return pair<char, rc>('D', black);
    if (whiteBottom)
      return pair<char, rc>('U', black);
    if (whiteLeft)
      return pair<char, rc>('R', black);
    if (whiteRight)
      return pair<char, rc>('L', black);
  }
}
