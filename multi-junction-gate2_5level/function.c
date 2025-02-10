#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <sys/time.h>
#include "function.h"

/*--------------------------------------------------振動子----------------------------------------------------------------------*/

// 振動子のパラメータ計算(&seo,足の本数,Cs,Cjs)
void seo_Pcalc(seo *p, int leg, double Cs, double Cjs)
{
    if (leg == 0)
    {
        p->Vn = p->Q / Cjs;
    }
    else if (leg == 1)
    {
        double q1 = 0;
        q1 = Cs * (Cjs * p->V1 - p->Q) / (Cs + Cjs);
        p->Vn = (p->Q + q1) / Cjs;
    }
    else if (leg == 2)
    {
        double q1 = 0;
        double q2 = 0;
        q1 = Cs * ((Cs + Cjs) * p->V1 - Cs * p->V2 - p->Q) / (2 * Cs + Cjs);
        q2 = Cs * ((Cs + Cjs) * p->V2 - Cs * p->V1 - p->Q) / (2 * Cs + Cjs);
        p->Vn = (p->Q + q1 + q2) / Cjs;
    }
    else if (leg == 3)
    {
        double q1 = 0;
        double q2 = 0;
        double q3 = 0;
        q1 = (Cs * (-p->Q + Cjs * p->V1 + Cs * (2 * p->V1 - p->V2 - p->V3))) / (3 * Cs + Cjs);
        q2 = -((Cs * (p->Q - Cjs * p->V2 + Cs * (p->V1 - 2 * p->V2 + p->V3))) / (3 * Cs + Cjs));
        q3 = -((Cs * (p->Q + Cs * (p->V1 + p->V2 - 2 * p->V3) - Cjs * p->V3)) / (3 * Cs + Cjs));
        p->Vn = (p->Q + q1 + q2 + q3) / Cjs;
    }
    else if (leg == 4)
    {
        double q1 = 0;
        double q2 = 0;
        double q3 = 0;
        double q4 = 0;
        q1 = Cs * ((3 * Cs + Cjs) * p->V1 - Cs * (p->V2 + p->V3 + p->V4) - p->Q) / (4 * Cs + Cjs);
        q2 = Cs * ((3 * Cs + Cjs) * p->V2 - Cs * (p->V1 + p->V3 + p->V4) - p->Q) / (4 * Cs + Cjs);
        q3 = Cs * ((3 * Cs + Cjs) * p->V3 - Cs * (p->V1 + p->V2 + p->V4) - p->Q) / (4 * Cs + Cjs);
        q4 = Cs * ((3 * Cs + Cjs) * p->V4 - Cs * (p->V1 + p->V2 + p->V3) - p->Q) / (4 * Cs + Cjs);
        p->Vn = (p->Q + q1 + q2 + q3 + q4) / Cjs;
    }
    else if (leg == 5)
    {
        double q1 = 0;
        double q2 = 0;
        double q3 = 0;
        double q4 = 0;
        double q5 = 0;
        q1 = (Cs * (-p->Q + Cjs * p->V1 + Cs * (4 * p->V1 - p->V2 - p->V3 - p->V4 - p->V5))) / (5 * Cs + Cjs);
        q2 = (Cs * (-p->Q + Cjs * p->V2 - Cs * (p->V1 - 4 * p->V2 + p->V3 + p->V4 + p->V5))) / (5 * Cs + Cjs);
        q3 = (Cs * (-p->Q + Cjs * p->V3 - Cs * (p->V1 + p->V2 - 4 * p->V3 + p->V4 + p->V5))) / (5 * Cs + Cjs);
        q4 = (Cs * (-p->Q + Cjs * p->V4 - Cs * (p->V1 + p->V2 + p->V3 - 4 * p->V4 + p->V5))) / (5 * Cs + Cjs);
        q5 = (Cs * (-p->Q + Cjs * p->V5 - Cs * (p->V1 + p->V2 + p->V3 + p->V4 - 4 * p->V5))) / (5 * Cs + Cjs);
        p->Vn = (p->Q + q1 + q2 + q3 + q4 + q5) / Cjs;
    }
        else if (leg == 6)
    {
        double q1 = 0;
        double q2 = 0;
        double q3 = 0;
        double q4 = 0;
        double q5 = 0;
        double q6 = 0;
        q1 = (Cs * (-p->Q + Cjs * p->V1 + Cs * (4 * p->V1 - p->V2 - p->V3 - p->V4 - p->V5 + p->V6))) / (6 * Cs + Cjs);
        q2 = (Cs * (-p->Q + Cjs * p->V2 - Cs * (p->V1 - 4 * p->V2 + p->V3 + p->V4 + p->V5 + p->V6))) / (6 * Cs + Cjs);
        q3 = (Cs * (-p->Q + Cjs * p->V3 - Cs * (p->V1 + p->V2 - 4 * p->V3 + p->V4 + p->V5 + p->V6))) / (6 * Cs + Cjs);
        q4 = (Cs * (-p->Q + Cjs * p->V4 - Cs * (p->V1 + p->V2 + p->V3 - 4 * p->V4 + p->V5 + p->V6))) / (6 * Cs + Cjs);
        q5 = (Cs * (-p->Q + Cjs * p->V5 - Cs * (p->V1 + p->V2 + p->V3 + p->V4 - 4 * p->V5 + p->V6))) / (6 * Cs + Cjs);
        q6 = (Cs * (-p->Q + Cjs * p->V6 - Cs * (p->V1 + p->V2 + p->V3 + p->V4 + p->V5 - 4 * p->V6))) / (6 * Cs + Cjs);
        p->Vn = (p->Q + q1 + q2 + q3 + q4 + q5 + q6) / Cjs;
    }
}

// 振動子のエネルギー計算(&seo,足の本数,Cs,Cjs)
void seo_Ecalc(seo *p, int leg, double Cs, double Cjs)
{
    if (leg == 0)
    {
        p->dE[0] = -e * (e - 2 * p->Q) / (2 * Cjs);
        p->dE[1] = -e * (e + 2 * p->Q) / (2 * Cjs);
    }
    if (leg == 1)
    {
        p->dE[0] = -e * (e - 2 * (Cs * p->V1 + p->Q)) / (2 * (Cs + Cjs));
        p->dE[1] = -e * (e + 2 * (Cs * p->V1 + p->Q)) / (2 * (Cs + Cjs));
    }
    else if (leg == 2)
    {
        p->dE[0] = e * (-e + 2 * (p->Q + Cs * (p->V1 + p->V2))) / (2 * (2 * Cs + Cjs));
        p->dE[1] = -e * (e + 2 * (p->Q + Cs * (p->V1 + p->V2))) / (2 * (2 * Cs + Cjs));
    }
    else if (leg == 3)
    {
        p->dE[0] = (e * (2 * p->Q - e + 2 * Cs * (p->V1 + p->V2 + p->V3))) / (2 * (3 * Cs + Cjs));
        p->dE[1] = -(e * (2 * p->Q + e + 2 * Cs * (p->V1 + p->V2 + p->V3))) / (2 * (3 * Cs + Cjs));
    }
    else if (leg == 4)
    {
        p->dE[0] = e * (-e + 2 * (p->Q + Cs * (p->V1 + p->V2 + p->V3 + p->V4))) / (2 * (4 * Cs + Cjs));
        p->dE[1] = -e * (e + 2 * (p->Q + Cs * (p->V1 + p->V2 + p->V3 + p->V4))) / (2 * (4 * Cs + Cjs));
    }
    else if (leg == 5)
    {
        p->dE[0] = e * (-e + 2 * (p->Q + Cs * (p->V1 + p->V2 + p->V3 + p->V4 + p->V5))) / (2 * (5 * Cs + Cjs));
        p->dE[1] = -e * (e + 2 * (p->Q + Cs * (p->V1 + p->V2 + p->V3 + p->V4 + p->V5))) / (2 * (5 * Cs + Cjs));
    }
    else if (leg == 6)
    {
        p->dE[0] = e * (-e + 2 * (p->Q + Cs * (p->V1 + p->V2 + p->V3 + p->V4 + p->V5 + p->V6))) / (2 * (6 * Cs + Cjs));
        p->dE[1] = -e * (e + 2 * (p->Q + Cs * (p->V1 + p->V2 + p->V3 + p->V4 + p->V5 + p->V6))) / (2 * (6 * Cs + Cjs));
    }
}

