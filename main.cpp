#include <bits/stdc++.h>
#include <fstream>
#include <stdlib.h>
#include <time.h>
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

//GLOBALS
ushort N, M;
int B, W;
int maxPoint = 0;

// Checks for an optimal rectangular solution. If it is found it is printed and
// the program exits w/ code 0. If a solution is found but it's not optimal it
// gets printed anyway. Returns ring count w/ a rectangular solution.
int checkForRectangles(int n, int m, vector<rc> w, vector<rc> b, ofstream *out);

// Modifies matrix "placing walls" (forbidding moves that would surely lead into
// darkness)
void placeWalls(int n, int m);

char pickRandom(bool U, bool D, bool R, bool L, char prec);
rc getNextCell(int x, int y, char dir);
void bound(int x, int y, char dir);
char muovi(int x, int y, char precDir);
string makePath(int start_x, int start_y, ofstream *out);

int main(int argc, char **argv) {
  srand(time(NULL));
 
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

  vector<vector<cell>> support = matrix;

  while(true){
    matrix = support;
    makePath(rand()%N, rand()%M, &out);
  }

  cout << endl;
  for (int i = 0; i < N; i++) {
    for (int j = 0; j < M; j++) {
      cout << matrix[i][j].type;
    }
    cout << endl;
  }
  out.close();
  in.close();
  return 0;
}

