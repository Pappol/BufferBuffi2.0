#include <bits/stdc++.h>
#include <fstream>

using namespace std;
//git testing

int punti = 0;
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

// Checks for an optimal rectangular solution. If it is found it is printed and
// the program exits w/ code 0. If a solution is found but it's not optimal it
// gets printed anyway.
string checkForRectangles(int n, int m, vector<rc> w, vector<rc> b, vector<vector<cell>> matrix);
string RecTl(rc TL, int n, int m, vector<rc> w, vector<rc> b, vector<vector<cell>> matrix);
int points(int A, int B, int W, rc start, rc end);


bool equals(rc a, rc b);

int main(int argc, char **argv) {
  ushort N, M;
  int B, W;
  ifstream in("input.txt");
  // Initialization
  in >> N >> M >> B >> W;
  vector<vector<cell>> matrix(M, vector<cell>(N, cell()));
  // Forbidding invalid moves
  for (int i = 0; i < N; i++) {
    matrix[0][i].U = false;
  }
  for (int i = 0; i < M; i++) {
    matrix[i][0].L = false;
    matrix[i][N - 1].R = false;
  }
  for (int i = 0; i < N; i++) {
    matrix[M - 1][i].D = false;
  }
  // Getting rings' poitions
  vector<rc> blacks = vector<rc>(B);
  for (int i = 0; i < B; i++) {
    ushort r, c;
    in >> r >> c;
    blacks[i] = rc(r, c);
    matrix[r][c].type = 2;
  }
  vector<rc> whites = vector<rc>(W);
  for (int i = 0; i < W; i++) {
    ushort r, c;
    in >> r >> c;
    whites[i] = rc(r, c);
    matrix[r][c].type = 1;
  }
  // Checking if a rectangle could be the optimal solution
  if (whites.size() <= 8 && blacks.size() <= 4) {
    checkForRectangles(N, M, whites, blacks, matrix);
  }
  ofstream out("output.txt");

  for (int i = 0; i < M; i++) {
    for (int j = 0; j < N; j++) {
      cout << matrix[i][j].type << " ";
    }
    cout << endl;
  }
  return 0;
}

bool equals(rc a, rc b){
  if(a.first == b.first && a.second == b.second) return true;
  return false;
}

string checkForRectangles(int n, int m, vector<rc> w, vector<rc> b, vector<vector<cell>> matrix){
  rc TL;
  int i, j;
  string ret;

  for(int i=0; i<m; i++){
    for(int j=0; j<n; j++){

      if(matrix[i][j].type == 2){
        TL.first = i;
        TL.second = j;
        string tmp=RecTl(TL,n,m,w,b,matrix);
          if(tmp!=" "){
            return tmp;            
          }
      } else if (matrix[i][j].type == 2){
        //codice x bianco
        if(i>0){
          TL.first=i-1;
          TL.second=j;
          string tmp=RecTl(TL,n,m,w,b,matrix);
          if(tmp!=" "){
            return tmp;            
          }
        }
        if(j>0){
          TL.first=i;
          TL.second=j-1;
          string tmp=RecTl(TL,n,m,w,b,matrix);
          if(tmp!=" "){
            return tmp;            
          }
        }
      }
    }
  }

}

string RecTl(rc TL, int n, int m, vector<rc> w, vector<rc> b, vector<vector<cell>> matrix){
  stringbuf percorso;
  string ret = " ";
  int n_anelli = 0, i, j;
  rc start;

  n_anelli++;
  start.first = TL.first;
  start.second = TL.second;
  
  i = TL.first;
  for(int j=TL.second; j<n; j++){
    percorso.sputc('R');
    if(matrix[i][j].type == 2 || matrix[i+1][j].type == 1){
      n_anelli++;
      TL.first = i;
      TL.second = j;
      break;
    } else if (matrix[i][j].type == 1){
        TL.first = i;
        TL.second = j+1;
        n_anelli++;
        break;
    }
  }

  j = TL.second;
  for(int i=TL.first; i<m; i++){
    percorso.sputc('D');
    if(matrix[i][j].type == 2 || matrix[i][j-1].type == 1){
      n_anelli++;
      TL.first = i;
      TL.second = j;
      break;
    } else if (matrix[i][j].type == 1){
        n_anelli++;
        TL.first = i+1;
        TL.second = j;
        break;
    }
  }

  i = TL.first;
  for(int j=TL.second; j>=0; j--){
    percorso.sputc('L');
    if(matrix[i][j].type == 2 || matrix[i-1][j].type == 1){
      n_anelli++;
      TL.first = i;
      TL.second = j;
      break;
    } else if (matrix[i][j].type == 1){
        n_anelli++;
        TL.first = i;
        TL.second = j-1;
        break;
    }
  }

  j = TL.second;
  for(int i=TL.first; i>=0; i--){
    percorso.sputc('U');
    if(matrix[i][j].type == 2 || matrix[i][j+1].type == 1){
      n_anelli++;
      TL.first = i;
      TL.second = j;
      break;
    } else if (matrix[i][j].type == 1){
        n_anelli++;
        TL.first = i-1;
        TL.second = j;
        break;
    }
  }

  if(!equals(start, TL)) return ret;
  else {
    ret = percorso.str();
    return ret;
  }

}

int points(int A, int B, int W, rc start, rc end){
  int tmp_point;
  if(start.first==end.first && start.second==end.second){
    tmp_point = 5*(A/(B+W));
  }else{
    tmp_point = 5*(A/(B+W))*0.5;
  }
  punti = max(punti, tmp_point);
}