// 振動子の電荷チャージ(&seo,R,dt)
double seo_charge(seo *p, seo *pmax, double R, double dt)
{
    for (p; p <= pmax; p++)
    {
        p->dQ = (p->Vd - p->Vn) * dt / R;
        p->Q += p->dQ;
    }
    return 0.1;
}

// 振動子の電荷チャージ(&seo,R,dt) *返り値なし
void Seo_charge(seo *p, seo *pmax, double R, double dt)
{
    for (p; p <= pmax; p++)
    {
        p->dQ = (p->Vd - p->Vn) * dt / R;
        p->Q += p->dQ;
    }
}

// 振動子の電荷チャージ(&seo,SEO_ROWS,SEO_COLUMNS,R,dt)(二次元配列) *返り値無し
void seo_2dimCharge(seo *p, int rows, int columns, double R, double dt)
{
    for (int i = 0; i < rows * columns; i++)
    {
        p->dQ = (p->Vd - p->Vn) * dt / R;
        p->Q += p->dQ;
        p++;
    }
}

// 振動子の電荷チャージ(&seo,SEO_PARTICLES,SEO_ROWS,SEO_COLUMNS,R,dt)(三次元配列)(返り値なし)
void seo_3dimcharge(seo *p, int particles, int rows, int columns, double R, double dt)
{
    for (int i = 0; i < particles * rows * columns; i++)
    {
        p->dQ = (p->Vd - p->Vn) * dt / R;
        p->Q += p->dQ;
        p++;
    }
}

// 振動子の電荷チャージ(&seo,SEO_PARTICLES,SEO_ROWS,SEO_COLUMNS,R,dt)(三次元配列)
double seo_3dimCharge(seo *p, int particles, int rows, int columns, double R, double dt)
{
    for (int i = 0; i < particles * rows * columns; i++)
    {
        p->dQ = (p->Vd - p->Vn) * dt / R;
        p->Q += p->dQ;
        p++;
    }
    return 0.1;
}

// 振動子のwt計算(&seo,&seomax,Rj)
seo *seo_wt(seo *p, seo *pmax, double Rj)
{
    int i = 0;
    double tmp = 100;
    seo *a = p;
    for (p; p <= pmax; p++)
    {
        for (i = 0; i < 3; i++)
        {
            if (i == 0)
            {
                p->wt[i] = 0.1;
            }
            else
            {
                if (p->dE[i - 1] > 0)
                {
                    p->wt[i] = (e * e * Rj / p->dE[i - 1]) * log(1 / Random());
                }
                else
                {
                    p->wt[i] = 100;
                }
            }

            if (tmp > p->wt[i])
            {
                tmp = p->wt[i];
                p->tunnel = i;
                a = p;
            }
        }
    }
    return a;
}

// 振動子のwt計算(二次元配列)(&seo,SEO_ROWS,SEO_COLUMNS,Rj)
seo *seo_2dimwt(seo *p, int rows, int columns, double Rj)
{
    int i = 0;
    int j = 0;
    double tmp = 100;
    seo *a = p;
    for (i; i < rows * columns; i++)
    {
        for (j = 0; j < 3; j++)
        {
            if (j == 0)
            {
                p->wt[j] = 0.1;
            }
            else
            {
                if (p->dE[j - 1] > 0)
                {
                    p->wt[j] = (e * e * Rj / p->dE[j - 1]) * log(1 / Random());
                }
                else
                {
                    p->wt[j] = 100;
                }
            }

            if (tmp > p->wt[j])
            {
                tmp = p->wt[j];
                p->tunnel = j;
                a = p;
            }
        }
        p++;
    }
    return a;
}

// 振動子のwt計算(三次元配列)(&seo,SEO_PARTICLES,SEO_ROWS,SEO_COLUMNS,Rj)
seo *seo_3dimwt(seo *p, int particles, int rows, int columns, double Rj)
{
    int i = 0;
    int j = 0;
    double tmp = 100;
    seo *a = p;
    for (i; i < particles * rows * columns; i++)
    {
        for (j = 0; j < 3; j++)
        {
            if (j == 0)
            {
                p->wt[j] = 0.1;
            }
            else
            {
                if (p->dE[j - 1] > 0)
                {
                    p->wt[j] = (e * e * Rj / p->dE[j - 1]) * log(1 / Random());
                }
                else
                {
                    p->wt[j] = 100;
                }
            }

            if (tmp > p->wt[j])
            {
                tmp = p->wt[j];
                p->tunnel = j;
                a = p;
            }
        }
        p++;
    }
    return a;
}

// 振動子のトンネル(&seo)
void seo_tunnel(seo *p)
{
    if (p->tunnel == 1)
    {
        p->Q += -e;
    }
    else if (p->tunnel == 2)
    {
        p->Q += e;
    }
}

/*--------------------------------------------------メモリ----------------------------------------------------------------------*/

// メモリのパラメータ計算(&memori,足の本数,Cm,Cjm)
void memori_Pcalc(memori *p, int leg, double Cm, double Cjm)
{
    double qa = 0;
    double qb = 0;
    if (leg == 1)
    {
        qa = (-2 * Cm * e * p->m + Cjm * e * p->n + Cm * Cjm * (p->Vd + p->V1)) / (4 * Cm + Cjm);
        qb = (2 * Cm * e * p->m + Cjm * e * (p->m + p->n) + Cm * Cjm * (p->Vd + p->V1)) / (4 * Cm + Cjm);
        p->Vn = (qa + qb) / Cjm;
    }
    else if (leg == 2)
    {
        qa = (-3 * Cm * e * p->m + Cjm * e * p->n + Cm * Cjm * (p->Vd + p->V1 + p->V2)) / (6 * Cm + Cjm);
        qb = (3 * Cm * e * p->m + Cjm * e * (p->m + p->n) + Cm * Cjm * (p->Vd + p->V1 + p->V2)) / (6 * Cm + Cjm);
        p->Vn = (qa + qb) / Cjm;
    }
    else if (leg == 3)
    {
        qa = (-4 * Cm * e * p->m + Cjm * e * p->n + Cm * Cjm * (p->Vd + p->V1 + p->V2 + p->V3)) / (8 * Cm + Cjm);
        qb = (4 * Cm * e * p->m + Cjm * e * (p->m + p->n) + Cm * Cjm * (p->Vd + p->V1 + p->V2 + p->V3)) / (8 * Cm + Cjm);
        p->Vn = (qa + qb) / Cjm;
    }
}

// メモリのパラメータ計算(CL有)(&memori,足の本数,Cm,Cjm,CL)
void memori_Pcalcl(memori *p, int leg, double Cm, double Cjm, double CL)
{
    double qa = 0;
    double qb = 0;
    if (leg == 1)
    {
        qa = (-Cm * e * p->m - CL * e * p->m + Cjm * e * p->n + Cjm * CL * p->Vd + Cm * Cjm * p->V1) / (2 * Cm + Cjm + 2 * CL);
        qb = (Cm * e * p->m + CL * e * p->m + Cjm * e * (p->n + p->m) + Cjm * CL * p->Vd + Cm * Cjm * p->V1) / (2 * Cm + Cjm + 2 * CL);
        p->Vn = (qa + qb) / Cjm;
    }
    else if (leg == 2)
    {
        qa = (-2 * Cm * e * p->m - CL * e * p->m + Cjm * e * p->n + Cjm * CL * p->Vd + Cm * Cjm * (p->V1 + p->V2)) / (4 * Cm + Cjm + 2 * CL);
        qb = (2 * Cm * e * p->m + CL * e * p->m + Cjm * e * (p->n + p->m) + Cjm * CL * p->Vd + Cm * Cjm * (p->V1 + p->V2)) / (4 * Cm + Cjm + 2 * CL);
        p->Vn = (qa + qb) / Cjm;
    }
    else if (leg == 3)
    {
        qa = (-3 * Cm * e * p->m - CL * e * p->m + Cjm * e * p->n + Cjm * CL * p->Vd + Cm * Cjm * (p->V1 + p->V2 + p->V3)) / (6 * Cm + Cjm + 2 * CL);
        qb = (3 * Cm * e * p->m + CL * e * p->m + Cjm * e * (p->n + p->m) + Cjm * CL * p->Vd + Cm * Cjm * (p->V1 + p->V2 + p->V3)) / (6 * Cm + Cjm + 2 * CL);
        p->Vn = (qa + qb) / Cjm;
    }
    else
    {
        double V = p->V1 + p->V2 + p->V3 + p->V4 + p->V5 + p->V6;
        qa = (-leg * Cm * e * p->m - CL * e * p->m + Cjm * e * p->n + Cjm * CL * p->Vd + Cm * Cjm * V) / (2 * leg * Cm + Cjm + 2 * CL);
        qb = (leg * Cm * e * p->m + CL * e * p->m + Cjm * e * (p->n + p->m) + Cjm * CL * p->Vd + Cm * Cjm * V) / (2 * leg * Cm + Cjm + 2 * CL);
        p->Vn = (qa + qb) / Cjm;
    }
}

