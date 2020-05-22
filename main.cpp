#include <bits/stdc++.h>
#include <fstream>

using namespace std;

// Pair: row and column
typedef pair<ushort, ushort> rc;

// Cell data structure
typedef struct{
	bool U = true;	// Can go up from here
	bool D = true;	// Can go down from here
	bool L = true;	// Can go left from here
	bool R = true;	// Can go right from here
	short type = 0; // 0=vuota,1=bianca, 2=nera, 3=gia passata
} cell;

int find(rc position,vector<vector<cell>> matrix,int rings, int max, int BW);
int find2(rc position,vector<vector<cell>> matrix,int rings, int max, int BW, int dir);


int main(int argc, char **argv){

	ushort M, N;
	int B, W;
	ifstream in("input.txt");
	// Initialization
	in >> M >> N >> B >> W;
	vector<vector<cell>> matrix(N, vector<cell>(M, cell()));


	// Forbidding invalid moves
	for (int i = 0; i < M; i++){
		matrix[0][i].U = false;
	}
	for (int i = 0; i < N; i++){
		matrix[i][0].L = false;
		matrix[i][M - 1].R = false;
	}
	for (int i = 0; i < M; i++){
		matrix[N - 1][i].D = false;
	}


	// Getting rings' positions
	vector<rc> blacks = vector<rc>(B);
	for (int i = 0; i < B; i++){
		ushort r, c;
		in >> r >> c;
		blacks[i] = rc(r, c);
		matrix[r][c].type = 2;
	}
	vector<rc> whites = vector<rc>(W);
	for (int i = 0; i < W; i++){
		ushort r, c;
		in >> r >> c;
		whites[i] = rc(r, c);
		matrix[r][c].type = 1;
	}

	rc start=rc(1,1);
	if(B>0){
		start=blacks[0];
	}else{
		start=whites[0];
	}
	int rings=1;
	

	ofstream out("output.txt");

	for (int i = 0; i < N; i++){
		for (int j = 0; j < M; j++){
			cout << matrix[i][j].type << " ";
		}
		cout << endl;
	}

	int res=find(start, matrix, rings, 0,B+W);
	cout << res;
	return 0;
}

int find(rc position,vector<vector<cell>> matrix,int rings, int max, int BW){
	cout<<"find called"<<endl;
	
	int rPath;
	if(rings==BW){
		cout<<"return Rings"<<endl;
		return rings;
	}
		if(matrix[position.first][position.second].type==1 || matrix[position.first][position.second].type==2){
			cout<<"Rings++"<<endl;
			rings++;
		}
	matrix[position.first][position.second].type=3;
	if(matrix[position.first][position.second].R && matrix[position.first][position.second+1].type!=3){
		//giro->sputc('R');
		cout<<"R"<<endl;
		rc tmp;
		tmp.first=position.first;
		tmp.second=position.second+1;
		rPath=find(tmp,matrix,rings,max,BW);
		if(rPath>max){
			max=rPath;
		}
	}
	if(matrix[position.first][position.second].D && matrix[position.first][position.second+1].type!=3){
		cout<<"D"<<endl;
		//giro->sputc('D');
		rc tmp;
		tmp.first=position.first;
		tmp.second=position.second+1;
		rPath=find(tmp,matrix,rings,max,BW);
		if(rPath>max){
			max=rPath;
		}	
	}
	if(matrix[position.first][position.second].L && matrix[position.first][position.second-1].type!=3){
		cout<<"L"<<endl;
		//giro->sputc('L');
		rc tmp;
		tmp.first=position.first;
		tmp.second=position.second-1;
		rPath=find(tmp,matrix,rings,max,BW);
		if(rPath>max){
			max=rPath;
		}
	}

	if(matrix[position.first][position.second].U && matrix[position.first-1][position.second].type!=3){
		cout<<"U"<<endl;
		//giro->sputc('U');
		rc tmp;
		tmp.first=position.first-1;
		tmp.second=position.second;
		rPath=find(tmp,matrix,rings,max, BW);
		if(rPath>max){
			max=rPath;
		}	
	}
	cout<<"Return max"<<endl;
	return max;
}

//dir e la direzione del movimento da effettuare
//1=destra 2=basso 3=sinistra 4=alto
int find2(rc position,vector<vector<cell>> matrix,int rings, int S[], int BW, int dir, int i){

	S[i]=dir;

	switch (dir){
	case 1: position.second++; break;
	case 2: position.first++; break;
	case 3: position.second--; break;
	case 4: position.first++; break;
	}

	if(matrix[position.first][position.second].type==1 || matrix[position.first][position.second].type==2){
			cout<<"Rings++"<<endl;
			rings++;
	}

	if(rings==BW){
		cout<<"soluzione trovata"<<endl;
		return rings;
	}else{
		//calcola l'insieme delle scelte in funzione di S[1...i-1]
			switch (dir){
		case 1: matrix[position.first][position.second].L=false; break;
		case 2: matrix[position.first][position.second].U=false; break;
		case 3: matrix[position.first][position.second].R=false; break;
		case 4: matrix[position.first][position.second].D=false; break;
	}
		//itera sull insieme delle scelte
	}
}