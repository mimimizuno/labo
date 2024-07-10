#include <iostream>
#include <vector>
#include "function.h"
#define SEO_PARTICLES 2  // 振動子 z
#define SEO_COLUMNS 16   // 振動子 x
#define SEO_ROWS 10      // 振動子 y
#define OWSEO_COLUMNS 15 // 一方通行回路 x
#define OWSEO_ROWS 9     // 一方通行回路 y
#define R 1.2            // 抵抗値[GΩ]
#define Rj 0.001         // トンネル抵抗[GΩ]
#define C 2              // 接合容量[aF]
#define Vd_seo 0.004     // 振動子のバイアス電圧
#define Vd_owseo 0.0039  // 一方通行回路のバイアス電圧
#define Cjs1 18          // 足1振動子のトンネル容量[aF]
#define Cjs2 16          // 足2振動子のトンネル容量[aF]
#define Cjs3 14          // 足3振動子のトンネル容量[aF]
#define Cjs4 12          // 足4振動子のトンネル容量[aF]
#define Cjs6 8           // 足6振動子のトンネル容量[aF]
#define left 0           // 左変数
#define right 1          // 右変数
#define seo_number 8     // 振動子の層の総数
#define owseo_number 12  // 一方通行振動子の層の総数

using namespace std;

int main()
{
    double t = 0, dt = 0.1, pt = 0; // 共通パラメータ
    int i = 0, j = 0, k = 0, rep = 0;
    double st_time = 0, end_time = 0;
    FILE *fp;

    st_time = getTime(); // 実行開始時刻を記録
    fp = fopen("connect.txt", "w");
    if (fp == NULL)
    {
        printf("file open error.\n");
        return -1;
    }

    /*初期化--------------------------------------------------------------------------------------------------------------------------------------------------------*/
    // 回路のポインタ

    seo *seoP[4];
    oneway_4seo *owseoP[4];
    seo seotrigger[2] = {0};
    seo command_d[2] = {0};
    seoP[1] = command_d;
    seo command_r[2] = {0};
    seoP[2] = command_r;
    seo detection[1] = {0};
    seoP[3] = detection;

    oneway_4seo dD[2] = {0};
    owseoP[1] = dD;
    oneway_4seo Dr[2] = {0};
    owseoP[2] = Dr;
    oneway_4seo dd[2] = {0};
    owseoP[3] = dd;

    /*----------------------------------------------------------------------------------------------------------------------------------------------------------------*/

    /*Vdの設定--------------------------------------------------------------------------------------------------------------------------------------------------------*/

    for (auto &x : command_d) x.Vd = Vd_seo;
    for (auto &x : command_r) x.Vd = Vd_seo;
    for (auto &x : detection) x.Vd = Vd_seo; // 一個分で変化するかどうか2つの時は-viasarrange(C, 4, 3, Cjs4, Cjs3)
    for (int i = 0; i < 2; i++) oneway_4seo_setVd(&dD[i], Vd_owseo, right, C, Cjs2, Cjs3);
    for (int i = 0; i < 2; i++) oneway_4seo_setVd(&Dr[i], Vd_owseo, right, C, Cjs2, Cjs3);
    for (int i = 0; i < 2; i++) oneway_4seo_setVd(&dd[i], Vd_owseo, right, C, Cjs2, Cjs3);
    int tin;
    cin >> tin;

    /*----------------------------------------------------------------------------------------------------------------------------------------------------------------*/
    while (t < 300)
    {
        // ファイル書き込み[V]
        fprintf(fp, "%f %f %f %f %f %f\n", t, command_d[0].Vn, command_d[1].Vn, detection[0].Vn, command_r[0].Vn, command_r[1].Vn);

        /*トリガの設定-----------------------------------------------------------------------------------------------------------------------------------------------------*/
        if (t > 200 && t < 201)
        {
            seotrigger[1].Vn = 0.006;
        }
        if (t > tin && t < tin + 1)
        {
            seotrigger[1].Vn = 0.006;
        }
        else
        {
            seotrigger[0].Vn = 0;
            seotrigger[1].Vn = 0;
        }
        /*----------------------------------------------------------------------------------------------------------------------------------------------------------------*/

        /*パラメータ設定---------------------------------------------------------------------------------------------------------------------------------------------------*/
        for (rep = 0; rep < 5; rep++) // 値の平均化
        {
            // command d
            for (i = 0; i < 2; i++)
            {
                command_d[i].V1 = seotrigger[i].Vn;
                command_d[i].V2 = dD[i].ows[0].Vn;
                if (i == 0) command_d[i].V3 = dd[1].ows[0].Vn;
                else command_d[i].V3 = dd[0].ows[0].Vn;
                seo_Pcalc(&command_d[i], 3, C, Cjs3);
            }

            // detection
            for (i = 0; i < 1; i++)
            {
                detection[i].V1 = dD[0].ows[3].Vn;
                detection[i].V2 = dD[1].ows[3].Vn;
                detection[i].V3 = Dr[0].ows[0].Vn;
                detection[i].V4 = Dr[1].ows[0].Vn;
                seo_Pcalc(&detection[i], 4, C, Cjs4);
            }

            // command r
            for (i = 0; i < 2; i++)
            {
                command_r[i].V1 = Dr[i].ows[3].Vn;
                command_r[i].V2 = dd[i].ows[3].Vn;
                seo_Pcalc(&command_r[i], 2, C, Cjs2);
            }

            // dD
            for (i = 0; i < 2; i++)
            {
                oneway_4seo_calcPara(&dD[i], C, Cjs2, Cjs3, command_d[i].Vn, detection[0].Vn);
            }

            // Dr
            for (i = 0; i < 2; i++)
            {
                oneway_4seo_calcPara(&Dr[i], C, Cjs2, Cjs3, detection[0].Vn, command_r[i].Vn);
            }

            // dd
            for (i = 0; i < 2; i++)
            {
                if (i == 0) oneway_4seo_calcPara(&dd[i], C, Cjs2, Cjs3, command_d[1].Vn, command_r[0].Vn);
                else oneway_4seo_calcPara(&dd[i], C, Cjs2, Cjs3, command_d[0].Vn, command_r[1].Vn);
            }
        }

        /*----------------------------------------------------------------------------------------------------------------------------------------------------------------*/

        /*エネルギー計算---------------------------------------------------------------------------------------------------------------------------------------------------*/
        // command d
        for (int i = 0; i < 2; i++) seo_Ecalc(&command_d[i], 3, C, Cjs3);
        // detection
        for (int i = 0; i < 1; i++) seo_Ecalc(&detection[i], 4, C, Cjs4);
        // command r
        for (int i = 0; i < 2; i++) seo_Ecalc(&command_r[i], 2, C, Cjs2);
        // dD
        for (i = 0; i < 2; i++) oneway_4seo_calcEner(&dD[i], C, Cjs2, Cjs3);
        // Dr
        for (i = 0; i < 2; i++) oneway_4seo_calcEner(&Dr[i], C, Cjs2, Cjs3);
        // dd
        for (i = 0; i < 2; i++) oneway_4seo_calcEner(&dd[i], C, Cjs2, Cjs3);
        /*----------------------------------------------------------------------------------------------------------------------------------------------------------------*/

        /*トンネル計算-----------------------------------------------------------------------------------------------------------------------------------------------------*/
        // command d
        seoP[1] = seo_2dimwt(command_d, 2, 1, Rj);

        // detection
        seoP[2] = seo_2dimwt(detection, 1, 1, Rj);

        // command r
        seoP[3] = seo_2dimwt(command_r, 2, 1, Rj);

        // dD
        owseoP[1] = oneway_4seo_3dimWt(dD, 2, 1, 1, Rj);

        // Dr
        owseoP[2] = oneway_4seo_3dimWt(Dr, 2, 1, 1, Rj);

        // dd
        owseoP[3] = oneway_4seo_3dimWt(dd, 2, 1, 1, Rj);

        /*----------------------------------------------------------------------------------------------------------------------------------------------------------------*/

        /*トンネルと電荷チャージ--------------------------------------------------------------------------------------------------------------------------------------------*/
        // トンネル待ち時間比較
        // 振動子比較(seo_p[0]が待ち時間の最小になるように)
        seoP[0] = seoP[1];
        for (i = 2; i < 4; i++)
        {
            if (seoP[0]->wt[seoP[0]->tunnel] > seoP[i]->wt[seoP[i]->tunnel])
            {
                seoP[0] = seoP[i];
            }
        }

        // 一方通行比較(owseo_p[0]が待ち時間の最小になるように)
        owseoP[0] = owseoP[1];
        for (i = 2; i < 4; i++)
        {
            if (owseoP[0]->ows[owseoP[0]->locate].wt[owseoP[0]->ows[owseoP[0]->locate].tunnel] > owseoP[i]->ows[owseoP[i]->locate].wt[owseoP[i]->ows[owseoP[i]->locate].tunnel])
            {
                owseoP[0] = owseoP[i];
            }
        }

        // トンネル発生
        Seo_onway4Seo_tunnel(seoP[0], owseoP[0], &t, &dt);

        // トンネルチャージとdtのリセット
        // 振動子のチャージ
        seo_3dimcharge(command_d, 2, 1, 1, R, dt);
        seo_3dimcharge(detection, 1, 1, 1, R, dt);
        seo_3dimcharge(command_r, 2, 1, 1, R, dt);

        // 一方通行のチャージ
        oneway_4seo_3dimCharge(dD, 2, 1, 1, R, dt);
        oneway_4seo_3dimCharge(Dr, 2, 1, 1, R, dt);
        oneway_4seo_3dimCharge(dd, 2, 1, 1, R, dt);

        // dtのリセット
        dt = 0.1;

        /*----------------------------------------------------------------------------------------------------------------------------------------------------------------*/
    }
    end_time = getTime();          // 実行終了時刻を記録
    getRunTime(st_time, end_time); // 実行時間を表示
}