// メモリのエネルギー計算(&memori,足の本数,Cm,Cjm)
void memori_Ecalc(memori *p, int leg, double Cm, double Cjm)
{
    if (leg == 1)
    {
        p->dE[0] = -e * (Cjm * e * (1 + 2 * p->m + 2 * p->n) + 2 * Cm * (e + 2 * e * p->m + Cjm * (p->Vd + p->V1))) / (2 * Cjm * (4 * Cm + Cjm));        // 接地から上(m+1)
        p->dE[1] = -e * (Cjm * e * (1 + 2 * p->n) + 2 * Cm * (e - 2 * e * p->m + Cjm * (p->Vd + p->V1))) / (2 * Cjm * (4 * Cm + Cjm));                   // 下から上2(m-1,n+1)
        p->dE[2] = e * (Cjm * e * (-1 + 2 * p->n) - 2 * Cm * (e + 2 * e * p->m - Cjm * (p->Vd + p->V1))) / (2 * Cjm * (4 * Cm + Cjm));                   // 上から下1(m+1,n-1)
        p->dE[3] = e * (Cjm * e * (-1 + 2 * p->m + 2 * p->n) + 2 * Cm * (e * (-1 + 2 * e * p->m) + Cjm * (p->Vd + p->V1))) / (2 * Cjm * (4 * Cm + Cjm)); // 上から下2(m-1)
    }
    else if (leg == 2)
    {
        p->dE[0] = -e * (Cjm * e * (1 + 2 * p->m + 2 * p->n) + Cm * e * (3 + 6 * p->m) + 2 * Cm * Cjm * (p->Vd + p->V1 + p->V2)) / (2 * Cjm * (6 * Cm + Cjm));
        p->dE[1] = -e * (Cjm * e * (1 + 2 * p->n) + Cm * e * (3 - 6 * p->m) + 2 * Cm * Cjm * (p->Vd + p->V1 + p->V2)) / (2 * Cjm * (6 * Cm + Cjm));
        p->dE[2] = e * (-3 * Cm * e * (1 + 2 * p->m) + Cjm * e * (-1 + 2 * p->n) + 2 * Cm * Cjm * (p->Vd + p->V1 + p->V2)) / (2 * Cjm * (6 * Cm + Cjm));
        p->dE[3] = e * (Cm * e * (-3 + 6 * p->m) + Cjm * e * (-1 + 2 * p->m + 2 * p->n) + 2 * Cm * Cjm * (p->Vd + p->V1 + p->V2)) / (2 * Cjm * (6 * Cm + Cjm));
    }
    else if (leg == 3)
    {
        p->dE[0] = -e * (Cjm * e * (1 + 2 * p->m + 2 * p->n) + 2 * Cm * (e * (2 + 4 * p->m) + Cjm * (p->Vd + p->V1 + p->V2 + p->V3))) / (2 * Cjm * (8 * Cm + Cjm));
        p->dE[1] = -e * (Cjm * e * (1 + 2 * p->n) + 2 * Cm * (e * (2 - 4 * p->m) + Cjm * (p->Vd + p->V1 + p->V2 + p->V3))) / (2 * Cjm * (8 * Cm + Cjm));
        p->dE[2] = e * (-4 * Cm * e * (1 + 2 * p->m) + Cjm * e * (-1 + 2 * p->n) + 2 * Cm * Cjm * (p->Vd + p->V1 + p->V2 + p->V3)) / (2 * Cjm * (8 * Cm + Cjm));
        p->dE[3] = e * (Cjm * e * (-1 + 2 * p->m + 2 * p->n) + 2 * Cm * (e * (-2 + 4 * p->m) + Cjm * (p->Vd + p->V1 + p->V2 + p->V3))) / (2 * Cjm * (8 * Cm + Cjm));
    }
}

// メモリのエネルギー計算(CL有)(&memori,足の本数,Cm,Cjm,CL)
void memori_Ecalcl(memori *p, int leg, double Cm, double Cjm, double CL)
{
    if (leg == 1)
    {
        p->dE[0] = -e * (CL * e * (1 + 2 * p->m) + Cjm * (e + 2 * e * (p->m + p->n) + 2 * CL * p->Vd) + Cm * (e + 2 * e * p->m + 2 * Cjm * p->V1)) / (2 * Cjm * (2 * Cm + Cjm + 2 * CL)); // 接地から上(m+1)
        p->dE[1] = -e * (CL * e * (1 - 2 * p->m) + Cjm * (e + 2 * e * p->n + 2 * CL * p->Vd) + Cm * (e - 2 * e * p->m + 2 * Cjm * p->V1)) / (2 * Cjm * (2 * Cm + Cjm + 2 * CL));          // 下から上2(m-1,n+1)
        p->dE[2] = -e * (CL * e * (1 + 2 * p->m) + Cjm * (e - 2 * e * p->n - 2 * CL * p->Vd) + Cm * (e + 2 * e * p->m - 2 * Cjm * p->V1)) / (2 * Cjm * (2 * Cm + Cjm + 2 * CL));          // 上から下1(m+1,n-1)
        p->dE[3] = -e * (CL * e * (1 - 2 * p->m) + Cjm * (e - 2 * e * (p->m + p->n) - 2 * CL * p->Vd) + Cm * (e - 2 * e * p->m - 2 * Cjm * p->V1)) / (2 * Cjm * (2 * Cm + Cjm + 2 * CL)); // 上から下2(m-1)
    }
    else if (leg == 2)
    {
        p->dE[0] = -e * (CL * e * (1 + 2 * p->m) + Cjm * (e + 2 * e * (p->m + p->n) + 2 * CL * p->Vd) + 2 * Cm * (e + 2 * e * p->m + Cjm * (p->V1 + p->V2))) / (2 * Cjm * (4 * Cm + Cjm + 2 * CL)); // 接地から上(m+1)
        p->dE[1] = -e * (CL * e * (1 - 2 * p->m) + Cjm * (e + 2 * e * p->n + 2 * CL * p->Vd) + 2 * Cm * (e - 2 * e * p->m + Cjm * (p->V1 + p->V2))) / (2 * Cjm * (4 * Cm + Cjm + 2 * CL));          // 下から上2(m-1,n+1)
        p->dE[2] = -e * (CL * e * (1 + 2 * p->m) + Cjm * (e - 2 * e * p->n - 2 * CL * p->Vd) + 2 * Cm * (e + 2 * e * p->m - Cjm * (p->V1 + p->V2))) / (2 * Cjm * (4 * Cm + Cjm + 2 * CL));          // 上から下1(m+1,n-1)
        p->dE[3] = -e * (CL * e * (1 - 2 * p->m) + Cjm * (e - 2 * e * (p->m + p->n) - 2 * CL * p->Vd) + 2 * Cm * (e - 2 * e * p->m - Cjm * (p->V1 + p->V2))) / (2 * Cjm * (4 * Cm + Cjm + 2 * CL)); // 上から下2(m-1)
    }
    else if (leg == 3)
    {
        p->dE[0] = -e * (CL * e * (1 + 2 * p->m) + Cjm * (e + 2 * e * (p->m + p->n) + 2 * CL * p->Vd) + Cm * (3 * e + 6 * e * p->m + 2 * Cjm * (p->V1 + p->V2 + p->V3))) / (2 * Cjm * (6 * Cm + Cjm + 2 * CL)); // 接地から上(m+1)
        p->dE[1] = -e * (CL * e * (1 - 2 * p->m) + Cjm * (e + 2 * e * p->n + 2 * CL * p->Vd) + Cm * (3 * e - 6 * e * p->m + 2 * Cjm * (p->V1 + p->V2 + p->V3))) / (2 * Cjm * (6 * Cm + Cjm + 2 * CL));          // 下から上2(m-1,n+1)
        p->dE[2] = -e * (CL * e * (1 + 2 * p->m) + Cjm * (e - 2 * e * p->n - 2 * CL * p->Vd) + Cm * (3 * e + 6 * e * p->m - 2 * Cjm * (p->V1 + p->V2 + p->V3))) / (2 * Cjm * (6 * Cm + Cjm + 2 * CL));          // 上から下1(m+1,n-1)
        p->dE[3] = -e * (CL * e * (1 - 2 * p->m) + Cjm * (e - 2 * e * (p->m + p->n) - 2 * CL * p->Vd) + Cm * (3 * e - 6 * e * p->m - 2 * Cjm * (p->V1 + p->V2 + p->V3))) / (2 * Cjm * (6 * Cm + Cjm + 2 * CL)); // 上から下2(m-1)
    }
    else
    {
        double V = p->V1 + p->V2 + p->V3 + p->V4 + p->V5 + p->V6;
        p->dE[0] = -e * (CL * e * (1 + 2 * p->m) + Cjm * (e + 2 * e * (p->m + p->n) + 2 * CL * p->Vd) + Cm * (leg * e + 2 * leg * e * p->m + 2 * Cjm * V)) / (2 * Cjm * (2 * leg * Cm + Cjm + 2 * CL)); // 接地から上(m+1)
        p->dE[1] = -e * (CL * e * (1 - 2 * p->m) + Cjm * (e + 2 * e * p->n + 2 * CL * p->Vd) + Cm * (leg * e - 2 * leg * e * p->m + 2 * Cjm * V)) / (2 * Cjm * (2 * leg * Cm + Cjm + 2 * CL));          // 下から上2(m-1,n+1)
        p->dE[2] = -e * (CL * e * (1 + 2 * p->m) + Cjm * (e - 2 * e * p->n - 2 * CL * p->Vd) + Cm * (leg * e + 2 * leg * e * p->m - 2 * Cjm * V)) / (2 * Cjm * (2 * leg + Cjm + 2 * CL));               // 上から下1(m+1,n-1)
        p->dE[3] = -e * (CL * e * (1 - 2 * p->m) + Cjm * (e - 2 * e * (p->m + p->n) - 2 * CL * p->Vd) + Cm * (leg * e - 2 * leg * e * p->m - 2 * Cjm * V)) / (2 * Cjm * (2 * leg * Cm + Cjm + 2 * CL)); // 上から下2(m-1)
    }
}

