#include <stdio.h>
#include <math.h>

#include "nrutil.h"

#pragma GCC diagnostic ignored "-Wimplicit-function-declaration"

void wyzM(float *xw,float *yw, float *m, int n, float alfa, float beta);
float wyzSx(float *xw,float *yw, float *m, int n, float x);
double f1x(double x);
double f2x(double x);

int main(void)
{
    const int n = 10; //5, 8, 21
    float xmin = -5.f;
    float xmax = 5.f;
    float h = (xmax - xmin) / (n - 1.f);

    float *xw = vector(1, n);
    float *yw = vector(1, n);
    float *m = vector(1, n);

    for(int i = 1; i <= n; i++)
    {
        xw[i] = xmin + h * (i-1);
        yw[i] = f1x(xw[i]);
        //yw[i] = f2x(xw[i]);
    }

    wyzM(xw, yw, m, n, 0.f, 0.f); //alfa = beta = 0

    FILE *f = fopen("preout.txt", "w");
    for(int i = 1; i <= n; i++)
    {
        fprintf(f, "%f %f %f\n", xw[i], m[i], (double)(f1x(xw[i]-0.01f) - 2*f1x(xw[i]) + f1x(xw[i]+0.01f))/0.0001);
        //fprintf(f, "%f %f %f %f\n", xw[i], m[i], (double)(f2x(xw[i]-0.01f) - 2*f2x(xw[i]) + f2x(xw[i]+0.01f))/0.0001, f2x(xw[i]));
    }
    fclose(f);

    FILE *analytic = fopen("anf1.txt", "w");
    for(float x = -5.0; x < 5.0; x+=0.01)
        fprintf(analytic, "%f %f\n", x, (double)(f1x(x-0.01) - 2*f1x(x) + f1x(x+0.01))/0.0001);
    fclose(analytic);

    // FILE *analytic = fopen("f2-analytic21.txt", "w");
    // for(float x = -5.0; x < 5.0; x+=0.01)
    //     fprintf(analytic, "%f %f %f\n", x, f1x(x), wyzSx(xw, yw, m, n, x));
    // fclose(analytic);

    free_vector(xw, 1, n);
    free_vector(yw, 1, n);
    free_vector(m, 1, n);
    return 0;
}

void wyzM(float *xw,float *yw, float *m, int n, float alfa, float beta)
{
    float h = (xw[n] - xw[1])/(n-1);
    float lambda = 0.5f;
    float mi = 0.5f;
    float **A = matrix(1, n, 1, n);
    float **d = matrix(1, n, 1, 1);

    for(int i = 1; i <= n; i++)
    {
        for(int j = 1; j <= n; j++)
        {
            A[i][j] = 0.f;
            if(i == (j-1))
                A[i][j] = lambda;
            if(i == j)
                A[i][j] = 2.f;
            if(i == (j+1))
                A[i][j] = mi;
        }
        d[i][1] = (6.f/(2*h))*((yw[i+1]-yw[i])/h - (yw[i]-yw[i-1])/h);
    }
    A[1][1] = A[n][n] = 1.f;
    A[1][2] = A[n][n-1] = 0.f;
    d[1][1] = alfa;
    d[n][1] = beta;

    gaussj(A, n, d, 1);
    free_matrix(A, 1, n, 1, n);

    for(int i = 1; i <= n; i++)
        m[i] = d[i][1];
    
    free_matrix(d, 1, n, 1, 1);
}

float wyzSx(float *xw,float *yw, float *m, int n, float x)
{
    float sx = 0.f;
    float h = (xw[n] - xw[1]) / (n-1);

    for(int i = 2; i <= n; i++)
    {
        if(x >= xw[i-1] && x <= xw[i])
        {
            float A = (yw[i] - yw[i-1])/h - h * (m[i] - m[i-1]) / 6.f;
            float B = yw[i-1] - (m[i-1] * h * h) / 6.f;
            sx =(m[i-1] * pow(xw[i] - x, 3) + m[i] * pow(x - xw[i-1], 3)) 
                / (6 * h) + A*(x - xw[i-1]) + B;
        }
    }
    return sx;
}

double f1x(double x)
{
    return (double)(1.f / (1.f + x * x));
}

double f2x(double x)
{
    return cos(2*x);
}