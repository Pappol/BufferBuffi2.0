#include <bits/stdc++.h>
#include <fstream>

using namespace std;

// Pair: row and column
typedef pair<ushort, ushort> rc;

int main(int argc, char **argv) {
  ushort N, M;
  int B, W;
  ifstream in("input.txt");
  ofstream out("output.txt");
  // Initialization
  in >> N >> M >> B >> W;
  vector<rc> blacks = vector<rc>(B);
  for (int i = 0; i < B; i++) {
    ushort r, c;
    in >> r >> c;
    blacks[i] = rc(r, c);
  }
  vector<rc> whites = vector<rc>(W);
  for (int i = 0; i < W; i++) {
    ushort r, c;
    in >> r >> c;
    whites[i] = rc(r, c);
  }
  
  cout << "hello word!\n\n";
  out << (N + M) << endl;
  return 0;
}
