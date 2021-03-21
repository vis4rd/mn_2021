#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define V0 0
#define x0 1
#define omega 1
#define n 2000
#define h 0.02

#define a1 1
#define a2 (omega*omega*h*h) - 2 - (beta*h)
#define a3 1 + beta*h

double *solution (const double beta, const double F0, const double OM);

int main(void)
{
    double *wynik1 = solution(0.0, 0.0, 0.8);
    double *wynik2 = solution(0.4, 0.0, 0.8);
    double *wynik3 = solution(0.4, 0.1, 0.8);

    FILE *f1 = fopen("out1.txt", "w");
    FILE *f2 = fopen("out2.txt", "w");
    FILE *f3 = fopen("out3.txt", "w");

    for(int i=0; i<n+1; i++)
    {
        fprintf(f1, "%f %f\n", h*i, wynik1[i]);
        fprintf(f2, "%f %f\n", h*i, wynik2[i]);
        fprintf(f3, "%f %f\n", h*i, wynik3[i]);
    }

    fclose(f1);
    fclose(f2);
    fclose(f3);

    free(wynik1-2);
    free(wynik2-2);
    free(wynik3-2);
    return 0;
}

double *solution(const double beta, const double F0, const double OM)
{
    double vecB[n+1];
    double d0[n+1];
    double d1[n+1];
    double d2[n+1];

    vecB[0]=1.0; vecB[1]=0.0;
    d0[0]=1.0; d0[1]=1.0;
    d1[0]=0.0; d1[1]=-1.0;
    d2[0]=0.0; d2[1]=0.0;
    for(int i=2; i<n+1; i++)
    {
        vecB[i] = F0*sin(OM*h*i)*h*h;
        d0[i] = a3;
        d1[i] = a2;
        d2[i] = a1;
    }

    double *temp = (double *)calloc(n+3, sizeof(double));
    double *vecX = temp+2;
    double *temp2 = (double *)calloc(n+3, sizeof(double));
    double *vecXs = temp2+2;
    vecX[-2] = 99.0;
    vecX[-1] = 101.0;
    vecXs[-2] = 99.0;
    vecXs[-1] = 101.0;
    vecXs[0] = 0.0;

    int it = 0;
    double sn = 0.0;
    double ss = 0.0;
    while(it < 1e05)
    {
        it++;
        sn = 0.0;
        ss = 0.0;
        for(int i=0; i<n+1; i++)
        {
            ss += vecXs[i]*vecXs[i];
            vecX[i] = (1.0/d0[i])*(vecB[i]-d1[i]*vecXs[i-1] - d2[i]*vecXs[i-2]);
            sn += vecX[i]*vecX[i];
        }
        for(int i=0; i<n+1; i++)
            vecXs[i] = vecX[i];
        
        //printf("it = %d, fabs(sn-ss) = %g\n", it, fabs(sn-ss));

        if(fabs(sn-ss) < 1e-06)
           break;
    }
    printf("it = %d", it);
    printf("\n");
    
    
    free(vecXs-2);
    return vecX;
}