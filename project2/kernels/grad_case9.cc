#include "../run2.h" 

void grad_case9(float (&dB)[4][6], float (&dA)[4]) {
float temp1[4];
float temp2[4];
  for (int i = 0;i < 4;i++) {
    temp1[i] = 0;
    temp2[i] = 0;
    for (int j = 0;j < 6;j++) {
      temp2[i] = (temp2[i] + dB[i][j]);
    }
    temp1[i] = temp2[i];
  }
  for (int i = 0;i < 4;i++) {
    dA[i] = temp1[i];
  }
}
