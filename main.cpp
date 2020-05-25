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

struct dist_comp {
  bool operator()(const distRC &lhs, const distRC &rhs) const {
    if (lhs.first == rhs.first)
      return !(lhs.second.first == rhs.second.first &&
               lhs.second.second == rhs.second.second);
    return lhs.first <= rhs.first;
  }
};

// Cell data structure
typedef struct {
  bool U = true;  // Can go up from here
  bool D = true;  // Can go down from here
  bool L = true;  // Can go left from here
  bool R = true;  // Can go right from here
  short type = 0; // 0=vuota,1=bianca, 2=nera
  bool vis = false;
} cell;

// Globals
vector<vector<cell>> matrix;
ushort n, m;
vector<rc> blacks, whites;
ofstream out("output.txt");
ifstream in("input.txt");
stack<bitset<24>> states;

string ricostruisciPercorso(stack<rc> celle);
string pathNeri(rc from, rc to, char dir);
string pathNeroToBianco(rc from, rc to);
string pathBiancoToNero(rc from, rc to);
string pathB2B(rc from, rc to);
string findPath(rc from, rc to);

// Checks for an optimal rectangular solution. If it is found it is printed and
// the program exits w/ code 0. If a solution is found but it's not optimal it
// gets printed anyway. Returns ring count w/ a rectangular solution.
int checkForRectangles();
void goRadar();
void placeWalls();

// Finds nearest non-visited ring
rc findNear(rc pos);

int main() {
  int B, W;
  // Initialization
  in >> n >> m >> B >> W;
  matrix = vector<vector<cell>>(n, vector<cell>(m, cell()));
  // Forbidding invalid moves
  for (int i = 0; i < m; i++)
    matrix[0][i].U = false;
  for (int i = 0; i < n; i++) {
    matrix[i][0].L = false;
    matrix[i][m - 1].R = false;
  }
  for (int i = 0; i < m; i++)
    matrix[n - 1][i].D = false;
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
    // if (checkForRectangles() == B + W)
    //   return 0;
  }
  placeWalls();
  matrix[0][1].vis = true;
  matrix[1][2].vis = true;
  matrix[2][3].vis = true;
  matrix[2][1].vis = true;
  matrix[3][3].vis = true;
  matrix[4][3].vis = true;
  matrix[5][3].vis = true;

  cout << findPath(rc(4, 4), rc(0, 0));
  // Go
  goRadar();

  // Will probably never get here, such a waste
  out.close();
  in.close();
  return 0;
}