// メモリのwt計算(&memori,&memorimax,Rj)
memori *memori_wt(memori *p, memori *pmax, double Rj)
{
    int i = 0;
    double tmp = 100;
    memori *a = p;
    for (p; p <= pmax; p++)
    {
        for (i = 0; i < 5; i++)
        {
            if (i == 0)
            {
                p->wt[i] = 0.1;
            }
            else
            {
                if (p->dE[i - 1] > 0)
                {
                    p->wt[i] = (e * e * Rj / p->dE[i - 1]) * log(1 / Random());
                }
                else
                {
                    p->wt[i] = 100;
                }
            }
            if (tmp > p->wt[i])
            {
                tmp = p->wt[i];
                p->tunnel = i;
                a = p;
            }
        }
    }
    return a;
}

// メモリのトンネル(&memori)
void memori_tunnel(memori *p)
{
    if (p->tunnel == 1)
    {
        p->m += 1;
    }
    else if (p->tunnel == 2)
    {
        p->m -= 1;
        p->n += 1;
    }
    else if (p->tunnel == 3)
    {
        p->m += 1;
        p->n -= 1;
    }
    else if (p->tunnel == 4)
    {
        p->m -= 1;
    }
}

/*--------------------------------------------------一方通行---------------------------------------------------------------------*/

// 一方通行のVd割り当て(&onewayseo, Vdの絶対値, 向き(0 left or 1 right),C,Cjs2,Cjs3)
void ow_seoVd(onewayseo *p, double Vd, int direction, double Cs, double Cjs2, double Cjs3)
{
    int i = 0;
    for (i = 0; i < 6; i++)
    {
        if (i == 0 || i == 2 || i == 3 || i == 5)
        {
            p->ows[i].Vd = -1 * Vd;
        }
        else if (i == 1 && direction == 0)
        { // 一方通行　左
            p->ows[i].Vd = Vd - ((Cs * e) / ((3 * Cs + Cjs3) * (2 * Cs + Cjs2)));
        }
        else if (i == 4 && direction == 0)
        {
            p->ows[i].Vd = Vd;
        }
        else if (i == 1 && direction == 1)
        { // 一方通行　右
            p->ows[i].Vd = Vd;
        }
        else if (i == 4 && direction == 1)
        {
            p->ows[i].Vd = Vd - ((Cs * e) / ((3 * Cs + Cjs3) * (2 * Cs + Cjs2)));
        }
    }
}

// 一方通行のパラメータ計算(&onewayseo,Cs,Cjs,左端のVn,右端のVn)
void ow_seoPcalc(onewayseo *p, double Cs, double Cjs, double Vn0, double Vn1)
{
    int i = 0;
    for (i = 0; i < 6; i++)
    {
        if (i == 0) // 足2
        {
            p->ows[i].V1 = Vn0;
            p->ows[i].V2 = p->ows[1].Vn;
            seo_Pcalc(&(p->ows[i]), 2, Cs, Cjs);
        }
        else if (i == 1) // 足3
        {
            p->ows[i].V1 = p->ows[0].Vn;
            p->ows[i].V2 = p->ows[2].Vn;
            p->ows[i].V3 = p->ows[3].Vn;
            seo_Pcalc(&(p->ows[i]), 3, Cs, Cjs);
        }
        else if (i == 2 || i == 3) // 足2
        {
            p->ows[i].V1 = p->ows[1].Vn;
            p->ows[i].V2 = p->ows[4].Vn;
            seo_Pcalc(&(p->ows[i]), 2, Cs, Cjs);
        }
        else if (i == 4) // 足3
        {
            p->ows[i].V1 = p->ows[2].Vn;
            p->ows[i].V2 = p->ows[3].Vn;
            p->ows[i].V3 = p->ows[5].Vn;
            seo_Pcalc(&(p->ows[i]), 3, Cs, Cjs);
        }
        else if (i == 5) // 足2
        {
            p->ows[i].V1 = p->ows[4].Vn;
            p->ows[i].V2 = Vn1;
            seo_Pcalc(&(p->ows[i]), 2, Cs, Cjs);
        }
    }
}

// 一方通行のパラメータ計算(足の本数でCj変更)(&onewayseo,Cs,Cjs足2,Cj足3,左端のVn,右端のVn)
void ow_seoPcalc2(onewayseo *p, double Cs, double Cjs2, double Cjs3, double Vn0, double Vn1)
{
    int i = 0;
    for (i = 0; i < 6; i++)
    {
        if (i == 0) // 足2
        {
            p->ows[i].V1 = Vn0;
            p->ows[i].V2 = p->ows[1].Vn;
            seo_Pcalc(&(p->ows[i]), 2, Cs, Cjs2);
        }
        else if (i == 1) // 足3
        {
            p->ows[i].V1 = p->ows[0].Vn;
            p->ows[i].V2 = p->ows[2].Vn;
            p->ows[i].V3 = p->ows[3].Vn;
            seo_Pcalc(&(p->ows[i]), 3, Cs, Cjs3);
        }
        else if (i == 2 || i == 3) // 足2
        {
            p->ows[i].V1 = p->ows[1].Vn;
            p->ows[i].V2 = p->ows[4].Vn;
            seo_Pcalc(&(p->ows[i]), 2, Cs, Cjs2);
        }
        else if (i == 4) // 足3
        {
            p->ows[i].V1 = p->ows[2].Vn;
            p->ows[i].V2 = p->ows[3].Vn;
            p->ows[i].V3 = p->ows[5].Vn;
            seo_Pcalc(&(p->ows[i]), 3, Cs, Cjs3);
        }
        else if (i == 5) // 足2
        {
            p->ows[i].V1 = p->ows[4].Vn;
            p->ows[i].V2 = Vn1;
            seo_Pcalc(&(p->ows[i]), 2, Cs, Cjs2);
        }
    }
}

// 一方通行のエネルギー計算(&onewayseo,Cs,Cjs)
void ow_seoEcalc(onewayseo *p, double Cs, double Cjs)
{
    int i = 0;
    for (i = 0; i < 6; i++)
    {
        if (i == 1 || i == 4)
        { // 足3本
            seo_Ecalc(&(p->ows[i]), 3, Cs, Cjs);
        }
        else if (i == 0 || i == 2 || i == 3 || i == 5)
        { // 足2本
            seo_Ecalc(&(p->ows[i]), 2, Cs, Cjs);
        }
    }
}