int checkForRectangles(int n, int m, vector<rc> w, vector<rc> b, ofstream *out) {
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

char pickRandom(bool U, bool D, bool R, bool L, char prec){
    //prec: movimento precedente
    //non posso andare nella direzione opposta di prec altrimenti torno indietro
    vector<char> direzioniCons = vector<char>();
    if(U && prec!='D') direzioniCons.push_back('U');
    if(D && prec!='U') direzioniCons.push_back('D');
    if(R && prec!='L') direzioniCons.push_back('R');
    if(L && prec!='R') direzioniCons.push_back('L');
    if(direzioniCons.size()>0)
      return direzioniCons[ rand()%direzioniCons.size() ];
    else
      return '#';
}

rc getNextCell(int x, int y, char dir){
    rc newCell;
    switch(dir){
        case 'U': 
            newCell.first = x-1;
            newCell.second = y;
            break;
        case 'D': 
            newCell.first = x+1;
            newCell.second = y;
            break;
        case 'R': 
            newCell.first = x;
            newCell.second = y+1;
            break;
        case 'L': 
            newCell.first = x;
            newCell.second = y-1;
            break;
    }
    return newCell;
}

void bound(int x, int y, char dir){
    switch(dir){
        case 'U': 
            matrix[x][y].U = false;
            break;
        case 'D': 
            matrix[x][y].D = false;
            break;
        case 'R': 
            matrix[x][y].R = false;
            break;
        case 'L': 
            matrix[x][y].L = false;
            break;
    }
}

char muovi(int x, int y, char precDir){
    char dir = pickRandom( matrix[x][y].U, matrix[x][y].D, matrix[x][y].R, matrix[x][y].L, precDir );
    if(dir == '#') return dir;

    rc nextCell = getNextCell(x, y, dir); 
    //controllo direzione se c'è un anello
    //serve??? (in teoria no perchè metto già tutti i muri giusti)

    //se la cella corrente è nera metto i muri im base allo spostamento ad "angolo"
    //se la cella corrente è vuota e la direzione precedente è diversa dalla corrente si comporta da nera
    if(matrix[x][y].type == 2 || (matrix[x][y].type == 0 && dir != precDir)){
        if (dir == 'U' && x+1<N) bound(x+1, y, dir);         //se sono andato su metto un muro sotto
        else if (dir == 'D' && x-1>=0) bound(x-1, y, dir);    //se sono andato giu metto un muro sopra
        else if (dir == 'R' && y-1>=0) bound(x, y-1, dir);    //se sono andato a dx metto un muro a sx
        else if (dir == 'L' && y+1<M) bound(x, y+1, dir);    //se sono andato a sx metto un muro a dx

        if(precDir == 'R' && y+1<M) bound(x, y+1, 'L');
        else if(precDir == 'L' && y-1>=0) bound(x, y-1, 'R');
        else if(precDir == 'U' && x-1>=0) bound(x-1, y, 'D');
        else if(precDir == 'D' && x+1<N) bound(x+1, y, 'U');
    }
    //se la cella è nera deve per forza andare dritta 
    if(matrix[x][y].type == 2){
      if(dir == 'L' || dir == 'R'){
        if(nextCell.first-1 >= 0) bound(nextCell.first-1, nextCell.second, 'U');
        if(nextCell.first+1 < N) bound(nextCell.first+1, nextCell.second, 'D');
      } else{
        if(nextCell.second-1 >= 0) bound(nextCell.first, nextCell.second-1, 'R');
        if(nextCell.second+1 < M) bound(nextCell.first, nextCell.second+1, 'L');
      }
    }
    //se la cella corrente è bianca metto i muri in modo concorde allo spostamento
    //se la cella corrente è vuota e la direzione precedente è uguale alla corrente si comporta da bianca
    if(matrix[x][y].type == 1 || (matrix[x][y].type == 0 && dir == precDir)){
        if(dir == 'U' || dir == 'D'){   //se sono andato a dx o sx metto i muri sopra e sotto
            if(y-1>=0) bound(x, y-1, 'R');
            if(y+1<M) bound(x, y+1, 'L');
        } else{                         //se sono andato su o giu metto i muri a dx e sx
            if(x-1>=0) bound(x-1, y, 'D');
            if(x+1<N) bound(x+1, y, 'U');
        }
    }

    //MURI IN BASE ALLA CELLA IN CUI MI TROVO DOPO LO SPOSTAMENTO CHE HO FATTO
     
    if(matrix[nextCell.first][nextCell.second].type == 2){  //se è nera metto un muro per evitare che la mossa dopo vada dritta
        bound(nextCell.first, nextCell.second, dir);
    }
    if(matrix[nextCell.first][nextCell.second].type == 1){  //se è bianca e mi sono già mosso 2 volte nella stessa direzione metto un muro per evitare che la mossa dopo vada ancora dritta
        if(dir == 'U' || dir == 'D'){   //se sono arrivato da dx o sx metto i muri sopra e sotto
            bound(nextCell.first, nextCell.second, 'R');
            bound(nextCell.first, nextCell.second, 'L');
        } else{                         //se sono arrivato da su o giu metto i muri a dx e sx
            bound(nextCell.first, nextCell.second, 'D');
            bound(nextCell.first, nextCell.second, 'U');
        }
        if(dir == precDir){
            nextCell = getNextCell(nextCell.first, nextCell.second, dir);
            bound(nextCell.first, nextCell.second, dir);
        }
    }
    
    //restituisco la direzione che ho preso in modo da passarla come precedenrte al prossimo passaggio
    return dir; 
}

string makePath(int start_x, int start_y, ofstream *out){
  stringbuf percorso;
  rc cell;
  cell.first = start_x;
  cell.second = start_y;
  char move;
  bool close = false;
  int n_anelli = 0;
  int punti;

  do{
    if(matrix[cell.first][cell.second].type != 0) n_anelli++;
    //cout<<"mossa";
    move = muovi(cell.first, cell.second, move);
    //cout<<"("<<move<<") - ";
    //cout<<"nuova cella";
    cell = getNextCell(cell.first, cell.second, move);
    //cout<<"("<<cell.first<<" , "<<cell.second<<") - ";
    if(cell.first==start_x && cell.second==start_y){
      if(move != '#'){
        percorso.sputc(move);
        move='#';
        close = true;
        //cout<<" #chiuso# ";
      }
    }
    //cout<<"inserimento - ";
    percorso.sputc(move);
    //cout<<"new round!"<<endl;
  }while( move != '#');

  punti = 5*n_anelli/(B+W);
  if(!close) punti = punti/2;
  //*out<<punti<<" ---> "<<percorso.str().length();
  if(maxPoint < punti){
    *out << endl << n_anelli <<" "<< percorso.str().length()-1 <<" "<< start_x <<" "<< start_y <<" "<<percorso.str();
    cout<<"START: ("<<start_x<<" , "<<start_y<<")"<<endl;
    cout<<"ANELLI: "<<n_anelli<<endl;
    cout<<"PUNTI: "<<punti<<endl;
    cout<<"PERCORSO GENERATO: "<<percorso.str()<<endl<<endl;

  }
  //*out<<endl;

  maxPoint = max(punti, maxPoint);

  return percorso.str();
}