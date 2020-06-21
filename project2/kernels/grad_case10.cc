#include "../run2.h" 

void grad_case10(float (&dA)[8][8], float (&dB)[10][8]) {
float temp1[10][8];
float temp2[10][8];
float temp3[10][8];
float temp4[10][8];
float temp5[10][8];
float temp6[10][8];
float temp7[10][8];
float temp8[10][8];
  for (int i = 0;i < 10;i++) {
    for (int j = 0;j < 8;j++) {
      temp1[i][j] = 0;
      temp2[i][j] = 0;
      temp2[i][j] = (temp2[i][j] + (dA[i][j] / 3));
      temp1[i][j] = temp2[i][j];
    }
  }
  for (int i = 0;i < 10;i++) {
    for (int j = 0;j < 8;j++) {
      dB[i][j] = temp1[i][j];
    }
  }
  for (int tmp1 = 0;tmp1 < 10;tmp1++) {
    for (int j = 0;j < 8;j++) {
      temp3[tmp1][j] = 0;
      temp4[tmp1][j] = 0;
      temp5[tmp1][j] = 0;
      temp4[tmp1][j] = (temp4[tmp1][j] + dB[tmp1][j]);
      if ((tmp1 - 1) >= 0) {
        if ((tmp1 - 1) < 8) {
          temp5[tmp1][j] = (temp5[tmp1][j] + (dA[(tmp1 - 1)][j] / 3));
        }
      }
      temp3[tmp1][j] = (temp4[tmp1][j] + temp5[tmp1][j]);
    }
  }
  for (int tmp1 = 0;tmp1 < 10;tmp1++) {
    for (int j = 0;j < 8;j++) {
      dB[tmp1][j] = temp3[tmp1][j];
    }
  }
  for (int tmp2 = 0;tmp2 < 10;tmp2++) {
    for (int j = 0;j < 8;j++) {
      temp6[tmp2][j] = 0;
      temp7[tmp2][j] = 0;
      temp8[tmp2][j] = 0;
      temp7[tmp2][j] = (temp7[tmp2][j] + dB[tmp2][j]);
      if ((tmp2 - 2) >= 0) {
        if ((tmp2 - 2) < 8) {
          temp8[tmp2][j] = (temp8[tmp2][j] + (dA[(tmp2 - 2)][j] / 3));
        }
      }
      temp6[tmp2][j] = (temp7[tmp2][j] + temp8[tmp2][j]);
    }
  }
  for (int tmp2 = 0;tmp2 < 10;tmp2++) {
    for (int j = 0;j < 8;j++) {
      dB[tmp2][j] = temp6[tmp2][j];
    }
  }
}
