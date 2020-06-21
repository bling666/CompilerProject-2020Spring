#include "../run2.h" 

void grad_case8(float (&dB)[32], float (&dA)[2][16]) {
float temp1[2][16];
float temp2[2][16];
  for (int tmp1 = 0;tmp1 < 2;tmp1++) {
    for (int tmp2 = 0;tmp2 < 16;tmp2++) {
      temp1[tmp1][tmp2] = 0;
      temp2[tmp1][tmp2] = 0;
      temp2[tmp1][tmp2] = (temp2[tmp1][tmp2] + dB[((tmp1 * 16) + tmp2)]);
      temp1[tmp1][tmp2] = temp2[tmp1][tmp2];
    }
  }
  for (int tmp1 = 0;tmp1 < 2;tmp1++) {
    for (int tmp2 = 0;tmp2 < 16;tmp2++) {
      dA[tmp1][tmp2] = temp1[tmp1][tmp2];
    }
  }
}
