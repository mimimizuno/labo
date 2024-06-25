#include <iostream>
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
    FILE *fp1, *fp2, *fp3, *fp4, *fp5, *fp6, *fp7, *fp8, *fp9;

    st_time = getTime(); // 実行開始時刻を記録

    fp1 = fopen("command_d.txt", "w"); // ファイル作成
    fp2 = fopen("detection_d.txt", "w");
    fp3 = fopen("command_l.txt", "w");
    fp4 = fopen("detection_l.txt", "w");
    fp5 = fopen("command_u.txt", "w");
    fp6 = fopen("detection_u.txt", "w");
    fp7 = fopen("command_r.txt", "w");
    fp8 = fopen("detection_r.txt", "w");
    fp9 = fopen("test.txt", "w");

    if (fp1 == NULL || fp2 == NULL || fp3 == NULL || fp4 == NULL || fp5 == NULL || fp6 == NULL || fp7 == NULL || fp8 == NULL || fp9 == NULL)
    {
        printf("file open error.\n");
        return -1;
    }

    /*初期化--------------------------------------------------------------------------------------------------------------------------------------------------------*/
    // 回路のポインタ

    seo *seoP[4];
    oneway_4seo *owseoP[7];
    seo seo_commandDirection_d[2] = {0};
    seoP[1] = &seo_commandDirection_d[0];
    seo seo_commandDirection_r[2];
    seoP[2] = &seo_commandDirection_r[0];
    seo seo_collision[1] = {0};
    seoP[3] = &seo_collision[0];

    oneway_4seo owseo_commandDirection_d[2][2] = {0};
    owseoP[1] = owseo_commandDirection_d[0];
    oneway_4seo owseo_commandDirection_r[2][2] = {0};
    owseoP[2] = owseo_commandDirection_r[0];
    oneway_4seo owseo_CtoD[2] = {0};
    owseoP[3] = owseo_CtoD;
    oneway_4seo owseo_DtoC[2] = {0};
    owseoP[4] = owseo_DtoC;
    oneway_4seo owseo_CtoC[2] = {0};
    owseoP[5] = owseo_CtoC;
    oneway_4seo owseo[2][4] = {0};
    owseoP[6] = owseo[0];


    /*----------------------------------------------------------------------------------------------------------------------------------------------------------------*/

    /*Vdの設定--------------------------------------------------------------------------------------------------------------------------------------------------------*/

    for(auto &x:seo_commandDirection_d) x.Vd = Vd_seo;
    for(auto &x:seo_commandDirection_r) x.Vd = Vd_seo;
    for(auto &x:seo_collision) x.Vd = Vd_seo; // 一個分で変化するかどうか2つの時は-viasarrange(C, 4, 6, Cjs4, Cjs6)
    for(auto &x:owseo_commandDirection_d) oneway_4seo_setVd(x, Vd_owseo, right, C, Cjs2, Cjs3);
    for(auto &x:owseo_commandDirection_r) oneway_4seo_setVd(x, Vd_owseo, right, C, Cjs2, Cjs3);
    for(auto &x:owseo_CtoD) oneway_4seo_setVd(&x, Vd_owseo, right, C, Cjs2, Cjs3);
    for(auto &x:owseo_DtoC) oneway_4seo_setVd(&x, Vd_owseo, right, C, Cjs2, Cjs3);
    for(auto &x:owseo_CtoC) oneway_4seo_setVd(&x, Vd_owseo, right, C, Cjs2, Cjs3);
    for(auto &x:owseo) oneway_4seo_setVd(x, Vd_owseo, right, C, Cjs2, Cjs3);

    int tin;
    cin >> tin;

    /*----------------------------------------------------------------------------------------------------------------------------------------------------------------*/
    while (t < 200)
    {
        // ファイル書き込み[V]
        // fprintlayrow(seo_command_d[0][0], SEO_PARTICLES, SEO_ROWS, SEO_COLUMNS, fp1, t, pt);
        // fprintcollisionlay(seo_detection_d[0], SEO_ROWS, SEO_COLUMNS, fp2, t, pt);
        // fprintlaycolumn(seo_command_l[0][0], SEO_PARTICLES, SEO_ROWS, SEO_COLUMNS, fp3, t, pt);
        // fprintcollisionlay(seo_detection_l[0], SEO_ROWS, SEO_COLUMNS, fp4, t, pt);
        // fprintlayrow(seo_command_u[0][0], SEO_PARTICLES, SEO_ROWS, SEO_COLUMNS, fp5, t, pt);
        // fprintcollisionlay(seo_detection_u[0], SEO_ROWS, SEO_COLUMNS, fp6, t, pt);
        // fprintlaycolumn(seo_command_r[0][0], SEO_PARTICLES, SEO_ROWS, SEO_COLUMNS, fp7, t, pt);
        // fprintcollisionlay(seo_detection_r[0], SEO_ROWS, SEO_COLUMNS, fp8, t, pt);
        // fprintf(fp9, "%f %f %f %f %f\n", t, seo_command_d[0][0][8].Vn, seo_command_d[0][9][8].Vn, seo_command_d[1][0][10].Vn, seo_command_d[1][9][10].Vn);
        // fprintf(fp9, "%f %f %f %f %f %f\n", t, seo_command_l[0][2][12].Vn, owseo_DtoC_dtol[2][12].ows[3].Vn, owseo_command_l[0][2][10].ows[3].Vn, owseo_command_l[0][2][11].ows[0].Vn, owseo_CtoD_l[0][1][11].ows[0].Vn);
        // fprintf(fp9, "%f %f %f %f %f %f\n", t, seo_command_l[0][6][8].Vn, owseo_DtoC_dtol[6][8].ows[3].Vn, owseo_command_l[0][6][6].ows[3].Vn, owseo_command_l[0][6][7].ows[0].Vn, owseo_CtoD_l[0][5][7].ows[0].Vn);
        //fprintf(fp9, "%f %f %f %f %f\n", t, seo_command_d[0][2][10].Vn, owseo_command_d[0][0][10].ows[3].Vn, owseo_command_d[0][0][10].ows[0].Vn, seo_command_d[0][1][10].Vn);
        // fprintf(fp9,"%f %f %f %f %f\n",t,seo_command_d[1][2][5].Vn,owseo_command_d[1][0][5].ows[3].Vn,owseo_command_d[1][0][5].ows[0].Vn,seo_command_d[1][1][5].Vn);
        // fprintf(fp9, "%f %f %f %f %f\n", t, seo_command_d[0][2][8].Vn, owseo_command_d[0][0][8].ows[3].Vn, owseo_command_d[0][0][8].ows[0].Vn, seo_command_d[0][1][8].Vn);
        if (pt <= t)
        {
            pt += 0.1;
        }

        /*トリガの設定-----------------------------------------------------------------------------------------------------------------------------------------------------*/
        if (t > 100 && t < 101)
        {
            owseo_commandDirection_d[0][0].ows[0].V1 = 0.006;
        }
        if (t > tin && t < tin + 1)
        {
            owseo_commandDirection_d[1][0].ows[0].V1 = 0.006;
        }
        else
        {
            owseo_commandDirection_d[0][0].ows[0].V1 = 0.0;
            owseo_commandDirection_d[1][0].ows[0].V1 = 0.0;
        }
        /*----------------------------------------------------------------------------------------------------------------------------------------------------------------*/

        /*パラメータ設定---------------------------------------------------------------------------------------------------------------------------------------------------*/
        for (rep = 0; rep < 5; rep++) // 値の平均化
        {
            // command direction d
            for (i = 0; i < 2; i++) // z
            {
                seo_commandDirection_d[i].V1 = owseo_commandDirection_d[i][0].ows[3].Vn;
                seo_commandDirection_d[i].V2 = owseo_commandDirection_d[i][1].ows[0].Vn;
                seo_commandDirection_d[i].V3 = owseo[0][i].ows[3].Vn;
                seo_commandDirection_d[i].V4 = owseo[1][i].ows[3].Vn;
                seo_commandDirection_d[i].V5 = owseo_CtoD[i].ows[0].Vn;
                seo_commandDirection_d[i].V6 = owseo_DtoC[i].ows[0].Vn;
                seo_Pcalc(&seo_commandDirection_d[i],6,C,Cjs6);
            }

            // 下衝突判定振動子計算
            for (i = 1; i < SEO_ROWS; i++)
            {
                for (j = 1; j < SEO_COLUMNS; j++)
                {
                    seo_detection_d[i][j].V1 = owseo_CtoD_d[0][i - 1][j - 1].ows[3].Vn;
                    seo_detection_d[i][j].V2 = owseo_CtoD_d[1][i - 1][j - 1].ows[3].Vn;
                    seo_detection_d[i][j].V3 = owseo_DtoC_dtol[i][j].ows[0].Vn;
                    seo_Pcalc(&seo_detection_d[i][j], 3, C, Cjs3);
                }
            }

            // 左命令計算 (jで場合分け)
            for (i = 0; i < SEO_PARTICLES; i++) // z
            {
                for (j = 1; j < SEO_ROWS; j++) // y
                {
                    for (k = 1; k < SEO_COLUMNS; k++) // x
                    {
                        if ((i == 0 && (j % 2 == 0)) || (i == 1 && (j % 2 == 1)))
                        {
                            if (k == 1)
                            {
                                seo_command_l[i][j][k].V1 = owseo_DtoC_dtol[j][k].ows[3].Vn;
                                seo_command_l[i][j][k].V2 = owseo_command_l[i][j][k - 1].ows[0].Vn;
                                seo_command_l[i][j][k].V3 = owseo_CtoD_l[i][j - 1][k - 1].ows[0].Vn;
                                seo_Pcalc(&seo_command_l[i][j][k], 3, C, Cjs3);
                            }
                            else if (k == SEO_COLUMNS - 1)
                            {
                                seo_command_l[i][j][k].V1 = owseo_DtoC_dtol[j][k].ows[3].Vn;
                                seo_command_l[i][j][k].V2 = owseo_command_l[i][j][k - 2].ows[3].Vn;
                                seo_command_l[i][j][k].V3 = owseo_CtoD_l[i][j - 1][k - 1].ows[0].Vn;
                                seo_Pcalc(&seo_command_l[i][j][k], 3, C, Cjs3);
                            }
                            else
                            {
                                seo_command_l[i][j][k].V1 = owseo_DtoC_dtol[j][k].ows[3].Vn;
                                seo_command_l[i][j][k].V2 = owseo_command_l[i][j][k - 2].ows[3].Vn;
                                seo_command_l[i][j][k].V3 = owseo_command_l[i][j][k - 1].ows[0].Vn;
                                seo_command_l[i][j][k].V4 = owseo_CtoD_l[i][j - 1][k - 1].ows[0].Vn;
                                seo_Pcalc(&seo_command_l[i][j][k], 4, C, Cjs4);
                            }
                        }
                        else
                        {
                            if (k == 1)
                            {
                                seo_command_l[i][j][k].V1 = owseo_command_l[i][j][k - 1].ows[0].Vn;
                                seo_command_l[i][j][k].V2 = owseo_CtoD_l[i][j - 1][k - 1].ows[0].Vn;
                                seo_Pcalc(&seo_command_l[i][j][k], 2, C, Cjs2);
                            }
                            else if (k == SEO_COLUMNS - 1)
                            {
                                seo_command_l[i][j][k].V1 = owseo_command_l[i][j][k - 2].ows[3].Vn;
                                seo_command_l[i][j][k].V2 = owseo_CtoD_l[i][j - 1][k - 1].ows[0].Vn;
                                seo_Pcalc(&seo_command_l[i][j][k], 2, C, Cjs2);
                            }
                            else
                            {
                                seo_command_l[i][j][k].V1 = owseo_command_l[i][j][k - 2].ows[3].Vn;
                                seo_command_l[i][j][k].V2 = owseo_command_l[i][j][k - 1].ows[0].Vn;
                                seo_command_l[i][j][k].V3 = owseo_CtoD_l[i][j - 1][k - 1].ows[0].Vn;
                                seo_Pcalc(&seo_command_l[i][j][k], 3, C, Cjs3);
                            }
                        }
                    }
                }
            }

            // 左衝突判定計算
            for (i = 1; i < SEO_ROWS; i++)
            {
                for (j = 1; j < SEO_COLUMNS; j++)
                {
                    seo_detection_l[i][j].V1 = owseo_CtoD_l[0][i - 1][j - 1].ows[3].Vn;
                    seo_detection_l[i][j].V2 = owseo_CtoD_l[1][i - 1][j - 1].ows[3].Vn;
                    seo_detection_l[i][j].V3 = owseo_DtoC_ltou[i][j].ows[0].Vn;
                    seo_Pcalc(&seo_detection_l[i][j], 3, C, Cjs3);
                }
            }

            // 上命令振動子計算
            for (i = 0; i < SEO_PARTICLES; i++) // z
            {
                for (j = 1; j < SEO_ROWS; j++) // y
                {
                    for (k = 1; k < SEO_COLUMNS; k++) // x
                    {
                        if ((i == 0 && (k % 2 == 0)) || (i == 1 && (k % 2 == 1)))
                        {
                            if (j == 1)
                            {
                                seo_command_u[i][j][k].V1 = owseo_DtoC_ltou[j][k].ows[3].Vn;
                                seo_command_u[i][j][k].V2 = owseo_command_u[i][j - 1][k].ows[0].Vn;
                                seo_command_u[i][j][k].V3 = owseo_CtoD_u[i][j - 1][k - 1].ows[0].Vn;
                                seo_Pcalc(&seo_command_u[i][j][k], 3, C, Cjs3);
                            }
                            else if (j == SEO_ROWS - 1)
                            {
                                seo_command_u[i][j][k].V1 = owseo_DtoC_ltou[j][k].ows[3].Vn;
                                seo_command_u[i][j][k].V2 = owseo_command_u[i][j - 2][k].ows[3].Vn;
                                seo_command_u[i][j][k].V3 = owseo_CtoD_u[i][j - 1][k - 1].ows[0].Vn;
                                seo_Pcalc(&seo_command_u[i][j][k], 3, C, Cjs3);
                            }
                            else
                            {
                                seo_command_u[i][j][k].V1 = owseo_DtoC_ltou[j][k].ows[3].Vn;
                                seo_command_u[i][j][k].V2 = owseo_command_u[i][j - 2][k].ows[3].Vn;
                                seo_command_u[i][j][k].V3 = owseo_command_u[i][j - 1][k].ows[0].Vn;
                                seo_command_u[i][j][k].V4 = owseo_CtoD_u[i][j - 1][k - 1].ows[0].Vn;
                                seo_Pcalc(&seo_command_u[i][j][k], 4, C, Cjs4);
                            }
                        }
                        else
                        {
                            if (j == 1)
                            {
                                seo_command_u[i][j][k].V1 = owseo_command_u[i][j - 1][k].ows[0].Vn;
                                seo_command_u[i][j][k].V2 = owseo_CtoD_u[i][j - 1][k - 1].ows[0].Vn;
                                seo_Pcalc(&seo_command_u[i][j][k], 2, C, Cjs2);
                            }
                            else if (j == SEO_ROWS - 1)
                            {
                                seo_command_u[i][j][k].V1 = owseo_command_u[i][j - 2][k].ows[3].Vn;
                                seo_command_u[i][j][k].V2 = owseo_CtoD_u[i][j - 1][k - 1].ows[0].Vn;
                                seo_Pcalc(&seo_command_u[i][j][k], 2, C, Cjs2);
                            }
                            else
                            {
                                seo_command_u[i][j][k].V1 = owseo_command_u[i][j - 2][k].ows[3].Vn;
                                seo_command_u[i][j][k].V2 = owseo_command_u[i][j - 1][k].ows[0].Vn;
                                seo_command_u[i][j][k].V3 = owseo_CtoD_u[i][j - 1][k - 1].ows[0].Vn;
                                seo_Pcalc(&seo_command_u[i][j][k], 3, C, Cjs3);
                            }
                        }
                    }
                }
            }

            // 上衝突判定
            for (i = 1; i < SEO_ROWS; i++)
            {
                for (j = 1; j < SEO_COLUMNS; j++)
                {
                    seo_detection_u[i][j].V1 = owseo_CtoD_u[0][i - 1][j - 1].ows[3].Vn;
                    seo_detection_u[i][j].V2 = owseo_CtoD_u[1][i - 1][j - 1].ows[3].Vn;
                    seo_detection_u[i][j].V3 = owseo_DtoC_utor[i][j].ows[0].Vn;
                    seo_Pcalc(&seo_detection_u[i][j], 3, C, Cjs3);
                }
            }

            // 右命令計算
            for (i = 0; i < SEO_PARTICLES; i++) // z
            {
                for (j = 1; j < SEO_ROWS; j++) // y
                {
                    for (k = 1; k < SEO_COLUMNS; k++) // x
                    {
                        if ((i == 0 && (k % 2 == 0)) || (i == 1 && (k % 2 == 1)))
                        {
                            if (k == 1)
                            {
                                seo_command_r[i][j][k].V1 = owseo_DtoC_utor[j][k].ows[3].Vn;
                                seo_command_r[i][j][k].V2 = owseo_command_r[i][j][k - 1].ows[0].Vn;
                                seo_command_r[i][j][k].V3 = owseo_CtoD_r[i][j - 1][k - 1].ows[0].Vn;
                                seo_Pcalc(&seo_command_r[i][j][k], 3, C, Cjs3);
                            }
                            else if (k == SEO_COLUMNS - 1)
                            {
                                seo_command_r[i][j][k].V1 = owseo_DtoC_utor[j][k].ows[3].Vn;
                                seo_command_r[i][j][k].V2 = owseo_command_r[i][j][k - 2].ows[3].Vn;
                                seo_command_r[i][j][k].V3 = owseo_CtoD_r[i][j - 1][k - 1].ows[0].Vn;
                                seo_Pcalc(&seo_command_r[i][j][k], 3, C, Cjs3);
                            }
                            else
                            {
                                seo_command_r[i][j][k].V1 = owseo_DtoC_utor[j][k].ows[3].Vn;
                                seo_command_r[i][j][k].V2 = owseo_command_r[i][j][k - 2].ows[3].Vn;
                                seo_command_r[i][j][k].V3 = owseo_command_r[i][j][k - 1].ows[0].Vn;
                                seo_command_r[i][j][k].V4 = owseo_CtoD_r[i][j - 1][k - 1].ows[0].Vn;
                                seo_Pcalc(&seo_command_r[i][j][k], 4, C, Cjs4);
                            }
                        }
                        else
                        {
                            if (k == 1)
                            {
                                seo_command_r[i][j][k].V1 = owseo_command_r[i][j][k - 1].ows[0].Vn;
                                seo_command_r[i][j][k].V2 = owseo_CtoD_r[i][j - 1][k - 1].ows[0].Vn;
                                seo_Pcalc(&seo_command_r[i][j][k], 2, C, Cjs2);
                            }
                            else if (k == SEO_COLUMNS - 1)
                            {
                                seo_command_r[i][j][k].V1 = owseo_command_r[i][j][k - 2].ows[3].Vn;
                                seo_command_r[i][j][k].V2 = owseo_CtoD_r[i][j - 1][k - 1].ows[0].Vn;
                                seo_Pcalc(&seo_command_r[i][j][k], 2, C, Cjs2);
                            }
                            else
                            {
                                seo_command_r[i][j][k].V1 = owseo_command_r[i][j][k - 2].ows[3].Vn;
                                seo_command_r[i][j][k].V2 = owseo_command_r[i][j][k - 1].ows[0].Vn;
                                seo_command_r[i][j][k].V3 = owseo_CtoD_r[i][j - 1][k - 1].ows[0].Vn;
                                seo_Pcalc(&seo_command_r[i][j][k], 3, C, Cjs3);
                            }
                        }
                    }
                }
            }

            // 右衝突判定計算
            for (i = 1; i < SEO_ROWS; i++)
            {
                for (j = 1; j < SEO_COLUMNS; j++)
                {
                    seo_detection_r[i][j].V1 = owseo_CtoD_r[0][i - 1][j - 1].ows[3].Vn;
                    seo_detection_r[i][j].V2 = owseo_CtoD_r[1][i - 1][j - 1].ows[3].Vn;
                    seo_detection_r[i][j].V3 = owseo_DtoC_rtod[i][j].ows[0].Vn;
                    seo_Pcalc(&seo_detection_r[i][j], 3, C, Cjs3);
                }
            }

            // 下命令一方通行計算
            for (i = 0; i < SEO_PARTICLES; i++)
            {
                for (j = 0; j < OWSEO_ROWS - 1; j++)
                {
                    for (k = 0; k < OWSEO_COLUMNS; k++)
                    {
                        oneway_4seo_calcPara(&owseo_command_d[i][j][k], C, Cjs2, Cjs3, seo_command_d[i][j + 1][k].Vn, seo_command_d[i][j + 2][k].Vn);
                    }
                }
            }

            // 下命令から衝突判定の一方通行計算
            for (i = 0; i < SEO_PARTICLES; i++)
            {
                for (j = 0; j < SEO_ROWS - 1; j++)
                {
                    for (k = 0; k < SEO_COLUMNS; k++)
                    {
                        oneway_4seo_calcPara(&owseo_CtoD_d[i][j][k], C, Cjs2, Cjs3, seo_command_d[i][j + 1][k + 1].Vn, seo_detection_d[j + 1][k + 1].Vn);
                    }
                }
            }

            // 下衝突判定から左命令の一方通行計算
            for (i = 1; i < SEO_ROWS; i++)
            {
                for (j = 1; j < SEO_COLUMNS; j++)
                {
                    if (i % 2 == 0)
                    {
                        oneway_4seo_calcPara(&owseo_DtoC_dtol[i][j], C, Cjs2, Cjs3, seo_detection_d[i][j].Vn, seo_command_l[0][i][j].Vn);
                    }
                    else
                    {
                        oneway_4seo_calcPara(&owseo_DtoC_dtol[i][j], C, Cjs2, Cjs3, seo_detection_d[i][j].Vn, seo_command_l[1][i][j].Vn);
                    }
                }
            }

            // 左命令の一方通行計算
            for (i = 0; i < SEO_PARTICLES; i++)
            {
                for (j = 0; j < OWSEO_ROWS; j++)
                {
                    for (k = 0; k < OWSEO_COLUMNS - 1; k++)
                    {
                        oneway_4seo_calcPara(&owseo_command_l[i][j][k], C, Cjs2, Cjs3, seo_command_l[i][j][k + 1].Vn, seo_command_l[i][j][k + 2].Vn);
                    }
                }
            }

            // 左命令から衝突判定の一方通行計算
            for (i = 0; i < SEO_PARTICLES; i++)
            {
                for (j = 0; j < SEO_ROWS; j++)
                {
                    for (k = 0; k < SEO_COLUMNS - 1; k++)
                    {
                        oneway_4seo_calcPara(&owseo_CtoD_l[i][j][k], C, Cjs2, Cjs3, seo_command_l[i][j + 1][k + 1].Vn, seo_detection_l[j + 1][k + 1].Vn);
                    }
                }
            }

            // 左衝突判定から上命令の一方通行計算
            for (i = 1; i < SEO_ROWS; i++)
            {
                for (j = 1; j < SEO_COLUMNS; j++)
                {
                    if (j % 2 == 0)
                    {
                        oneway_4seo_calcPara(&owseo_DtoC_ltou[i][j], C, Cjs2, Cjs3, seo_detection_l[i][j].Vn, seo_command_u[0][i][j].Vn);
                    }
                    else
                    {
                        oneway_4seo_calcPara(&owseo_DtoC_ltou[i][j], C, Cjs2, Cjs3, seo_detection_l[i][j].Vn, seo_command_u[1][i][j].Vn);
                    }
                }
            }

            // 上命令の一方通行計算
            for (i = 0; i < SEO_PARTICLES; i++)
            {
                for (j = 0; j < OWSEO_ROWS - 1; j++)
                {
                    for (k = 0; k < OWSEO_COLUMNS; k++)
                    {
                        oneway_4seo_calcPara(&owseo_command_u[i][j][k], C, Cjs2, Cjs3, seo_command_u[i][j + 1][k].Vn, seo_command_u[i][j + 2][k].Vn);
                    }
                }
            }

            // 上命令から衝突判定の一方通行計算
            for (i = 0; i < SEO_PARTICLES; i++)
            {
                for (j = 0; j < SEO_ROWS - 1; j++)
                {
                    for (k = 0; k < SEO_COLUMNS; k++)
                    {
                        oneway_4seo_calcPara(&owseo_CtoD_u[i][j][k], C, Cjs2, Cjs3, seo_command_u[i][j + 1][k + 1].Vn, seo_detection_u[j + 1][k + 1].Vn);
                    }
                }
            }

            // 上衝突判定から右命令一方通行計算
            for (i = 1; i < SEO_ROWS; i++)
            {
                for (j = 1; j < SEO_COLUMNS; j++)
                {
                    if (i % 2 == 0)
                    {
                        oneway_4seo_calcPara(&owseo_DtoC_utor[i][j], C, Cjs2, Cjs3, seo_detection_u[i][j].Vn, seo_command_r[0][i][j].Vn);
                    }
                    else
                    {
                        oneway_4seo_calcPara(&owseo_DtoC_utor[i][j], C, Cjs2, Cjs3, seo_detection_u[i][j].Vn, seo_command_r[1][i][j].Vn);
                    }
                }
            }
        }

        // 右命令の一方通行計算
        for (i = 0; i < SEO_PARTICLES; i++)
        {
            for (j = 0; j < OWSEO_ROWS; j++)
            {
                for (k = 0; k < OWSEO_COLUMNS - 1; k++)
                {
                    oneway_4seo_calcPara(&owseo_command_r[i][j][k], C, Cjs2, Cjs3, seo_command_r[i][j][k + 1].Vn, seo_command_r[i][j][k + 2].Vn);
                }
            }
        }

        // 右命令から衝突判定の一方通行計算
        for (i = 0; i < SEO_PARTICLES; i++)
        {
            for (j = 0; j < SEO_ROWS; j++)
            {
                for (k = 0; k < SEO_COLUMNS - 1; k++)
                {
                    oneway_4seo_calcPara(&owseo_CtoD_r[i][j][k], C, Cjs2, Cjs3, seo_command_r[i][j + 1][k + 1].Vn, seo_detection_r[j + 1][k + 1].Vn);
                }
            }
        }

        // 右衝突判定から下命令の一方通行計算
        for (i = 1; i < SEO_ROWS; i++)
        {
            for (j = 1; j < SEO_COLUMNS; j++)
            {
                if (j % 2 == 0)
                {
                    oneway_4seo_calcPara(&owseo_DtoC_rtod[i][j], C, Cjs2, Cjs3, seo_detection_r[i][j].Vn, seo_command_d[0][i][j].Vn);
                }
                else
                {
                    oneway_4seo_calcPara(&owseo_DtoC_rtod[i][j], C, Cjs2, Cjs3, seo_detection_r[i][j].Vn, seo_command_d[1][i][j].Vn);
                }
            }
        }

        /*----------------------------------------------------------------------------------------------------------------------------------------------------------------*/

        /*エネルギー計算---------------------------------------------------------------------------------------------------------------------------------------------------*/
        // 下命令振動子エネルギー計算
        for (i = 0; i < SEO_PARTICLES; i++)
        {
            for (j = 1; j < SEO_ROWS; j++)
            {
                for (k = 1; k < SEO_COLUMNS; k++)
                {
                    if ((i == 0 && (k % 2 == 0)) || (i == 1 && (k % 2 == 1)))
                    {
                        if (j == 1)
                        {
                            seo_Ecalc(&seo_command_d[i][j][k], 3, C, Cjs3); // 足3
                        }
                        else // トリガの振動子も含む
                        {
                            seo_Ecalc(&seo_command_d[i][j][k], 4, C, Cjs4); // 足4
                        }
                    }
                    else
                    {
                        if (j == 1)
                        {
                            seo_Ecalc(&seo_command_d[i][j][k], 2, C, Cjs2); // 足2
                        }
                        else // トリガの振動子も含む
                        {
                            seo_Ecalc(&seo_command_d[i][j][k], 3, C, Cjs3); // 足3
                        }
                    }
                }
            }
        }

        // 下衝突判定振動子エネルギー計算
        for (i = 1; i < SEO_ROWS; i++)
        {
            for (j = 1; j < SEO_COLUMNS; j++)
            {
                seo_Ecalc(&seo_detection_d[i][j], 3, C, Cjs3); // 足3
            }
        }

        // 左命令振動子エネルギー計算
        for (i = 0; i < SEO_PARTICLES; i++)
        {
            for (j = 1; j < SEO_ROWS; j++)
            {
                for (k = 1; k < SEO_COLUMNS; k++)
                {
                    if ((i == 0 && (j % 2 == 0)) || (i == 1 && (j % 2 == 1)))
                    {
                        if (k == 1 || k == SEO_COLUMNS - 1)
                        {
                            seo_Ecalc(&seo_command_l[i][j][k], 3, C, Cjs3); // 足3
                        }
                        else
                        {
                            seo_Ecalc(&seo_command_l[i][j][k], 4, C, Cjs4); // 足4
                        }
                    }
                    else
                    {
                        if (k == 1 || k == SEO_COLUMNS - 1)
                        {
                            seo_Ecalc(&seo_command_l[i][j][k], 2, C, Cjs2); // 足2
                        }
                        else
                        {
                            seo_Ecalc(&seo_command_l[i][j][k], 3, C, Cjs3); // 足3
                        }
                    }
                }
            }
        }

        // 左衝突判定振動子エネルギー計算
        for (i = 1; i < SEO_ROWS; i++)
        {
            for (j = 1; j < SEO_COLUMNS; j++)
            {
                seo_Ecalc(&seo_detection_l[i][j], 3, C, Cjs3); // 足3
            }
        }

        // 上命令振動子エネルギー計算
        for (i = 0; i < SEO_PARTICLES; i++)
        {
            for (j = 1; j < SEO_ROWS; j++)
            {
                for (k = 1; k < SEO_COLUMNS; k++)
                {
                    if ((i == 0 && (k % 2 == 0)) || (i == 1 && (k % 2 == 1)))
                    {
                        if (j == 1 || j == SEO_ROWS - 1)
                        {
                            seo_Ecalc(&seo_command_u[i][j][k], 3, C, Cjs3); // 足3
                        }
                        else
                        {
                            seo_Ecalc(&seo_command_u[i][j][k], 4, C, Cjs4); // 足4
                        }
                    }
                    else
                    {
                        if (j == 1 || j == SEO_ROWS - 1)
                        {
                            seo_Ecalc(&seo_command_u[i][j][k], 2, C, Cjs2); // 足2
                        }
                        else
                        {
                            seo_Ecalc(&seo_command_u[i][j][k], 3, C, Cjs3); // 足3
                        }
                    }
                }
            }
        }

        // 上衝突判定振動子エネルギー計算
        for (i = 1; i < SEO_ROWS; i++)
        {
            for (j = 1; j < SEO_COLUMNS; j++)
            {
                seo_Ecalc(&seo_detection_u[i][j], 3, C, Cjs3); // 足2
            }
        }

        // 右命令振動子エネルギー計算
        for (i = 0; i < SEO_PARTICLES; i++)
        {
            for (j = 1; j < SEO_ROWS; j++)
            {
                for (k = 1; k < SEO_COLUMNS; k++)
                {
                    if ((i == 0 && (k % 2 == 0)) || (i == 1 && (k % 2 == 1)))
                    {
                        if (k == 1 || k == SEO_COLUMNS - 1)
                        {
                            seo_Ecalc(&seo_command_r[i][j][k], 3, C, Cjs3); // 足3
                        }
                        else
                        {
                            seo_Ecalc(&seo_command_r[i][j][k], 4, C, Cjs4); // 足4
                        }
                    }
                    else
                    {
                        if (k == 1 || k == SEO_COLUMNS - 1)
                        {
                            seo_Ecalc(&seo_command_r[i][j][k], 2, C, Cjs2); // 足2
                        }
                        else
                        {
                            seo_Ecalc(&seo_command_r[i][j][k], 3, C, Cjs3); // 足3
                        }
                    }
                }
            }
        }

        // 右衝突判定振動子エネルギー計算
        for (i = 1; i < SEO_ROWS; i++)
        {
            for (j = 1; j < SEO_COLUMNS; j++)
            {
                seo_Ecalc(&seo_detection_r[i][j], 3, C, Cjs3); // 足3
            }
        }

        // 下命令一方通行エネルギー計算
        for (i = 0; i < SEO_PARTICLES; i++)
        {
            for (j = 0; j < OWSEO_ROWS - 1; j++)
            {
                for (k = 0; k < OWSEO_COLUMNS; k++)
                {
                    oneway_4seo_calcEner(&owseo_command_d[i][j][k], C, Cjs2, Cjs3);
                }
            }
        }

        // 下命令から衝突判定一方通行エネルギー計算
        for (i = 0; i < SEO_PARTICLES; i++)
        {
            for (j = 0; j < SEO_ROWS - 1; j++)
            {
                for (k = 0; k < SEO_COLUMNS; k++)
                {
                    oneway_4seo_calcEner(&owseo_CtoD_d[i][j][k], C, Cjs2, Cjs3);
                }
            }
        }

        // 下衝突判定から左命令一方通行エネルギー計算
        for (i = 1; i < SEO_ROWS; i++)
        {
            for (j = 1; j < SEO_COLUMNS; j++)
            {
                oneway_4seo_calcEner(&owseo_DtoC_dtol[i][j], C, Cjs2, Cjs3);
            }
        }

        // 左命令一方通行エネルギー計算
        for (i = 0; i < SEO_PARTICLES; i++)
        {
            for (j = 0; j < OWSEO_ROWS; j++)
            {
                for (k = 0; k < OWSEO_COLUMNS - 1; k++)
                {
                    oneway_4seo_calcEner(&owseo_command_l[i][j][k], C, Cjs2, Cjs3);
                }
            }
        }

        // 左命令から衝突判定の一方通行エネルギー計算
        for (i = 0; i < SEO_PARTICLES; i++)
        {
            for (j = 0; j < SEO_ROWS; j++)
            {
                for (k = 0; k < SEO_COLUMNS - 1; k++)
                {
                    oneway_4seo_calcEner(&owseo_CtoD_l[i][j][k], C, Cjs2, Cjs3);
                }
            }
        }

        // 左衝突判定から上命令の振動子エネルギー計算
        for (i = 1; i < SEO_ROWS; i++)
        {
            for (j = 1; j < SEO_COLUMNS; j++)
            {
                oneway_4seo_calcEner(&owseo_DtoC_ltou[i][j], C, Cjs2, Cjs3);
            }
        }

        // 上命令一方通行エネルギー計算
        for (i = 0; i < SEO_PARTICLES; i++)
        {
            for (j = 0; j < OWSEO_ROWS - 1; j++)
            {
                for (k = 0; k < OWSEO_COLUMNS; k++)
                {
                    oneway_4seo_calcEner(&owseo_command_u[i][j][k], C, Cjs2, Cjs3);
                }
            }
        }

        // 上命令から衝突判定の一方通行エネルギー計算
        for (i = 0; i < SEO_PARTICLES; i++)
        {
            for (j = 0; j < SEO_ROWS - 1; j++)
            {
                for (k = 0; k < SEO_COLUMNS; k++)
                {
                    oneway_4seo_calcEner(&owseo_CtoD_u[i][j][k], C, Cjs2, Cjs3);
                }
            }
        }

        // 上衝突判定から右命令のエネルギー計算
        for (i = 1; i < SEO_ROWS; i++)
        {
            for (j = 1; j < SEO_COLUMNS; j++)
            {
                oneway_4seo_calcEner(&owseo_DtoC_utor[i][j], C, Cjs2, Cjs3);
            }
        }

        // 右命令一方通行エネルギー計算
        for (i = 0; i < SEO_PARTICLES; i++)
        {
            for (j = 0; j < OWSEO_ROWS; j++)
            {
                for (k = 0; k < OWSEO_COLUMNS - 1; k++)
                {
                    oneway_4seo_calcEner(&owseo_command_r[i][j][k], C, Cjs2, Cjs3);
                }
            }
        }

        // 右命令から衝突判定の一方通行エネルギー計算
        for (i = 0; i < SEO_PARTICLES; i++)
        {
            for (j = 0; j < SEO_ROWS; j++)
            {
                for (k = 0; k < SEO_COLUMNS - 1; k++)
                {
                    oneway_4seo_calcEner(&owseo_CtoD_r[i][j][k], C, Cjs2, Cjs3);
                }
            }
        }

        // 右衝突判定から下命令の振動子エネルギー計算
        for (i = 1; i < SEO_ROWS; i++)
        {
            for (j = 1; j < SEO_COLUMNS; j++)
            {
                oneway_4seo_calcEner(&owseo_DtoC_rtod[i][j], C, Cjs2, Cjs3);
            }
        }

        /*----------------------------------------------------------------------------------------------------------------------------------------------------------------*/

        /*トンネル計算-----------------------------------------------------------------------------------------------------------------------------------------------------*/
        // 下命令振動子トンネル計算
        seo_p[1] = seo_3dimwt(seo_command_d[0][0], SEO_PARTICLES, SEO_ROWS, SEO_COLUMNS, Rj);

        // 下衝突判定振動子トンネル計算
        seo_p[2] = seo_2dimwt(seo_detection_d[0], SEO_ROWS, SEO_COLUMNS, Rj);

        // 左命令振動子トンネル計算
        seo_p[3] = seo_3dimwt(seo_command_l[0][0], SEO_PARTICLES, SEO_ROWS, SEO_COLUMNS, Rj);

        // 左衝突判定振動子トンネル計算
        seo_p[4] = seo_2dimwt(seo_detection_l[0], SEO_ROWS, SEO_COLUMNS, Rj);

        // 上命令振動子トンネル計算
        seo_p[5] = seo_3dimwt(seo_command_u[0][0], SEO_PARTICLES, SEO_ROWS, SEO_COLUMNS, Rj);

        // 上衝突判定振動子トンネル計算
        seo_p[6] = seo_2dimwt(seo_detection_u[0], SEO_ROWS, SEO_COLUMNS, Rj);

        // 右命令振動子トンネル計算
        seo_p[7] = seo_3dimwt(seo_command_r[0][0], SEO_PARTICLES, SEO_ROWS, SEO_COLUMNS, Rj);

        // 左衝突判定振動子トンネル計算
        seo_p[8] = seo_2dimwt(seo_detection_r[0], SEO_ROWS, SEO_COLUMNS, Rj);

        // 下命令一方通行トンネル計算
        owseo_p[1] = oneway_4seo_3dimWt(owseo_command_d[0][0], SEO_PARTICLES, OWSEO_ROWS - 1, OWSEO_COLUMNS, Rj);

        // 下命令から衝突判定の一方通行トンネル計算
        owseo_p[2] = oneway_4seo_3dimWt(owseo_CtoD_d[0][0], SEO_PARTICLES, SEO_ROWS - 1, SEO_COLUMNS, Rj);

        // 下衝突判定から左命令の一方通行トンネル計算
        owseo_p[3] = oneway_4seo_3dimWt(owseo_DtoC_dtol[0], 1, SEO_ROWS, SEO_COLUMNS, Rj);

        // 左命令一方通行トンネル計算
        owseo_p[4] = oneway_4seo_3dimWt(owseo_command_l[0][0], SEO_PARTICLES, OWSEO_ROWS, OWSEO_COLUMNS - 1, Rj);

        // 左命令から衝突判定の一方通行トンネル計算
        owseo_p[5] = oneway_4seo_3dimWt(owseo_CtoD_l[0][0], SEO_PARTICLES, SEO_ROWS, SEO_COLUMNS - 1, Rj);

        // 左衝突判定から上命令の一方通行トンネル計算
        owseo_p[6] = oneway_4seo_3dimWt(owseo_DtoC_ltou[0], 1, SEO_ROWS, SEO_COLUMNS, Rj);

        // 上命令一方通行トンネル計算
        owseo_p[7] = oneway_4seo_3dimWt(owseo_command_u[0][0], SEO_PARTICLES, OWSEO_ROWS - 1, OWSEO_COLUMNS, Rj);

        // 上命令から衝突判定の一方通行トンネル計算
        owseo_p[8] = oneway_4seo_3dimWt(owseo_CtoD_u[0][0], SEO_PARTICLES, SEO_ROWS - 1, SEO_COLUMNS, Rj);

        // 上衝突判定から右命令の一方通行トンネル計算
        owseo_p[9] = oneway_4seo_3dimWt(owseo_DtoC_utor[0], 1, SEO_ROWS, SEO_COLUMNS, Rj);

        // 右命令一方通行トンネル計算
        owseo_p[10] = oneway_4seo_3dimWt(owseo_command_r[0][0], SEO_PARTICLES, OWSEO_ROWS, OWSEO_COLUMNS - 1, Rj);

        // 右命令から衝突判定の一方通行トンネル計算
        owseo_p[11] = oneway_4seo_3dimWt(owseo_CtoD_r[0][0], SEO_PARTICLES, SEO_ROWS, SEO_COLUMNS - 1, Rj);

        // 右衝突判定から下命令の一方通行トンネル計算
        owseo_p[12] = oneway_4seo_3dimWt(owseo_DtoC_rtod[0], 1, SEO_ROWS, SEO_COLUMNS, Rj);
        /*----------------------------------------------------------------------------------------------------------------------------------------------------------------*/

        /*トンネルと電荷チャージ--------------------------------------------------------------------------------------------------------------------------------------------*/
        // トンネル待ち時間比較
        // 振動子比較(seo_p[0]が待ち時間の最小になるように)
        seo_p[0] = seo_p[1];
        for (i = 2; i < seo_number + 1; i++)
        {
            if (seo_p[0]->wt[seo_p[0]->tunnel] > seo_p[i]->wt[seo_p[i]->tunnel])
            {
                seo_p[0] = seo_p[i];
            }
        }

        // 一方通行比較(owseo_p[0]が待ち時間の最小になるように)
        owseo_p[0] = owseo_p[1];
        for (i = 2; i < owseo_number + 1; i++)
        {
            if (owseo_p[0]->ows[owseo_p[0]->locate].wt[owseo_p[0]->ows[owseo_p[0]->locate].tunnel] > owseo_p[i]->ows[owseo_p[i]->locate].wt[owseo_p[i]->ows[owseo_p[i]->locate].tunnel])
            {
                owseo_p[0] = owseo_p[i];
            }
        }

        // トンネル発生
        Seo_onway4Seo_tunnel(seo_p[0], owseo_p[0], &t, &dt);

        // トンネルチャージとdtのリセット
        // 振動子のチャージ
        seo_3dimcharge(seo_command_d[0][0], SEO_PARTICLES, SEO_ROWS, SEO_COLUMNS, R, dt);
        seo_2dimCharge(seo_detection_d[0], SEO_ROWS, SEO_COLUMNS, R, dt);
        seo_3dimcharge(seo_command_l[0][0], SEO_PARTICLES, SEO_ROWS, SEO_COLUMNS, R, dt);
        seo_2dimCharge(seo_detection_l[0], SEO_ROWS, SEO_COLUMNS, R, dt);
        seo_3dimcharge(seo_command_u[0][0], SEO_PARTICLES, SEO_ROWS, SEO_COLUMNS, R, dt);
        seo_2dimCharge(seo_detection_u[0], SEO_ROWS, SEO_COLUMNS, R, dt);
        seo_3dimcharge(seo_command_r[0][0], SEO_PARTICLES, SEO_ROWS, SEO_COLUMNS, R, dt);
        seo_2dimCharge(seo_detection_r[0], SEO_ROWS, SEO_COLUMNS, R, dt);

        // 一方通行のチャージ
        oneway_4seo_3dimCharge(owseo_command_d[0][0], SEO_PARTICLES, OWSEO_ROWS - 1, OWSEO_COLUMNS, R, dt);
        oneway_4seo_3dimCharge(owseo_CtoD_d[0][0], SEO_PARTICLES, SEO_ROWS - 1, SEO_COLUMNS, R, dt);
        oneway_4seo_3dimCharge(owseo_DtoC_dtol[0], 1, SEO_ROWS, SEO_COLUMNS, R, dt);
        oneway_4seo_3dimCharge(owseo_command_l[0][0], SEO_PARTICLES, OWSEO_COLUMNS, OWSEO_ROWS - 1, R, dt);
        oneway_4seo_3dimCharge(owseo_CtoD_l[0][0], SEO_PARTICLES, SEO_ROWS, SEO_COLUMNS - 1, R, dt);
        oneway_4seo_3dimCharge(owseo_DtoC_ltou[0], 1, SEO_ROWS, SEO_COLUMNS, R, dt);
        oneway_4seo_3dimCharge(owseo_command_u[0][0], SEO_PARTICLES, OWSEO_ROWS - 1, OWSEO_COLUMNS, R, dt);
        oneway_4seo_3dimCharge(owseo_CtoD_u[0][0], SEO_PARTICLES, SEO_ROWS - 1, SEO_COLUMNS, R, dt);
        oneway_4seo_3dimCharge(owseo_DtoC_utor[0], 1, SEO_ROWS, SEO_COLUMNS, R, dt);
        oneway_4seo_3dimCharge(owseo_command_r[0][0], SEO_PARTICLES, OWSEO_COLUMNS, OWSEO_ROWS - 1, R, dt);
        oneway_4seo_3dimCharge(owseo_CtoD_r[0][0], SEO_PARTICLES, SEO_ROWS, SEO_COLUMNS - 1, R, dt);
        oneway_4seo_3dimCharge(owseo_DtoC_rtod[0], 1, SEO_ROWS, SEO_COLUMNS, R, dt);

        // dtのリセット
        dt = 0.1;

        /*----------------------------------------------------------------------------------------------------------------------------------------------------------------*/
    }
    end_time = getTime();          // 実行終了時刻を記録
    getRunTime(st_time, end_time); // 実行時間を表示
}