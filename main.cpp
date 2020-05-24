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
  bool vis = false;
} cell;

// Globals
vector<vector<cell>> matrix;
ushort n, m;
vector<rc> blacks, whites;
ofstream out("output.txt");
ifstream in("input.txt");
stack<bitset<24>> states;
string moves;
int nmoves=0;
int rings=0;


// Checks for an optimal rectangular solution. If it is found it is printed and
// the program exits w/ code 0. If a solution is found but it's not optimal it
// gets printed anyway. Returns ring count w/ a rectangular solution.
int checkForRectangles();


//soluzione per rettangoloni

bool checkRight(rc pos);
bool checkLeft(rc pos);
bool checkUp(rc pos);
bool checkDown(rc pos);

void rettangoloni();
rc whileRight(rc pos);
rc whileLeft(rc pos);
rc whileUp(rc pos);
rc whileDown(rc pos);
void mossa(string dir,rc pos);

int main(int argc, char **argv) {
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
    if (checkForRectangles() == B + W)
      return 0;
  }

  //rettangoloni
  rettangoloni();
  out<<rings<<" "<<nmoves<<" "<<blacks[0].first<<" "<<blacks[0].second<<" "<<moves<<"#"<<endl;
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

void rettangoloni(){
  rc start;
  int i=0;
  do{
    start=blacks[i];
    i++;
  }while(matrix[start.first][start.second+1].type!=1 &&
          matrix[start.first][start.second-1].type!=1 &&
          matrix[start.first+1][start.second].type!=1 &&
          matrix[start.first-1][start.second].type!=1);

  rc pos=start;
  bool nearWhite = true;
  
  do{
    nearWhite = true;
    if(matrix[pos.first][pos.second].R && matrix[pos.first][pos.second+1].type==1){
      pos=whileRight(pos);
      matrix[pos.first][pos.second].L=false;
      }else{
        if(matrix[pos.first][pos.second].L && matrix[pos.first][pos.second-1].type==1){
          pos=whileLeft(pos);
          matrix[pos.first][pos.second].R=false;
        }else{
          if(matrix[pos.first][pos.second].U && matrix[pos.first-1][pos.second].type==1){
            pos=whileUp(pos);
            matrix[pos.first][pos.second].D=false;
          }else{
            if(matrix[pos.first][pos.second].D && matrix[pos.first+1][pos.second].type==1){
              pos=whileDown(pos);
              matrix[pos.first][pos.second].U=false;
          }else{
            nearWhite = false;
          }
        }
      } 
    }
    if( !nearWhite){
      char m = moves[moves.length()-1];
      
      if(m == 'R' || m == 'L'){
          if (checkDown(pos)){
            pos = whileDown(pos);
          } else if (checkUp(pos)){
            pos = whileUp(pos);
          }
      }
      if(m == 'U' || m == 'D'){
          if (checkRight(pos)) pos = whileRight(pos);
          else if (checkLeft(pos)) pos = whileLeft(pos);
      }
    }
  
  }while(pos != start);
  
}

bool checkRight(rc pos){
  rc tmp = pos;
  do{
    tmp.second++;
  }while(matrix[tmp.first][tmp.second].type == 0 && tmp.second < m-1);

  if(matrix[tmp.first][tmp.second].type != 0 && !matrix[tmp.first][tmp.second].vis) return true;
  else return false;
}

bool checkLeft(rc pos){
  rc tmp = pos;
  do{
    tmp.second--;
  }while(matrix[tmp.first][tmp.second].type == 0 && tmp.second > 0);

  if(matrix[tmp.first][tmp.second].type != 0 && !matrix[tmp.first][tmp.second].vis) return true;
  else return false;
}

bool checkUp(rc pos){
  rc tmp = pos;
  do{
    tmp.first--;
  }while(matrix[tmp.first][tmp.second].type == 0 && tmp.first > 0);

  if(matrix[tmp.first][tmp.second].type != 0 && !matrix[tmp.first][tmp.second].vis) return true;
  else return false;
}

bool checkDown(rc pos){
  rc tmp = pos;
  do{
    tmp.first++;
  }while(matrix[tmp.first][tmp.second].type == 0 && tmp.first < n-1);

  if(matrix[tmp.first][tmp.second].type != 0 && !matrix[tmp.first][tmp.second].vis) return true;
  else return false;
}


rc whileRight(rc pos){
  if(pos.second < m-2 && matrix[pos.first][pos.second].type == 2){
    pos.second++;
    mossa("R",pos);
  }
  do{
    pos.second++;
    mossa("R",pos);
  }while(matrix[pos.first][pos.second].type == 0 && pos.second < m-1);

  if(matrix[pos.first][pos.second].type == 2){
    matrix[pos.first][pos.second].L = false;
    return pos;
  }
  if(pos.second<m-1){
    if(matrix[pos.first][pos.second].type == 1 && matrix[pos.first][pos.second+1].type == 2){
      pos.second++;
      matrix[pos.first][pos.second].L = false;
      mossa("R",pos);
      return pos;
    }
    if(matrix[pos.first][pos.second].type == 1){
      if(matrix[pos.first][pos.second+1].type == 1){
        return pos;
      }
      if( pos.second<m-2){
        if(pos.first>=1 && matrix[pos.first-1][pos.second+2].type == 1 ){
          pos.second++;
          mossa("R", pos);
          pos.first--;
          mossa("U",pos);
          pos.second++;
          mossa("R", pos);
          return whileRight(pos); 
        }else{
          if(pos.first<n-1 && matrix[pos.first+1][pos.second+2].type == 1 ){
            pos.second++;
            mossa("R", pos);
            pos.first++;
            mossa("D",pos);
            pos.second++;
            mossa("R", pos);
            return whileRight(pos); 
          }
        }
      }
    }
  }
  return pos;
}

