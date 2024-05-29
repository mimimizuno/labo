#include <stdio.h>
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
#define left 0           // 左変数
#define right 1          // 右変数
#define seo_number 8     // 振動子の層の総数
#define owseo_number 12  // 一方通行振動子の層の総数

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
    seo *seo_p[9];            // 振動子型ポインタ配列
    oneway_4seo *owseo_p[13]; // 一方通行型ポインタ配列

    // 下方向回路
    seo seo_command_d[SEO_PARTICLES][SEO_ROWS][SEO_COLUMNS] = {0}; // 振動子のパラメータ初期化 [z][y][x]
    seo_p[1] = seo_command_d[0][0];                                // 振動子型のポインタ

    // 下方向回路の衝突判定回路
    seo seo_detection_d[SEO_ROWS][SEO_COLUMNS] = {0}; // 振動子のパラメータ初期化 [y][x]
    seo_p[2] = seo_detection_d[0];                    // 振動子型のポインタ

    // 左方向回路
    seo seo_command_l[SEO_PARTICLES][SEO_ROWS][SEO_COLUMNS] = {0}; // 振動子のパラメータ初期化 [z][y][x]
    seo_p[3] = seo_command_l[0][0];                                // 振動子型のポインタ

    // 左方向回路の衝突判定回路
    seo seo_detection_l[SEO_ROWS][SEO_COLUMNS] = {0}; // 振動子のパラメータ初期化 [y][x]
    seo_p[4] = seo_detection_l[0];                    // 振動子型のポインタ

    // 上方向回路
    seo seo_command_u[SEO_PARTICLES][SEO_ROWS][SEO_COLUMNS] = {0}; // 振動子のパラメータ初期化 [z][y][x]
    seo_p[5] = seo_command_u[0][0];                                // 振動子l5型のポインタ

    // 上方向回路の衝突判定回路
    seo seo_detection_u[SEO_ROWS][SEO_COLUMNS] = {0}; // 振動子のパラメータ初期化 [y][x]
    seo_p[6] = seo_detection_u[0];                    // 振動子型のポインタ

    // 右方向回路
    seo seo_command_r[SEO_PARTICLES][SEO_ROWS][SEO_COLUMNS] = {0}; // 振動子のパラメータ初期化 [z][y][x]
    seo_p[7] = seo_command_r[0][0];                                // 振動子型のポインタ

    // 右方向回路の衝突判定回路
    seo seo_detection_r[SEO_ROWS][SEO_COLUMNS] = {0}; // 振動子のパラメータ初期化 [y][x]
    seo_p[8] = seo_detection_r[0];                    // 振動子型のポインタ

    // 下方向回路の一方通行回路
    oneway_4seo owseo_command_d[SEO_PARTICLES][OWSEO_ROWS - 1][OWSEO_COLUMNS] = {0}; // 一方通行のパラメータ初期化 [z][y][x]
    owseo_p[1] = owseo_command_d[0][0];                                              // 一方通行型のポインタ

    // 下方向回路と衝突判定回路を繋ぐ一方通行回路
    oneway_4seo owseo_CtoD_d[SEO_PARTICLES][SEO_ROWS - 1][SEO_COLUMNS] = {0}; // 一方通行のパラメータ初期化 [z][y][x]
    owseo_p[2] = owseo_CtoD_d[0][0];                                          // 一方通行型のポインタ

    // 下方向の衝突判定回路と左方向回路を繋ぐ振動子
    oneway_4seo owseo_DtoC_dtol[SEO_ROWS][SEO_COLUMNS] = {0}; // 一方通行のパラメータ初期化 [y][x]
    owseo_p[3] = owseo_DtoC_dtol[0];                          // 一方通行型のポインタ

    // 左方向回路の一方通行回路
    oneway_4seo owseo_command_l[SEO_PARTICLES][OWSEO_ROWS][OWSEO_COLUMNS - 1] = {0}; // 一方通行のパラメータ初期化 [z][y][x]
    owseo_p[4] = owseo_command_l[0][0];                                              // 一方通行型のポインタ

    // 左方向回路と衝突判定回路を繋ぐ一方通行回路
    oneway_4seo owseo_CtoD_l[SEO_PARTICLES][SEO_ROWS][SEO_COLUMNS - 1] = {0}; // 一方通行のパラメータ初期化 [z][y][x]
    owseo_p[5] = owseo_CtoD_l[0][0];                                          // 一方通行型のポインタ

    // 左方向の衝突判定回路と上方向回路を繋ぐ振動子
    oneway_4seo owseo_DtoC_ltou[SEO_ROWS][SEO_COLUMNS] = {0}; // 一方通行のパラメータ初期化 [y][x]
    owseo_p[6] = owseo_DtoC_ltou[0];                          // 一方通行型のポインタ

    // 上方向回路の一方通行回路
    oneway_4seo owseo_command_u[SEO_PARTICLES][OWSEO_ROWS - 1][OWSEO_COLUMNS] = {0}; // 一方通行のパラメータ初期化 [z][y][x]
    owseo_p[7] = owseo_command_u[0][0];                                              // 一方通行型のポインタ

    // 上方向回路と衝突判定回路を繋ぐ一方通行回路
    oneway_4seo owseo_CtoD_u[SEO_PARTICLES][SEO_ROWS - 1][SEO_COLUMNS] = {0}; // 一方通行のパラメータ初期化 [z][y][x]
    owseo_p[8] = owseo_CtoD_u[0][0];                                          // 一方通行型のポインタ

    // 上方向の衝突判定回路と右方向回路を繋ぐ振動子
    oneway_4seo owseo_DtoC_utor[SEO_ROWS][SEO_COLUMNS] = {0}; // 一方通行のパラメータ初期化 [y][x]
    owseo_p[9] = owseo_DtoC_utor[0];                          // 一方通行型のポインタ

    // 右方向回路の一方通行回路
    oneway_4seo owseo_command_r[SEO_PARTICLES][OWSEO_ROWS][OWSEO_COLUMNS - 1] = {0}; // 一方通行のパラメータ初期化 [z][y][x]
    owseo_p[10] = owseo_command_r[0][0];                                             // 一方通行型のポインタ

    // 右方向回路と衝突判定回路を繋ぐ一方通行回路
    oneway_4seo owseo_CtoD_r[SEO_PARTICLES][SEO_ROWS][SEO_COLUMNS - 1] = {0}; // 一方通行のパラメータ初期化 [z][y][x]
    owseo_p[11] = owseo_CtoD_r[0][0];                                         // 一方通行型のポインタ

    // 右方向の衝突判定回路と下方向回路を繋ぐ振動子
    oneway_4seo owseo_DtoC_rtod[SEO_ROWS][SEO_COLUMNS] = {0}; // 一方通行のパラメータ初期化 [y][x]
    owseo_p[12] = owseo_DtoC_rtod[0];                         // 一方通行型のポインタ

    /*----------------------------------------------------------------------------------------------------------------------------------------------------------------*/

    /*Vdの設定--------------------------------------------------------------------------------------------------------------------------------------------------------*/

    /*
    迷路の構造
  j
  10　　　　　　　A   B  ~A  ~B
    壁壁壁壁壁壁壁〇壁〇壁〇壁〇壁
    壁壁〇壁壁壁壁〇壁〇壁〇壁〇壁
    壁〇〇〇〇壁壁〇壁〇壁〇壁〇壁
    壁〇〇〇〇〇〇〇〇〇〇〇〇〇壁
   5壁壁〇壁〇壁壁壁壁壁壁〇壁〇壁
    壁壁〇壁〇壁壁〇壁壁壁〇壁〇壁
    壁壁〇壁〇壁〇〇〇〇壁〇壁〇壁
    壁壁〇壁〇壁〇〇〇〇〇〇〇〇壁
   1壁壁〇壁〇壁壁〇壁〇壁壁壁壁壁
    1       5       10        15 k
    */

    // 下命令のバイアス電圧
    for (i = 0; i < SEO_PARTICLES; i++)
    {
        for (j = 1; j < SEO_ROWS; j++)
        {
            for (k = 1; k < SEO_COLUMNS; k++)
            {

                if ((j == 1) && (k == 3 || k == 5 || k == 8 || k == 10))
                {
                    seo_command_d[i][j][k].Vd = Vd_seo; //[V]
                }
                else if ((j == 2) && (k == 3 || k == 5 || (k >= 7 && k <= 14)))
                {
                    seo_command_d[i][j][k].Vd = Vd_seo; //[V]
                }
                else if ((j == 3) && (k == 3 || k == 5 || (k >= 7 && k <= 10) || k == 12 || k == 14))
                {
                    seo_command_d[i][j][k].Vd = Vd_seo; //[V]
                }
                else if ((j == 4) && (k == 3 || k == 5 || k == 8 || k == 12 || k == 14))
                {
                    seo_command_d[i][j][k].Vd = Vd_seo; //[V]
                }
                else if ((j == 5) && (k == 3 || k == 5 || k == 12 || k == 14))
                {
                    seo_command_d[i][j][k].Vd = Vd_seo; //[V]
                }
                else if ((j == 6) && (k >= 2 && k <= 14))
                {
                    seo_command_d[i][j][k].Vd = Vd_seo; //[V]
                }
                else if ((j == 7) && ((k >= 2 && k <= 5) || k == 8 || k == 10 || k == 12 || k == 14))
                {
                    seo_command_d[i][j][k].Vd = Vd_seo; //[V]
                }
                else if ((j == 8) && (k == 3 || k == 8 || k == 10 || k == 12 || k == 14))
                {
                    seo_command_d[i][j][k].Vd = Vd_seo; //[V]
                }
                else if ((j == 9) && (k == 8 || k == 10 || k == 12 || k == 14))
                {
                    seo_command_d[i][j][k].Vd = Vd_seo; //[V]
                }
            }
        }
    }

    // 下衝突判定のバイアス電圧
    for (i = 1; i < SEO_ROWS; i++) // y
    {
        for (j = 1; j < SEO_COLUMNS; j++) // x
        {
            if ((i == 1) && (j == 3 || j == 5 || j == 8 || j == 10)) // 壁隣接マス
            {
                seo_detection_d[i][j].Vd = Vd_seo; //[V]
            }
            else if ((i == 2) && (j == 7 || j == 9 || (j >= 11 && j <= 14))) // 壁隣接マス
            {
                seo_detection_d[i][j].Vd = Vd_seo; //[V]
            }
            else if ((i == 3) && (j == 7 || j == 9 || j == 12 || j == 14)) // 壁2隣接マス
            {
                seo_detection_d[i][j].Vd = Vd_seo - viasarrange(C, 2, 3, Cjs2, Cjs3); //[V]
            }
            else if ((i == 6) && (j == 2 || j == 4 || (j >= 6 && j <= 11) || j == 13)) // 壁隣接マス
            {
                seo_detection_d[i][j].Vd = Vd_seo; //[V]
            }
            else if ((i == 7) && (j == 2 || j == 4 || j == 8 || j == 10)) // 壁2隣接マス
            {
                seo_detection_d[i][j].Vd = Vd_seo - viasarrange(C, 2, 3, Cjs2, Cjs3); //[V]
            }
        }
    }

    // 左命令のバイアス電圧
    for (i = 0; i < SEO_PARTICLES; i++)
    {
        for (j = 1; j < SEO_ROWS; j++)
        {
            for (k = 1; k < SEO_COLUMNS; k++)
            {
                if (j == 1 && (k == 3 || k == 5 || k == 8 || k == 10))
                {
                    seo_command_l[i][j][k].Vd = Vd_seo; //[V]
                }
                else if ((j == 2) && (k == 3 || k == 5 || (k >= 7 && k <= 14)))
                {
                    seo_command_l[i][j][k].Vd = Vd_seo; //[V]
                }
                else if ((j == 3) && (k == 3 || k == 5 || (k >= 7 && k <= 10) || k == 12 || k == 14))
                {
                    seo_command_l[i][j][k].Vd = Vd_seo; //[V]
                }
                else if ((j == 4) && (k == 3 || k == 5 || k == 8 || k == 12 || k == 14))
                {
                    seo_command_l[i][j][k].Vd = Vd_seo; //[V]
                }
                else if ((j == 5) && (k == 3 || k == 5 || k == 12 || k == 14))
                {
                    seo_command_l[i][j][k].Vd = Vd_seo; //[V]
                }
                else if ((j == 6) && (k >= 2 && k <= 14))
                {
                    seo_command_l[i][j][k].Vd = Vd_seo; //[V]
                }
                else if ((j == 7) && ((k >= 2 && k <= 5) || k == 8 || k == 10 || k == 12 || k == 14))
                {
                    seo_command_l[i][j][k].Vd = Vd_seo; //[V]
                }
                else if ((j == 8) && (k == 3 || k == 8 || k == 10 || k == 12 || k == 14))
                {
                    seo_command_l[i][j][k].Vd = Vd_seo; //[V]
                }
                else if ((j == 9) && (k == 8 || k == 10 || k == 12 || k == 14))
                {
                    seo_command_l[i][j][k].Vd = Vd_seo; //[V]
                }
            }
        }
    }

    // 左衝突判定のバイアス電圧
    for (i = 1; i < SEO_ROWS; i++)
    {
        for (j = 1; j < SEO_COLUMNS; j++)
        {
            if (j == 2 && (i == 6 || i == 7)) // 壁隣接マス
            {
                seo_detection_l[i][j].Vd = Vd_seo; //[V]
            }
            else if (j == 3 && ((i >= 1 && i <= 5) || i == 8)) // 壁隣接マス
            {
                seo_detection_l[i][j].Vd = Vd_seo; //[V]
            }
            else if (j == 3 && (i == 6 || i == 7)) // 壁2隣接マス
            {
                seo_detection_l[i][j].Vd = Vd_seo - viasarrange(C, 2, 3, Cjs2, Cjs3); //[V]
            }
            else if (j == 5 && (i >= 1 && i <= 5)) // 壁隣接マス
            {
                seo_detection_l[i][j].Vd = Vd_seo; //[V]
            }
            else if (j == 7 && (i == 2 || i == 3)) // 壁隣接マス
            {
                seo_detection_l[i][j].Vd = Vd_seo; //[V]
            }
            else if (j == 8 && (i == 1 || i == 4 || (i >= 7 && i <= 9))) // 壁隣接マス
            {
                seo_detection_l[i][j].Vd = Vd_seo; //[V]
            }
            else if (j == 8 && (i == 2 || i == 3)) // 壁2隣接マス
            {
                seo_detection_l[i][j].Vd = Vd_seo - viasarrange(C, 2, 3, Cjs2, Cjs3); //[V]
            }
            else if (j == 10 && (i == 1 || (i >= 7 && i <= 9))) // 壁隣接マス
            {
                seo_detection_l[i][j].Vd = Vd_seo; //[V]
            }
            else if ((j == 12 || j == 14) && ((i >= 3 && i <= 5) || (i >= 7 && i <= 9))) // 壁隣接マス
            {
                seo_detection_l[i][j].Vd = Vd_seo; //[V]
            }
        }
    }

    // 上方向命令のバイアス電圧
    for (i = 0; i < SEO_PARTICLES; i++)
    {
        for (j = 1; j < SEO_ROWS; j++)
        {
            for (k = 1; k < SEO_COLUMNS; k++)
            {
                if (j == 1 && (k == 3 || k == 5 || k == 8 || k == 10))
                {
                    seo_command_u[i][j][k].Vd = Vd_seo; //[V]
                }
                else if ((j == 2) && (k == 3 || k == 5 || (k >= 7 && k <= 14)))
                {
                    seo_command_u[i][j][k].Vd = Vd_seo; //[V]
                }
                else if ((j == 3) && (k == 3 || k == 5 || (k >= 7 && k <= 10) || k == 12 || k == 14))
                {
                    seo_command_u[i][j][k].Vd = Vd_seo; //[V]
                }
                else if ((j == 4) && (k == 3 || k == 5 || k == 8 || k == 12 || k == 14))
                {
                    seo_command_u[i][j][k].Vd = Vd_seo; //[V]
                }
                else if ((j == 5) && (k == 3 || k == 5 || k == 12 || k == 14))
                {
                    seo_command_u[i][j][k].Vd = Vd_seo; //[V]
                }
                else if ((j == 6) && (k >= 2 && k <= 14))
                {
                    seo_command_u[i][j][k].Vd = Vd_seo; //[V]
                }
                else if ((j == 7) && ((k >= 2 && k <= 5) || k == 8 || k == 10 || k == 12 || k == 14))
                {
                    seo_command_u[i][j][k].Vd = Vd_seo; //[V]
                }
                else if ((j == 8) && (k == 3 || k == 8 || k == 10 || k == 12 || k == 14))
                {
                    seo_command_u[i][j][k].Vd = Vd_seo; //[V]
                }
                else if ((j == 9) && (k == 8 || k == 10 || k == 12 || k == 14))
                {
                    seo_command_u[i][j][k].Vd = Vd_seo; //[V]
                }
            }
        }
    }

    // 上衝突判定のバイアス電圧
    for (i = 1; i < SEO_ROWS; i++) // y
    {
        for (j = 1; j < SEO_COLUMNS; j++) // x
        {
            if ((i == 2) && (j == 11 || j == 13)) // 壁隣接マス
            {
                seo_detection_u[i][j].Vd = Vd_seo; //[V]
            }
            else if ((i == 2) && (j == 7 || j == 9 || j == 10)) // 壁2隣接マス
            {
                seo_detection_u[i][j].Vd = Vd_seo - viasarrange(C, 2, 3, Cjs2, Cjs3); //[V]
            }
            else if ((i == 3) && (j == 7 || j == 9 || j == 10)) // 壁隣接マス
            {
                seo_detection_u[i][j].Vd = Vd_seo; //[V]
            }
            else if ((i == 3) && (j == 8)) // 壁2隣接マス
            {
                seo_detection_u[i][j].Vd = Vd_seo - viasarrange(C, 2, 3, Cjs2, Cjs3); //[V]
            }
            else if ((i == 4) && (j == 8)) // 壁隣接マス
            {
                seo_detection_u[i][j].Vd = Vd_seo - viasarrange(C, 2, 3, Cjs2, Cjs3); //[V]
            }
            else if ((i == 6) && (j == 6 || j == 7 || j == 9 || j == 11 || j == 13)) // 壁隣接マス
            {
                seo_detection_u[i][j].Vd = Vd_seo; //[V]
            }
            else if ((i == 6) && (j == 2 || j == 4 || j == 5)) // 壁2隣接マス
            {
                seo_detection_u[i][j].Vd = Vd_seo - viasarrange(C, 2, 3, Cjs2, Cjs3); //[V]
            }
            else if ((i == 7) && (j == 2 || j == 4 || j == 5)) // 壁隣接マス
            {
                seo_detection_u[i][j].Vd = Vd_seo; //[V]
            }
            else if ((i == 7) && (j == 3)) // 壁2隣接マス
            {
                seo_detection_u[i][j].Vd = Vd_seo - viasarrange(C, 2, 3, Cjs2, Cjs3); //[V]
            }
            else if ((i == 7) && (j == 3)) // 壁隣接マス
            {
                seo_detection_u[i][j].Vd = Vd_seo; //[V]
            }
        }
    }

    // 右方向命令のバイアス電圧
    for (i = 0; i < SEO_PARTICLES; i++)
    {
        for (j = 1; j < SEO_ROWS; j++)
        {
            for (k = 1; k < SEO_COLUMNS; k++)
            {
                if (j == 1 && (k == 3 || k == 5 || k == 8 || k == 10))
                {
                    seo_command_r[i][j][k].Vd = Vd_seo; //[V]
                }
                else if ((j == 2) && (k == 3 || k == 5 || (k >= 7 && k <= 14)))
                {
                    seo_command_r[i][j][k].Vd = Vd_seo; //[V]
                }
                else if ((j == 3) && (k == 3 || k == 5 || (k >= 7 && k <= 10) || k == 12 || k == 14))
                {
                    seo_command_r[i][j][k].Vd = Vd_seo; //[V]
                }
                else if ((j == 4) && (k == 3 || k == 5 || k == 8 || k == 12 || k == 14))
                {
                    seo_command_r[i][j][k].Vd = Vd_seo; //[V]
                }
                else if ((j == 5) && (k == 3 || k == 5 || k == 12 || k == 14))
                {
                    seo_command_r[i][j][k].Vd = Vd_seo; //[V]
                }
                else if ((j == 6) && (k >= 2 && k <= 14))
                {
                    seo_command_r[i][j][k].Vd = Vd_seo; //[V]
                }
                else if ((j == 7) && ((k >= 2 && k <= 5) || k == 8 || k == 10 || k == 12 || k == 14))
                {
                    seo_command_r[i][j][k].Vd = Vd_seo; //[V]
                }
                else if ((j == 8) && (k == 3 || k == 8 || k == 10 || k == 12 || k == 14))
                {
                    seo_command_r[i][j][k].Vd = Vd_seo; //[V]
                }
                else if ((j == 9) && (k == 8 || k == 10 || k == 12 || k == 14))
                {
                    seo_command_r[i][j][k].Vd = Vd_seo; //[V]
                }
            }
        }
    }

    // 右衝突判定のバイアス電圧
    for (i = 1; i < SEO_ROWS; i++) // y
    {
        for (j = 1; j < SEO_COLUMNS; j++) // x
        {
            if (j == 3 && ((i >= 1 && i <= 5) || i == 8)) // 壁隣接マス
            {
                seo_detection_r[i][j].Vd = Vd_seo; //[V]
            }
            else if ((j == 4) && (i == 7)) // 壁2隣接マス
            {
                seo_detection_r[i][j].Vd = Vd_seo - viasarrange(C, 2, 3, Cjs2, Cjs3); //[V]
            }
            else if (j == 5 && ((i >= 1 && i <= 5) || i == 7)) // 壁隣接マス
            {
                seo_detection_r[i][j].Vd = Vd_seo; //[V]
            }
            else if (j == 5 && (i >= 1 && i <= 5)) // 壁隣接マス
            {
                seo_detection_r[i][j].Vd = Vd_seo; //[V]
            }
            else if (j == 8 && (i == 1 || i == 4 || (i >= 7 && i <= 9))) // 壁隣接マス
            {
                seo_detection_r[i][j].Vd = Vd_seo; //[V]
            }
            else if (j == 9 && (i == 3)) // 壁2隣接マス
            {
                seo_detection_r[i][j].Vd = Vd_seo - viasarrange(C, 2, 3, Cjs2, Cjs3); //[V]
            }
            else if (j == 10 && (i == 1 || i == 3 || (i >= 7 && i <= 9))) // 壁隣接マス
            {
                seo_detection_r[i][j].Vd = Vd_seo; //[V]
            }
            else if ((j == 12) && ((i >= 3 && i <= 5) || (i >= 7 && i <= 9))) // 壁隣接マス
            {
                seo_detection_r[i][j].Vd = Vd_seo; //[V]
            }
            else if (j == 13 && (i == 2 || i == 6)) // 壁2隣接マス
            {
                seo_detection_r[i][j].Vd = Vd_seo - viasarrange(C, 2, 3, Cjs2, Cjs3); //[V]
            }
            else if ((j == 14) && ((i >= 2 && i <= 9))) // 壁隣接マス
            {
                seo_detection_r[i][j].Vd = Vd_seo; //[V]
            }
        }
    }

    // 下命令一方通行のバイアス電圧
    for (i = 0; i < SEO_PARTICLES; i++)
    {
        for (j = 0; j < OWSEO_ROWS - 1; j++)
        {
            for (k = 0; k < OWSEO_COLUMNS; k++)
            {
                oneway_4seo_setVd(&owseo_command_d[i][j][k], Vd_owseo, left, C, Cjs2, Cjs3); // 右から左への伝搬
            }
        }
    }

    // 下命令から衝突の一方通行のバイアス電圧
    for (i = 0; i < SEO_PARTICLES; i++)
    {
        for (j = 0; j < SEO_ROWS; j++)
        {
            for (k = 0; k < SEO_COLUMNS - 1; k++)
            {
                oneway_4seo_setVd(&owseo_CtoD_d[i][j][k], Vd_owseo, right, C, Cjs2, Cjs3);
            }
        }
    }

    // 下衝突判定から左命令の一方通行のバイアス電圧
    for (i = 1; i < SEO_ROWS; i++)
    {
        for (j = 1; j < SEO_COLUMNS; j++)
        {
            oneway_4seo_setVd(&owseo_DtoC_dtol[i][j], Vd_owseo, right, C, Cjs2, Cjs3);
        }
    }

    // 左命令の一方通行バイアス電圧
    for (i = 0; i < SEO_PARTICLES; i++)
    {
        for (j = 0; j < OWSEO_ROWS; j++)
        {
            for (k = 0; k < OWSEO_COLUMNS - 1; k++)
            {
                oneway_4seo_setVd(&owseo_command_l[i][j][k], Vd_owseo, left, C, Cjs2, Cjs3); // 右から左への伝搬
            }
        }
    }

    // 左命令から衝突判定のバイアス電圧
    for (i = 0; i < SEO_PARTICLES; i++)
    {
        for (j = 0; j < SEO_ROWS - 1; j++)
        {
            for (k = 0; k < SEO_COLUMNS; k++)
            {
                oneway_4seo_setVd(&owseo_CtoD_l[i][j][k], Vd_owseo, right, C, Cjs2, Cjs3);
            }
        }
    }

    // 左衝突判定から上命令のバイアス電圧
    for (i = 1; i < SEO_ROWS; i++)
    {
        for (j = 1; j < SEO_COLUMNS; j++)
        {
            oneway_4seo_setVd(&owseo_DtoC_ltou[i][j], Vd_owseo, right, C, Cjs2, Cjs3);
        }
    }

    // 上命令一方通行のバイアス電圧
    for (i = 0; i < SEO_PARTICLES; i++)
    {
        for (j = 0; j < OWSEO_ROWS - 1; j++)
        {
            for (k = 0; k < OWSEO_COLUMNS; k++)
            {
                oneway_4seo_setVd(&owseo_command_u[i][j][k], Vd_owseo, right, C, Cjs2, Cjs3);
            }
        }
    }

    // 上命令から衝突判定一方通行のバイアス電圧
    for (i = 0; i < SEO_PARTICLES; i++)
    {
        for (j = 0; j < SEO_ROWS; j++)
        {
            for (k = 0; k < SEO_COLUMNS - 1; k++)
            {
                oneway_4seo_setVd(&owseo_CtoD_u[i][j][k], Vd_owseo, right, C, Cjs2, Cjs3);
            }
        }
    }

    // 上衝突判定から右命令の一方通行回路のバイアス電圧
    for (i = 1; i < SEO_ROWS; i++)
    {
        for (j = 1; j < SEO_COLUMNS; j++)
        {
            oneway_4seo_setVd(&owseo_DtoC_utor[i][j], Vd_owseo, right, C, Cjs2, Cjs3);
        }
    }

    // 右命令の一方通行バイアス電圧
    for (i = 0; i < SEO_PARTICLES; i++)
    {
        for (j = 0; j < OWSEO_ROWS; j++)
        {
            for (k = 0; k < OWSEO_COLUMNS - 1; k++)
            {
                oneway_4seo_setVd(&owseo_command_r[i][j][k], Vd_owseo, right, C, Cjs2, Cjs3);
            }
        }
    }

    // 右命令から衝突判定のバイアス電圧
    for (i = 0; i < SEO_PARTICLES; i++)
    {
        for (j = 0; j < SEO_ROWS - 1; j++)
        {
            for (k = 0; k < SEO_COLUMNS; k++)
            {
                oneway_4seo_setVd(&owseo_CtoD_r[i][j][k], Vd_owseo, right, C, Cjs2, Cjs3);
            }
        }
    }

    // 右衝突判定から下命令のバイアス電圧
    for (i = 1; i < SEO_ROWS; i++)
    {
        for (j = 1; j < SEO_COLUMNS; j++)
        {
            oneway_4seo_setVd(&owseo_DtoC_rtod[i][j], Vd_owseo, right, C, Cjs2, Cjs3);
        }
    }

    /*----------------------------------------------------------------------------------------------------------------------------------------------------------------*/
    while (t < 200)
    {
        // ファイル書き込み[V]
        fprintlayrow(seo_command_d[0][0], SEO_PARTICLES, SEO_ROWS, SEO_COLUMNS, fp1, t, pt);
        fprintcollisionlay(seo_detection_d[0], SEO_ROWS, SEO_COLUMNS, fp2, t, pt);
        fprintlaycolumn(seo_command_l[0][0], SEO_PARTICLES, SEO_ROWS, SEO_COLUMNS, fp3, t, pt);
        fprintcollisionlay(seo_detection_l[0], SEO_ROWS, SEO_COLUMNS, fp4, t, pt);
        fprintlayrow(seo_command_u[0][0], SEO_PARTICLES, SEO_ROWS, SEO_COLUMNS, fp5, t, pt);
        fprintcollisionlay(seo_detection_u[0], SEO_ROWS, SEO_COLUMNS, fp6, t, pt);
        fprintlaycolumn(seo_command_r[0][0], SEO_PARTICLES, SEO_ROWS, SEO_COLUMNS, fp7, t, pt);
        fprintcollisionlay(seo_detection_r[0], SEO_ROWS, SEO_COLUMNS, fp8, t, pt);
        // fprintf(fp9, "%f %f %f %f %f\n", t, seo_command_d[0][0][8].Vn, seo_command_d[0][9][8].Vn, seo_command_d[1][0][10].Vn, seo_command_d[1][9][10].Vn);
        // fprintf(fp9, "%f %f %f %f %f %f\n", t, seo_command_l[0][2][12].Vn, owseo_DtoC_dtol[2][12].ows[3].Vn, owseo_command_l[0][2][10].ows[3].Vn, owseo_command_l[0][2][11].ows[0].Vn, owseo_CtoD_l[0][1][11].ows[0].Vn);
        // fprintf(fp9, "%f %f %f %f %f %f\n", t, seo_command_l[0][6][8].Vn, owseo_DtoC_dtol[6][8].ows[3].Vn, owseo_command_l[0][6][6].ows[3].Vn, owseo_command_l[0][6][7].ows[0].Vn, owseo_CtoD_l[0][5][7].ows[0].Vn);
        fprintf(fp9,"%f %f %f %f %f %f %f\n",t,seo_command_r[1][3][9].Vn,owseo_command_r[1][3][8].ows[0].Vn,seo_command_r[1][3][10].Vn,owseo_command_r[1][3][8].ows[3].Vn,owseo_command_r[1][3][9].ows[0].Vn,owseo_CtoD_r[1][2][9].ows[0].Vn);
        // fprintf(fp9,"%f %f %f %f %f\n",t,seo_command_l[0][2][12].Vn,owseo_command_l[0][2][10].ows[3].Vn,owseo_command_l[0][2][10].ows[3].dE[1],owseo_command_l[0][6][6].ows[3].dE[1]);
        if (pt <= t)
        {
            pt += 0.1;
        }

        /*トリガの設定-----------------------------------------------------------------------------------------------------------------------------------------------------*/
        if (t > 100 && t < 101)
        {
            seo_command_d[0][0][8].Vn = 0.006;  // 入力A
            seo_command_d[1][0][10].Vn = 0.006; // 入力B
            seo_command_d[0][0][12].Vn = 0.006; // 入力Aバー
            seo_command_d[1][0][14].Vn = 0.006; // 入力Bバー
        }
        else
        {
            seo_command_d[0][0][8].Vn = 0;  // 入力A
            seo_command_d[1][0][10].Vn = 0; // 入力B
            seo_command_d[0][0][12].Vn = 0; // 入力Aバー
            seo_command_d[1][0][14].Vn = 0; // 入力Bバー
        }
        /*----------------------------------------------------------------------------------------------------------------------------------------------------------------*/

        /*パラメータ設定---------------------------------------------------------------------------------------------------------------------------------------------------*/
        for (rep = 0; rep < 5; rep++) // 値の平均化
        {
            // 下命令振動子計算
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
                                seo_command_d[i][j][k].V1 = owseo_DtoC_rtod[j][k].ows[3].Vn;
                                seo_command_d[i][j][k].V2 = owseo_command_d[i][j - 1][k].ows[0].Vn;
                                seo_command_d[i][j][k].V3 = owseo_CtoD_d[i][j - 1][k - 1].ows[0].Vn;
                                seo_Pcalc(&seo_command_d[i][j][k], 3, C, Cjs3);
                            }
                            else if (j == SEO_ROWS - 1)
                            {
                                seo_command_d[i][j][k].V1 = owseo_DtoC_rtod[j][k].ows[3].Vn;
                                seo_command_d[i][j][k].V2 = owseo_command_d[i][j - 2][k].ows[3].Vn;
                                seo_command_d[i][j][k].V3 = owseo_CtoD_d[i][j - 1][k - 1].ows[0].Vn;
                                seo_command_d[i][j][k].V4 = seo_command_d[i][0][k].Vn; // トリガ
                                seo_Pcalc(&seo_command_d[i][j][k], 4, C, Cjs4);
                            }
                            else
                            {
                                seo_command_d[i][j][k].V1 = owseo_DtoC_rtod[j][k].ows[3].Vn;
                                seo_command_d[i][j][k].V2 = owseo_command_d[i][j - 2][k].ows[3].Vn;
                                seo_command_d[i][j][k].V3 = owseo_command_d[i][j - 1][k].ows[0].Vn;
                                seo_command_d[i][j][k].V4 = owseo_CtoD_d[i][j - 1][k - 1].ows[0].Vn;
                                seo_Pcalc(&seo_command_d[i][j][k], 4, C, Cjs4);
                            }
                        }
                        else
                        {
                            if (j == 1)
                            {
                                seo_command_d[i][j][k].V1 = owseo_command_d[i][j - 1][k].ows[0].Vn;
                                seo_command_d[i][j][k].V2 = owseo_CtoD_d[i][j - 1][k - 1].ows[0].Vn;
                                seo_Pcalc(&seo_command_d[i][j][k], 2, C, Cjs2);
                            }
                            else if (j == SEO_ROWS - 1)
                            {
                                seo_command_d[i][j][k].V1 = owseo_command_d[i][j - 2][k].ows[3].Vn;
                                seo_command_d[i][j][k].V2 = owseo_CtoD_d[i][j - 1][k - 1].ows[0].Vn;
                                seo_command_d[i][j][k].V3 = seo_command_d[i][0][k].Vn; // トリガ
                                seo_Pcalc(&seo_command_d[i][j][k], 3, C, Cjs3);
                            }
                            else
                            {
                                seo_command_d[i][j][k].V1 = owseo_command_d[i][j - 2][k].ows[3].Vn;
                                seo_command_d[i][j][k].V2 = owseo_command_d[i][j - 1][k].ows[0].Vn;
                                seo_command_d[i][j][k].V3 = owseo_CtoD_d[i][j - 1][k - 1].ows[0].Vn;
                                seo_Pcalc(&seo_command_d[i][j][k], 3, C, Cjs3);
                            }
                        }
                    }
                }
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