#include "../run2.h" 

void grad_case4(float (&B)[16][32], float (&C)[32][32], float (&dA)[16][32], float (&dB)[16][32], float (&dC)[32][32]) {
float temp1[16][32];
float temp2[16][32];
float temp3[32][32];
float temp4[32][32];
  for (int i = 0;i < 16;i++) {
    for (int k = 0;k < 32;k++) {
      temp1[i][k] = 0;
      temp2[i][k] = 0;
      for (int j = 0;j < 32;j++) {
        temp2[i][k] = (temp2[i][k] + (dA[i][j] * C[k][j]));
      }
      temp1[i][k] = temp2[i][k];
    }
  }
  for (int i = 0;i < 16;i++) {
    for (int k = 0;k < 32;k++) {
      dB[i][k] = temp1[i][k];
    }
  }
  for (int k = 0;k < 32;k++) {
    for (int j = 0;j < 32;j++) {
      temp3[k][j] = 0;
      temp4[k][j] = 0;
      for (int i = 0;i < 16;i++) {
        temp4[k][j] = (temp4[k][j] + (dA[i][j] * B[i][k]));
      }
      temp3[k][j] = temp4[k][j];
    }
  }
  for (int k = 0;k < 32;k++) {
    for (int j = 0;j < 32;j++) {
      dC[k][j] = temp3[k][j];
    }
  }
}