rc whileLeft(rc pos){
  if(pos.second>1 && matrix[pos.first][pos.second].type == 2){
    pos.second--;
    mossa("L",pos);
  }
  do{
    pos.second--;
    mossa("L",pos);
  }while(matrix[pos.first][pos.second].type == 0 && pos.second > 0);

  if(matrix[pos.first][pos.second].type == 2){
    matrix[pos.first][pos.second].R = false;
    return pos;
  }
  
  if(pos.second>0){
    if(matrix[pos.first][pos.second].type == 1 && matrix[pos.first][pos.second-1].type == 2){
      pos.second--;
      matrix[pos.first][pos.second].R = false;
      mossa("L",pos);
      return pos;
    }
    if(matrix[pos.first][pos.second].type == 1){
      if(matrix[pos.first][pos.second-1].type == 1){
        return pos;
      }
      if(pos.second > 1){
        if(pos.first > 0 &&  matrix[pos.first-1][pos.second-2].type == 1 ){
          pos.second--;
          mossa("L", pos);
          pos.first--;
          mossa("U",pos);
          pos.second--;
          mossa("L", pos);
          return whileLeft(pos); 
        }
        if(pos.first < n-1 && matrix[pos.first+1][pos.second-2].type == 1 ){
          pos.second--;
          mossa("L", pos);
          pos.first++;
          mossa("D",pos);
          pos.second--;
          mossa("L", pos);
          return whileLeft(pos); 
        }
      }
    }
  }
  return pos;
}

rc whileUp(rc pos){
  if(pos.first > 1 && matrix[pos.first][pos.second].type == 2){
    pos.first--;
    mossa("U",pos);
  }
  do{
    pos.first--;
    mossa("U",pos);
  }while(matrix[pos.first][pos.second].type == 0 && pos.first > 0);

  if(matrix[pos.first][pos.second].type == 2){
    matrix[pos.first][pos.second].D = false;
    return pos;
  }

  if(pos.first>0){
    if(matrix[pos.first][pos.second].type == 1 && matrix[pos.first-1][pos.second].type == 2){
      pos.first--;
      matrix[pos.first][pos.second].D = false;
      mossa("U",pos);
      return pos;
    }
    if(matrix[pos.first][pos.second].type == 1){
      if(matrix[pos.first-1][pos.second].type == 1){
        return pos;
      }
      if(pos.first > 1){
        if(pos.second<m-1 && matrix[pos.first-2][pos.second+1].type == 1 ){
          pos.first--;
          mossa("U",pos);
          pos.second++;
          mossa("R", pos);
          pos.first--;
          mossa("U",pos);
          return whileUp(pos); 
        }
          if(pos.second>0 && matrix[pos.first-2][pos.second-1].type == 1 ){
            pos.first--;
            mossa("U",pos);
            pos.second--;
            mossa("L", pos);
            pos.first--;
            mossa("U",pos);
            return whileUp(pos);  
          }
      }
    }
  }

  

  
  return pos;
}

rc whileDown(rc pos){
  if(pos.second < n-2 && matrix[pos.first][pos.second].type == 2){
    pos.first++;
    mossa("D",pos);
  }
  do{
    pos.first++;
    mossa("D",pos);
  }while(matrix[pos.first][pos.second].type == 0 && pos.first < n-1);
  
  if(matrix[pos.first][pos.second].type == 2){
    matrix[pos.first][pos.second].U = false;
    return pos;
  }

  if(pos.first < n-1){
    if(matrix[pos.first][pos.second].type == 1 && matrix[pos.first+1][pos.second].type == 2){
      pos.first++;
      matrix[pos.first][pos.second].U = false;
      mossa("D",pos);
      return pos;
    }

    if(matrix[pos.first][pos.second].type == 1){
      if(matrix[pos.first-1][pos.second].type == 1){
        return pos;
      }
      if(pos.first<n-2){
        if(pos.second<n-1 && matrix[pos.first+2][pos.second+1].type == 1 ){
          pos.first++;
          mossa("D",pos);
          pos.second++;
          mossa("R", pos);
          pos.first++;
          mossa("D",pos);
          return whileDown(pos); 
        }
        if(pos.second > 0 && matrix[pos.first+2][pos.second-1].type == 1 ){
           pos.first++;
           mossa("D",pos);
           pos.second--;
           mossa("L", pos);
           pos.first++;
           mossa("D",pos);
           return whileDown(pos);  
        }
      }
    }
  }
  
  return pos;
}


void mossa(string dir,rc pos){
  if(matrix[pos.first][pos.second].type!=0)rings++;
  matrix[pos.first][pos.second].vis=true;
  moves.append(dir);
  nmoves++;
}