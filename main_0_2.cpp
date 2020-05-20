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
  bool U = true;  // Can go vert from here
  bool D = true;  // Can go down from here
  bool L = true;  // Can go left from here
  bool R = true;  // Can go right from here
  short type = VUOTO; // 0=vuota,1=bianca, 2=nera
} cell;

// Checks for an optimal rectangular solution. If it is found it is printed and
// the program exits w/ code 0. If a solution is found but it's not optimal it
// gets printed anyway.

string findRect(int n, int m, vector<vector<cell>> matrix, rc *start);
int points(int A, int B, int W, rc start, rc end);
void printResults(int n_anelli, int len, int x_start, int y_start, string perc);
bool equals(rc a, rc b);
int contaAnelli(string percorso, int x_start, int y_start, vector<vector<cell>> matrix);

int main(int argc, char **argv) {
  ushort N, M;
  int B, W;
  ifstream in("input.txt");
  // Initialization
  in >> N >> M >> B >> W;
  vector<vector<cell>> matrix(N, vector<cell>(M, cell()));
  // Forbidding invalid moves
  for (int j = 0; j< M; j++) {
    matrix[0][j].U = false;
  }
  for (int i = 0; i < N; i++) {
    matrix[i][0].L = false;
    matrix[i][M - 1].R = false;
  }
  for (int j = 0; j < M; j++) {
    matrix[N - 1][j].D = false;
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
    rc start; 
  // Checking if a rectangle could be the optimal solution
  if (whites.size() <= 8 && blacks.size() <= 4) {
    findRect(N, M, matrix, &start);
  }

  /*for (int i = 0; i < N; i++) {
    for (int j = 0; j < M; j++) {
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

string findRect(int n, int m, vector<vector<cell>> matrix,rc *start){
    int T, B, L, R;
    string ret;
    //cerco il top
    for(int i=0; i<n; i++){
        for(int j=0; j<m; j++){
            //cout<<i<<" ";
            if(matrix[i][j].type != VUOTO){
                T = i;
                //cout<<"TOP =  "<<T<<endl;
                i = n;
                j = m;
                //break;
            }
        }
        //cout<<endl;
    }
    //cerco il bottom
    for(int i=n-1; i>=0; i--){
        for(int j=m-1; j>=0; j--){
            //cout<<i<<" ";
            if(matrix[i][j].type != VUOTO){
                B = i;
                i = -1;
                j = -1;
                //cout<<"BOT =  "<<B<<endl;
                //break;
            }
        }
       // cout<<endl;
    }
    
    //cerco il left
    for(int j=0; j<m; j++){
        for(int i=T; i<B; i++){
            //cout<<i<<" ";
            if(matrix[i][j].type != VUOTO){
                L = j;
                i = B;
                j = m;
                //cout<<"LEFT =  "<<L<<endl;
                //break;
            }
        }
        //cout<<endl;
    }
    //cerco il bottom
    for(int j=m-1; j>=0; j--){
        for(int i=B; i>=T; i--){
            //cout<<i<<" ";
            if(matrix[i][j].type != VUOTO){
                R = j;
                i = -1;
                j = -1;
                //cout<<"RIGHT =  "<<R<<endl;
                //break;
            }
        }
        //cout<<endl;
    }

    start->first = T;
    start->second = min(R,L); 

    int orizz = max(L,R)-min(L,R),
        vert = max(T,B)-min(B,T);
    stringbuf percorso=stringbuf();

    if(T == B && R == L){//1 anello
        if(matrix[T][R].type == BIANCO) ret =  "RRDLLU";
        if(matrix[T][R].type == NERO) ret =  "RDLU";
    }

    if(T == B){//2 anelli orizzontali
        int extra = 0;
        bool bordo = false;
        if(matrix[T][L].D == false) bordo = true;
        if(matrix[T][L].type == BIANCO) extra++;
        if(matrix[B][R].type == BIANCO) extra++;

        for(int i=0; i<orizz+extra; i++){
            percorso.sputc('R');
        }
        if(!bordo)
            percorso.sputc('D');
        else
            percorso.sputc('U');
        for(int i=0; i<orizz+extra; i++){
            percorso.sputc('L');
        }
        if(!bordo)
            percorso.sputc('U');
        else
            percorso.sputc('D');
        ret =  percorso.str();
    }


    if(R == L){//2 anelli verticali
        int extra = 0;
        bool bordo = false;
        if(matrix[T][L].R == false) bordo = true;
        if(matrix[T][L].type == BIANCO) extra++;
        if(matrix[B][R].type == BIANCO) extra++;
        if(!bordo)
            percorso.sputc('R');
        else
            percorso.sputc('L');
        for(int i=0; i<vert+extra; i++){
            percorso.sputc('D');
        }
        if(!bordo)
            percorso.sputc('L');
        else
            percorso.sputc('R');
        for(int i=0; i<vert+extra; i++){
            percorso.sputc('U');
        }      
        ret =  percorso.str();
    }
    if(T != B && R != L){
        for(int i=0; i<orizz; i++){
            percorso.sputc('R');
        }
        for(int i=0; i<vert; i++){
            percorso.sputc('D');
        }
        for(int i=0; i<orizz; i++){
            percorso.sputc('L');
        }
        for(int i=0; i<vert; i++){
            percorso.sputc('U');
        }
    }

    ret =  percorso.str();
    int anell = contaAnelli(ret, T, min(R,L), matrix);
    printResults(anell, ret.length(), T, min(R,L), ret);
    //cout<<percorso.str()<<endl;
    
    return ret;
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
  //cout << n_anelli<<" " << len<<" " << x_start<<" " << y_start <<" "<< perc << "#"<<endl;
  out << n_anelli<<" " << len<<" " << x_start<<" " << y_start <<" "<< perc << "#"<<endl;
  out.close();
}

int contaAnelli(string percorso, int x_start, int y_start, vector<vector<cell>> matrix){
    
    int n_anelli = 0;
    for(int i=0; i<percorso.length(); i++){
        if(matrix[x_start][y_start].type != VUOTO) n_anelli++;
        switch(percorso[i]){
            case 'R' : y_start++;
                break;
            case 'D' : x_start++;
                break;
            case 'L' : y_start--;
                break;
            case 'U' : x_start--;
                break;
        }
    }
    //cout<<"anelli: "<<n_anelli<<endl;
    return n_anelli;
}