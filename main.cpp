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

// Globals
vector<vector<cell>> matrix;
ushort n, m;
vector<rc> blacks, whites;
ofstream out("output.txo");
ifstream in("input.txt");
stringbuf sb;

// Checks for an optimal rectangular solution. If it is found it is printed and
// the program exits w/ code 0. If a solution is found but it's not optimal it
// gets printed anyway. Returns ring count w/ a rectangular solution.
int checkForRectangles();

// Modifies matrix "placing walls" (forbidding moves that would surely lead into
// darkness)
void placeWalls();

// Search for a solution via backtracking
void solve();

int main(int argc, char **argv) {
  int B, W;
  // Initialization
  in >> n >> m >> B >> W;
  matrix = vector<vector<cell>>(n, vector<cell>(m, cell()));
  sb = stringbuf();
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
    if (checkForRectangles() == B + W)
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

// Safe method that tries to put a wall in a given cell returning the previous
// value of the wall:
// -true  if there was not a wall or the cell is outside the matrix
// -false if the wall was already there
bool wallIfPossible(int row, int column, char directionToWall) {
  bool prevWallState = true;
  if (row >= 0 && row < n && column >= 0 && column < m)
    switch (directionToWall) {
    case 'U':
      prevWallState = matrix[row][column].U;
      matrix[row][column].U = false;
      break;
    case 'D':
      prevWallState = matrix[row][column].D;
      matrix[row][column].D = false;
      break;
    case 'L':
      prevWallState = matrix[row][column].L;
      matrix[row][column].L = false;
      break;
    case 'R':
      prevWallState = matrix[row][column].R;
      matrix[row][column].R = false;
      break;

    default:
      perror("invalid direction");
      exit(1);
    }
  return prevWallState;
}

// Sets matrix[row][column] to prevState if possible
void restorePrevWallState(int row, int column, char dir, bool prevState) {
  if (row >= 0 && row < n && column >= 0 && column < m)
    switch (dir) {
    case 'U':
      matrix[row][column].U = prevState;
      break;
    case 'D':
      matrix[row][column].D = prevState;
      break;
    case 'L':
      matrix[row][column].L = prevState;
      break;
    case 'R':
      matrix[row][column].R = prevState;
      break;

    default:
      perror("invalid direction");
      exit(1);
    }
}

// Ritorna il numero di anelli attraversati
int solveRec(rc currentPosition, rc arrivo, int anelliAttraversati,
             char previousMove, char prevPrevMove) {
  if (prevPrevMove == '\0') { // 1st move
    // start up
    int anelliTot = whites.size() + blacks.size();
    if (matrix[currentPosition.first][currentPosition.second].U) {
      switch (matrix[currentPosition.first - 1][currentPosition.second].type) {
      case VUOTO:
        bool previous = wallIfPossible(currentPosition.first - 1,
                                       currentPosition.second, 'D');
        sb.sputc('U');
        if (solveRec(rc(currentPosition.first - 1, currentPosition.second),
                     arrivo, anelliAttraversati, 'U',
                     previousMove) == anelliTot)
          return 0;
        sb.stossc();
        restorePrevWallState(currentPosition.first - 1, currentPosition.second,
                             'D', previous);
        break;

      default:
        break;
      }
    }
    // if (solveRec(rc(currentPosition.first, currentPosition.second), arrivo,
    //              mosse, anelliAttraversati,
    //              'U') == whites.size() + blacks.size()) {
    // }
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

void solve() {
  rc start = rc(n / 2, n / 2);
  while (matrix[start.first][start.second].type != VUOTO)
    start = rc(rand() % n, rand() % m);
  /*
  In ciascuna posizione i valori del muro prima.
  Ne bastano 6 e non 7 perché quello nella direz in cui arrivo lo aggiungo
  sempre
          ----------
          |   4    |
          |        |
          |        |
          ----------
          |   3    |
          1        2
          | sempre |
          ----------
          |        |
          5   ↑    6
          |current |

  */
  stack<bool[6]> prevState = stack<bool[6]>();
  stack<char> stateMoves = stack<char>();
  stack<char> moves = stack<char>(); // most probably can use a 1024 bool array
  int bestPossible = whites.size() + blacks.size();
  int bestFound = 0;
  { // insert initial moves
    if (matrix[start.first][start.second].U)
      moves.push('U');
    if (matrix[start.first][start.second].L)
      moves.push('L');
    if (matrix[start.first][start.second].D)
      moves.push('D');
    if (matrix[start.first][start.second].R)
      moves.push('R');
  }
  rc currentCell = rc(start.first, start.second);
  char previousMove;
  bool mustGoStraight = false; // for blacks
  do {
    rc nextCell = rc(currentCell.first, currentCell.second);
    // Direzione in cui da current arrivo a next
    char dir;
    { // get next direction from the queue and initialize nextCell
      dir = moves.top();
      moves.pop();
      if (dir != 'T') {
        restoreState(prevState, stateMoves, currentCell);
      } else {
        prevState.pop();
        stateMoves.pop();
        continue;
      }
      { // initialize nextCell to correct value
        switch (dir) {
        case 'U':
          nextCell.first--;
          break;
        case 'D':
          nextCell.first++;
          break;
        case 'L':
          nextCell.second--;
          break;
        case 'R':
          nextCell.second++;
          break;
        }
      }
    }

    switch (matrix[nextCell.first][nextCell.second].type) { // limit next moves
    case BIANCO: {
      if (previousMove == dir) { // Did not turn before crossing
        switch (dir) {           // must turn afterwards
        case 'U':
          wallIfPossible(nextCell.first - 1, nextCell.second, 'U');
          break;
        case 'D':
          wallIfPossible(nextCell.first + 1, nextCell.second, 'D');
          break;
        case 'R':
          wallIfPossible(nextCell.first, nextCell.second + 1, 'R');
          break;
        case 'L':
          wallIfPossible(nextCell.first, nextCell.second - 1, 'L');
          break;
        }
      }
      bool canContinue = false;
      switch (dir) {
      case 'U':
        canContinue = matrix[nextCell.first][nextCell.second].U;
        break;
      case 'D':
        canContinue = matrix[nextCell.first][nextCell.second].D;
        break;
      case 'L':
        canContinue = matrix[nextCell.first][nextCell.second].L;
        break;
      case 'R':
        canContinue = matrix[nextCell.first][nextCell.second].R;
        break;
      }
      if (canContinue)
        moves.push(dir); // can only go straight
      break;
    }
    case NERO: {
      mustGoStraight = true;
      wallIfPossible(nextCell.first, nextCell.second, dir); // must turn
      if (matrix[nextCell.first][nextCell.second].R) {
        moves.push('R');
      }
      if (matrix[nextCell.first][nextCell.second].L) {
        moves.push('L');
      }
      break;
    }
    case VUOTO: {
      if (mustGoStraight) {
        moves.push(dir);
        mustGoStraight = false;
      } else { // can go anywhere he wants
        if (matrix[nextCell.first][nextCell.second].R)
          moves.push('R');
        if (matrix[nextCell.first][nextCell.second].L)
          moves.push('L');
        if (matrix[nextCell.first][nextCell.second].D)
          moves.push('D');
        if (matrix[nextCell.first][nextCell.second].U)
          moves.push('U');
      }
      break;
    }
    }
    { // prevent going backwards
      switch (dir) {
      case 'U':
        wallIfPossible(nextCell.first, nextCell.second, 'D');
        break;
      case 'D':
        wallIfPossible(nextCell.first, nextCell.second, 'U');
        break;
      case 'L':
        wallIfPossible(nextCell.first, nextCell.second, 'R');
        break;
      case 'R':
        wallIfPossible(nextCell.first, nextCell.second, 'L');
        break;
      }
    }

    if (nextCell.first == start.first && nextCell.second == start.second) {
      // TODO yay
    }

    // set walls and memorize prevState
    // restore walls to try previous direction
    { // Un-wall anti going back
      switch (dir) {
      case 'U':
        restorePrevWallState(nextCell.first, nextCell.second, 'D', true);
        break;
      case 'D':
        restorePrevWallState(nextCell.first, nextCell.second, 'U', true);
        break;
      case 'L':
        restorePrevWallState(nextCell.first, nextCell.second, 'R', true);
        break;
      case 'R':
        restorePrevWallState(nextCell.first, nextCell.second, 'L', true);
        break;
      }
    }
    previousMove = dir;
  } while (bestFound < bestPossible || moves.empty());
}

// Saves the state in the stack
void saveState(stack<bool[6]> s, rc nextCell, rc currentCell, char dir) {
  bool state[6];
  switch (dir) {
  case 'U': {
    state[0] = matrix[nextCell.first][nextCell.second].L;
    state[1] = matrix[nextCell.first][nextCell.second].R;
    state[2] = matrix[nextCell.first][nextCell.second].U;
    state[3] =
        nextCell.first > 0 && matrix[nextCell.first - 1][nextCell.second].U;
    state[4] = matrix[currentCell.first][currentCell.second].L;
    state[5] = matrix[currentCell.first][currentCell.second].R;
    break;
  }
  case 'D': {
    state[0] = matrix[nextCell.first][nextCell.second].L;
    state[1] = matrix[nextCell.first][nextCell.second].R;
    state[2] = matrix[nextCell.first][nextCell.second].D;
    state[3] =
        nextCell.first + 1 < n && matrix[nextCell.first + 1][nextCell.second].D;
    state[4] = matrix[currentCell.first][currentCell.second].L;
    state[5] = matrix[currentCell.first][currentCell.second].R;
    break;
  }
  case 'R': {
    state[0] = matrix[nextCell.first][nextCell.second].U;
    state[1] = matrix[nextCell.first][nextCell.second].D;
    state[2] = matrix[nextCell.first][nextCell.second].R;
    state[3] =
        nextCell.first + 1 < m && matrix[nextCell.first + m][nextCell.second].R;
    state[4] = matrix[currentCell.first][currentCell.second].U;
    state[5] = matrix[currentCell.first][currentCell.second].D;
    break;
  }
  case 'L': {
    state[0] = matrix[nextCell.first][nextCell.second].U;
    state[1] = matrix[nextCell.first][nextCell.second].D;
    state[2] = matrix[nextCell.first][nextCell.second].L;
    state[3] =
        nextCell.first > 0 && matrix[nextCell.first - 1][nextCell.second].L;
    state[4] = matrix[currentCell.first][currentCell.second].U;
    state[5] = matrix[currentCell.first][currentCell.second].D;
    break;
  }
  }
  s.push(state);
}

// Pops one state from the stack and applies it back to the matrix, restoring it
void restoreState(stack<bool[6]> s, stack<char> moves, rc currentCell) {
  rc previousCell = rc(currentCell.first, currentCell.second);
  switch (moves.top()) { // determine previousCell coordinates
  case 'U':
    previousCell.first--;
    break;
  case 'D':
    previousCell.first++;
    break;
  case 'L':
    previousCell.second++;
    break;
  case 'R':
    previousCell.second--;
    break;
  default:
    break;
  }
  bool state[6];
  for (int i = 0; i < 6; i++)
    state[i] = s.top()[i];
  // restore
}