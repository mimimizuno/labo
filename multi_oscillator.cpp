/*多重振動子一次元配列*/
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#define e 0.1602 /*電荷素量*/
#define T 300    /*シミュレーション終了時間*/
#define N 50     /*多重振動子接合数*/
#define X 10     /*振動子数*/

double wait_time(double Rj, double dE)/*待ち時間計算関数(トンネル接合抵抗, エネルギー変化)*/
{
    double r = (double)rand()/RAND_MAX;
    double wt = (pow(e,2) * Rj / dE) * log(1/r);
    return wt;
}
/*構造体宣言*/
struct oscillator
{
    double Vd[X];
    double Vn[X];
    double Q[X];
    double qsum[X];
    double R;
    double Cj;
    double Rj;
    double dEup[X];
    double dEdown[X];
    double wt;
};

struct joint
{
    double V0[X];
    double V1[X];
    double C;
    double q0[X];
    double q1[X];
};

int main()
{
    /*初期パラメータ入力*/
    /*****************************************/
    struct oscillator osc;
    osc.R = 1;
    osc.Rj = 0.002;
    osc.Cj = 500;//多重の接合数を考慮して設定
    osc.wt = 0;
    FILE *fp;

    
    fp = fopen("multi.txt", "w");
    if (fp == NULL)
    {
        printf("file open error.\n");
        return -1;
    }

    for(int i=0;i<X;i++)
    {
        if(i%2 == 0)
        {
            osc.Vd[i] = 0.0075;
        }
        else
        {
            osc.Vd[i] = -0.0075;
        }
        osc.Vn[i]     = 0;
        osc.Q[i]      = 0;
        osc.qsum[i]   = 0;
        osc.dEup[i]   = 0;
        osc.dEdown[i] = 0;
    }
    /*****************************************/
    struct joint jo;
    jo.C = 2;
    for(int i=0;i<X;i++)
    {
        jo.V0[i] = 0;
        jo.V1[i] = 0;
        jo.q0[i] = 0;
        jo.q1[i] = 0;
    }
    /*****************************************/

    double Vth = 0;/*1回目トンネル閾値*/
    double t   = 0;
    double dt  = 0.1;
    double WT  = 0;/*最小待ち時間記録用*/
    double Vt  = 0;/*トリガ電圧*/
    int L[X]   = {0};/*トンネル回数*/
    int I      = 0;
    int up     = 0;
    int down   = 0;
    int i      = 0;
    int j      = 0;
    int rep    = 0;/*くり返し用*/
    srand((unsigned)time(NULL));

    Vth = N*(jo.C*(N-1)+osc.Cj)*e / (2*osc.Cj*(N*jo.C + osc.Cj));

    while(t <= T)
    {
        fprintf(fp, "%f %f %f %f %f %f %f %f %f %f %f %f %f\n",t,osc.Vn[0]*1000,osc.Vn[1]*1000,osc.Vn[2]*1000,osc.Vn[3]*1000,osc.Vn[4]*1000,osc.Vn[5]*1000,osc.Vn[6]*1000,osc.Vn[7]*1000,osc.Vn[8]*1000,osc.Vn[9]*1000);

        if(t>150 && t<160)
        {
            Vt = 0.006;
        }
        else
        {
            Vt = 0;
        }

        for(rep=0;rep<5;rep++)/*偏りが出ないように５回繰り返し*/
        {
            for(i=0;i<X;i++)
            {
                if(i==0)//片側トリガ、片側振動子
                {
                    jo.V0[i]  = Vt;
                    jo.V1[i]  = osc.Vn[i+1];
                }
                else if(i == X-1)//片側振動子
                {
                    jo.V0[i]  = osc.Vn[i-1];
                }
                else//両側振動子
                {
                    jo.V0[i]  = osc.Vn[i-1];
                    jo.V1[i]  = osc.Vn[i+1];
                }
                osc.qsum[i]   = -N*(-osc.Cj*osc.Q[i] - jo.C*osc.Cj*(jo.V0[i] + jo.V1[i]))/(2*N*jo.C + osc.Cj) - L[i]*osc.Cj*e/(2*N*jo.C + osc.Cj);//トンネル接合の電荷の足し合わせ
                osc.Vn[i]     = osc.qsum[i] / osc.Cj;//ノード電圧はq/Cjの足し合わせなのでこのように計算できる
            }
        }

        for(i=0;i<X;i++)//トンネル回数に応じたエネルギー変化の式
        {
            osc.dEup[i]   = e * ((-(N-1)*2 + 2*2*L[i])*jo.C*e - osc.Cj*e + 2*osc.Q[i]*osc.Cj + 2*jo.C*osc.Cj*(jo.V0[i] + jo.V1[i])) / (2*osc.Cj*(2*N*jo.C + osc.Cj));
            osc.dEdown[i] = -e * (-(-(N-1)*2 - 2*2*L[i])*jo.C*e + osc.Cj*e + 2*osc.Q[i]*osc.Cj + 2*jo.C*osc.Cj*(jo.V0[i] + jo.V1[i])) / (2*osc.Cj*(2*N*jo.C + osc.Cj));
        }

        for(i=0;i<X;i++)
        {
            if(osc.dEup[i] > 0)
            {
                for(rep=0;rep<N-L[i];rep++)//同じdEを使ってまだトンネルしていないトンネル接合の数だけ待ち時間を計算する。同じdEでも乱数によって異なる待ち時間になる
                {
                    osc.wt = wait_time(osc.Rj, osc.dEup[i]);
                    if(osc.wt<dt && osc.wt<WT)
                    {
                        up   = 1;
                        down = 0;
                        I    = i;
                        WT   = osc.wt;
                    }
                }
            }
            
            if(osc.dEdown[i] > 0)
            {
                for(rep=0;rep<N+L[i];rep++)
                {
                    osc.wt = wait_time(osc.Rj, osc.dEdown[i]);
                    if(osc.wt<dt && osc.wt<WT)
                    {
                        up   = 0;
                        down = 1;
                        I    = i;
                        WT   = osc.wt;
                    }
                }
            }
        }

        if(up == 1)
        {
            for(i=0;i<X;i++)
            {
                osc.Q[i] += (osc.Vd[i] - osc.Vn[i]) * WT / osc.R;
            }
            L[I] += 1;
            if(L[I] == N)//多重の数だけトンネルしたら回数を初期化する。急に回数を初期化すると計算結果がおかしな値になるのでノードの電荷を-eする。すべてのトンネルで電子トンネルするとちょうど電荷が-eされた値と同等であるため
            {
                L[I] = 0;
                osc.Q[I] -= e;
            }
            t     += WT;
            up = 0;
        }
        else if(down == 1)
        {
            for(i=0;i<X;i++)
            {
                osc.Q[i] += (osc.Vd[i] - osc.Vn[i]) * WT / osc.R;
            }
            L[I] -= 1;
            if(L[I] == -N)
            {
                L[I] = 0;
                osc.Q[I] += e;//逆も同じ
            }
            t     += WT;
            down = 0;
        }
        else
        {
            for(i=0;i<X;i++)
            {
                osc.Q[i] += (osc.Vd[i] - osc.Vn[i]) * dt / osc.R;
            }
            t     += dt;
        }

        up = 0;
        down = 0;
        I = 0;
        WT = 100;
    }
    return 0;
}
// 単品
// #include <stdio.h>
// #include <math.h>
// #include <stdlib.h>
// #include <time.h>

