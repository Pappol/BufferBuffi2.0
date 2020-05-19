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
string RecTl(int n, int m, int w, int b, vector<vector<cell>> matrix);
int points(int A, int B, int W, rc start, rc end);
void printResults(int n_anelli, int len, int x_start, int y_start, string perc);
bool equals(rc a, rc b);

int main(int argc, char **argv) {
  ushort N, M;
  int B, W;
  ifstream in("input.txt");
  // Initialization
  in >> N >> M >> B >> W;
  swap(N, M);
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
    RecTl(N, M, W, B, matrix);
  }

  /*for (int i = 0; i < M; i++) {
    for (int j = 0; j < N; j++) {
      if(matrix[i][j].type == 0) cout << "-"<< " ";
      else cout << matrix[i][j].type << " ";
    }
    cout << endl;
  }*/
  return 0;
}

bool equals(rc a, rc b){
  if(a.first == b.first && a.second == b.second) return true;
  return false;
}




string RecTl(int n, int m, int w, int b, vector<vector<cell>> matrix){
    int T, B, L, R;
    //cerco il top
    for(int i=0; i<m; i++){
        for(int j=0; j<n; j++){
            //cout<<i<<" ";
            if(matrix[i][j].type != VUOTO){
                T = i;
                cout<<"TOP =  "<<T<<endl;
                i = m;
                j = n;
                break;
            }
        }
        //cout<<endl;
    }
    //cerco il bottom
    for(int i=m-1; i>=0; i--){
        for(int j=n-1; j>=0; j--){
            //cout<<i<<" ";
            if(matrix[i][j].type != VUOTO){
                B = i;
                i = -1;
                j = -1;
                cout<<"BOT =  "<<B<<endl;
            }
        }
        //cout<<endl;
    }
    swap(n,m);
    //cerco il left
    for(int j=0; j<n; j++){
        for(int i=0; i<m; i++){
            //cout<<i<<" ";
            if(matrix[j][i].type != VUOTO){
                L = i;
                i = m;
                j = n;
                cout<<"LEFT =  "<<L<<endl;
            }
        }
        //cout<<endl;
    }
    //cerco il bottom
    for(int j=n-1; j>=0; j--){
        for(int i=m-1; i>=0; i--){
            //cout<<i<<" ";
            if(matrix[j][i].type != VUOTO){
                R = i;
                i = 0;
                j = 0;
                cout<<"RIGHT =  "<<R<<endl;
            }
        }
        //cout<<endl;
    }

    if(T == B && R == L){
        if(matrix[T][R].type == BIANCO) return "RRDLLU";
        if(matrix[T][R].type == NERO) return "RDLU";
    }

    stringbuf percorso;
    int side = max(L,R)-min(L,R),
        up = max(T,B)-min(B,T);

    for(int i=0; i<side; i++){
        percorso.sputc('R');
    }
    for(int i=0; i<up; i++){
        percorso.sputc('D');
    }
    for(int i=0; i<side; i++){
        percorso.sputc('L');
    }
    for(int i=0; i<up; i++){
        percorso.sputc('U');
    }

    cout<<percorso.str()<<endl;
    return percorso.str();

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