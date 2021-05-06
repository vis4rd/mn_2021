#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#define N 201
#define x_min -4.0
#define x_max 4.0
#define x_0 2.0
#define sigma ((x_max - x_min) / 16)

#define frand() ((double)rand())/(RAND_MAX+1.0)
#define Crand(Y) (((Y)-0.5)/5.0)
#define f(x) ( sin(14*M_PI*(x))/(x_max-x_min) * \
            (exp(-pow((x)-x_0, 2)/(2*sigma*sigma)) + \
            exp(-pow((x)+x_0, 2)/(2*sigma*sigma))) )

double calcAlpha(double phi[N], double x[N]);
double calcBeta(double old_phi[N], double phi[N], double x[N]);
double calcC(double phi[N], double y[N]);
double calcS(double phi[N]);

int main(void)
{
    srand(time(NULL));

    double x[N] = {0.0};
    double y[N] = {0.0};
    double phi[51][N] = {0.0};

    double h = (x_max - x_min) / (N - 1);
    for(int i = 0; i < N - 1; i++)
    {
        x[i] = x_min + i*h;
        y[i] = f(x[i]) + Crand(frand());
    }
    x[N-1] = x_max;
    y[N-1] = f(x[N-1]) + Crand(frand());

    FILE *wezly = fopen("wezly.txt", "w");
    for(int i = 0; i < N; i++)
        fprintf(wezly, "%d %g\n", i, x[i]);
    fclose(wezly);

    for(int i = 0; i < N; i++)
        phi[0][i] = 1.0;

    double alpha = calcAlpha(phi[0], x);
    for(int i = 0; i < N; i++)
        phi[1][i] = phi[0][i] * (x[i] - alpha);

    double beta;
    for(int i = 2; i < 51; i++)
    {
        alpha = calcAlpha(phi[i-1], x);
        beta = calcBeta(phi[i-2], phi[i-1], x);
        printf("%d  %g  %g\n", i-1, alpha, beta);
        for(int j = 0; j < N; j++)
            phi[i][j] = ((x[j] - alpha) * phi[i-1][j]) - (beta * phi[i-2][j]);
    }

    FILE *gram = fopen("gram.txt", "w");
    for(int i = 0; i < N; i++)
    {
        fprintf(gram, "%g ", x[i]);
        for(int j = 0; j < 7; j++)
            fprintf(gram, "%g ", phi[j][i]/phi[j][0]);
        fprintf(gram, "\n");
    }
    fclose(gram);

    double c[51] = {0.0};
    double s[51] = {0.0};
    for(int i = 0; i < 51; i++)
    {
        c[i] = calcC(phi[i], y);
        s[i] = calcS(phi[i]);
        //printf("%d %g\n", i, s[i]);
    }

    FILE *appr = fopen("approximation.txt", "w");
    int cases[3] = {10, 30, 50};
    double F[3][N] = {0.0};
    double result;
    for(int p = 0; p < 3; p++)
    {
        for(int i = 0; i < N; i++)
        {
            result = 0.0;
            for(int j = 0; j < cases[p]; j++)
                result += phi[j][i]*c[j] / s[j];

            F[p][i] = result;
            fprintf(appr, "%g %g\n", x[i], result);
        }
        fprintf(appr, "\n");
    }
    fclose(appr);

    return 0;
}

double calcAlpha(double phi[N], double x[N])
{
    double num, den;
    num = den = 0.0;
    for(int i = 0; i < N; i++)
    {
        num += x[i] * phi[i] * phi[i];
        den += phi[i] * phi[i];
    }
    return num/den;
}

double calcBeta(double old_phi[N], double phi[N], double x[N])
{
    double num, den;
    num = den = 0.0;
    for(int i = 0; i < N; i++)
    {
        num += x[i] * old_phi[i] * phi[i];
        den += old_phi[i] * old_phi[i];
    }
    return num/den;
}

double calcC(double phi[N], double y[N])
{
    double result = 0.0;
    for(int i = 0; i < N; i++)
        result += y[i]*phi[i];
    
    return result;
}

double calcS(double phi[N])
{
    double result = 0.0;
    for(int i = 0; i < N; i++)
        result += phi[i]*phi[i];

    return result;
}