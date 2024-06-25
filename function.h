#ifndef _function_h
#define _function_h
#define e 0.1602

/*--------------------------------------------------振動子----------------------------------------------------------------------*/

typedef struct
{
    double Vd;
    double Vn;
    double V1;
    double V2;
    double V3;
    double V4;
    double V5;
    double V6;
    double Q;
    double dQ;
    double dE[2];
    double wt[3];
    int tunnel;
} seo;

// 振動子のパラメータ計算(&seo,足の本数,Cs,Cjs)
void seo_Pcalc(seo *p, int leg, double Cs, double Cjs);

// 振動子のエネルギー計算(&seo,足の本数,Cs,Cjs)
void seo_Ecalc(seo *p, int leg, double Cs, double Cjs);

// 振動子の電荷チャージ(&seo,R,dt)
double seo_charge(seo *p, seo *pmax, double R, double dt);

// 振動子の電荷チャージ(&seo,SEO_ROWS,SEO_COLUMNS,R,dt)(二次元配列) *返り値無し
void seo_2dimCharge(seo *p, int rows, int columns, double R, double dt);

// 振動子の電荷チャージ(&seo,SEO_PARTICLES,SEO_ROWS,SEO_COLUMNS,R,dt)(三次元配列) *返り値なし
void seo_3dimcharge(seo *p, int particles, int rows, int columns, double R, double dt);

// 振動子の電荷チャージ(&seo,SEO_PARTICLES,SEO_ROWS,SEO_COLUMNS,R,dt)(三次元配列)
double seo_3dimCharge(seo *p, int particles, int rows, int columuns, double R, double dt);

// 振動子の電荷チャージ(&seo,R,dt) *返り値なし
void Seo_charge(seo *p, seo *pmax, double R, double dt);

// 振動子のwt計算(&seo,&seomax,Rj)
seo *seo_wt(seo *p, seo *pmax, double Rj);

// 振動子のwt計算(二次元配列)(&seo,SEO_ROWS,SEO_COLUMNS,Rj)
seo *seo_2dimwt(seo *p, int rows, int columns, double Rj);

// 振動子のwt計算(&seo,SEO_PARTICLES,SEO_ROWS,SEO_COLUMNS,Rj)(三次元配列)
seo *seo_3dimwt(seo *p, int particles, int rows, int columns, double Rj);

// 振動子のトンネル(&seo)
void seo_tunnel(seo *p);

/*--------------------------------------------------メモリ----------------------------------------------------------------------*/

typedef struct
{
    double Vd;
    double Vn;
    double V1;
    double V2;
    double V3;
    double V4;
    double V5;
    double V6;
    double dE[4];
    double wt[5];
    double WT;
    int m;
    int n;
    int tunnel;
} memori;

// メモリのパラメータ計算(&memori,足の本数,Cm,Cjm)
void memori_Pcalc(memori *p, int leg, double Cm, double Cjm);

// メモリのパラメータ計算(CL有)(&memori,足の本数,Cm,Cjm,CL)
void memori_Pcalcl(memori *p, int leg, double Cm, double Cjm, double CL);

// メモリのエネルギー計算(&memori,足の本数,Cm,Cjm)
void memori_Ecalc(memori *p, int leg, double Cm, double Cjm);

// メモリのエネルギー計算(CL有)(&memori,足の本数,Cm,Cjm,CL)
void memori_Ecalcl(memori *p, int leg, double Cm, double Cjm, double CL);

// メモリのwt計算(&memori,&memorimax,Rj)
memori *memori_wt(memori *p, memori *pmax, double Rj);

// メモリのトンネル(&memori)
void memori_tunnel(memori *p);

/*--------------------------------------------------一方通行(振動子6個)-------------------------------------------------------------*/

typedef struct
{
    seo ows[6];
    int locate;
} onewayseo;

// 一方通行のVd割り当て(&onewayseo, Vdの絶対値, 向き(0 left or 1 right),C,Cjs2,Cjs3)
void ow_seoVd(onewayseo *p, double Vd, int direction, double Cs, double Cjs2, double Cjs3);

// 一方通行のパラメータ計算(&onewayseo,Cs,Cjs,左端のVn,右端のVn)
void ow_seoPcalc(onewayseo *p, double Cs, double Cjs, double Vn0, double Vn1);

// 一方通行のパラメータ計算(足の本数でCj変更)(&onewayseo,Cs,Cjs足2,Cj足3,左端のVn,右端のVn)
void ow_seoPcalc2(onewayseo *p, double Cs, double Cjs2, double Cjs3, double Vn0, double Vn1);

// 一方通行のエネルギー計算(&onewayseo,Cs,Cjs)
void ow_seoEcalc(onewayseo *p, double Cs, double Cjs);

// 一方通行のエネルギー計算(足の本数でCj変更)(&onewayseo,Cs,Cjs足2,Cj足3)
void ow_seoEcalc2(onewayseo *p, double Cs, double Cjs2, double Cjs3);

// 一方通行の電荷チャージ(&onewayseo,&onewayseomax,R,dt) *返り値ありのseo_chargeと併用する場合dtの値に注意
void ow_seocharge(onewayseo *p, onewayseo *pmax, double R, double dt);

// 一方通行の電荷チャージ(&onewayseo,OWSEO_PARTICLES,OWSEO_ROWS,OWSEO_COLUMNS,R,dt)(三次元配列)
void ow_seo3dimcharge(onewayseo *p, int particles, int rows, int columns, double R, double dt);