// 一方通行のエネルギー計算(足の本数でCj変更)(&onewayseo,Cs,Cjs足2,Cj足3)
void ow_seoEcalc2(onewayseo *p, double Cs, double Cjs2, double Cjs3)
{
    int i = 0;
    for (i = 0; i < 6; i++)
    {
        if (i == 1 || i == 4)
        { // 足3本
            seo_Ecalc(&(p->ows[i]), 3, Cs, Cjs3);
        }
        else if (i == 0 || i == 2 || i == 3 || i == 5)
        { // 足2本
            seo_Ecalc(&(p->ows[i]), 2, Cs, Cjs2);
        }
    }
}

// 一方通行の電荷チャージ(&onewayseo,&onewayseomax,R,dt) *返り値ありのseo_chargeと併用する場合dtの値に注意
void ow_seocharge(onewayseo *p, onewayseo *pmax, double R, double dt)
{
    for (p; p <= pmax; p++)
    {
        Seo_charge(&(p->ows[0]), &(p->ows[5]), R, dt); // 返り値なし
    }
}

// 一方通行の電荷チャージ(&onewayseo,OWSEO_PARTICLES,OWSEO_ROWS,OWSEO_COLUMNS,R,dt)(三次元配列)
void ow_seo3dimcharge(onewayseo *p, int particles, int rows, int columns, double R, double dt)
{
    for (int i = 0; i < particles * rows * columns; i++)
    {
        Seo_charge(&(p->ows[0]), &(p->ows[5]), R, dt); // 返り値なし
        p++;
    }
}

// 一方通行の待ち時間計算(&onewayseo,&onewayseomax,Rj)
onewayseo *ow_seowt(onewayseo *p, onewayseo *pmax, double Rj)
{
    int i = 0;
    int k = 0;
    double tmp = 100;
    onewayseo *a = p;
    for (p; p <= pmax; p++)
    {
        for (k = 0; k < 6; k++)
        {
            for (i = 0; i < 3; i++)
            {
                if (i == 0)
                {
                    p->ows[k].wt[i] = 0.1;
                }
                else
                {
                    if (p->ows[k].dE[i - 1] > 0)
                    {
                        p->ows[k].wt[i] = (e * e * Rj / p->ows[k].dE[i - 1]) * log(1 / Random());
                    }
                    else
                    {
                        p->ows[k].wt[i] = 100;
                    }
                }
                if (tmp >= p->ows[k].wt[i])
                {
                    tmp = p->ows[k].wt[i];
                    p->ows[k].tunnel = i;
                    p->locate = k;
                    a = p;
                }
            }
        }
    }
    return a;
}

// 一方通行の待ち時間計算(&onewayseo,OWSEO_PARTICLES,OWSEO_ROWS,OWSEO_COLUMNS,Rj)(三次元配列)
onewayseo *ow_seo3dimwt(onewayseo *p, int particles, int rows, int columns, double Rj)
{
    int i = 0;
    int j = 0;
    int k = 0;
    double tmp = 100;
    onewayseo *a = p;
    for (i; i < particles * rows * columns; i++)
    {
        for (k = 0; k < 6; k++)
        {
            for (j = 0; j < 3; j++)
            {
                if (j == 0)
                {
                    p->ows[k].wt[j] = 0.1;
                }
                else
                {
                    if (p->ows[k].dE[j - 1] > 0)
                    {
                        p->ows[k].wt[j] = (e * e * Rj / p->ows[k].dE[j - 1]) * log(1 / Random());
                    }
                    else
                    {
                        p->ows[k].wt[j] = 100;
                    }
                }
                if (tmp >= p->ows[k].wt[j])
                {
                    tmp = p->ows[k].wt[j];
                    p->ows[k].tunnel = j;
                    p->locate = k;
                    a = p;
                }
            }
        }
        p++;
    }
    return a;
}
/*--------------------------------------------------一方通行(振動子4個)---------------------------------------------------------------------*/
// 一方通行のVd割り当て(&oneway_4seo, Vdの絶対値, 向き(0 left or 1 right),C,Cjs2,Cjs3)
void oneway_4seo_setVd(oneway_4seo *p, double Vd, int direction, double Cs, double Cjs2, double Cjs3)
{
    for (int i = 0; i < 4; i++)
    {
        if (i == 1 || i == 2)
        {
            p->ows[i].Vd = Vd;
        }
        else if (i == 0 && direction == 0)
        { // 一方通行　左
            p->ows[i].Vd = -Vd + ((Cs * e) / ((3 * Cs + Cjs3) * (2 * Cs + Cjs2)));
        }
        else if (i == 3 && direction == 0)
        {
            p->ows[i].Vd = -Vd;
        }
        else if (i == 0 && direction == 1)
        { // 一方通行　右
            p->ows[i].Vd = -Vd;
        }
        else if (i == 3 && direction == 1)
        {
            p->ows[i].Vd = -Vd + ((Cs * e) / ((3 * Cs + Cjs3) * (2 * Cs + Cjs2)));
        }
    }
}
// 一方通行のパラメータ計算(&oneway_4seo,Cs,Cjs足2,Cj足3,左端のVn,右端のVn)
void oneway_4seo_calcPara(oneway_4seo *p, double Cs, double Cjs2, double Cjs3, double Vn0, double Vn1)
{
    int i = 0;
    for (i = 0; i < 4; i++)
    {
        if (i == 0) // 足3
        {
            p->ows[i].V1 = Vn0;
            p->ows[i].V2 = p->ows[1].Vn;
            p->ows[i].V3 = p->ows[2].Vn;
            seo_Pcalc(&(p->ows[i]), 3, Cs, Cjs3);
        }
        else if (i == 1 || i == 2) // 足2
        {
            p->ows[i].V1 = p->ows[0].Vn;
            p->ows[i].V2 = p->ows[3].Vn;
            seo_Pcalc(&(p->ows[i]), 2, Cs, Cjs2);
        }
        else if (i == 3) // 足3
        {
            p->ows[i].V1 = Vn1;
            p->ows[i].V2 = p->ows[1].Vn;
            p->ows[i].V3 = p->ows[2].Vn;
            seo_Pcalc(&(p->ows[i]), 3, Cs, Cjs3);
        }
    }
}
// 一方通行のエネルギー計算(&oneway_4seo,Cs,Cjs足2,Cj足3)
void oneway_4seo_calcEner(oneway_4seo *p, double Cs, double Cjs2, double Cjs3)
{
    int i = 0;
    for (i = 0; i < 4; i++)
    {
        if (i == 0 || i == 3)
        { // 足3本
            seo_Ecalc(&(p->ows[i]), 3, Cs, Cjs3);
        }
        else if (i == 1 || i == 2)
        { // 足2本
            seo_Ecalc(&(p->ows[i]), 2, Cs, Cjs2);
        }
    }
}
// 一方通行の電荷チャージ(&oneway_4seo,OWSEO_PARTICLES,OWSEO_ROWS,OWSEO_COLUMNS,R,dt)(三次元配列)
void oneway_4seo_3dimCharge(oneway_4seo *p, int particles, int rows, int columns, double R, double dt)
{
    int i = 0;
    for (i = 0; i < particles * rows * columns; i++)
    {
        Seo_charge(&(p->ows[0]), &(p->ows[3]), R, dt); // 返り値なし
        p++;
    }
}

// 一方通行の待ち時間計算(&onewayseo,OWSEO_PARTICLES,OWSEO_ROWS,OWSEO_COLUMNS,Rj)(三次元配列)
oneway_4seo *oneway_4seo_3dimWt(oneway_4seo *p, int particles, int rows, int columns, double Rj)
{
    int i = 0;
    int j = 0;
    int k = 0;
    double tmp = 100;
    oneway_4seo *a = p;
    for (i; i < particles * rows * columns; i++)
    {
        for (k = 0; k < 4; k++)
        {
            for (j = 0; j < 3; j++)
            {
                if (j == 0)
                {
                    p->ows[k].wt[j] = 0.1;
                }
                else
                {
                    if (p->ows[k].dE[j - 1] > 0)
                    {
                        p->ows[k].wt[j] = (e * e * Rj / p->ows[k].dE[j - 1]) * log(1 / Random());
                    }
                    else
                    {
                        p->ows[k].wt[j] = 100;
                    }
                }
                if (tmp >= p->ows[k].wt[j])
                {
                    tmp = p->ows[k].wt[j];
                    p->ows[k].tunnel = j;
                    p->locate = k;
                    a = p;
                }
            }
        }
        p++;
    }
    return a;
}
/*--------------------------------------------------汎用-----------------------------------------------------------------------*/

