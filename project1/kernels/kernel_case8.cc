#include "../run.h" 

void kernel_case8(float (&B)[8][16], float (&A)[8][2][16]) {
float temp1[8][2][16];
float temp2[8][2][16];
  for (int i = 0;i < 8;i++) {
    for (int j = 0;j < 2;j++) {
      for (int k = 0;k < 16;k++) {
        temp1[i][j][k] = 0;
        temp2[i][j][k] = 0;
        temp2[i][j][k] = (temp2[i][j][k] + B[i][k]);
        temp1[i][j][k] = temp2[i][j][k];
      }
    }
  }
  for (int i = 0;i < 8;i++) {
    for (int j = 0;j < 2;j++) {
      for (int k = 0;k < 16;k++) {
        A[i][j][k] = temp1[i][j][k];
      }
    }
  }
}