// 一方通行の待ち時間計算(&onewayseo,&onewayseomax,Rj)
onewayseo *ow_seowt(onewayseo *p, onewayseo *pmax, double Rj);

// 一方通行の待ち時間計算(&onewayseo,OWSEO_PARTICLES,OWSEO_ROWS,OWSEO_COLUMNS,Rj)(三次元配列)
onewayseo *ow_seo3dimwt(onewayseo *p, int particles, int rows, int columns, double Rj);

/*--------------------------------------------------一方通行(振動子4個)-------------------------------------------------------------*/

typedef struct
{
    seo ows[4];
    int locate;
} oneway_4seo;

// 一方通行のVd割り当て(&oneway_4seo, Vdの絶対値, 向き(0 left or 1 right),C,Cjs2,Cjs3)
void oneway_4seo_setVd(oneway_4seo *p, double Vd, int direction, double Cs, double Cjs2, double Cjs3);

// 一方通行のパラメータ計算(&oneway_4seo,Cs,Cjs足2,Cj足3,左端のVn,右端のVn)
void oneway_4seo_calcPara(oneway_4seo *p, double Cs, double Cjs2, double Cjs3, double Vn0, double Vn1);

// 一方通行のエネルギー計算(&oneway_4seo,Cs,Cjs足2,Cj足3)
void oneway_4seo_calcEner(oneway_4seo *p, double Cs, double Cjs2, double Cjs3);

// 一方通行の電荷チャージ(&oneway_4seo,OWSEO_PARTICLES,OWSEO_ROWS,OWSEO_COLUMNS,R,dt)(三次元配列)
void oneway_4seo_3dimCharge(oneway_4seo *p, int particles, int rows, int columns, double R, double dt);

// 一方通行の待ち時間計算(&onewayseo,OWSEO_PARTICLES,OWSEO_ROWS,OWSEO_COLUMNS,Rj)(三次元配列)
oneway_4seo *oneway_4seo_3dimWt(oneway_4seo *p, int particles, int rows, int columns, double Rj);

/*--------------------------------------------------汎用-----------------------------------------------------------------------*/
// 0から1の間の乱数生成
double Random(void);

// 小さい方の値を返す(a,b)
double min(double a, double b);

// 現在の時間を取得する
double getTime();

// 実行時間を表示する(実行開始時間,実行終了時間)
void getRunTime(double st_time, double end_time);

// 隣接振動子からの影響を考慮したバイアス電圧(Cs,本体の足,本体のCjs,隣接振動子の足,隣接振動子のCjs)
double viasarrange(double Cs, int myleg, int yourleg, double myCjs, double yourCjs);

// 振動子と一方通行のチャージ(&s,&smax,&os,&osmax,R,&dt)
void charge(seo *p, seo *pmax, onewayseo *op, onewayseo *opmax, double R, double *dt);

// 振動子と一方通行のチャージ(&seofirst,&owseofirst,SEO_PARTICLES,SEO_ROWS,SEO_COLUMNS,OWSEO_PARTICLES,OWSEO_ROWS,OWSEO_COLUMNS,R,&dt)(三次元配列)
void seoowseo3dimcharge(seo *p, int seoparticles, int seorows, int seocolumns, onewayseo *op, int owseoparticles, int owseorows, int owseocolumns, double R, double *dt);

// 振動子のトンネル(表示無し)(sp,&t,&dt)
void tunnelseo(seo *sp, double *t, double *dt);

// 振動子のトンネル(表示有)(spfirst,sp,&t,&dt)
void tunnelprintseo(seo *spfirst, seo *sp, double *t, double *dt);

// 振動子,メモリ,一方通行のトンネル(sp,mp,osp,&t,&dt)
void tunnel(seo *sp, memori *mp, onewayseo *osp, double *t, double *dt);

// 振動子,一方通行のトンネル(sp,mp,osp,&t,&dt)
void tunnelseoowseo(seo *sp, onewayseo *osp, double *t, double *dt);

// 振動子,一方通行のトンネル(sp,mp,osp,&t,&dt)
void Seo_onway4Seo_tunnel(seo *sp, oneway_4seo *osp, double *t, double *dt);

// 振動子,メモリ,一方通行のトンネル(sp,mp,osp,&t,&dt)(print表示有)
void tunnelprint(seo *spfirst, seo *sp, memori *mpfirst, memori *mp, onewayseo *ospfirst, onewayseo *osp, double *t, double *dt);

// 振動子,一方通行のトンネル(sp,osp,&t,&dt)(print表示有)
void tunnelprintso(seo *spfirst, seo *sp, onewayseo *ospfirst, onewayseo *osp, double *t, double *dt);

// 振動子,メモリのトンネル(sp,mp,&t,&dt)(print表示有)
void tunnelprintsm(seo *spfirst, seo *sp, memori *mpfirst, memori *mp, double *t, double *dt);

// ファイルにlayer1の2次元データを読み込む(pt+=0.1)
void fprintlay1(seo *spfirst, int seoparticles, int seorows, int seocolumns, FILE *fp, double t, double *pt);

// ファイルにlayer(縦)の2次元データを読み込む
void fprintlayrow(seo *spfirst, int seoparticles, int seorows, int seocolumns, FILE *fp, double t, double pt);

// ファイルにlayer(横)の2次元データを読み込む
void fprintlaycolumn(seo *spfirst, int seoparticles, int seorows, int seocolumns, FILE *fp, double t, double pt);

// ファイルに衝突判定回路の2次元データを読み込む
void fprintcollisionlay(seo *spfirst, int seorows, int seocolumns, FILE *fp, double t, double pt);

#endif