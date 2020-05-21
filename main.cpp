#include <bits/stdc++.h>
#include <fstream>
#define VUOTO 0
#define BIANCO 1
#define NERO 2

using namespace std;
// Pair: row and column
typedef pair<ushort, ushort> rc;

// Cell data structure
typedef struct {
  bool U = true;  // Can go up from here
  bool D = true;  // Can go down from here
  bool L = true;  // Can go left from here
  bool R = true;  // Can go right from here
  short type = 0; // 0=vuota,1=bianca, 2=nera
} cell;
vector<vector<cell>> matrix;

// Checks for an optimal rectangular solution. If it is found it is printed and
// the program exits w/ code 0. If a solution is found but it's not optimal it
// gets printed anyway. Returns ring count w/ a rectangular solution.
int checkForRectangles(int n, int m, vector<rc> w, vector<rc> b, ofstream *out);

// Modifies matrix "placing walls" (forbidding moves that would surely lead into
// darkness)
void placeWalls(int n, int m);

int main(int argc, char **argv) {
  ushort N, M;
  int B, W;
  ifstream in("input.txt");
  ofstream out("output.txt");
  // Initialization
  in >> N >> M >> B >> W;
  matrix = vector<vector<cell>>(N, vector<cell>(M, cell()));
  // Forbidding invalid moves
  for (int i = 0; i < M; i++) {
    matrix[0][i].U = false;
  }
  for (int i = 0; i < N; i++) {
    matrix[i][0].L = false;
    matrix[i][M - 1].R = false;
  }
  for (int i = 0; i < M; i++) {
    matrix[N - 1][i].D = false;
  }
  // Getting rings' poitions
  vector<rc> blacks = vector<rc>(B);
  for (int i = 0; i < B; i++) {
    ushort r, c;
    in >> r >> c;
    blacks[i] = rc(r, c);
    matrix[r][c].type = NERO;
  }
  vector<rc> whites = vector<rc>(W);
  for (int i = 0; i < W; i++) {
    ushort r, c;
    in >> r >> c;
    whites[i] = rc(r, c);
    matrix[r][c].type = BIANCO;
  }
  // Checking if a rectangle could be the optimal solution
  if (W <= 8 && B <= 4) {
    if (checkForRectangles(N, M, whites, blacks, &out) == B + W)
      return 0;
  }
  // Placing the walls
  placeWalls(N, M);

  out << endl;
  for (int i = 0; i < N; i++) {
    for (int j = 0; j < M; j++) {
      out << matrix[i][j].type;
    }
    out << endl;
  }
  out.close();
  in.close();
  return 0;
}

int checkForRectangles(int n, int m, vector<rc> w, vector<rc> b,
                       ofstream *out) {
  pair<short, short> topLeft, topRight, bottomRight;
  topLeft = topRight = bottomRight = pair<short, short>(-1, -1);
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < m; j++) {
      if (matrix[i][j].type != VUOTO) {
        topLeft = pair<short, short>(i, j);
        break;
      }
    }
    if (topLeft.first != -1)
      break;
  }
  // Searching bottomRight
  for (int i = n - 1; i >= 0; i--) {
    for (int j = m - 1; j >= 0; j--)
      if (matrix[i][j].type != VUOTO) {
        bottomRight = pair<short, short>(i, j);
        break;
      }
    if (bottomRight.first != -1)
      break;
  }

  stringbuf path = stringbuf();
  for (int i = topLeft.second; i < bottomRight.second; i++)
    path.sputc('R');
  for (int i = topLeft.first; i < bottomRight.first; i++)
    path.sputc('D');
  for (int i = bottomRight.second; i > topLeft.second; i--)
    path.sputc('L');
  for (int i = bottomRight.first; i > topLeft.first; i--)
    path.sputc('U');
  int anelli = 0;
  rc p = topLeft;
  string s = path.str();
  for (int i = 0; i < s.length(); i++) {
    if (matrix[p.first][p.second].type != VUOTO)
      anelli++;
    switch (s[i]) {
    case 'U':
      p.first--;
      break;
    case 'D':
      p.first++;
      break;
    case 'L':
      p.second--;
      break;
    case 'R':
      p.second++;
      break;
    }
  }
  *out << anelli << " " << s.length() << " " << topLeft.first << " "
       << topLeft.second << " " << s << "#" << endl;
  return anelli;
}

void placeWalls(int n, int m) {
  for (int j = 0; j < m; j++) // BLUE  TOP
    if (matrix[0][j].type == BIANCO) {
      matrix[1][j].U = false;
      matrix[0][j].D = false;
    }
  for (int i = 0; i < n; i++) // BLUE  RIGHT
    if (matrix[i][m - 1].type == BIANCO) {
      matrix[i][m - 1].L = false;
      matrix[i][m - 2].R = false;
    }
  for (int j = 0; j < m; j++) // BLUE  BOTTOM
    if (matrix[n - 1][j].type == BIANCO) {
      matrix[n - 1][j].U = false;
      matrix[n - 2][j].D = false;
    }
  for (int i = 0; i < n; i++) // BLUE LEFT
    if (matrix[i][0].type == BIANCO) {
      matrix[i][0].R = false;
      matrix[i][1].L = false;
    }
  for (int i = 1; i < n; i++) // GREEN VERTICAL
    for (int j = 0; j < m; j++)
      if (matrix[i][j].type == BIANCO && matrix[i - 1][j].type == BIANCO &&
          matrix[i - 1][j].D == true) {
        // Laterali (verticali)
        matrix[i][j].L = matrix[i][j].R = false;
        matrix[i - 1][j].L = matrix[i - 1][j].R = false;
        if (i - 2 >= 0) {
          matrix[i - 2][j].U = false;
        }
        if (i + 1 < n) {
          matrix[i + 1][j].D = false;
        }
      }
  for (int j = 1; j < m; j++) // GREEN HORIZONTAL
    for (int i = 0; i < n; i++)
      if (matrix[i][j].type == BIANCO && matrix[i][j - 1].type == BIANCO &&
          matrix[i][j - 1].R == true) {
        // Laterali (orizzontali)
        matrix[i][j].U = matrix[i][j].D = false;
        matrix[i][j - 1].U = matrix[i][j - 1].D = false;
        if (j - 2 >= 0)
          matrix[i][j - 2].L = false;
        if (j + 1 < m)
          matrix[i][j + 1].R = false;
      }
  // TODO: rosa
}
