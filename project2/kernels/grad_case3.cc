#include "../run2.h" 

void grad_case3(float (&B)[16][16], float (&dC)[4][16], float (&dA)[4][16]) {
float temp1[4][16];
float temp2[4][16];
  for (int i = 0;i < 4;i++) {
    for (int k = 0;k < 16;k++) {
      temp1[i][k] = 0;
      temp2[i][k] = 0;
      for (int j = 0;j < 16;j++) {
        temp2[i][k] = (temp2[i][k] + (dC[i][j] * B[k][j]));
      }
      temp1[i][k] = temp2[i][k];
    }
  }
  for (int i = 0;i < 4;i++) {
    for (int k = 0;k < 16;k++) {
      dA[i][k] = temp1[i][k];
    }
  }
}
