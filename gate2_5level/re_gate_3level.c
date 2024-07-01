#include <stdio.h>
#include "function.h"
#define SEO_PARTICLES 2  // 振動子 z
#define SEO_COLUMNS 12   // 振動子 x
#define SEO_ROWS 11      // 振動子 y
#define OWSEO_COLUMNS 11 // 一方通行回路 x
#define OWSEO_ROWS 10    // 一方通行回路 y
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

int main()
{
    double t = 0, dt = 0.1, pt = 0; // 共通パラメータ
    int i = 0, j = 0, k = 0, rep = 0;
    FILE *fp1, *fp2, *fp3, *fp4, *fp5, *fp6, *fp7;


    fp1 = fopen("layer1.txt", "w"); // ファイル作成
    fp2 = fopen("layer2.txt", "w");
    fp3 = fopen("layer3.txt", "w");
    fp4 = fopen("layer4.txt", "w");
    fp5 = fopen("layer5.txt", "w");
    fp6 = fopen("layer6.txt", "w");
    fp7 = fopen("laytest.txt", "w");

    if (fp1 == NULL || fp2 == NULL || fp3 == NULL || fp4 == NULL || fp5 == NULL || fp6 == NULL || fp7 == NULL)
    {
        printf("file open error.\n");
        return -1;
    }

    /*初期化--------------------------------------------------------------------------------------------------------------------------------------------------------*/
    // 回路のポインタ
    seo *seop[7];           // 振動子型ポインタ配列
    oneway_4seo *owseop[9]; // 一方通行型ポインタ配列

    // 上方向回路
    seo seol1[SEO_PARTICLES][SEO_ROWS][SEO_COLUMNS] = {0}; // 振動子l1のパラメータ初期化 [z][y][x]
    seop[1] = seol1[0][0];                                 // 振動子l1型のポインタ

    // 上方向回路の衝突判定回路
    seo seol2[SEO_ROWS][SEO_COLUMNS] = {0}; // 振動子l2のパラメータ初期化 [y][x]
    seop[2] = seol2[0];                     // 振動子l2型のポインタ

    // 右方向回路
    seo seol3[SEO_PARTICLES][SEO_ROWS][SEO_COLUMNS] = {0}; // 振動子l3のパラメータ初期化 [z][y][x]
    seop[3] = seol3[0][0];                                 // 振動子l3型のポインタ

    // 右方向回路の衝突判定回路
    seo seol4[SEO_ROWS][SEO_COLUMNS] = {0}; // 振動子l4のパラメータ初期化 [y][x]
    seop[4] = seol4[0];                     // 振動子l4型のポインタ

    // 上方向回路2
    seo seol5[SEO_PARTICLES][SEO_ROWS][SEO_COLUMNS] = {0}; // 振動子l5のパラメータ初期化 [z][y][x]
    seop[5] = seol5[0][0];                                 // 振動子l5型のポインタ

    // 上方向回路2の衝突判定回路
    seo seol6[SEO_ROWS][SEO_COLUMNS] = {0}; // 振動子l6のパラメータ初期化 [y][x]
    seop[6] = seol6[0];                     // 振動子l6型のポインタ

    // 上方向回路の一方通行回路
    oneway_4seo owseol1[SEO_PARTICLES][OWSEO_ROWS - 1][OWSEO_COLUMNS] = {0}; // 一方通行l1のパラメータ初期化 [z][y][x]
    owseop[1] = owseol1[0][0];                                               // 一方通行l1型のポインタ

    // 上方向回路と衝突判定回路を繋ぐ一方通行回路
    oneway_4seo owseol2[SEO_PARTICLES][SEO_ROWS - 1][SEO_COLUMNS] = {0}; // 一方通行l2のパラメータ初期化 [z][y][x]
    owseop[2] = owseol2[0][0];                                           // 一方通行l2型のポインタ

    // 右方向回路の一方通行回路
    oneway_4seo owseol3[SEO_PARTICLES][OWSEO_ROWS][OWSEO_COLUMNS - 1] = {0}; // 一方通行l3のパラメータ初期化 [z][y][x]
    owseop[3] = owseol3[0][0];                                               // 一方通行l3型のポインタ

    // 右方向回路と衝突判定回路を繋ぐ一方通行回路
    oneway_4seo owseol4[SEO_PARTICLES][SEO_ROWS][SEO_COLUMNS - 1] = {0}; // 一方通行l4のパラメータ初期化 [z][y][x]
    owseop[4] = owseol4[0][0];                                           // 一方通行l4型のポインタ

    // 上方向の衝突判定回路と右方向回路を繋ぐ振動子
    oneway_4seo owseol5[SEO_ROWS][SEO_COLUMNS] = {0}; // 一方通行l5のパラメータ初期化 [y][x]
    owseop[5] = owseol5[0];                           // 一方通行l5型のポインタ

    // 上方向回路2の一方通行回路
    oneway_4seo owseol6[SEO_PARTICLES][OWSEO_ROWS - 1][OWSEO_COLUMNS] = {0}; // 一方通行l6のパラメータ初期化 [z][y][x]
    owseop[6] = owseol6[0][0];                                               // 一方通行l6型のポインタ

    // 上方向回路と衝突判定回路を繋ぐ一方通行回路
    oneway_4seo owseol7[SEO_PARTICLES][SEO_ROWS - 1][SEO_COLUMNS] = {0}; // 一方通行l7のパラメータ初期化 [z][y][x]
    owseop[7] = owseol7[0][0];                                           // 一方通行l7型のポインタ

    // 右方向の衝突判定回路と上方向回路を繋ぐ振動子
    oneway_4seo owseol8[SEO_ROWS][SEO_COLUMNS] = {0}; // 一方通行l8のパラメータ初期化 [y][x]
    owseop[8] = owseol8[0];                           // 一方通行l8型のポインタ

    /*----------------------------------------------------------------------------------------------------------------------------------------------------------------*/

    /*Vdの設定--------------------------------------------------------------------------------------------------------------------------------------------------------*/

    /*
    迷路の構造
  j
  10壁壁壁壁壁〇〇壁〇〇壁
    壁壁壁壁壁〇〇壁〇〇壁
    壁壁壁壁壁〇〇壁〇〇壁
    壁壁壁壁壁〇〇壁〇〇壁
    壁〇〇〇〇〇〇壁〇〇壁
   5壁〇壁壁〇壁壁壁〇〇壁
    壁〇壁壁〇壁壁壁〇〇壁
    〇〇〇〇〇〇〇〇〇〇壁
    〇〇壁〇〇壁壁壁壁壁壁
   1〇〇壁〇〇壁壁壁壁壁壁
    1       5       10 k
    */

    // 振動子l1のバイアス電圧
    for (i = 0; i < SEO_PARTICLES; i++)
    {
        for (j = 1; j < SEO_ROWS; j++)
        {
            for (k = 1; k < SEO_COLUMNS; k++)
            {
                if (k == 1 && (j < 4))
                {
                    seol1[i][j][k].Vd = Vd_seo; //[V]
                }
                else if ((k == 2 || k == 5) && (j < 7))
                {
                    seol1[i][j][k].Vd = Vd_seo; //[V]
                }
                else if ((k == 3) && (j == 3 || j == 6))
                {
                    seol1[i][j][k].Vd = Vd_seo; //[V]
                }
                else if ((k == 4) && (j <= 3 || j == 6))
                {
                    seol1[i][j][k].Vd = Vd_seo; //[V]
                }
                else if ((k == 6 || k == 7) && (j == 3 || j > 5))
                {
                    seol1[i][j][k].Vd = Vd_seo; //[V]
                }
                else if ((k == 8) && j == 3)
                {
                    seol1[i][j][k].Vd = Vd_seo; //[V]
                }
                else if ((k == 9 || k == 10) && (j > 2))
                {
                    seol1[i][j][k].Vd = Vd_seo; //[V]
                }
            }
        }
    }

    // 振動子l2のバイアス電圧
    for (i = 1; i < SEO_ROWS; i++) // y
    {
        for (j = 1; j < SEO_COLUMNS; j++) // x
        {
            if ((j == 1 || j == 3 || j == 4 || j == 6 || j == 7 || j == 8) && i == 3) // 壁隣接マス
            {
                seol2[i][j].Vd = Vd_seo; //[V]
            }
            else if ((j == 1 || j == 4) && i == 2) // 壁2隣接マス
            {
                seol2[i][j].Vd = Vd_seo - viasarrange(C, 2, 3, Cjs2, Cjs3); //[V]
            }
            else if ((j >= 2 && j <= 5) && i == 6) // 壁隣接マス
            {
                seol2[i][j].Vd = Vd_seo; //[V]
            }
            else if ((j == 2 || j == 5) && i == 5) // 壁2隣接マス
            {
                seol2[i][j].Vd = Vd_seo - viasarrange(C, 2, 3, Cjs2, Cjs3); //[V]
            }
            else if ((j == 6 || j == 7 || j == 9 || j == 10) && (i == 9)) // 壁2隣接マス
            {
                seol2[i][j].Vd = Vd_seo - viasarrange(C, 2, 3, Cjs2, Cjs3); //[V]
            }
            else if ((j == 6 || j == 7 || j == 9 || j == 10) && (i == 10)) // 壁隣接マス
            {
                seol2[i][j].Vd = Vd_seo; //[V]
            }
        }
    }

    // 振動子l3のバイアス電圧
    for (i = 0; i < SEO_PARTICLES; i++)
    {
        for (j = 1; j < SEO_ROWS; j++)
        {
            for (k = 1; k < SEO_COLUMNS; k++)
            {
                if (k == 1 && (j < 4))
                {
                    seol3[i][j][k].Vd = Vd_seo; //[V]
                }
                else if ((k == 2 || k == 5) && (j < 7))
                {
                    seol3[i][j][k].Vd = Vd_seo; //[V]
                }
                else if ((k == 3) && (j == 3 || j == 6))
                {
                    seol3[i][j][k].Vd = Vd_seo; //[V]
                }
                else if ((k == 4) && (j <= 3 || j == 6))
                {
                    seol3[i][j][k].Vd = Vd_seo; //[V]
                }
                else if ((k == 6 || k == 7) && (j == 3 || j > 5))
                {
                    seol3[i][j][k].Vd = Vd_seo; //[V]
                }
                else if ((k == 8) && j == 3)
                {
                    seol3[i][j][k].Vd = Vd_seo; //[V]
                }
                else if ((k == 9 || k == 10) && (j > 2))
                {
                    seol3[i][j][k].Vd = Vd_seo; //[V]
                }
            }
        }
    }

    // 振動子l4のバイアス電圧
    for (i = 1; i < SEO_ROWS; i++)
    {
        for (j = 1; j < SEO_COLUMNS; j++)
        {
            if (j == 2 && (i == 1 || i == 2 || i == 4 || i == 5)) // 壁隣接マス
            {
                seol4[i][j].Vd = Vd_seo; //[V]
            }
            else if (j == 5 && (i == 1 || i == 2 || i == 4 || i == 5)) // 壁隣接マス
            {
                seol4[i][j].Vd = Vd_seo; //[V]
            }
            else if (j == 4 && (i == 1 || i == 2)) // 壁2隣接マス
            {
                seol4[i][j].Vd = Vd_seo - viasarrange(C, 2, 3, Cjs2, Cjs3); //[V]
            }
            else if (j == 7 && i >= 6) // 壁隣接マス
            {
                seol4[i][j].Vd = Vd_seo; //[V]
            }
            else if (j == 6 && i >= 6) // 壁2隣接マス
            {
                seol4[i][j].Vd = Vd_seo - viasarrange(C, 2, 3, Cjs2, Cjs3); //[V]
            }
            else if (j == 10 && i >= 3) // 壁隣接マス
            {
                seol4[i][j].Vd = Vd_seo; //[V]
            }
            else if (j == 9 && i >= 3) // 壁2隣接マス
            {
                seol4[i][j].Vd = Vd_seo - viasarrange(C, 2, 3, Cjs2, Cjs3); //[V]
            }
        }
    }

    // 振動子l5のバイアス電圧
    for (i = 0; i < SEO_PARTICLES; i++)
    {
        for (j = 1; j < SEO_ROWS; j++)
        {
            for (k = 1; k < SEO_COLUMNS; k++)
            {
                if (k == 1 && (j < 4))
                {
                    seol5[i][j][k].Vd = Vd_seo; //[V]
                }
                else if ((k == 2 || k == 5) && (j < 7))
                {
                    seol5[i][j][k].Vd = Vd_seo; //[V]
                }
                else if ((k == 3) && (j == 3 || j == 6))
                {
                    seol5[i][j][k].Vd = Vd_seo; //[V]
                }
                else if ((k == 4) && (j <= 3 || j == 6))
                {
                    seol5[i][j][k].Vd = Vd_seo; //[V]
                }
                else if ((k == 6 || k == 7) && (j == 3 || j > 5))
                {
                    seol5[i][j][k].Vd = Vd_seo; //[V]
                }
                else if ((k == 8) && j == 3)
                {
                    seol5[i][j][k].Vd = Vd_seo; //[V]
                }
                else if ((k == 9 || k == 10) && (j > 2))
                {
                    seol5[i][j][k].Vd = Vd_seo; //[V]
                }
            }
        }
    }

    // 振動子l6のバイアス電圧
    for (i = 1; i < SEO_ROWS; i++) // y
    {
        for (j = 1; j < SEO_COLUMNS; j++) // x
        {
            if ((j == 1 || j == 3 || j == 4 || j == 6 || j == 7 || j == 8) && i == 3) // 壁隣接マス
            {
                seol6[i][j].Vd = Vd_seo; //[V]
            }
            else if ((j == 1 || j == 4) && i == 2) // 壁2隣接マス
            {
                seol6[i][j].Vd = Vd_seo - viasarrange(C, 2, 3, Cjs2, Cjs3); //[V]
            }
            else if ((j >= 2 && j <= 5) && i == 6) // 壁隣接マス
            {
                seol6[i][j].Vd = Vd_seo; //[V]
            }
            else if ((j == 2 || j == 5) && i == 5) // 壁2隣接マス
            {
                seol6[i][j].Vd = Vd_seo - viasarrange(C, 2, 3, Cjs2, Cjs3); //[V]
            }
            else if ((j == 6 || j == 7 || j == 9 || j == 10) && (i == 9)) // 壁2隣接マス
            {
                seol6[i][j].Vd = Vd_seo - viasarrange(C, 2, 3, Cjs2, Cjs3); //[V]
            }
            else if ((j == 6 || j == 7 || j == 9 || j == 10) && (i == 10)) // 壁隣接マス
            {
                seol6[i][j].Vd = Vd_seo; //[V]
            }
        }
    }

    // 一方通行l1のバイアス電圧
    for (i = 0; i < SEO_PARTICLES; i++)
    {
        for (j = 0; j < OWSEO_ROWS - 1; j++)
        {
            for (k = 0; k < OWSEO_COLUMNS; k++)
            {
                oneway_4seo_setVd(&owseol1[i][j][k], Vd_owseo, right, C, Cjs2, Cjs3);
            }
        }
    }

    // 一方通行l2のバイアス電圧
    for (i = 0; i < SEO_PARTICLES; i++)
    {
        for (j = 0; j < SEO_ROWS; j++)
        {
            for (k = 0; k < SEO_COLUMNS - 1; k++)
            {
                oneway_4seo_setVd(&owseol2[i][j][k], Vd_owseo, right, C, Cjs2, Cjs3);
            }
        }
    }

    // 一方通行l3のバイアス電圧
    for (i = 0; i < SEO_PARTICLES; i++)
    {
        for (j = 0; j < OWSEO_ROWS; j++)
        {
            for (k = 0; k < OWSEO_COLUMNS - 1; k++)
            {
                oneway_4seo_setVd(&owseol3[i][j][k], Vd_owseo, right, C, Cjs2, Cjs3);
            }
        }
    }

    // 一方通行l4のバイアス電圧
    for (i = 0; i < SEO_PARTICLES; i++)
    {
        for (j = 0; j < SEO_ROWS - 1; j++)
        {
            for (k = 0; k < SEO_COLUMNS; k++)
            {
                oneway_4seo_setVd(&owseol4[i][j][k], Vd_owseo, right, C, Cjs2, Cjs3);
            }
        }
    }

    // 一方通行l5のバイアス電圧
    for (i = 1; i < SEO_ROWS; i++)
    {
        for (j = 1; j < SEO_COLUMNS; j++)
        {
            oneway_4seo_setVd(&owseol5[i][j], Vd_owseo, right, C, Cjs2, Cjs3);
        }
    }

    // 一方通行l6のバイアス電圧
    for (i = 0; i < SEO_PARTICLES; i++)
    {
        for (j = 0; j < OWSEO_ROWS - 1; j++)
        {
            for (k = 0; k < OWSEO_COLUMNS; k++)
            {
                oneway_4seo_setVd(&owseol6[i][j][k], Vd_owseo, right, C, Cjs2, Cjs3);
            }
        }
    }

    // 一方通行l7のバイアス電圧
    for (i = 0; i < SEO_PARTICLES; i++)
    {
        for (j = 0; j < SEO_ROWS; j++)
        {
            for (k = 0; k < SEO_COLUMNS - 1; k++)
            {
                oneway_4seo_setVd(&owseol7[i][j][k], Vd_owseo, right, C, Cjs2, Cjs3);
            }
        }
    }

    // 一方通行l8のバイアス電圧
    for (i = 1; i < SEO_ROWS; i++)
    {
        for (j = 1; j < SEO_COLUMNS; j++)
        {
            oneway_4seo_setVd(&owseol8[i][j], Vd_owseo, right, C, Cjs2, Cjs3);
        }
    }

    /*----------------------------------------------------------------------------------------------------------------------------------------------------------------*/
    while (t < 200)
    {
        // ファイル書き込み[V]
        fprintlayrow(seol1[0][0], SEO_PARTICLES, SEO_ROWS, SEO_COLUMNS, fp1, t, pt);
        fprintcollisionlay(seol2[0], SEO_ROWS, SEO_COLUMNS, fp2, t, pt);
        fprintlaycolumn(seol3[0][0], SEO_PARTICLES, SEO_ROWS, SEO_COLUMNS, fp3, t, pt);
        fprintcollisionlay(seol4[0], SEO_ROWS, SEO_COLUMNS, fp4, t, pt);
        fprintlayrow(seol5[0][0], SEO_PARTICLES, SEO_ROWS, SEO_COLUMNS, fp5, t, pt);
        fprintcollisionlay(seol6[0], SEO_ROWS, SEO_COLUMNS, fp6, t, pt);
        fprintf(fp7, "%f %f %f %f %f %f %f %f %f\n", t, seol4[3][5].Vn, owseol8[3][5].ows[0].Vn, owseol8[3][5].ows[1].Vn, owseol8[3][5].ows[2].Vn, owseol8[3][5].ows[3].Vn, owseol8[3][5].ows[4].Vn, owseol8[3][5].ows[3].Vn, seol5[1][3][5].Vn);
        if (pt <= t)
        {
            pt += 0.1;
        }

        /*トリガの設定-----------------------------------------------------------------------------------------------------------------------------------------------------*/
        if (t > 100 && t < 101)
        {
            seol1[0][0][1].Vn = 0.006; // 入力A
            seol1[1][0][4].Vn = 0.006; // 入力B
            seol1[0][0][5].Vn = 0.006; // 入力Aバー
            seol1[1][0][2].Vn = 0.006; // 入力Bバー
        }
        else
        {
            seol1[0][0][1].Vn = 0; // 入力A
            seol1[1][0][4].Vn = 0; // 入力B
            seol1[0][0][5].Vn = 0; // 入力Aバー
            seol1[1][0][2].Vn = 0; // 入力Bバー
        }
        /*----------------------------------------------------------------------------------------------------------------------------------------------------------------*/

        /*パラメータ設定---------------------------------------------------------------------------------------------------------------------------------------------------*/
        for (rep = 0; rep < 5; rep++) // 値の平均化
        {
            // 振動子l1計算
            for (i = 0; i < SEO_PARTICLES; i++) // z
            {
                for (j = 1; j < SEO_ROWS; j++) // y
                {
                    for (k = 1; k < SEO_COLUMNS; k++) // x
                    {
                        if (j == 1)
                        {
                            seol1[i][j][k].V1 = seol1[i][0][k].Vn;
                            seol1[i][j][k].V2 = owseol1[i][j - 1][k].ows[0].Vn;
                            seol1[i][j][k].V3 = owseol2[i][j - 1][k - 1].ows[0].Vn;
                            seo_Pcalc(&seol1[i][j][k], 3, C, Cjs3);
                        }
                        else if (j == SEO_ROWS - 1)
                        {
                            seol1[i][j][k].V1 = owseol1[i][j - 2][k].ows[3].Vn;
                            seol1[i][j][k].V2 = owseol2[i][j - 1][k - 1].ows[0].Vn;
                            seo_Pcalc(&seol1[i][j][k], 2, C, Cjs2);
                        }
                        else
                        {
                            seol1[i][j][k].V1 = owseol1[i][j - 2][k].ows[3].Vn;
                            seol1[i][j][k].V2 = owseol1[i][j - 1][k].ows[0].Vn;
                            seol1[i][j][k].V3 = owseol2[i][j - 1][k - 1].ows[0].Vn;
                            seo_Pcalc(&seol1[i][j][k], 3, C, Cjs3);
                        }
                    }
                }
            }

            // 振動子l2計算
            for (i = 1; i < SEO_ROWS; i++)
            {
                for (j = 1; j < SEO_COLUMNS; j++)
                {
                    seol2[i][j].V1 = owseol2[0][i - 1][j - 1].ows[3].Vn;
                    seol2[i][j].V2 = owseol2[1][i - 1][j - 1].ows[3].Vn;
                    seol2[i][j].V3 = owseol5[i][j].ows[0].Vn;
                    seo_Pcalc(&seol2[i][j], 3, C, Cjs3);
                }
            }

            // 振動子l3計算
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
                                seol3[i][j][k].V1 = owseol5[j][k].ows[3].Vn;
                                seol3[i][j][k].V2 = owseol3[i][j][k - 1].ows[0].Vn;
                                seol3[i][j][k].V3 = owseol4[i][j - 1][k - 1].ows[0].Vn;
                                seo_Pcalc(&seol3[i][j][k], 3, C, Cjs3);
                            }
                            else if (k == SEO_COLUMNS - 1)
                            {
                                seol3[i][j][k].V1 = owseol5[j][k].ows[3].Vn;
                                seol3[i][j][k].V2 = owseol3[i][j][k - 2].ows[3].Vn;
                                seol3[i][j][k].V3 = owseol4[i][j - 1][k - 1].ows[0].Vn;
                                seo_Pcalc(&seol3[i][j][k], 3, C, Cjs3);
                            }
                            else
                            {
                                seol3[i][j][k].V1 = owseol5[j][k].ows[3].Vn;
                                seol3[i][j][k].V2 = owseol3[i][j][k - 2].ows[3].Vn;
                                seol3[i][j][k].V3 = owseol3[i][j][k - 1].ows[0].Vn;
                                seol3[i][j][k].V4 = owseol4[i][j - 1][k - 1].ows[0].Vn;
                                seo_Pcalc(&seol3[i][j][k], 4, C, Cjs4);
                            }
                        }
                        else
                        {
                            if (k == 1)
                            {
                                seol3[i][j][k].V1 = owseol3[i][j][k - 1].ows[0].Vn;
                                seol3[i][j][k].V2 = owseol4[i][j - 1][k - 1].ows[0].Vn;
                                seo_Pcalc(&seol3[i][j][k], 2, C, Cjs2);
                            }
                            else if (k == SEO_COLUMNS - 1)
                            {
                                seol3[i][j][k].V1 = owseol3[i][j][k - 2].ows[3].Vn;
                                seol3[i][j][k].V2 = owseol4[i][j - 1][k - 1].ows[0].Vn;
                                seo_Pcalc(&seol3[i][j][k], 2, C, Cjs2);
                            }
                            else
                            {
                                seol3[i][j][k].V1 = owseol3[i][j][k - 2].ows[3].Vn;
                                seol3[i][j][k].V2 = owseol3[i][j][k - 1].ows[0].Vn;
                                seol3[i][j][k].V3 = owseol4[i][j - 1][k - 1].ows[0].Vn;
                                seo_Pcalc(&seol3[i][j][k], 3, C, Cjs3);
                            }
                        }
                    }
                }
            }

            // 振動子l4計算
            for (i = 1; i < SEO_ROWS; i++)
            {
                for (j = 1; j < SEO_COLUMNS; j++)
                {
                    seol4[i][j].V1 = owseol4[0][i - 1][j - 1].ows[3].Vn;
                    seol4[i][j].V2 = owseol4[1][i - 1][j - 1].ows[3].Vn;
                    seol4[i][j].V3 = owseol8[i][j].ows[0].Vn;
                    seo_Pcalc(&seol4[i][j], 3, C, Cjs3);
                }
            }

            // 振動子l5計算
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
                                seol5[i][j][k].V1 = owseol8[j][k].ows[3].Vn;
                                seol5[i][j][k].V2 = owseol6[i][j - 1][k].ows[0].Vn;
                                seol5[i][j][k].V3 = owseol7[i][j - 1][k - 1].ows[0].Vn;
                                seo_Pcalc(&seol5[i][j][k], 3, C, Cjs3);
                            }
                            else if (j == SEO_ROWS - 1)
                            {
                                seol5[i][j][k].V1 = owseol8[j][k].ows[3].Vn;
                                seol5[i][j][k].V2 = owseol6[i][j - 2][k].ows[3].Vn;
                                seol5[i][j][k].V3 = owseol7[i][j - 1][k - 1].ows[0].Vn;
                                seo_Pcalc(&seol5[i][j][k], 3, C, Cjs3);
                            }
                            else
                            {
                                seol5[i][j][k].V1 = owseol8[j][k].ows[3].Vn;
                                seol5[i][j][k].V2 = owseol6[i][j - 2][k].ows[3].Vn;
                                seol5[i][j][k].V3 = owseol6[i][j - 1][k].ows[0].Vn;
                                seol5[i][j][k].V4 = owseol7[i][j - 1][k - 1].ows[0].Vn;
                                seo_Pcalc(&seol5[i][j][k], 4, C, Cjs4);
                            }
                        }
                        else
                        {
                            if (j == 1)
                            {
                                seol5[i][j][k].V1 = owseol6[i][j - 1][k].ows[0].Vn;
                                seol5[i][j][k].V2 = owseol7[i][j - 1][k - 1].ows[0].Vn;
                                seo_Pcalc(&seol5[i][j][k], 2, C, Cjs2);
                            }
                            else if (j == SEO_ROWS - 1)
                            {
                                seol5[i][j][k].V1 = owseol6[i][j - 2][k].ows[3].Vn;
                                seol5[i][j][k].V2 = owseol7[i][j - 1][k - 1].ows[0].Vn;
                                seo_Pcalc(&seol5[i][j][k], 2, C, Cjs2);
                            }
                            else
                            {
                                seol5[i][j][k].V1 = owseol6[i][j - 2][k].ows[3].Vn;
                                seol5[i][j][k].V2 = owseol6[i][j - 1][k].ows[0].Vn;
                                seol5[i][j][k].V3 = owseol7[i][j - 1][k - 1].ows[0].Vn;
                                seo_Pcalc(&seol5[i][j][k], 3, C, Cjs3);
                            }
                        }
                    }
                }
            }

            // 振動子l6計算
            for (i = 1; i < SEO_ROWS; i++)
            {
                for (j = 1; j < SEO_COLUMNS; j++)
                {
                    seol6[i][j].V1 = owseol7[0][i - 1][j - 1].ows[3].Vn;
                    seol6[i][j].V2 = owseol7[1][i - 1][j - 1].ows[3].Vn;
                    seo_Pcalc(&seol6[i][j], 2, C, Cjs2);
                }
            }

            // 一方通行l1計算
            for (i = 0; i < SEO_PARTICLES; i++)
            {
                for (j = 0; j < OWSEO_ROWS - 1; j++)
                {
                    for (k = 0; k < OWSEO_COLUMNS; k++)
                    {
                        oneway_4seo_calcPara(&owseol1[i][j][k], C, Cjs2, Cjs3, seol1[i][j + 1][k].Vn, seol1[i][j + 2][k].Vn);
                    }
                }
            }

            // 一方通行l2計算
            for (i = 0; i < SEO_PARTICLES; i++)
            {
                for (j = 0; j < SEO_ROWS - 1; j++)
                {
                    for (k = 0; k < SEO_COLUMNS; k++)
                    {
                        oneway_4seo_calcPara(&owseol2[i][j][k], C, Cjs2, Cjs3, seol1[i][j + 1][k + 1].Vn, seol2[j + 1][k + 1].Vn);
                    }
                }
            }

            // 一方通行l3計算
            for (i = 0; i < SEO_PARTICLES; i++)
            {
                for (j = 0; j < OWSEO_ROWS + 1; j++)
                {
                    for (k = 0; k < OWSEO_COLUMNS - 1; k++)
                    {
                        oneway_4seo_calcPara(&owseol3[i][j][k], C, Cjs2, Cjs3, seol3[i][j][k + 1].Vn, seol3[i][j][k + 2].Vn);
                    }
                }
            }

            // 一方通行l4計算
            for (i = 0; i < SEO_PARTICLES; i++)
            {
                for (j = 0; j < SEO_ROWS; j++)
                {
                    for (k = 0; k < SEO_COLUMNS - 1; k++)
                    {
                        oneway_4seo_calcPara(&owseol4[i][j][k], C, Cjs2, Cjs3, seol3[i][j + 1][k + 1].Vn, seol4[j + 1][k + 1].Vn);
                    }
                }
            }

            // 一方通行l5計算
            for (i = 1; i < SEO_ROWS; i++)
            {
                for (j = 1; j < SEO_COLUMNS; j++)
                {
                    if (j % 2 == 0)
                    {
                        oneway_4seo_calcPara(&owseol5[i][j], C, Cjs2, Cjs3, seol2[i][j].Vn, seol3[0][i][j].Vn);
                    }
                    else
                    {
                        oneway_4seo_calcPara(&owseol5[i][j], C, Cjs2, Cjs3, seol2[i][j].Vn, seol3[1][i][j].Vn);
                    }
                }
            }

            // 一方通行l6計算
            for (i = 0; i < SEO_PARTICLES; i++)
            {
                for (j = 0; j < OWSEO_ROWS - 1; j++)
                {
                    for (k = 0; k < OWSEO_COLUMNS; k++)
                    {
                        oneway_4seo_calcPara(&owseol6[i][j][k], C, Cjs2, Cjs3, seol5[i][j + 1][k].Vn, seol5[i][j + 2][k].Vn);
                    }
                }
            }

            // 一方通行l7計算
            for (i = 0; i < SEO_PARTICLES; i++)
            {
                for (j = 0; j < SEO_ROWS - 1; j++)
                {
                    for (k = 0; k < SEO_COLUMNS; k++)
                    {
                        oneway_4seo_calcPara(&owseol7[i][j][k], C, Cjs2, Cjs3, seol5[i][j + 1][k + 1].Vn, seol6[j + 1][k + 1].Vn);
                    }
                }
            }

            // 一方通行l8計算
            for (i = 1; i < SEO_ROWS; i++)
            {
                for (j = 1; j < SEO_COLUMNS; j++)
                {
                    if (i % 2 == 0)
                    {
                        oneway_4seo_calcPara(&owseol8[i][j], C, Cjs2, Cjs3, seol4[i][j].Vn, seol5[0][i][j].Vn);
                    }
                    else
                    {
                        oneway_4seo_calcPara(&owseol8[i][j], C, Cjs2, Cjs3, seol4[i][j].Vn, seol5[1][i][j].Vn);
                    }
                }
            }
        }

        /*----------------------------------------------------------------------------------------------------------------------------------------------------------------*/

        /*エネルギー計算---------------------------------------------------------------------------------------------------------------------------------------------------*/
        // 振動子l1エネルギー計算
        for (i = 0; i < SEO_PARTICLES; i++)
        {
            for (j = 1; j < SEO_ROWS; j++)
            {
                for (k = 1; k < SEO_COLUMNS; k++)
                {
                    if (j == SEO_ROWS - 1)
                    {
                        seo_Ecalc(&seol1[i][j][k], 2, C, Cjs2); // 足2
                    }
                    else
                    {
                        seo_Ecalc(&seol1[i][j][k], 3, C, Cjs3); // 足3
                    }
                }
            }
        }

        // 振動子l2エネルギー計算
        for (i = 1; i < SEO_ROWS; i++)
        {
            for (j = 1; j < SEO_COLUMNS; j++)
            {
                seo_Ecalc(&seol2[i][j], 3, C, Cjs3); // 足3
            }
        }

        // 振動子l3エネルギー計算
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
                            seo_Ecalc(&seol3[i][j][k], 3, C, Cjs3); // 足3
                        }
                        else
                        {
                            seo_Ecalc(&seol3[i][j][k], 4, C, Cjs4); // 足4
                        }
                    }
                    else
                    {
                        if (k == 1 || k == SEO_COLUMNS - 1)
                        {
                            seo_Ecalc(&seol3[i][j][k], 2, C, Cjs2); // 足2
                        }
                        else
                        {
                            seo_Ecalc(&seol3[i][j][k], 3, C, Cjs3); // 足3
                        }
                    }
                }
            }
        }

        // 振動子l4エネルギー計算
        for (i = 1; i < SEO_ROWS; i++)
        {
            for (j = 1; j < SEO_COLUMNS; j++)
            {
                seo_Ecalc(&seol4[i][j], 3, C, Cjs3); // 足3
            }
        }

        // 振動子l5エネルギー計算
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
                            seo_Ecalc(&seol5[i][j][k], 3, C, Cjs3); // 足3
                        }
                        else
                        {
                            seo_Ecalc(&seol5[i][j][k], 4, C, Cjs4); // 足4
                        }
                    }
                    else
                    {
                        if (j == 1 || j == SEO_ROWS - 1)
                        {
                            seo_Ecalc(&seol5[i][j][k], 2, C, Cjs2); // 足2
                        }
                        else
                        {
                            seo_Ecalc(&seol5[i][j][k], 3, C, Cjs3); // 足3
                        }
                    }
                }
            }
        }

        // 振動子l6エネルギー計算
        for (i = 1; i < SEO_ROWS; i++)
        {
            for (j = 1; j < SEO_COLUMNS; j++)
            {
                seo_Ecalc(&seol6[i][j], 2, C, Cjs2); // 足2
            }
        }

        // 一方通行l1エネルギー計算
        for (i = 0; i < SEO_PARTICLES; i++)
        {
            for (j = 0; j < OWSEO_ROWS - 1; j++)
            {
                for (k = 0; k < OWSEO_COLUMNS; k++)
                {
                    oneway_4seo_calcEner(&owseol1[i][j][k], C, Cjs2, Cjs3);
                }
            }
        }

        // 一方通行l2エネルギー計算
        for (i = 0; i < SEO_PARTICLES; i++)
        {
            for (j = 0; j < SEO_ROWS - 1; j++)
            {
                for (k = 0; k < SEO_COLUMNS; k++)
                {
                    oneway_4seo_calcEner(&owseol2[i][j][k], C, Cjs2, Cjs3);
                }
            }
        }

        // 一方通行l3エネルギー計算
        for (i = 0; i < SEO_PARTICLES; i++)
        {
            for (j = 0; j < OWSEO_COLUMNS; j++)
            {
                for (k = 0; k < OWSEO_ROWS - 1; k++)
                {
                    oneway_4seo_calcEner(&owseol3[i][j][k], C, Cjs2, Cjs3);
                }
            }
        }

        // 一方通行l4エネルギー計算
        for (i = 0; i < SEO_PARTICLES; i++)
        {
            for (j = 0; j < SEO_ROWS; j++)
            {
                for (k = 0; k < SEO_COLUMNS - 1; k++)
                {
                    oneway_4seo_calcEner(&owseol4[i][j][k], C, Cjs2, Cjs3);
                }
            }
        }

        // 振動子l5エネルギー計算
        for (i = 1; i < SEO_ROWS; i++)
        {
            for (j = 1; j < SEO_COLUMNS; j++)
            {
                oneway_4seo_calcEner(&owseol5[i][j], C, Cjs2, Cjs3);
            }
        }

        // 一方通行l6エネルギー計算
        for (i = 0; i < SEO_PARTICLES; i++)
        {
            for (j = 0; j < OWSEO_ROWS - 1; j++)
            {
                for (k = 0; k < OWSEO_COLUMNS; k++)
                {
                    oneway_4seo_calcEner(&owseol6[i][j][k], C, Cjs2, Cjs3);
                }
            }
        }

        // 一方通行l7エネルギー計算
        for (i = 0; i < SEO_PARTICLES; i++)
        {
            for (j = 0; j < SEO_ROWS - 1; j++)
            {
                for (k = 0; k < SEO_COLUMNS; k++)
                {
                    oneway_4seo_calcEner(&owseol7[i][j][k], C, Cjs2, Cjs3);
                }
            }
        }

        // 振動子l8エネルギー計算
        for (i = 1; i < SEO_ROWS; i++)
        {
            for (j = 1; j < SEO_COLUMNS; j++)
            {
                oneway_4seo_calcEner(&owseol8[i][j], C, Cjs2, Cjs3);
            }
        }

        /*----------------------------------------------------------------------------------------------------------------------------------------------------------------*/

        /*トンネル計算-----------------------------------------------------------------------------------------------------------------------------------------------------*/
        // 振動子l1トンネル計算
        seop[1] = seo_3dimwt(seol1[0][0], SEO_PARTICLES, SEO_ROWS, SEO_COLUMNS, Rj);

        // 振動子l2トンネル計算
        seop[2] = seo_2dimwt(seol2[0], SEO_ROWS, SEO_COLUMNS, Rj);

        // 振動子l3トンネル計算
        seop[3] = seo_3dimwt(seol3[0][0], SEO_PARTICLES, SEO_ROWS, SEO_COLUMNS, Rj);

        // 振動子l4トンネル計算
        seop[4] = seo_2dimwt(seol4[0], SEO_ROWS, SEO_COLUMNS, Rj);

        // 振動子l5トンネル計算
        seop[5] = seo_3dimwt(seol5[0][0], SEO_PARTICLES, SEO_ROWS, SEO_COLUMNS, Rj);

        // 振動子l6トンネル計算
        seop[6] = seo_2dimwt(seol6[0], SEO_ROWS, SEO_COLUMNS, Rj);

        // 一方通行l1トンネル計算
        owseop[1] = oneway_4seo_3dimWt(owseol1[0][0], SEO_PARTICLES, OWSEO_ROWS - 1, OWSEO_COLUMNS, Rj);

        // 一方通行l2トンネル計算
        owseop[2] = oneway_4seo_3dimWt(owseol2[0][0], SEO_PARTICLES, SEO_ROWS - 1, SEO_COLUMNS, Rj);

        // 一方通行l3トンネル計算
        owseop[3] = oneway_4seo_3dimWt(owseol3[0][0], SEO_PARTICLES, OWSEO_COLUMNS, OWSEO_ROWS - 1, Rj);

        // 一方通行l4トンネル計算
        owseop[4] = oneway_4seo_3dimWt(owseol4[0][0], SEO_PARTICLES, SEO_ROWS, SEO_COLUMNS - 1, Rj);

        // 一方通行l5トンネル計算
        owseop[5] = oneway_4seo_3dimWt(owseol5[0], 1, SEO_ROWS, SEO_COLUMNS, Rj);

        // 一方通行l6トンネル計算
        owseop[6] = oneway_4seo_3dimWt(owseol6[0][0], SEO_PARTICLES, OWSEO_ROWS - 1, OWSEO_COLUMNS, Rj);

        // 一方通行l7トンネル計算
        owseop[7] = oneway_4seo_3dimWt(owseol7[0][0], SEO_PARTICLES, SEO_ROWS - 1, SEO_COLUMNS, Rj);

        // 一方通行l8トンネル計算
        owseop[8] = oneway_4seo_3dimWt(owseol8[0], 1, SEO_ROWS, SEO_COLUMNS, Rj);

        /*----------------------------------------------------------------------------------------------------------------------------------------------------------------*/

        /*トンネルと電荷チャージ--------------------------------------------------------------------------------------------------------------------------------------------*/
        // トンネル待ち時間比較
        // 振動子比較
        seop[0] = seop[1];
        for (i = 2; i < 7; i++)
        {
            if (seop[0]->wt[seop[0]->tunnel] > seop[i]->wt[seop[i]->tunnel])
            {
                seop[0] = seop[i];
            }
        }

        // 一方通行比較
        owseop[0] = owseop[1];
        for (i = 2; i < 9; i++)
        {
            if (owseop[0]->ows[owseop[0]->locate].wt[owseop[0]->ows[owseop[0]->locate].tunnel] > owseop[i]->ows[owseop[i]->locate].wt[owseop[i]->ows[owseop[i]->locate].tunnel])
            {
                owseop[0] = owseop[i];
            }
        }

        // トンネル発生
        Seo_onway4Seo_tunnel(seop[0], owseop[0], &t, &dt);

        // トンネルチャージとdtのリセット
        // 振動子のチャージ
        seo_3dimcharge(seol1[0][0], SEO_PARTICLES, SEO_ROWS, SEO_COLUMNS, R, dt);
        seo_2dimCharge(seol2[0], SEO_ROWS, SEO_COLUMNS, R, dt);
        seo_3dimcharge(seol3[0][0], SEO_PARTICLES, SEO_ROWS, SEO_COLUMNS, R, dt);
        seo_2dimCharge(seol4[0], SEO_ROWS, SEO_COLUMNS, R, dt);
        seo_3dimcharge(seol5[0][0], SEO_PARTICLES, SEO_ROWS, SEO_COLUMNS, R, dt);
        seo_2dimCharge(seol6[0], SEO_ROWS, SEO_COLUMNS, R, dt);

        // 一方通行のチャージ
        oneway_4seo_3dimCharge(owseol1[0][0], SEO_PARTICLES, OWSEO_ROWS - 1, OWSEO_COLUMNS, R, dt);
        oneway_4seo_3dimCharge(owseol2[0][0], SEO_PARTICLES, SEO_ROWS - 1, SEO_COLUMNS, R, dt);
        oneway_4seo_3dimCharge(owseol3[0][0], SEO_PARTICLES, OWSEO_COLUMNS, OWSEO_ROWS - 1, R, dt);
        oneway_4seo_3dimCharge(owseol4[0][0], SEO_PARTICLES, SEO_ROWS, SEO_COLUMNS - 1, R, dt);
        oneway_4seo_3dimCharge(owseol5[0], 1, SEO_ROWS, SEO_COLUMNS, R, dt);
        oneway_4seo_3dimCharge(owseol6[0][0], SEO_PARTICLES, OWSEO_ROWS - 1, OWSEO_COLUMNS, R, dt);
        oneway_4seo_3dimCharge(owseol7[0][0], SEO_PARTICLES, SEO_ROWS - 1, SEO_COLUMNS, R, dt);
        oneway_4seo_3dimCharge(owseol8[0], 1, SEO_ROWS, SEO_COLUMNS, R, dt);

        // dtのリセット
        dt = 0.1;

        /*----------------------------------------------------------------------------------------------------------------------------------------------------------------*/
    }
}