// #define NUMBER_OF_JUNCTION 10
// #define START_OF_TRIGER 1000
// #define END_OF_TRIGER 1005

// int main(){
//   double Cj = 100.0; //[aF]
//   double C = 2.0;//[aF]
//   double R = 10.0; //[G]
//   double Rj = 0.001;//[G]
//   double Vd = 0.0074; //[V]
//   double V1 = 0.0;//[V]
//   double Vin = 0.005;//[V]
//   double Vn = 0.0;  //[V]
//   double Q = 0.0; 
//   double t = 0.0; //[ns]
//   double dt = 0.1, dq;
//   double e = 0.1602,r;
//   int L = 0;
//   double q_sum = 0.0;
//   double dEup = 0.0;
//   double dEdown = 0.0;
//   int up = 0;
//   int down = 0;
//   double wt = 0;
//   int i = 0;
//   double wt_min = 1000.0;


  
//   srand((unsigned)time(NULL));
// 	while(t <= 2000.0){


//         if(t > START_OF_TRIGER && t < END_OF_TRIGER){
//             V1 = Vin;
//         }else{
//             V1 = 0;
//         }

//         q_sum = (-NUMBER_OF_JUNCTION*(-Cj*Q-C*Cj*V1)-Cj*L*e)/(NUMBER_OF_JUNCTION*C+Cj);
//         Vn = q_sum/Cj;
//         printf("%f %f\n",t,Vn);

//         dEup = e*((-(NUMBER_OF_JUNCTION-1)*1+2*1*L)*C*e+Cj*(2*Q-e)+2*C*Cj*V1)/(2*Cj*(NUMBER_OF_JUNCTION*C+Cj));
//         dEdown = -e*(-(-(NUMBER_OF_JUNCTION-1)*1-2*1*L)*C*e+Cj*(2*Q+e)+2*C*Cj*V1)/(2*Cj*(NUMBER_OF_JUNCTION*C+Cj));

//         if(dEup > 0){
//             for(i = 0; i < NUMBER_OF_JUNCTION-L; i++){
//                 r = (double)rand()/RAND_MAX;
//                 wt = (e*e*Rj/dEup)*log(1/r);
                
//                 if(wt < dt && wt < wt_min){
//                     up = 1;
//                     down = 0;
//                     wt_min = wt;
//                 }
//             }
//         }
//         if(dEdown > 0){
//             for(i = 0; i < NUMBER_OF_JUNCTION; i++){
//                 r=(double)rand()/RAND_MAX;
//                 wt = (e*e*Rj/dEdown)*log(1/r);

//                 if(wt < dt && wt < wt_min){
//                     up = 0;
//                     down = 1;
//                     wt_min = wt;
//                 }
//             }
//         }

//         if(up == 1){
//             Q += (Vd-Vn)*wt/R;
//             L += 1;

//             if(L == NUMBER_OF_JUNCTION){
//                 L = 0;
//                 Q -= e;
//             }
//             t += wt_min;
//             up = 0;
//             printf("%f %f\n",t,Vn);
//         }else if(down == 1){
//             Q += (Vd-Vn)*wt/R;
//             L -= 1;
//             if(L == -NUMBER_OF_JUNCTION){
//                 L = 0;
//                 Q += e;
//             }
//             t += wt_min;
//             down = 0;
//             printf("%f %f\n",t,Vn);
//         }else{
//             Q += (Vd-Vn)*dt/R;
//             t += dt;
//         }
        
//         up = 0;
//         down = 0;
//         wt = 1000.0;
//         wt_min = 1000.0;

// 	}
// 	return 0;
// }