// 0から1の間の乱数生成
double Random(void)
{
    static int flag;

    if (flag == 0)
    {
        srand((unsigned int)time(NULL));
        flag = 1;
    }

    return rand() / (double)RAND_MAX;
}

// 小さい方の値を返す(a,b)
double min(double a, double b)
{
    if (a - b > 0)
    {
        return b;
    }
    else
    {
        return a;
    }
}

// 現在の時間を取得する
double getTime()
{
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return tv.tv_sec + (double)tv.tv_usec * 1e-6;
}

// 実行時間を表示する(実行開始時間,実行終了時間)
void getRunTime(double st_time, double end_time)
{
    printf("Run Time = %.6f s\n", end_time - st_time);
}

// 隣接振動子からの影響を考慮したバイアス電圧(Cs,本体の足,本体のCjs,隣接振動子の足,隣接振動子のCjs)
double viasarrange(double Cs, int myleg, int yourleg, double myCjs, double yourCjs)
{
    return ((Cs * e) / ((myleg * Cs + myCjs) * (yourleg * Cs + yourCjs)));
}

// 振動子と一方通行のチャージ(&s,&smax,&os,&osmax,R,&dt)
void charge(seo *p, seo *pmax, onewayseo *op, onewayseo *opmax, double R, double *dt)
{
    ow_seocharge(op, opmax, R, *dt);
    *dt = seo_charge(p, pmax, R, *dt);
}

// 振動子と一方通行のチャージ(&seofirst,&owseofirst,SEO_PARTICLES,SEO_ROWS,SEO_COLUMNS,OWSEO_PARTICLES,OWSEO_ROWS,OWSEO_COLUMNS,R,&dt)(三次元配列)
void seoowseo3dimcharge(seo *p, int seoparticles, int seorows, int seocolumns, onewayseo *op, int owseoparticles, int owseorows, int owseocolumns, double R, double *dt)
{
    ow_seo3dimcharge(op, owseoparticles, owseorows, owseocolumns, R, *dt);
    *dt = seo_3dimCharge(p, seoparticles, seorows, seocolumns, R, *dt);
}

// 振動子のトンネル(表示なし)(sp,&t,&dt)
void tunnelseo(seo *sp, double *t, double *dt)
{
    // トンネル待ち時間比較
    if (sp->tunnel == 0)
    { // トンネルしないとき
        *t += *dt;
    }
    else if (sp->wt[sp->tunnel] < *dt && sp->wt[sp->tunnel] > 0.0)
    { // 振動子トンネル
        seo_tunnel(sp);
        *t += sp->wt[sp->tunnel];
        *dt = sp->wt[sp->tunnel];
    }
    else
    { // トンネルしないとき
        *t += *dt;
    }
}

// 振動子のトンネル(表示有)(spfirst,sp,&t,&dt)
void tunnelprintseo(seo *spfirst, seo *sp, double *t, double *dt)
{
    // トンネル待ち時間比較
    if (sp->tunnel == 0)
    { // トンネルしないとき
        *t += *dt;
    }
    else if (sp->wt[sp->tunnel] < *dt && sp->wt[sp->tunnel] > 0.0)
    { // 振動子トンネル
        seo_tunnel(sp);
        printf("t = %f seo[%d]\n", *t, (sp - spfirst));
        *t += sp->wt[sp->tunnel];
        *dt = sp->wt[sp->tunnel];
    }
    else
    { // トンネルしないとき
        *t += *dt;
    }
}
// 振動子,メモリ,一方通行のトンネル(sp,mp,osp,&t,&dt)
void tunnel(seo *sp, memori *mp, onewayseo *osp, double *t, double *dt)
{
    // トンネル待ち時間比較
    if (sp->tunnel == 0 && mp->tunnel == 0 && osp->ows[osp->locate].tunnel == 0)
    { // トンネルしないとき
        *t += *dt;
    }
    else if (sp->wt[sp->tunnel] < *dt && sp->wt[sp->tunnel] > 0.0 && sp->wt[sp->tunnel] < mp->wt[mp->tunnel] && sp->wt[sp->tunnel] < osp->ows[osp->locate].wt[osp->ows[osp->locate].tunnel])
    { // 振動子トンネル
        seo_tunnel(sp);
        *t += sp->wt[sp->tunnel];
        *dt = sp->wt[sp->tunnel];
    }
    else if (mp->wt[mp->tunnel] < *dt && mp->wt[mp->tunnel] > 0.0 && mp->wt[mp->tunnel] < sp->wt[sp->tunnel] && mp->wt[mp->tunnel] < osp->ows[osp->locate].wt[osp->ows[osp->locate].tunnel])
    { // メモリトンネル
        memori_tunnel(mp);
        *t += mp->wt[mp->tunnel];
        *dt = mp->wt[mp->tunnel];
    }
    else if (osp->ows[osp->locate].wt[osp->ows[osp->locate].tunnel] < *dt && osp->ows[osp->locate].wt[osp->ows[osp->locate].tunnel] > 0.0 && osp->ows[osp->locate].wt[osp->ows[osp->locate].tunnel] < sp->wt[mp->tunnel] && osp->ows[osp->locate].wt[osp->ows[osp->locate].tunnel] < mp->wt[mp->tunnel])
    { // 一方通行トンネル
        seo_tunnel(&(osp->ows[osp->locate]));
        *t += osp->ows[osp->locate].wt[osp->ows[osp->locate].tunnel];
        *dt = osp->ows[osp->locate].wt[osp->ows[osp->locate].tunnel];
    }
    else
    { // トンネルしないとき
        *t += *dt;
    }
}

// 振動子,一方通行のトンネル(sp,mp,osp,&t,&dt)
void tunnelseoowseo(seo *sp, onewayseo *osp, double *t, double *dt)
{
    // トンネル待ち時間比較
    if (sp->tunnel == 0 && osp->ows[osp->locate].tunnel == 0)
    { // トンネルしないとき
        *t += *dt;
    }
    else if (sp->wt[sp->tunnel] < *dt && sp->wt[sp->tunnel] > 0.0 && sp->wt[sp->tunnel] < osp->ows[osp->locate].wt[osp->ows[osp->locate].tunnel])
    { // 振動子トンネル
        seo_tunnel(sp);
        *t += sp->wt[sp->tunnel];
        *dt = sp->wt[sp->tunnel];
    }
    else if (osp->ows[osp->locate].wt[osp->ows[osp->locate].tunnel] < *dt && osp->ows[osp->locate].wt[osp->ows[osp->locate].tunnel] > 0.0)
    { // 一方通行トンネル
        seo_tunnel(&(osp->ows[osp->locate]));
        *t += osp->ows[osp->locate].wt[osp->ows[osp->locate].tunnel];
        *dt = osp->ows[osp->locate].wt[osp->ows[osp->locate].tunnel];
    }
    else
    { // トンネルしないとき
        *t += *dt;
    }
}

