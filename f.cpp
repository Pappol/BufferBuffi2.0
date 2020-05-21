#include <bits/stdc++.h>
#include <fstream>
#include <vector>
#include <random>

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

//TO DO
char pickRandom(bool U, bool D, bool R, bool L, char prec){
    //prec: movimento precedente
    //non posso andare nella direzione opposta di prec altrimenti torno indietro
    vector<char> direzioniCons = vector<char>();
    if(U && prec!='D') direzioniCons.push_back('U');
    if(D && prec!='U') direzioniCons.push_back('D');
    if(R && prec!='L') direzioniCons.push_back('R');
    if(L && prec!='R') direzioniCons.push_back('L');
    return direzioniCons[ rand()%direzioniCons.size() ];
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
    char dir = pickRandom( matrix[x][y].D, matrix[x][y].U, matrix[x][y].L, matrix[x][y].R, precDir );
    //controllo direzione se c'è un anello
    //serve??? (in teoria no perchè metto già tutti i muri giusti)

    //se la cella corrente è nera metto i muri im base allo spostamento ad "angolo"
    //se la cella corrente è vuota e la direzione precedente è diversa dalla corrente si comporta da nera
    if(matrix[x][y].type == 2 || (matrix[x][y].type == 0 && dir != precDir)){
        if (dir == 'U') bound(x+1, y, dir);         //se sono andato su metto un muro sotto
        else if (dir == 'D') bound(x-1, y, dir);    //se sono andato giu metto un muro sopra
        else if (dir == 'R') bound(x, y-1, dir);    //se sono andato a dx metto un muro a sx
        else if (dir == 'L') bound(x, y-1, dir);    //se sono andato a sx metto un muro a dx
    }
    //se la cella corrente è bianca metto i muri in modo concorde allo spostamento
    //se la cella corrente è vuota e la direzione precedente è uguale alla corrente si comporta da bianca
    if(matrix[x][y].type == 2 || (matrix[x][y].type == 0 && dir == precDir)){
        if(dir == 'U' || dir == 'D'){   //se sono andato a dx o sx metto i muri sopra e sotto
            bound(x, y-1, 'R');
            bound(x, y+1, 'L');
        } else{                         //se sono andato su o giu metto i muri a dx e sx
            bound(x-1, y, 'D');
            bound(x+1, y, 'U');
        }
    }

    //MURI IN BASE ALLA CELLA IN CUI MI TROVO DOPO LO SPOSTAMENTO CHE HO FATTO
    rc nextCell = getNextCell(x, y, dir);  
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