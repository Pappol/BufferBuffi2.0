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
ushort n, m;
vector<rc> blacks, whites;

// Checks for an optimal rectangular solution. If it is found it is printed and
// the program exits w/ code 0. If a solution is found but it's not optimal it
// gets printed anyway. Returns ring count w/ a rectangular solution.
int checkForRectangles(ofstream *out);

// Modifies matrix "placing walls" (forbidding moves that would surely lead into
// darkness)
void placeWalls();

// Search for a solution via backtracking
void solve(ofstream *out);

int main(int argc, char **argv) {
  int B, W;
  ifstream in("input.txt");
  ofstream out("output.txt");
  // Initialization
  in >> n >> m >> B >> W;
  matrix = vector<vector<cell>>(n, vector<cell>(m, cell()));
  // Forbidding invalid moves
  for (int i = 0; i < m; i++) {
    matrix[0][i].U = false;
  }
  for (int i = 0; i < n; i++) {
    matrix[i][0].L = false;
    matrix[i][m - 1].R = false;
  }
  for (int i = 0; i < m; i++) {
    matrix[n - 1][i].D = false;
  }
  // Getting rings' poitions
  blacks = vector<rc>(B);
  for (int i = 0; i < B; i++) {
    ushort r, c;
    in >> r >> c;
    blacks[i] = rc(r, c);
    matrix[r][c].type = NERO;
  }
  whites = vector<rc>(W);
  for (int i = 0; i < W; i++) {
    ushort r, c;
    in >> r >> c;
    whites[i] = rc(r, c);
    matrix[r][c].type = BIANCO;
  }
  // Checking if a rectangle could be the optimal solution
  if (W <= 8 && B <= 4) {
    if (checkForRectangles(&out) == B + W)
      return 0;
  }
  // Placing the walls
  placeWalls();

  out << endl;
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < m; j++) {
      out << matrix[i][j].type;
    }
    out << endl;
  }
  out.close();
  in.close();
  return 0;
}

