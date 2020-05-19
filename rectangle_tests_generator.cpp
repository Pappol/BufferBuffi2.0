#include <bits/stdc++.h>
#include <fstream>
using namespace std;

int main() {
  int seed = time(0);
  srand(seed);
  const string folder = "rectangles";
  system("mkdir rectangles");
  ofstream inputFile(folder + "/r_" + to_string(seed) + ".txt");
  ofstream outFile(folder + "/r_sol_" + to_string(seed) + ".txt");
  int dim = rand() % 256;
  int m = rand() % (dim - 2) + 2;
  int n = rand() % (dim - 2) + 2;
  int blacks = rand() % 5;
  int whites = rand() % 9;
  cout << blacks << " blacks\n";
  cout << whites << " whites\n";
  pair<int, int> upperLeft = pair<int, int>(rand() % n, rand() % m);
  int mat[m][n];
  for (int i = 0; i < m; i++)
    for (int j = 0; j < n; j++)
      mat[i][j] = 0;

  pair<int, int> uL, uR, bL, bR;
  uL = uR = bL = bR = pair<int, int>(-1, -1);
  int top, bottom, left, right;
  bottom = rand() % (m - 1) + 1;
  right = rand() % (n - 1) + 1;
  top = rand() % bottom;
  left = rand() % right;
  while (blacks > 0) {
    int whereDoIPutIt = -1;
    while (whereDoIPutIt == -1) {
      whereDoIPutIt = rand() % 4;
      switch (whereDoIPutIt) {
      case 0: // uL
        if (uL.first != -1)
          whereDoIPutIt = -1;
        else {
          uL.first = top, uL.second = left;
          mat[uL.first][uL.second] = 2;
        }
        break;
      case 1: // uR
        if (uR.first != -1)
          whereDoIPutIt = -1;
        else {
          uR.first = top, uR.second = right;
          mat[uR.first][uR.second] = 2;
        }
        break;
      case 2: // bL
        if (bL.first != -1)
          whereDoIPutIt = -1;
        else {
          bL.first = bottom, bL.second = left;
          mat[bL.first][bL.second] = 2;
        }
        break;
      case 3: // bR
        if (bR.first != -1)
          whereDoIPutIt = -1;
        else {
          bR.first = bottom, bR.second = right;
          mat[bR.first][bR.second] = 2;
        }
      }
    }
    blacks--;
  }
  cout << "uL: " << uL.first << " " << uL.second << "\n";
  cout << "uR: " << uR.first << " " << uR.second << "\n";
  cout << "bL: " << bL.first << " " << bL.second << "\n";
  cout << "bR: " << bR.first << " " << bR.second << "\n";

  //   w1    w2
  // w3        w4
  //
  // w5        w6
  //   w7    w8
  int w1, w2, w3, w4, w5, w6, w7, w8;
  w1 = w2 = w3 = w4 = w5 = w6 = w7 = w8 = 0;
  while (whites > 0) {
    int pos = -1;
    while (pos == -1) {
      pos = rand() % 7 + 1;
      switch (pos) {
      case 1:
        if (w1 == 0)
          w1 = 1;
        else
          pos = -1;
        break;
      case 2:
        if (w2 == 0)
          w2 = 1;
        else
          pos = -1;
        break;
      case 3:
        if (w3 == 0)
          w3 = 1;
        else
          pos = -1;
        break;
      case 4:
        if (w4 == 0)
          w4 = 1;
        else
          pos = -1;
        break;
      case 5:
        if (w5 == 0)
          w5 = 1;
        else
          pos = -1;
        break;
      case 6:
        if (w6 == 0)
          w6 = 1;
        else
          pos = -1;
        break;
      case 7:
        if (w7 == 0)
          w7 = 1;
        else
          pos = -1;
        break;
      }
    }
    whites--;
  }
  if (right - left > 1) {
    mat[top][left + 1] = w1;
    mat[top][right - 1] = w2;
    mat[bottom][left + 1] = w7;
    mat[bottom][right - 1] = w8;
  } else
    cout << "right - left <= 1\n";
  if (bottom - top > 1) {
    mat[top + 1][left] = w3;
    mat[top + 1][right] = w4;
    mat[bottom - 1][left] = w5;
    mat[bottom - 1][right] = w6;
  } else
    cout << "top - botttom <= 1\n";
  for (int i = 0; i < m; i++) {
    for (int j = 0; j < n; j++) {
      if (mat[i][j] > 0) {
        cout << mat[i][j];
      } else
        cout << ".";
      cout << " ";
    }
    cout << endl;
  }

  int anelliTot = 0;
  int btot = 0;
  int wTot = 0;
  stringbuf directions = stringbuf();
  for (int i = left; i < right; i++) {
    directions.sputc('R');
  }
  for (int i = top + 1; i <= bottom; i++) {
    directions.sputc('D');
  }
  for (int i = right - 1; i >= left; i--) {
    directions.sputc('L');
  }
  for (int i = bottom; i > top; i--) {
    directions.sputc('U');
  }
  for (int i = 0; i < m; i++) {
    for (int j = 0; j < n; j++) {
      if (mat[i][j] != 0)
        anelliTot++;
      if (mat[i][j] == 1)
        wTot++;
      else if (mat[i][j] == 2)
        btot++;
    }
  }
  outFile << anelliTot << " " << (right - left + bottom - top) * 2 << " " << top
          << " " << left << " " << directions.str() << "#";
  outFile.close();

  inputFile << m << " " << n << " " << btot << " " << wTot << endl;
  for (int i = 0; i < m; i++) {
    for (int j = 0; j < n; j++) {
      if (mat[i][j] == 2)
        inputFile << i << " " << j << endl;
    }
  }
  for (int i = 0; i < m; i++) {
    for (int j = 0; j < n; j++) {
      if (mat[i][j] == 1)
        inputFile << i << " " << j << endl;
    }
  }
}