// 振動子,一方通行のトンネル(sp,mp,osp,&t,&dt)
void Seo_onway4Seo_tunnel(seo *sp, oneway_4seo *osp, double *t, double *dt)
{ // トンネル待ち時間比較
    if (sp->tunnel == 0 && osp->ows[osp->locate].tunnel == 0)
    { // トンネルしないとき
        *t += *dt;
    }
    else if (sp->wt[sp->tunnel] < *dt && sp->wt[sp->tunnel] > 0.0 && sp->wt[sp->tunnel] < osp->ows[osp->locate].wt[osp->ows[osp->locate].tunnel])
    { // 振動子トンネル
        seo_tunnel(sp);
        *t += sp->wt[sp->tunnel];
        *dt = sp->wt[sp->tunnel];
    }
    else if (osp->ows[osp->locate].wt[osp->ows[osp->locate].tunnel] < *dt && osp->ows[osp->locate].wt[osp->ows[osp->locate].tunnel] > 0.0)
    { // 一方通行トンネル
        seo_tunnel(&(osp->ows[osp->locate]));
        *t += osp->ows[osp->locate].wt[osp->ows[osp->locate].tunnel];
        *dt = osp->ows[osp->locate].wt[osp->ows[osp->locate].tunnel];
    }
    else
    { // トンネルしないとき
        *t += *dt;
    }
}
// 振動子,メモリ,一方通行のトンネル(表示有)(sp,mp,osp,&t,&dt)
void tunnelprint(seo *spfirst, seo *sp, memori *mpfirst, memori *mp, onewayseo *ospfirst, onewayseo *osp, double *t, double *dt)
{
    // トンネル待ち時間比較
    if (sp->tunnel == 0 && mp->tunnel == 0 && osp->ows[osp->locate].tunnel == 0)
    { // トンネルしないとき
        *t += *dt;
    }
    else if (sp->wt[sp->tunnel] < *dt && sp->wt[sp->tunnel] > 0.0 && sp->wt[sp->tunnel] < mp->wt[mp->tunnel] && sp->wt[sp->tunnel] < osp->ows[osp->locate].wt[osp->ows[osp->locate].tunnel])
    { // 振動子トンネル
        seo_tunnel(sp);
        printf("t = %f seo[%d]\n", *t, (sp - spfirst));
        *t += sp->wt[sp->tunnel];
        *dt = sp->wt[sp->tunnel];
    }
    else if (mp->wt[mp->tunnel] < *dt && mp->wt[mp->tunnel] > 0.0 && mp->wt[mp->tunnel] < sp->wt[sp->tunnel] && mp->wt[mp->tunnel] < osp->ows[osp->locate].wt[osp->ows[osp->locate].tunnel])
    { // メモリトンネル
        memori_tunnel(mp);
        printf("t = %f memori[%d]\n", *t, (mp - mpfirst));
        *t += mp->wt[mp->tunnel];
        *dt = mp->wt[mp->tunnel];
    }
    else if (osp->ows[osp->locate].wt[osp->ows[osp->locate].tunnel] < *dt && osp->ows[osp->locate].wt[osp->ows[osp->locate].tunnel] > 0.0 && osp->ows[osp->locate].wt[osp->ows[osp->locate].tunnel] < sp->wt[sp->tunnel] && osp->ows[osp->locate].wt[osp->ows[osp->locate].tunnel] < mp->wt[mp->tunnel])
    { // 一方通行トンネル
        seo_tunnel(&(osp->ows[osp->locate]));
        printf("t = %f onewayseo[%d]seo[%d]\n", *t, (osp - ospfirst), (&(osp->ows[osp->locate]) - &(osp->ows[0])));
        *t += osp->ows[osp->locate].wt[osp->ows[osp->locate].tunnel];
        *dt = osp->ows[osp->locate].wt[osp->ows[osp->locate].tunnel];
    }
    else
    { // トンネルしないとき
        *t += *dt;
    }
}

// 振動子,一方通行のトンネル(表示有)(sp,osp,&t,&dt)
void tunnelprintso(seo *spfirst, seo *sp, onewayseo *ospfirst, onewayseo *osp, double *t, double *dt)
{
    // トンネル待ち時間比較
    if (sp->tunnel == 0 && osp->ows[osp->locate].tunnel == 0)
    { // トンネルしないとき
        *t += *dt;
    }
    else if (sp->wt[sp->tunnel] < *dt && sp->wt[sp->tunnel] > 0.0 && sp->wt[sp->tunnel] < osp->ows[osp->locate].wt[osp->ows[osp->locate].tunnel])
    { // 振動子トンネル
        seo_tunnel(sp);
        printf("t = %f seo[%d]\n", *t, (sp - spfirst));
        *t += sp->wt[sp->tunnel];
        *dt = sp->wt[sp->tunnel];
    }
    else if (osp->ows[osp->locate].wt[osp->ows[osp->locate].tunnel] < *dt && osp->ows[osp->locate].wt[osp->ows[osp->locate].tunnel] > 0.0 && osp->ows[osp->locate].wt[osp->ows[osp->locate].tunnel] < sp->wt[sp->tunnel])
    { // 一方通行トンネル
        seo_tunnel(&(osp->ows[osp->locate]));
        printf("t = %f onewayseo[%d]seo[%d]\n", *t, (osp - ospfirst), (&(osp->ows[osp->locate]) - &(osp->ows[0])));
        *t += osp->ows[osp->locate].wt[osp->ows[osp->locate].tunnel];
        *dt = osp->ows[osp->locate].wt[osp->ows[osp->locate].tunnel];
    }
    else
    { // トンネルしないとき
        *t += *dt;
    }
}

// 振動子,メモリのトンネル(sp,mp,&t,&dt)(print表示有)
void tunnelprintsm(seo *spfirst, seo *sp, memori *mpfirst, memori *mp, double *t, double *dt)
{
    // トンネル待ち時間比較
    if (sp->tunnel == 0 && mp->tunnel == 0)
    { // トンネルしないとき
        *t += *dt;
    }
    else if (sp->wt[sp->tunnel] < *dt && sp->wt[sp->tunnel] > 0.0 && sp->wt[sp->tunnel] < mp->wt[mp->tunnel])
    { // 振動子トンネル
        seo_tunnel(sp);
        printf("t = %f seo[%d]\n", *t, (sp - spfirst));
        *t += sp->wt[sp->tunnel];
        *dt = sp->wt[sp->tunnel];
    }
    else if (mp->wt[mp->tunnel] < *dt && mp->wt[mp->tunnel] > 0.0 && mp->wt[mp->tunnel] < sp->wt[sp->tunnel])
    { // メモリトンネル
        memori_tunnel(mp);
        printf("t = %f memori[%d]\n", *t, (mp - mpfirst));
        *t += mp->wt[mp->tunnel];
        *dt = mp->wt[mp->tunnel];
    }
    else
    { // トンネルしないとき
        *t += *dt;
    }
}

// ファイルにlayer1の2次元データを読み込む(pt+=0.1)
void fprintlay1(seo *spfirst, int seoparticles, int seorows, int seocolumns, FILE *fp, double t, double *pt)
{
    if (*pt <= t)
    { // 表示
        int x = 0;
        int y = 0;
        int z = 0;
        int count = 0;
        for (y = 0; y < seorows; y++)
        {
            for (z = 0; z < seoparticles; z++)
            {
                for (x = 1; x < seocolumns; x++)
                {
                    if (y == 0 && z == 0)
                    {
                        fprintf(fp, "%d %d %f\n", 2 * y + z, x, fabs((spfirst + x + y * seocolumns + z * (seocolumns * seorows))->Vn));
                    }
                    else if (y == seorows - 1 && z == 1)
                    {
                        fprintf(fp, "%d %d %f\n", 2 * y + z, x, fabs((spfirst + (x - 1) + y * seocolumns + z * (seocolumns * seorows))->Vn));
                    }
                    else
                    {
                        fprintf(fp, "%d %d %f\n", 2 * y + z, x, fabs((spfirst + (x - 1) + y * seocolumns + z * (seocolumns * seorows))->Vn));
                        fprintf(fp, "%d %d %f\n", 2 * y + z, x, fabs((spfirst + x + y * seocolumns + z * (seocolumns * seorows))->Vn));
                    }
                }
                fprintf(fp, "\n");
                for (x = 1; x < seocolumns; x++)
                {
                    if (y == 0 && z == 0)
                    {
                        fprintf(fp, "%d %d %f\n", 2 * y + z + 1, x, fabs((spfirst + x + y * seocolumns + z * (seocolumns * seorows))->Vn));
                    }
                    else if (y == seorows - 1 && z == 1)
                    {
                        fprintf(fp, "%d %d %f\n", 2 * y + z + 1, x, fabs((spfirst + (x - 1) + y * seocolumns + z * (seocolumns * seorows))->Vn));
                    }
                    else
                    {
                        fprintf(fp, "%d %d %f\n", 2 * y + z + 1, x, fabs((spfirst + (x - 1) + y * seocolumns + z * (seocolumns * seorows))->Vn));
                        fprintf(fp, "%d %d %f\n", 2 * y + z + 1, x, fabs((spfirst + x + y * seocolumns + z * (seocolumns * seorows))->Vn));
                    }
                }
            }
            fprintf(fp, "\n");
        }
        /*
        for (y = 0; y < seorows; y++)
        {
            for (z = 0; z < seoparticles; z++)
            {
                for (x = 1; x < seocolumns; x++)
                {
                    fprintf(fp, "%d %d %f\n", 2 * y + z, x, fabs((spfirst + x + y * seocolumns + z * (seocolumns * seorows))->Vn));
                }
                fprintf(fp, "\n");
            }
        }
        */
        *pt += 0.1;
        fprintf(fp, "\n");
    }
}

