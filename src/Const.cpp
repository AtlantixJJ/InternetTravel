#include "Const.h"
#include <stdio.h>

extern int  Const::FACTORIAL[6] = {1, 1, 2, 6, 24, 120};
extern bool Const::fac_num[6] = {false};
extern int  Const::fac_cnt = 0;
extern int  Const::PERMUTATIONS[6][121][5]= {0};

void Const::write_permutations(int depth, int N) {
    int i;
    for(i = 0; i < N; i++) {
        if (!Const::fac_num[i]) {
            Const::PERMUTATIONS[N][Const::fac_cnt][depth] = i;
            Const::fac_num[i] = true;
            if(depth == N-1)
                Const::fac_cnt += 1;
            else
                Const::write_permutations(depth + 1, N);
            for(int j = 0; j <= depth; j++)
                Const::PERMUTATIONS[N][Const::fac_cnt][j] = Const::PERMUTATIONS[N][Const::fac_cnt-1][j];
    
            Const::fac_num[i] = false;
        }
    }
}
