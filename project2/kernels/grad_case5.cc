#include "../run2.h" 

void grad_case5(float (&C)[32][32], float (&D)[4][32], float (&dA)[16][32], float (&dB)[16][32][4]) {
float temp1[16][32][4];
float temp2[16][32][4];
  for (int i = 0;i < 16;i++) {
    for (int k = 0;k < 32;k++) {
      for (int l = 0;l < 4;l++) {
        temp1[i][k][l] = 0;
        temp2[i][k][l] = 0;
        for (int j = 0;j < 32;j++) {
          temp2[i][k][l] = (temp2[i][k][l] + (dA[i][j] * (C[k][j] * D[l][j])));
        }
        temp1[i][k][l] = temp2[i][k][l];
      }
    }
  }
  for (int i = 0;i < 16;i++) {
    for (int k = 0;k < 32;k++) {
      for (int l = 0;l < 4;l++) {
        dB[i][k][l] = temp1[i][k][l];
      }
    }
  }
}
