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

// Checks for an optimal rectangular solution. If it is found it is printed and
// the program exits w/ code 0. If a solution is found but it's not optimal it
// gets printed anyway. Returns ring count w/ a rectangular solution.
int checkForRectangles();

// Finds nearest non-visited ring
rc findNear(rc pos);
vector<pair<rc, string>> esciDaNero(rc nero, char dir);
vector<pair<rc, string>> entraInNero(rc nero, rc other);
vector<pair<rc, string>> esciDaBianco(rc bianco, char dir, char prevDir);
vector<pair<rc, string>> entraInBianco(rc bianco);
void goRadar();
string ricostruisciPercorso(stack<rc> celle);
string findPath(rc from, rc to);

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

  cout << findPath(rc(4, 4), rc(4, 5));
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

vector<pair<rc, string>> esciDaNero(rc nero, char dir) {
  vector<pair<rc, string>> res = vector<pair<rc, string>>();
  if (dir == 'U' || dir == 'D') {
    if (nero.second - 2 > 0 && !matrix[nero.first][nero.second - 1].vis &&
        !matrix[nero.first][nero.second - 2].vis) {
      res.push_back(pair<rc, string>(rc(nero.first, nero.second - 2), "LL"));
    }
    if (nero.second + 2 < n && !matrix[nero.first][nero.second + 1].vis &&
        !matrix[nero.first][nero.second + 2].vis) {
      res.push_back(pair<rc, string>(rc(nero.first, nero.second + 2), "RR"));
    }
  } else {
    if (nero.first - 2 > 0 && !matrix[nero.first - 1][nero.second].vis &&
        !matrix[nero.first - 2][nero.second].vis) {
      res.push_back(pair<rc, string>(rc(nero.first - 2, nero.second), "UU"));
    }
    if (nero.first + 2 < n && !matrix[nero.first + 1][nero.second].vis &&
        !matrix[nero.first + 2][nero.second].vis) {
      res.push_back(pair<rc, string>(rc(nero.first + 2, nero.second), "DD"));
    }
  }
  return res;
}

vector<pair<rc, string>> entraInNero(rc nero, rc other) {
  vector<pair<rc, string>> sol = vector<pair<rc, string>>();
  if (nero.first - 1 > 0) // top
    if (!matrix[nero.first - 1][nero.second].vis)
      if (!matrix[nero.first - 2][nero.second].vis)
        sol.push_back(pair<rc, string>(rc(nero.first - 2, nero.second), "DD"));
  if (nero.second - 1 > 0) // left
    if (!matrix[nero.first][nero.second - 1].vis)
      if (!matrix[nero.first][nero.second - 2].vis)
        sol.push_back(pair<rc, string>(rc(nero.first, nero.second - 2), "RR"));
  if (nero.first + 2 < n) // bottom
    if (!matrix[nero.first + 1][nero.second].vis)
      if (!matrix[nero.first + 2][nero.second].vis)
        sol.push_back(pair<rc, string>(rc(nero.first + 2, nero.second), "UU"));
  if (nero.second + 2 < m) { // left
    if (!matrix[nero.first][nero.second + 1].vis) {
      if (!matrix[nero.first][nero.second + 2].vis) {
        sol.push_back(pair<rc, string>(rc(nero.first, nero.second + 2), "LL"));
        return sol;
      }
    }
  }
}

vector<pair<rc, string>> esciDaBianco(rc bianco, char dir, char prevDir) {
  vector<pair<rc, string>> sol = vector<pair<rc, string>>();
  if (dir != prevDir) { // entra con curva, esci dritto
    if (dir == 'U' && bianco.second - 1 > 0 &&
        !matrix[bianco.first][bianco.second - 1].vis) {
      sol.push_back(pair<rc, string>(rc(bianco.first, bianco.second - 1), "U"));

    } else if (dir == 'D' && bianco.second + 1 < m &&
               !matrix[bianco.first][bianco.second + 1].vis) {
      sol.push_back(pair<rc, string>(rc(bianco.first, bianco.second + 1), "D"));

    } else if (dir == 'R' && bianco.first + 1 < n &&
               !matrix[bianco.first + 1][bianco.second].vis) {
      sol.push_back(pair<rc, string>(rc(bianco.first + 1, bianco.second), "R"));

    } else if (dir == 'L' && bianco.first - 1 > 0 &&
               !matrix[bianco.first - 1][bianco.second].vis) {
      sol.push_back(pair<rc, string>(rc(bianco.first - 1, bianco.second), "L"));
    }
  } else {
    if (dir == 'U' && bianco.second - 1 > 0 &&
        !matrix[bianco.first][bianco.second - 1].vis) {
      if (bianco.first - 1 > 0 &&
          !matrix[bianco.first - 1][bianco.second - 1].vis)
        sol.push_back(
            pair<rc, string>(rc(bianco.first, bianco.second - 1), "UL"));
      if (bianco.first + 1 < n &&
          !matrix[bianco.first - 1][bianco.second - 1].vis)
        sol.push_back(
            pair<rc, string>(rc(bianco.first, bianco.second - 1), "UR"));

    } else if (dir == 'D' && bianco.second + 1 < m &&
               !matrix[bianco.first][bianco.second + 1].vis) {
      if (bianco.first - 1 > 0 &&
          !matrix[bianco.first - 1][bianco.second + 1].vis)
        sol.push_back(
            pair<rc, string>(rc(bianco.first - 1, bianco.second + 1), "DL"));
      if (bianco.first + 1 < n &&
          !matrix[bianco.first + 1][bianco.second + 1].vis)
        sol.push_back(
            pair<rc, string>(rc(bianco.first + 1, bianco.second + 1), "DL"));

    } else if (dir == 'R' && bianco.first + 1 < n &&
               !matrix[bianco.first + 1][bianco.second].vis) {
      if (bianco.second - 1 > 0 &&
          !matrix[bianco.first + 1][bianco.second - 1].vis)
        sol.push_back(
            pair<rc, string>(rc(bianco.first + 1, bianco.second - 1), "RD"));
      if (bianco.second + 1 < m &&
          !matrix[bianco.first + 1][bianco.second + 1].vis)
        sol.push_back(
            pair<rc, string>(rc(bianco.first + 1, bianco.second + 1), "RU"));

    } else if (dir == 'L' && bianco.first - 1 > 0 &&
               !matrix[bianco.first - 1][bianco.second].vis) {
      if (bianco.second - 1 > 0 &&
          !matrix[bianco.first + 1][bianco.second - 1].vis)
        sol.push_back(
            pair<rc, string>(rc(bianco.first - 1, bianco.second - 1), "LU"));
      if (bianco.second + 1 < m &&
          !matrix[bianco.first + 1][bianco.second + 1].vis)
        sol.push_back(
            pair<rc, string>(rc(bianco.first - 1, bianco.second + 1), "LU"));
    }
  }
  return sol;
}

