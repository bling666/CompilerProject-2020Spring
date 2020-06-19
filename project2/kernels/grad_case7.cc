#include "../run2.h" 

void grad_case7(float (&dB)[16][32], float (&dA)[32][16]) {
float temp1[32][16];
float temp2[32][16];
  for (int j = 0;j < 32;j++) {
    for (int i = 0;i < 16;i++) {
      temp1[j][i] = 0;
      temp2[j][i] = 0;
      temp2[j][i] = (temp2[j][i] + dB[i][j]);
      temp1[j][i] = temp2[j][i];
    }
  }
  for (int j = 0;j < 32;j++) {
    for (int i = 0;i < 16;i++) {
      dA[j][i] = temp1[j][i];
    }
  }
}
