#include <bits/stdc++.h>
#include <fstream>
#include <cstdlib>
#include <vector>

using namespace std;

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
string checkForRectangles(int n, int m, int w, int b, vector<vector<cell>> matrix);
string RecTl(rc TL, int n, int m, int w, int b, vector<vector<cell>> matrix);
int points(int A, int B, int W, rc start, rc end);
void printResults(int n_anelli, int len, int x_start, int y_start, string perc);


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

  ofstream out("output.txt");

  // Checking if a rectangle could be the optimal solution
  if (whites.size() <= 8 && blacks.size() <= 4) {
    out << checkForRectangles(N, M, W, B, matrix);
  }

  for (int i = 0; i < M; i++) {
    for (int j = 0; j < N; j++) {
      if(matrix[i][j].type == 0) cout << "-"<< " ";
      else cout << matrix[i][j].type << " ";
    }
    cout << endl;
  }
  return 0;
}

bool equals(rc a, rc b){
  if(a.first == b.first && a.second == b.second) return true;
  return false;
}

string checkForRectangles(int n, int m, int w, int b, vector<vector<cell>> matrix){
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
      } else if (matrix[i][j].type == 1){
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
  return "someting wrong!";
}

string RecTl(rc TL, int n, int m, int w, int b, vector<vector<cell>> matrix){
  stringbuf percorso;
  string ret = " ";
  int n_anelli = 0, i, j;
  rc start;
  bool can_turn = false;

  n_anelli++;
  start.first = TL.first;
  start.second = TL.second;
  
  cout<<endl; 
  cout<<"> "<<TL.first<<" "<<TL.second<<" (start)"<<endl;

  i = TL.first;
  for(int j=TL.second+1; j<n; j++){
    percorso.sputc('R');
    //cout<<'R' ;
    //se trovo un nero o nella cella sotto trovo un bianco
    if(matrix[i][j].type == 2 || matrix[i+1][j].type == 1){    
      n_anelli++;
      //se ho trovato un bianco controllo se nella riga ci sono altri anelli
      if(matrix[i+1][j].type == 1){
        int k = j+1;
        while(matrix[i][k].type == 0 && k != n){
          k++;
        }
        if(k >= n) can_turn = true;
      } else 
        can_turn = true;
      if(can_turn == true){
        cout<<">> "<<i<<" "<<j<<endl;
        TL.first = i;
        TL.second = j;
        break;
      }
    } else if (matrix[i][j].type == 1 && matrix[i][j-1].type != 2){
        int k = j+1;
          while(matrix[i][k].type == 0 && k != n){
            k++;
          }
        if(k >= n){
          TL.first = i;
          TL.second = j+1;
          n_anelli++;
          if(matrix[TL.first][TL.second].type == 2) n_anelli++;
          cout<<">>> "<<i<<" "<<j<<endl;
          break;
        }
    }
  }

  can_turn = false;
  j = TL.second;
  for(int i=TL.first+1; i<m; i++){
    percorso.sputc('D');
    //cout<<'D' ;
    if(matrix[i][j].type == 2 || matrix[i][j-1].type == 1){
      //se ho trovato un bianco controllo se nella colonna ci sono altri anelli
      if(matrix[i][j-1].type == 1){
        int k = i+1;
        while(matrix[k][j].type == 0 && k != m){
          k++;
        }
        if(k >= m) can_turn = true;
      } else 
        can_turn = true;
      if(can_turn == true){
        n_anelli++;
        TL.first = i;
        TL.second = j;
        cout<<">> "<<i<<" "<<j<<endl;
        break;
      }
    } else if (matrix[i][j].type == 1){
        n_anelli++;
        TL.first = i+1;
        TL.second = j;
        if(matrix[TL.first][TL.second].type == 2) n_anelli++;
        cout<<"> "<<i<<" "<<j<<endl;
        break;
    }
  }

  can_turn = false;
  i = TL.first;
  for(int j=TL.second-1; j>=0; j--){
    percorso.sputc('L');
    //cout<<'L' ;
    if(matrix[i][j].type == 2 || matrix[i-1][j].type == 1){
      //se ho trovato un bianco controllo se nella riga ci sono altri anelli
      if(matrix[i-1][j].type == 1){
        int k = j-1;
        while(matrix[i][k].type == 0 && k != 0){
          k--;
        }
        if(k <= 0) can_turn = true;
      } else 
        can_turn = true;
      if(can_turn == true && matrix[i][j].type != 1){
        n_anelli++;
        TL.first = i;
        TL.second = j;
        cout<<">> "<<i<<" "<<j<<endl;
        break;
      }
    } 
    if (matrix[i][j].type == 1){
        n_anelli++;
        TL.first = i;
        TL.second = j-1;
        if(matrix[TL.first][TL.second].type == 2) n_anelli++;
        cout<<"> "<<i<<" "<<j<<endl;
        break;
    }
    if (j == start.second){
        TL.first = i;
        TL.second = j;
        break;
    }
  }

  can_turn = false;
  j = TL.second;
  cout<<">< "<<TL.first<<" "<<j<<endl;
  for(int i=TL.first-1; i>=0; i--){
    percorso.sputc('U');
    //cout<<'U' ;
    if(matrix[i][j].type == 2 || matrix[i][j+1].type == 1){
      //se ho trovato un bianco controllo se nella colonna ci sono altri anelli
      if(matrix[i][j+1].type == 1 && matrix[i][j].type != 2){
        int k = i-1;
        while(matrix[k][j].type == 0 && k != m){
          k--;
        }
        if(k <= 0) can_turn = true;
      } else 
        can_turn = true;
      if(can_turn == true){
        n_anelli++;
        TL.first = i;
        TL.second = j;
        cout<<">> "<<i<<" "<<j<<endl;
        break;
      }
    } else if (matrix[i][j].type == 1){
        n_anelli++;
        TL.first = i-1;
        TL.second = j;
        if(matrix[TL.first][TL.second].type == 2) n_anelli++;
        cout<<"> "<<i<<" "<<j<<endl;
        break;
    } else if(equals(TL, start)){
        TL.first = i;
        TL.second = j;
        break;
    }
  }
  cout<<" n anelli : "<<n_anelli<<endl;
  cout<<" len  : "<<percorso.str().length()<<endl;
  if(!equals(start, TL)) 
    return ret;
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
  return punti;
}

void printResults(int n_anelli, int len, int x_start, int y_start, string perc){
  ofstream out("output.txt");
  out << n_anelli<<" " << len<<" " << x_start<<" " << y_start <<" "<< perc << "#"<<endl;
  out.close();
}