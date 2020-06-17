#include "../run.h" 

void kernel_case3(int (&B)[16][32], int (&C)[16][32], int (&A)[16][32]) {
int temp1[16][32];
int temp2[16][32];
int temp3[16][32];
  for (int i = 0;i < 16;i++) {
    for (int j = 0;j < 32;j++) {
      temp1[i][j] = 0;
      temp2[i][j] = 0;
      temp3[i][j] = 0;
      temp2[i][j] = (temp2[i][j] + B[i][j]);
      temp3[i][j] = (temp3[i][j] + C[i][j]);
      temp1[i][j] = (temp2[i][j] + temp3[i][j]);
    }
  }
  for (int i = 0;i < 16;i++) {
    for (int j = 0;j < 32;j++) {
      A[i][j] = temp1[i][j];
    }
  }
}
