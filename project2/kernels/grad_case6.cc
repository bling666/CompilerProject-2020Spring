#include "../run2.h" 

void grad_case6(float (&C)[8][16][3][3], float (&dA)[2][8][5][5], float (&dB)[2][16][7][7]) {
float temp1[2][16][7][7];
float temp2[2][16][7][7];
  for (int n = 0;n < 2;n++) {
    for (int c = 0;c < 16;c++) {
      for (int tmp1 = 0;tmp1 < 7;tmp1++) {
        for (int tmp2 = 0;tmp2 < 7;tmp2++) {
          temp1[n][c][tmp1][tmp2] = 0;
          temp2[n][c][tmp1][tmp2] = 0;
          for (int k = 0;k < 8;k++) {
            for (int r = 0;r < 3;r++) {
              for (int s = 0;s < 3;s++) {
                if ((tmp2 - s) >= 0) {
                  if ((tmp2 - s) < 5) {
                    if ((tmp1 - r) >= 0) {
                      if ((tmp1 - r) < 5) {
                        temp2[n][c][tmp1][tmp2] = (temp2[n][c][tmp1][tmp2] + (dA[n][k][(tmp1 - r)][(tmp2 - s)] * C[k][c][r][s]));
                      }
                    }
                  }
                }
              }
            }
          }
          temp1[n][c][tmp1][tmp2] = temp2[n][c][tmp1][tmp2];
        }
      }
    }
  }
  for (int n = 0;n < 2;n++) {
    for (int c = 0;c < 16;c++) {
      for (int tmp1 = 0;tmp1 < 7;tmp1++) {
        for (int tmp2 = 0;tmp2 < 7;tmp2++) {
          dB[n][c][tmp1][tmp2] = temp1[n][c][tmp1][tmp2];
        }
      }
    }
  }
}
