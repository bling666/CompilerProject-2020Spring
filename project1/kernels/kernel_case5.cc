#include "../run.h" 

void kernel_case5(float (&B)[16][32], float (&C)[32][32], float (&D)[16][32], float (&alpha), float (&beta), float (&A)[16][32]) {
float temp1[16][32];
float temp2[16][32];
float temp3[16][32];
float temp4[16][32];
float temp5[16][32];
float temp6[16][32];
  for (int i = 0;i < 16;i++) {
    for (int j = 0;j < 32;j++) {
      temp1[i][j] = 0;
      temp2[i][j] = 0;
      temp3[i][j] = 0;
      temp2[i][j] = (temp2[i][j] + A[i][j]);
      for (int k = 0;k < 32;k++) {
        temp3[i][j] = (temp3[i][j] + (alpha * (B[i][k] * C[k][j])));
      }
      temp1[i][j] = (temp2[i][j] + temp3[i][j]);
    }
  }
  for (int i = 0;i < 16;i++) {
    for (int j = 0;j < 32;j++) {
      A[i][j] = temp1[i][j];
    }
  }
  for (int i = 0;i < 16;i++) {
    for (int j = 0;j < 32;j++) {
      temp4[i][j] = 0;
      temp5[i][j] = 0;
      temp6[i][j] = 0;
      temp5[i][j] = (temp5[i][j] + A[i][j]);
      temp6[i][j] = (temp6[i][j] + (beta * D[i][j]));
      temp4[i][j] = (temp5[i][j] + temp6[i][j]);
    }
  }
  for (int i = 0;i < 16;i++) {
    for (int j = 0;j < 32;j++) {
      A[i][j] = temp4[i][j];
    }
  }
}
