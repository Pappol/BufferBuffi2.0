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

int find(rc start,rc position,vector<vector<cell>> matrix,int rings, int max);

int main(int argc, char **argv){

	ushort N, M;
	int B, W;
	ifstream in("input.txt");
	// Initialization
	in >> N >> M >> B >> W;
	vector<vector<cell>> matrix(M, vector<cell>(N, cell()));


	// Forbidding invalid moves
	for (int i = 0; i < N; i++){
		matrix[0][i].U = false;
	}
	for (int i = 0; i < M; i++){
		matrix[i][0].L = false;
		matrix[i][N - 1].R = false;
	}
	for (int i = 0; i < N; i++){
		matrix[M - 1][i].D = false;
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

	for (int i = 0; i < M; i++){
		for (int j = 0; j < N; j++){
			cout << matrix[i][j].type << " ";
		}
		cout << endl;
	}
	cout<< "ciao" <<endl;

	//int res=find(start, start, matrix, rings, 0);
	return 0;
}

int find(rc start,rc position,vector<vector<cell>> matrix,int rings, int max){
	int rPath;
	cout << 1;
	if(position==start && rings>1){
		return rings;
		cout << 1;
	}
	if(position!=start){
		if(matrix[position.first][position.second].type==1 || matrix[position.first][position.second].type==2){
			rings++;
		}
	}
	if(matrix[position.first][position.second].R && matrix[position.first+1][position.second].type!=3){
		cout << 1;
		//giro->sputc('R');
		matrix[position.first+1][position.second].type=3;
		rc tmp;
		tmp.first=position.first+1;
		tmp.second=position.second;
		rPath=find(start,tmp,matrix,rings,max);
		if(rPath>max){
			max=rPath;
		}
	}


	if(matrix[position.first][position.second].D && matrix[position.first][position.second+1].type!=3){
		cout << 1;
		//giro->sputc('D');
		matrix[position.first][position.second+1].type=3;
		rc tmp;
		tmp.first=position.first;
		tmp.second=position.second+1;
		rPath=find(start,tmp,matrix,rings,max);
		if(rPath>max){
			max=rPath;
		}	
	}

	if(matrix[position.first][position.second].L && matrix[position.first-1][position.second].type!=3){
		cout << 1;
		//giro->sputc('L');
		matrix[position.first-1][position.second].type=3;
		rc tmp;
		tmp.first=position.first-1;
		tmp.second=position.second;
		rPath=find(start,tmp,matrix,rings,max);
		if(rPath>max){
			max=rPath;
		}
	}

	if(matrix[position.first][position.second].U && matrix[position.first][position.second-1].type!=3){
		//giro->sputc('U');
		matrix[position.first][position.second-1].type=3;
		rc tmp;
		tmp.first=position.first;
		tmp.second=position.second-1;
		rPath=find(start,tmp,matrix,rings,max);
		if(rPath>max){
			max=rPath;
		}	
		}
	return max;
}