vector<pair<rc, string>> entraInBianco(rc bianco) {
  vector<pair<rc, string>> sol = vector<pair<rc, string>>();
  int row = bianco.first;
  int col = bianco.second;
  rc tl, tr, bl, br;
  tl = tr = bl = br = bianco;
  tl.first--;
  tl.second--;
  tr.first--;
  tr.second++;
  bl.first++;
  bl.second--;
  br.first++;
  br.second++;
  bool arriveFromTop, arriveFromBottom, arriveFromLeft, arriveFromRight;
  arriveFromTop = arriveFromBottom = arriveFromLeft = arriveFromRight = false;
  if (tl.first >= 0 && tr.second >= 0) { // TL
    if (!matrix[row - 1][col].vis) {     // tl->t
      sol.push_back(pair<rc, string>(tl, "RD"));
      arriveFromTop = true;
      arriveFromLeft = true;
    }
    if (!matrix[row][col - 1].vis) { // tl->l
      arriveFromTop = true;
      arriveFromLeft = true;
      sol.push_back(pair<rc, string>(tl, "DR"));
    }
  }
  if (tr.first >= 0 && tr.first < n && tr.second < m) { // TR
    if (!matrix[row - 1][col].vis) {                    // tr->t
      sol.push_back(pair<rc, string>(tr, "LD"));
      arriveFromTop = true;
      arriveFromRight = true;
    }
    if (!matrix[row][col + 1].vis) { // tr->r
      sol.push_back(pair<rc, string>(tr, "DL"));
      arriveFromTop = true;
      arriveFromRight = true;
    }
  }
  if (bl.first < n && bl.second >= 0) { // BL
    if (!matrix[row + 1][col].vis) {    // bl->b
      sol.push_back(pair<rc, string>(bl, "RU"));
      arriveFromBottom = true;
      arriveFromLeft = true;
    }
    if (!matrix[row][col - 1].vis) { // bl->l
      sol.push_back(pair<rc, string>(bl, "UR"));
      arriveFromBottom = true;
      arriveFromLeft = true;
    }
  }
  if (br.first < n && br.second < m) { // BR
    if (!matrix[row + 1][col].vis) {   // br->b
      sol.push_back(pair<rc, string>(br, "LU"));
      arriveFromBottom = true;
      arriveFromRight = true;
    }
    if (!matrix[row][col + 1].vis) { // br->r
      sol.push_back(pair<rc, string>(br, "UL"));
      arriveFromBottom = true;
      arriveFromRight = true;
    }
  }
  if (!arriveFromTop && row > 0 && !matrix[row - 1][col].vis)
    sol.push_back(pair<rc, string>(rc(row - 1, col), "D"));
  if (!arriveFromBottom && row + 1 < n && !matrix[row + 1][col].vis)
    sol.push_back(pair<rc, string>(rc(row + 1, col), "U"));
  if (!arriveFromLeft && col > 0 && !matrix[row][col - 1].vis)
    sol.push_back(pair<rc, string>(rc(row, col - 1), "R"));
  if (!arriveFromRight && col + 1 < m && !matrix[row][col + 1].vis)
    sol.push_back(pair<rc, string>(rc(row, col + 1), "L"));
  return sol;
}

int getDistance(rc from, rc to) {
  if (from.first < 0 || from.first >= n || to.first < 0 || to.first >= m)
    return 9999999;
  return pow(from.first - to.first, 2) + pow(from.second - to.second, 2);
}

string findPath(rc from, rc to) {
  priority_queue<distRC, vector<distRC>, dist_comp> pq =
      priority_queue<distRC, vector<distRC>, dist_comp>();
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
  pq.push(distRC(baseDistance, current));
  do {
    const distRC distanzaECellaPiuVicina = pq.top();
    current = distanzaECellaPiuVicina.second;
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
            pq.push(distRC(distTT, tt));
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
            pq.push(distRC(distrRR, rr));
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
            pq.push(distRC(distLL, ll));
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
            pq.push(distRC(distBB, bb));
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
    vector<distRC> smaller = vector<distRC>();
    distRC s;
    while (1) {
      s = pq.top();
      pq.pop();
      if (s.first != distanzaECellaPiuVicina.first ||
          s.second.first != distanzaECellaPiuVicina.second.first ||
          s.second.second != distanzaECellaPiuVicina.second.second)
        smaller.push_back(s);
      else
        break;
    }
    int smsize = smaller.size();
    for (int i = 0; i < smsize; i++) {
      pq.push(smaller[smsize - i - 1]);
      smaller.pop_back();
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