int checkForRectangles() {
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
  out << anelli << " " << s.length() << " " << topLeft.first << " "
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

void goRadar() {
  for (int i = 0; i < n; i++)
    for (int j = 0; j < m; j++)
      matrix[i][j].vis = false;
  rc cur = blacks[0];
  vector<bool> visited = vector<bool>(blacks.size());
}

rc findNear(rc pos) {
  rc bestCell = rc(n, m);
  float bestDistance = 99999999;
  for (int misuraLato = 1; misuraLato <= max(n, m); misuraLato++) {
    if (bestCell.first != n)
      return bestCell;

    if (pos.first - misuraLato >= 0) { // top
      int row = pos.first - misuraLato;
      for (int i = -misuraLato; i <= misuraLato; i++) {
        cell c = matrix[row][pos.second + i];
        if (c.type != VUOTO && !c.vis) {
          float distance =
              pow(pos.first - row, 2) + pow(pos.second - pos.second + i, 2);
          if (distance < bestDistance) {
            bestDistance = distance;
            bestCell = rc(row, pos.second + i);
          }
        }
      }
    }
    if (pos.first + misuraLato < n) { // bottom
      int row = pos.first + misuraLato;
      for (int i = -misuraLato; i <= misuraLato; i++) {
        cell c = matrix[row][pos.second + i];
        if (c.type != VUOTO && !c.vis) {
          float distance =
              pow(pos.first - row, 2) + pow(pos.second - pos.second + i, 2);
          if (distance < bestDistance) {
            bestDistance = distance;
            bestCell = rc(row, pos.second + i);
          }
        }
      }
    }
    if (pos.second - misuraLato >= 0) { // left
      int col = pos.second - misuraLato;
      for (int i = -misuraLato + 1; i < misuraLato; i++) {
        cell c = matrix[pos.first + i][col];
        if (c.type != VUOTO && !c.vis) {
          float distance =
              pow(pos.first - pos.first + i, 2) + pow(pos.second - col, 2);
          if (distance < bestDistance) {
            bestDistance = distance;
            bestCell = rc(pos.first + i, col);
          }
        }
      }
    }
    if (pos.second + misuraLato < m) { // right
      int col = pos.second + misuraLato;
      for (int i = -misuraLato + 1; i < misuraLato; i++) {
        cell c = matrix[pos.first + i][col];
        if (c.type != VUOTO && !c.vis) {
          float distance =
              pow(pos.first - pos.first + i, 2) + pow(pos.second - col, 2);
          if (distance < bestDistance) {
            bestDistance = distance;
            bestCell = rc(pos.first + i, col);
          }
        }
      }
    }
  }
  return pos;
}

string makePath(rc from, rc to, char dirArrivatiAFrom) {
  stringbuf percorsoParz = stringbuf();
  if (matrix[from.first][from.second].type == NERO &&
      matrix[to.first][to.second].type == NERO) {
    return pathNeri(from, to, dirArrivatiAFrom);
  } else if (matrix[from.first][from.second].type == NERO &&
             matrix[to.first][to.second].type == BIANCO) {
    return pathNeroToBianco(from, to);
  } else if (matrix[from.first][from.second].type == BIANCO &&
             matrix[to.first][to.second].type == NERO) {
    return pathBiancoToNero(from, to);
  } else
    return pathB2B(from, to);

  if (from.first == to.first) { // stessa riga
    int dist = from.second - to.second;
    if (dist > 0) {
      for (int i = 0; i < dist; i++) {
        percorsoParz.sputc('L');
      }
    } else {
      dist *= -1;
      for (int i = 0; i < dist; i++) {
        percorsoParz.sputc('R');
      }
    }
  }

  if (from.second == to.second) { // stessa colonna
    int dist = from.first - to.first;
    if (dist > 0) {
      for (int i = 0; i < dist; i++) {
        percorsoParz.sputc('U');
      }
    } else {
      dist *= -1;
      for (int i = 0; i < dist; i++) {
        percorsoParz.sputc('D');
      }
    }
  }

  int distVert = from.second - to.second;
  int distOriz = from.first - to.first;

  bool sopra, sotto, dx, sx;
  sopra = distVert > 0;
  sotto = distVert < 0;
  dx = distOriz < 0;
  sx = distOriz > 0;

  distVert = abs(distVert) / 2;
  distOriz = abs(distOriz) / 2;

  if (sopra && dx) {
  }
}

string pathNeri(rc from, rc to, char dir) {
  stringbuf result = stringbuf();
  int distVert = from.second - to.second;
  int distOriz = from.first - to.first;
  bool sopra, sotto, dx, sx;
  sopra = distVert > 0;
  sotto = distVert < 0;
  dx = distOriz < 0;
  sx = distOriz > 0;

  distVert = abs(distVert) / 2;
  distOriz = abs(distOriz) / 2;

  if (sopra && dx) {
    if (dir == 'R') {
    }
  }
}
string pathNeroToBianco(rc from, rc to) {}
string pathBiancoToNero(rc from, rc to) {}
string pathB2B(rc from, rc to) {}

int getDistance(rc from, rc to) {
  if (from.first < 0 || from.first >= n || to.first < 0 || to.first >= m)
    return 9999999;
  return pow(from.first - to.first, 2) + pow(from.second - to.second, 2);
}

string findPath(rc from, rc to) {
  set<distRC, dist_comp> pq = set<distRC, dist_comp>();
  rc current = from;
  int baseDistance = getDistance(from, to);
  int maxDist = 2 * baseDistance - 1; // at max go this far
  bool visitedHere[n][m];
  for (int i = 0; i < n; i++)
    for (int j = 0; j < m; j++)
      visitedHere[i][j] = false;
  rc parent[n][m];
  parent[from.first][from.second] = from;
  visitedHere[from.first][from.second] = true;
  pq.insert(distRC(baseDistance, current));
  do {
    const distRC distancaECellaPiuVicina = *pq.begin();
    current = distancaECellaPiuVicina.second;
    int pqSizeBefore = pq.size();
    {                          // aggiunge celle possibili
      if (current.first > 0) { // top
        {                      // top left
          // if (current.second > 0) { // top left
          //   rc tl = rc(current.first - 1, current.second - 1);
          //   if (!matrix[tl.first][tl.second].vis) {
          //     int distTL = getDistance(tl, to);
          //     pq.insert(distRC(distTL, tl));
          //   }
          // }
          // top center
        }
        rc tt = rc(current.first - 1, current.second);
        if (!matrix[tt.first][tt.second].vis) {
          int distTT = getDistance(tt, to);
          if (!visitedHere[tt.first][tt.second] && distTT <= maxDist) {
            pq.insert(distRC(distTT, tt));
            parent[tt.first][tt.second] = current;
            visitedHere[tt.first][tt.second] = true;
          }
        }
        { // top right
          // if (current.second + 1 < m) { // top right
          //   rc tr = rc(current.first - 1, current.second + 1);
          //   if (!matrix[tr.first][tr.second].vis) {
          //     int distrRR = getDistance(tr, to);
          //     pq.insert(distRC(distrRR, tr));
          //   }
          // }
        }
      }
      if (current.second + 1 < m) { // right
        if (!matrix[current.first][current.second + 1].vis) {
          rc rr = rc(current.first, current.second + 1);
          int distrRR = getDistance(rr, to);
          if (!visitedHere[rr.first][rr.second] && distrRR <= maxDist) {
            pq.insert(distRC(distrRR, rr));
            parent[rr.first][rr.second] = current;
            visitedHere[rr.first][rr.second] = true;
          }
        }
      }
      if (current.second > 0) { // left
        if (!matrix[current.first][current.second - 1].vis) {
          rc ll = rc(current.first, current.second - 1);
          int distLL = getDistance(ll, to);
          if (!visitedHere[ll.first][ll.second] && distLL <= maxDist) {
            pq.insert(distRC(distLL, ll));
            parent[ll.first][ll.second] = current;
            visitedHere[ll.first][ll.second] = true;
          }
        }
      }
      if (current.first + 1 < n) { // bottom
        {                          // bottom left
          // if (current.second > 0) {  // bl
          //   rc bl = rc(current.first + 1, current.second - 1);
          //   if (!matrix[bl.first][bl.second].vis) {
          //     int distBL = getDistance(bl, to);
          //     pq.insert(distRC(distBL, bl));
          //   }
          // }
        }
        // center
        rc bb = rc(current.first + 1, current.second);
        if (!matrix[bb.first][bb.second].vis) {
          int distBB = getDistance(bb, to);
          if (!visitedHere[bb.first][bb.second] && distBB <= maxDist) {
            pq.insert(distRC(distBB, bb));
            parent[bb.first][bb.second] = current;
            visitedHere[bb.first][bb.second] = true;
          }
        }
        { // bottom right
          // if (current.second + 1 < m) { // br
          //   rc br = rc(current.first + 1, current.second + 1);
          //   if (!matrix[br.first][br.second].vis) {
          //     int distBR = getDistance(br, to);
          //     pq.insert(distRC(distBR, br));
          //   }
          // }
        }
      }
    }
    pq.erase(distancaECellaPiuVicina);
    rc cursore = current;
    { // TODO toglimi, stampa debug
      stack<rc> percorsoBackwards = stack<rc>();
      do {
        percorsoBackwards.push(cursore);
        cursore = parent[cursore.first][cursore.second];
      } while (cursore.first != from.first || cursore.second != from.second);
      percorsoBackwards.push(from);
      out << ricostruisciPercorso(percorsoBackwards) << endl;
    }
  } while (!pq.empty() &&
           (current.first != to.first || current.second != to.second));
  if (current.first == to.first && current.second == to.second) {
    stack<rc> percorsoBackwards = stack<rc>();
    do {
      percorsoBackwards.push(current);
      current = parent[current.first][current.second];
    } while (current.first != from.first || current.second != from.second);
    percorsoBackwards.push(from);
    return ricostruisciPercorso(percorsoBackwards);
  } else
    return "paccone";
}

string ricostruisciPercorso(stack<rc> celle) {
  int numCelle = celle.size();
  stringbuf res = stringbuf();
  rc cella = celle.top();
  celle.pop();
  for (int i = 0; i < numCelle - 1; i++) {
    matrix[cella.first][cella.second].vis = true;
    rc nuova = celle.top();
    celle.pop();
    if (cella.first < nuova.first)
      res.sputc('D');
    else if (cella.first != nuova.first)
      res.sputc('U');
    else if (cella.second < nuova.second)
      res.sputc('R');
    else
      res.sputc('L');
    cella = nuova;
  }
  matrix[cella.first][cella.second].vis = true;
  return res.str();
}