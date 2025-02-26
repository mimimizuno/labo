#include <stdio.h>
#include "function.h"
#define SEO_PARTICLES 2       // 振動子 z
#define SEO_COLUMNS 16        // 振動子 x
#define SEO_ROWS 10           // 振動子 y
#define OWSEO_COLUMNS 16      // 一方通行回路 x
#define OWSEO_ROWS 10         // 一方通行回路 y
#define R 1.5                 // 抵抗値[GΩ]
#define Rsmall 0.8            // 小さめの抵抗値[GΩ]
#define Rj 0.001              // トンネル抵抗[GΩ]
#define C 2                   // 接合容量[aF]
#define Vd_seo 0.004          // 振動子のバイアス電圧
#define Vd_owseo 0.0039       // 一方通行回路のバイアス電圧
#define Cjs1 18               // 足1振動子のトンネル容量[aF]
#define Cjs2 16               // 足2振動子のトンネル容量[aF]
#define Cjs3 14               // 足3振動子のトンネル容量[aF]
#define Cjs4 12               // 足4振動子のトンネル容量[aF]
#define Cjs5 10               // 足5振動子のトンネル容量[aF]
#define Cjs6 8                // 足6振動子のトンネル容量[aF]
#define multi_Cjs1 360        // 20重における足1振動子のトンネル容量(18 * 20)[aF]
#define multi_Cjs2 320        // 20重における足2振動子のトンネル容量(16 * 20)[aF]
#define multi_Cjs3 280        // 20重における足3振動子のトンネル容量(14 * 20)[aF]
#define multi_Cjs4 240        // 20重における足4振動子のトンネル容量(12 * 20)[aF]
#define multi_Cjs5 200        // 20重における足5振動子のトンネル容量(10 * 20)[aF]
#define multi_Cjs6 160        // 20重における足6振動子のトンネル容量(8 * 20)[aF]
#define multi_junction_num 20 // 多重トンネル接合の数
#define left 0                // 左変数
#define right 1               // 右変数
#define seo_number 8          // 振動子の層の総数
#define owseo_number 12       // 一方通行振動子の層の総数

int main()
{
    double t = 0, dt = 0.1, pt = 0; // 共通パラメータ
    int i = 0, j = 0, k = 0, rep = 0;
    double st_time = 0, end_time = 0;
    FILE *fp;
    st_time = getTime(); // 実行開始時刻を記録

    fp = fopen("single_test.txt", "w");

    if (fp == NULL)
    {
        printf("file open error.\n");
    }
    /* ------------初期設定---------------------------------------------　*/
    multiseo *seo_pointor[2];
    multiseo seo[2] = {0};
    seo_pointor[0] = seo;
    seo_pointor[1] = seo;


    /* ------------バイアス電圧---------------------------------------------　*/
    seo[1].Vd = Vd_seo;
    seo[1].multi_num = multi_junction_num;

    printf("%f %f %f %f %f %f\n", seo[1].V1, seo[1].V2, seo[1].V3, seo[1].V4, seo[1].V5, seo[1].V6);
    printf("%f\n",multiseo_vth(&seo[1], 1, C, multi_Cjs1));

    /* ------------whileループ---------------------------------------------　*/
    int roop_num = 0;
    while (t < 200)
    {
        // 無限ループの回避
        if(roop_num < 10000){
            roop_num++;
        }
        else break;
        /* ----------------出力-----------------------　*/
        fprintf(fp, "%f %f %f %f %f %f %f %f\n", t, seo[1].Vn, seo[1].Vd, seo[1].dE, seo[1].V1, seo[1].dQ, seo[1].Q, seo[0].Vn);
        /* ----------------トリガ-----------------------　*/
        if (t > 100 && t < 101)
        {
            printf("%f tunnel_sum = %d\n", t,seo[1].tunnel_num);
            seo[0].Vn = 0.006;
        }
        else
        {
            seo[0].Vn = 0;
        }
        /* ----------------パラメータ計算-----------------------　*/
        seo[1].V1 = seo[0].Vn;
        multiseo_Pcalc(&seo[1], 1, C, multi_Cjs1);
        /* ----------------エネルギー計算-----------------------　*/
        multiseo_Ecalc(&seo[1], 1, C, multi_Cjs1);
        /* ----------------待ち時間計算-----------------------　*/
        seo_pointor[1] = multiseo_3dimwt(&seo[1], 1, 1, 1, Rj);
        /* ----------------トンネル待ち時間比較-----------------------　*/
        seo_pointor[0] = seo_pointor[1];
        /* ----------------トンネル発生-----------------------　*/
        multitunnelprintseo(seo, seo_pointor[0], &t, &dt);
        /* ----------------チャージとdtリセット-----------------------　*/
        multiseo_3dimCharge(seo, 1, 1, 2, R, dt);
        dt = 0.1;
    }
    end_time = getTime();          // 実行終了時刻を記録
    getRunTime(st_time, end_time); // 実行時間を表示
}