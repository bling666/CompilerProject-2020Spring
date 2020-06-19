#include "../run2.h" 

void grad_case1(float (&B)[4][16], float (&dC)[4][16], float (&dA)[4][16]) {
float temp1[4][16];
float temp2[4][16];
float temp3[4][16];
  for (int i = 0;i < 4;i++) {
    for (int j = 0;j < 16;j++) {
      temp1[i][j] = 0;
      temp2[i][j] = 0;
      temp3[i][j] = 0;
      temp2[i][j] = (temp2[i][j] + (dC[i][j] * B[i][j]));
      temp3[i][j] = (temp3[i][j] + 0);
      temp1[i][j] = (temp2[i][j] + temp3[i][j]);
    }
  }
  for (int i = 0;i < 4;i++) {
    for (int j = 0;j < 16;j++) {
      dA[i][j] = temp1[i][j];
    }
  }
}
