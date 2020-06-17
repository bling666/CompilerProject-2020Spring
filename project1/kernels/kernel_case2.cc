#include "../run.h" 

void kernel_case2(float (&A)[16][8]) {
float temp1[16][8];
float temp2[16][8];
float temp3[16][8];
  for (int i = 0;i < 16;i++) {
    for (int j = 0;j < 8;j++) {
      temp1[i][j] = 0;
      temp2[i][j] = 0;
      temp3[i][j] = 0;
      temp2[i][j] = (temp2[i][j] + A[i][j]);
      temp3[i][j] = (temp3[i][j] + 2);
      temp1[i][j] = (temp2[i][j] + temp3[i][j]);
    }
  }
  for (int i = 0;i < 16;i++) {
    for (int j = 0;j < 8;j++) {
      A[i][j] = temp1[i][j];
    }
  }
}
