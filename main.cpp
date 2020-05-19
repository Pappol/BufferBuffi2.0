#include <bits/stdc++.h>
#include <fstream>
#include <cstdlib>
#include <vector>

using namespace std;

int punti = 0;
// Pair: row and column
typedef pair<ushort, ushort> rc;

//tipo di cella
typedef enum {VUOTO, BIANCO, NERO} tipo;

// Cell data structure
typedef struct {
  bool U = true;  // Can go up from here
  bool D = true;  // Can go down from here
  bool L = true;  // Can go left from here
  bool R = true;  // Can go right from here
  short type = VUOTO; // 0=vuota,1=bianca, 2=nera
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
    int n_anelli = 0, i, j;
    rc start;

    n_anelli++;

    //memorizzo la posizione di partenza
    start.first = TL.first;
    start.second = TL.second;

    //SX ---> DX (i fisso, j cambia)
    i = TL.first;
    for(j=TL.second+1; j<n; j++){
        if(matrix[i][j].type == NERO){//nero
            n_anelli++;
            percorso.sputc('D');
            TL.first = i+1;
            TL.second = j;
            break;
        }
        if(matrix[i][j].type == BIANCO){//bianco
            n_anelli++;
            percorso.sputc('R');
            if(matrix[i][j+1].type == NERO){//il prossimo è nero
                n_anelli++;
                percorso.sputc('D');
                TL.first = i+1;
                TL.second = j+1;
                break;
            }
            int k = j+1;
            //controllo se ci sono alti anelli nella riga
            while(matrix[i][k].type == VUOTO && k != n){
                k++;
            }
            if(k >= n){
                percorso.sputc('D');
                TL.first = i+1;
                TL.second = j+1;
                break;
            }
        }
        if(matrix[i+1][j].type == BIANCO){//bianco una cella sotto
            int k = j+1;
            //controllo se ci sono alti anelli nella riga
            while(matrix[i][k].type == VUOTO && k != n){
                k++;
            }
            if(k >= n){
                percorso.sputc('D');
                TL.first = i+1;
                TL.second = j;
                break;
            }

        }
        percorso.sputc('R');
    }

    //UP ---> DOWN (i cambia, j fisso)
    j = TL.second;
    for(int i=TL.first; i<m; i++){
        if(matrix[i][j].type == NERO){//nero
            n_anelli++;
            percorso.sputc('L');
            TL.first = i;
            TL.second = j-1;
            break;
        }
        if(matrix[i][j].type == BIANCO){//bianco
            n_anelli++;
            percorso.sputc('D');
            if(matrix[i+1][j].type == NERO){//il prossimo è nero
                n_anelli++;
                percorso.sputc('L');
                TL.first = i+1;
                TL.second = j-1;
                break;
            }
            int k = i+1;
            //controllo se ci sono alti anelli nella riga
            while(matrix[i][k].type == VUOTO && k != m){
                k++;
            }
            if(k >= m){
                percorso.sputc('L');
                TL.first = i+1;
                TL.second = j-1;
                break;
            }
        }
        if(matrix[i][j-1].type == BIANCO){//bianco una cella a SX
            int k = i+1;
            //controllo se ci sono alti anelli nella riga
            while(matrix[i][k].type == VUOTO && k != m){
                k++;
            }
            if(k >= m){
                percorso.sputc('L');
                TL.first = i;
                TL.second = j-1;
                break;
            }
        }
        percorso.sputc('D');
    }

    //DX ---> SX (i fisso, j cambia)
    i = TL.first;
    for(j=TL.second; j>=0; j--){
        if(matrix[i][j].type == NERO){//nero
            n_anelli++;
            percorso.sputc('U');
            TL.first = i-1;
            TL.second = j;
            break;
        }
        if(matrix[i][j].type == BIANCO){//bianco
            n_anelli++;
            percorso.sputc('L');
            if(matrix[i][j+1].type == NERO){ //il prossimo è nero
                n_anelli++;
                percorso.sputc('U');
                TL.first = i-1;
                TL.second = j-1;
                break;
            }
            int k = j-1;
            //controllo se ci sono alti anelli nella riga
            while(matrix[i][k].type == VUOTO && k != 0){
                k--;
            }
            if(k <= 0){
                percorso.sputc('U');
                TL.first = i-1;
                TL.second = j-1;
                break;
            }
        }
        if(matrix[i+1][j].type == BIANCO){//bianco una cella sopra
            int k = j-1;
            //controllo se ci sono alti anelli nella riga
            while(matrix[i][k].type == VUOTO && k != 0){
                k--;
            }
            if(k <= 0){
                percorso.sputc('U');
                TL.first = i-1;
                TL.second = j;
                break;
            }

        }
        if(j == start.second){ //controllo se mi trovo sotto (qualsiasi altezza) alla partenza
            percorso.sputc('U');
            TL.first = i-1;
            TL.second = j;
            break;
        }
        percorso.sputc('L');
    }

    //DOWN ---> UP (i cambia, j fisso)
    j = TL.second;
    for(int i=TL.first; i>=0; i--){
        TL.first = i;
        TL.second = j;
        if(equals(TL, start)){ //controllo se sono arrivato dove ero partito
            break;
        } 
        percorso.sputc('U');
    }

    cout<<"percorso: "<<percorso.str()<<endl;
    cout<<"anelli attraversati: "<<n_anelli<<endl;
    cout<<"lunghezza percorso: "<<percorso.str().length()<<endl;

    if(equals(TL, start))
        return percorso.str();
    return " ";
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