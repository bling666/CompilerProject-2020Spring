#include "../run2.h" 

void grad_case2(float (&A)[4][16], float (&dB)[4][16], float (&dA)[4][16]) {
float temp1[4][16];
float temp2[4][16];
float temp3[4][16];
  for (int i = 0;i < 4;i++) {
    for (int j = 0;j < 16;j++) {
      temp1[i][j] = 0;
      temp2[i][j] = 0;
      temp3[i][j] = 0;
      temp2[i][j] = (temp2[i][j] + (dB[i][j] * (A[i][j] * 2)));
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