// ファイルにlayer1の2次元データを読み込む(pt+=0.1)
void fprintlayrow(seo *spfirst, int seoparticles, int seorows, int seocolumns, FILE *fp, double t, double pt)
{
    if (pt <= t)
    { // 表示
        int x = 0;
        int y = 0;
        int z = 0;
        int counts = 1;
        for (x = 1; x < seocolumns + 1; x++)
        {
            for (z = 0; z < seoparticles; z++)
            {
                for (y = 1; y < seorows + 1; y++)
                {
                    if (y == 1)
                    {
                        fprintf(fp, "%d %d %f\n", counts, y, (spfirst + x + y * seocolumns + z * (seocolumns * seorows))->Vn);
                        //fprintf(fp, "x = %d y = %d z = %d\n", x, y, z);
                    }
                    else if (y == seorows)
                    {
                        fprintf(fp, "%d %d %f\n", counts, y, (spfirst + x + (y - 1) * seocolumns + z * (seocolumns * seorows))->Vn);
                        //fprintf(fp, "x = %d y = %d z = %d\n", x, y, z);
                    }
                    else
                    {
                        fprintf(fp, "%d %d %f\n", counts, y, (spfirst + x + (y - 1) * seocolumns + z * (seocolumns * seorows))->Vn);
                        fprintf(fp, "%d %d %f\n", counts, y, (spfirst + x + y * seocolumns + z * (seocolumns * seorows))->Vn);
                        //fprintf(fp, "x = %d y = %d z = %d\n", x, y, z);
                    }
                }

                fprintf(fp, "\n");
                if (x == seocolumns && z == 1)
                {
                }
                else
                {
                    for (y = 1; y < seorows + 1; y++)
                    {
                        if (y == 1)
                        {
                            fprintf(fp, "%d %d %f\n", counts + 1, y, (spfirst + x + y * seocolumns + z * (seocolumns * seorows))->Vn);
                            //fprintf(fp, "x = %d y = %d z = %d\n", x, y, z);
                        }
                        else if (y == seorows)
                        {
                            fprintf(fp, "%d %d %f\n", counts + 1, y, (spfirst + x + (y - 1) * seocolumns + z * (seocolumns * seorows))->Vn);
                            //fprintf(fp, "x = %d y = %d z = %d\n", x, y, z);
                            fprintf(fp, "\n");
                        }
                        else
                        {
                            fprintf(fp, "%d %d %f\n", counts + 1, y, (spfirst + x + (y - 1) * seocolumns + z * (seocolumns * seorows))->Vn);
                            fprintf(fp, "%d %d %f\n", counts + 1, y, (spfirst + x + y * seocolumns + z * (seocolumns * seorows))->Vn);
                            //fprintf(fp, "x = %d y = %d z = %d\n", x, y, z);
                        }
                    }
                }
                counts += 1;
            }
        }
        fprintf(fp, "\n");
    }
}
// ファイルにlayer(横)の2次元データを読み込む
void fprintlaycolumn(seo *spfirst, int seoparticles, int seorows, int seocolumns, FILE *fp, double t, double pt)
{
    if (pt <= t)
    { // 表示
        int x = 0;
        int y = 0;
        int z = 0;
        int counts = 1;

        for (x = 1; x < seocolumns + 1; x++)
        {
            for (y = 1; y < seorows + 1; y++)
            {
                for (z = 0; z < seoparticles; z++)
                {
                    if (y == 1 && z == 0)
                    {
                        fprintf(fp, "%d %d %f\n", x, counts, (spfirst + x + y * seocolumns + z * (seocolumns * seorows))->Vn);
                        // fprintf(fp, "  %d %d %d\n", x, y, z);
                    }
                    else if (y == seorows && z == 1)
                    {
                        fprintf(fp, "%d %d %f\n", x, counts, (spfirst + x + y * seocolumns + (z - 1) * (seocolumns * seorows))->Vn);
                        // fprintf(fp, "  %d %d %d\n", x, y, z - 1);
                    }
                    else
                    {
                        if (z == 0)
                        {
                            fprintf(fp, "%d %d %f\n", x, counts, (spfirst + x + (y - 1) * seocolumns + (z + 1) * (seocolumns * seorows))->Vn);
                            // fprintf(fp, "  %d %d %d\n", x, y - 1, z + 1);
                            fprintf(fp, "%d %d %f\n", x, counts, (spfirst + x + y * seocolumns + z * (seocolumns * seorows))->Vn);
                            // fprintf(fp, "  %d %d %d\n", x, y, z);
                        }
                        else if (z == 1)
                        {
                            fprintf(fp, "%d %d %f\n", x, counts, (spfirst + x + y * seocolumns + (z - 1) * (seocolumns * seorows))->Vn);
                            // fprintf(fp, "  %d %d %d\n", x, y, z - 1);
                            fprintf(fp, "%d %d %f\n", x, counts, (spfirst + x + y * seocolumns + z * (seocolumns * seorows))->Vn);
                            // fprintf(fp, "  %d %d %d\n", x, y, z);
                        }
                    }
                    counts += 1;
                }
            }
            fprintf(fp, "\n");
            counts = 1;
            for (y = 1; y < seorows + 1; y++)
            {
                for (z = 0; z < seoparticles; z++)
                {
                    if (y == 1 && z == 0)
                    {
                        fprintf(fp, "%d %d %f\n", x + 1, counts, (spfirst + x + y * seocolumns + z * (seocolumns * seorows))->Vn);
                        // fprintf(fp, "  %d %d %d\n", x, y, z);
                    }
                    else if (y == seorows && z == 1)
                    {
                        fprintf(fp, "%d %d %f\n", x + 1, counts, (spfirst + x + y * seocolumns + (z - 1) * (seocolumns * seorows))->Vn);
                        // fprintf(fp, "  %d %d %d\n", x, y, z - 1);
                    }
                    else
                    {
                        if (z == 0)
                        {
                            fprintf(fp, "%d %d %f\n", x + 1, counts, (spfirst + x + (y - 1) * seocolumns + (z + 1) * (seocolumns * seorows))->Vn);
                            // fprintf(fp, "  %d %d %d\n", x, y - 1, z + 1);
                            fprintf(fp, "%d %d %f\n", x + 1, counts, (spfirst + x + y * seocolumns + z * (seocolumns * seorows))->Vn);
                            // fprintf(fp, "  %d %d %d\n", x, y, z);
                        }
                        else if (z == 1)
                        {
                            fprintf(fp, "%d %d %f\n", x + 1, counts, (spfirst + x + y * seocolumns + (z - 1) * (seocolumns * seorows))->Vn);
                            // fprintf(fp, "  %d %d %d\n", x, y, z - 1);
                            fprintf(fp, "%d %d %f\n", x + 1, counts, (spfirst + x + y * seocolumns + z * (seocolumns * seorows))->Vn);
                            // fprintf(fp, "  %d %d %d\n", x, y, z);
                        }
                    }
                    counts += 1;
                }
            }
            counts = 1;
            fprintf(fp, "\n");
        }
        fprintf(fp, "\n");
    }
}

// ファイルに衝突判定回路の2次元データを読み込む
void fprintcollisionlay(seo *spfirst, int seorows, int seocolumns, FILE *fp, double t, double pt)
{
    if (pt <= t)
    { // 表示
        int x = 0;
        int y = 0;
        for (x = 1; x < seocolumns + 1; x++)
        {
            for (y = 1; y < seorows + 1; y++)
            {
                if (y == 1)
                {
                    fprintf(fp, "%d %d %f\n", x, y, (spfirst + x + y * seocolumns)->Vn);
                }
                else if (y == seorows)
                {
                    fprintf(fp, "%d %d %f\n", x, y, (spfirst + x + (y - 1) * seocolumns)->Vn);
                }
                else
                {
                    fprintf(fp, "%d %d %f\n", x, y, (spfirst + x + (y - 1) * seocolumns)->Vn);
                    fprintf(fp, "%d %d %f\n", x, y, (spfirst + x + y * seocolumns)->Vn);
                }
            }
            fprintf(fp, "\n");
            if (x < seocolumns + 1)
            {
                for (y = 1; y < seorows + 1; y++)
                {
                    if (y == 1)
                    {
                        fprintf(fp, "%d %d %f\n", x + 1, y, (spfirst + x + y * seocolumns)->Vn);
                    }
                    else if (y == seorows)
                    {
                        fprintf(fp, "%d %d %f\n", x + 1, y, (spfirst + x + (y - 1) * seocolumns)->Vn);
                        fprintf(fp, "\n");
                    }
                    else
                    {
                        fprintf(fp, "%d %d %f\n", x + 1, y, (spfirst + x + (y - 1) * seocolumns)->Vn);
                        fprintf(fp, "%d %d %f\n", x + 1, y, (spfirst + x + y * seocolumns)->Vn);
                    }
                }
            }
        }
        fprintf(fp, "\n");
    }
}