int checkForRectangles(ofstream *out) {
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

void placeWalls() {
  bool addedSomeWalls;
  int border = 0;
  do {
    addedSomeWalls = false;
    int n1 = n - border;
    int m1 = m - border;
    for (int j = border; j < m1; j++) { // BLUE  TOP
      cell c = matrix[border][j];
      if (c.type == BIANCO && c.U == false) {
        matrix[border][j].D = false;
        matrix[border + 1][j].U = false;
        addedSomeWalls = true;
      }
    }
    for (int i = border; i < n1; i++) { // BLUE  RIGHT
      cell c = matrix[i][m1 - 1];
      if (c.type == BIANCO && c.R == false) {
        matrix[i][m1 - 1].L = false;
        matrix[i][m1 - 2].R = false;
        addedSomeWalls = true;
      }
    }
    for (int j = border; j < m1; j++) { // BLUE  BOTTOM
      cell c = matrix[n1 - 1][j];
      if (c.type == BIANCO && c.D == false) {
        matrix[n1 - 1][j].U = false;
        matrix[n1 - 2][j].D = false;
        addedSomeWalls = true;
      }
    }
    for (int i = border; i < n1; i++) { // BLUE LEFT
      cell c = matrix[i][border];
      if (c.type == BIANCO && c.L == false) {
        matrix[i][border].R = false;
        matrix[i][border + 1].L = false;
        addedSomeWalls = true;
      }
    }
    border++;
  } while (addedSomeWalls);

  for (int i = 1; i < n; i++) // GREEN VERTICAL
    for (int j = 0; j < m; j++)
      if (matrix[i][j].type == BIANCO && matrix[i - 1][j].type == BIANCO &&
          matrix[i - 1][j].D == true) {
        // Laterali (verticali)
        matrix[i][j].L = matrix[i][j].R = false;
        matrix[i - 1][j].L = matrix[i - 1][j].R = false;
        if (j + 1 < m)
          matrix[i - 1][j + 1].L = matrix[i][j + 1].L = false;
        if (j - 1 > 0)
          matrix[i - 1][j - 1].R = matrix[i][j - 1].R = false;
        if (i - 2 >= 0) {
          matrix[i - 2][j].U = false;
          if (i - 3 >= 0)
            matrix[i - 3][j].D = false;
        }
        if (i + 1 < n) {
          matrix[i + 1][j].D = false;
          if (i + 2 < n)
            matrix[i + 2][j].U = false;
        }
      }
  for (int j = 1; j < m; j++) // GREEN HORIZONTAL
    for (int i = 0; i < n; i++)
      if (matrix[i][j].type == BIANCO && matrix[i][j - 1].type == BIANCO &&
          matrix[i][j - 1].R == true) {
        // Laterali (orizzontali)
        matrix[i][j].U = matrix[i][j].D = false;
        matrix[i][j - 1].U = matrix[i][j - 1].D = false;
        if (i - 1 >= 0)
          matrix[i - 1][j].D = matrix[i - 1][j - 1].D = false;
        if (i + 1 < n)
          matrix[i + 1][j].U = matrix[i + 1][j - 1].U = false;
        if (j - 2 >= 0) {
          matrix[i][j - 2].L = false;
          if (j - 3 >= 0)
            matrix[i][j - 3].R = false;
        }
        if (j + 1 < m) {
          matrix[i][j + 1].R = false;
          if (j + 2 < m)
            matrix[i][j + 2].L = false;
        }
      }
  // TODO: rosa
}

// Ritorna il numero di anelli attraversati
int solveRec(ofstream *out, rc currentPosition, rc arrivo, stringbuf mosse,
             int anelliAttraversati, char previousMove) {
  if (previousMove == '\0') {
    // start up
    if (solveRec(out, rc(currentPosition.first, currentPosition.second), arrivo,
                 mosse, anelliAttraversati,
                 'U') == whites.size() + blacks.size()) {
    }
  }
  if (currentPosition.first == arrivo.first &&
      currentPosition.second == arrivo.second) {
    // TODO printsol();
    return anelliAttraversati;
  } else {
    cell c = matrix[currentPosition.first][currentPosition.second];
    if (c.U) {
    }
    if (c.D) {
    }
    if (c.L) {
    }
    if (c.R) {
    }
  }
}

void solve(ofstream *out) {
  // for (rc r : blacks) {
  //   int bianchiVicini = 0;
  //   if (r.first >= 2 && matrix[r.first - 1][r.second].type == BIANCO &&
  //       matrix[r.first - 1][r.second].D &&
  //       (r.first + 1 == n ||
  //        (r.first + 1 < n &&
  //         matrix[r.first + 1][r.second].type != BIANCO))) { // bianco sopra
  //         nero
  //     if (r.second >= 2 && matrix[r.first][r.second - 1].type == BIANCO &&
  //         matrix[r.first][r.second - 1].R &&
  //         ((r.second + 1 < m && matrix[r.first][r.second + 1].type != BIANCO)
  //         ||
  //          r.second + 1 == m)) { // secondo bianco a sx
  //       /*
  //          w
  //         wb
  //       */
  //       matrix[r.first][r.second - 1].U = false;
  //       matrix[r.first][r.second - 1].D = false;
  //       matrix[r.first - 1][r.second].R = false;
  //       matrix[r.first - 1][r.second].L = false;
  //       matrix[r.first - 1][r.second - 1].R = false;
  //       matrix[r.first - 1][r.second - 1].D = false;
  //       matrix[r.first][r.second].R = false;
  //       matrix[r.first][r.second].D = false;
  //       if (r.first - 2 >= 0) {
  //         matrix[r.first - 2][r.second].U = false;
  //         if (r.first - 3 >= 0) {
  //           matrix[r.first - 3][r.second].D = false;
  //         }
  //       }
  //       if (r.second - 2 >= 0) {
  //         matrix[r.first][r.second - 2].L = false;
  //         if (r.second - 3 >= 0) {
  //           matrix[r.first][r.second - 3].R = false;
  //         }
  //       }
  //       if (r.first + 1 < n) {
  //         matrix[r.first + 1][r.second].U = false;
  //         matrix[r.first + 1][r.second - 1].U = false;
  //       }
  //       if (r.second + 1 < m) {
  //         matrix[r.first][r.second + 1].L = false;
  //         matrix[r.first - 1][r.second + 1].L = false;
  //       }
  rc r = rc(n / 2, m / 2);
  solveRec(out, r, r, stringbuf(), 0, '\0');
  //     } else if (r.second + 2 < m &&
  //                matrix[r.first][r.second + 1].type == BIANCO &&
  //                matrix[r.first][r.second + 1].L &&
  //                ((r.second - 1 >= 0 &&
  //                  matrix[r.first][r.second - 1].type != BIANCO) ||
  //                 r.second == 0)) { // secondo bianco a dx
  //       /* w
  //          bw
  //       */
  //       matrix[r.first][r.second + 1].U = false;
  //       matrix[r.first][r.second + 1].D = false;
  //       matrix[r.first - 1][r.second].R = false;
  //       matrix[r.first - 1][r.second].L = false;
  //       matrix[r.first - 1][r.second + 1].L = false;
  //       matrix[r.first - 1][r.second + 1].D = false;
  //       matrix[r.first][r.second].L = false;
  //       matrix[r.first][r.second].D = false;
  //       if (r.first - 2 >= 0) {
  //         matrix[r.first - 2][r.second].U = false;
  //         if (r.first - 3 >= 0) {
  //           matrix[r.first - 3][r.second].D = false;
  //         }
  //       }
  //       if (r.second + 2 < m) {
  //         matrix[r.first][r.second + 2].R = false;
  //         if (r.second + 3 < m) {
  //           matrix[r.first][r.second + 3].L = false;
  //         }
  //       }
  //       if (r.first + 1 < n) {
  //         matrix[r.first + 1][r.second].U = false;
  //         matrix[r.first + 1][r.second + 1].U = false;
  //       }
  //       if (r.second - 1 >= 0) {
  //         matrix[r.first][r.second - 1].L = false;
  //         matrix[r.first - 1][r.second - 1].L = false;
  //       }
  //       solveRec(out, r, r, stringbuf(), 0, '\0');
  //     }
  //   }
  // }
}
