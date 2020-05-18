#include <bits/stdc++.h>
#include <fstream>

using namespace std;

int main(int argc, char **argv) {
  short N, M;
  int B, W;
  ifstream in("input.txt");
  ofstream out("output.txt");
  in >> N >> M >> B >> W;
  vector<pair<short, short>> blacks = vector<pair<short, short>>(B);
  for (int i = 0; i < B; i++) {
    short r, c;
    in >> r >> c;
    blacks[i] = pair<short, short>(r, c);
  }
  vector<pair<short, short>> whites = vector<pair<short, short>>(W);
  for (int i = 0; i < W; i++) {
    short r, c;
    in >> r >> c;
    whites[i] = pair<short, short>(r, c);
  }

  cout << "hello word!\n\n";
  out << (N + M) << endl;
  return 0;
}
