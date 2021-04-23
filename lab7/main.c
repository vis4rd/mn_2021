#include <stdio.h>
#include <math.h>

void licz(int SIZE);
double fx(double x);
double wnx(double x, const int n, double xm[n+1], double fd[n+1]);
double x_equidistant(double xmin, double xmax, int n, int i);
double x_chebyshev(double xmin, double xmax, int n, int i);
void printv(double v[], int n);
void printm(int n, double m[n][n]);

int main(void)
{
    for(int i = 5; i <= 20; i+=5)
        licz(i);
    return 0;
}

void licz(int SIZE)
{
    double xm[SIZE+1];
    double ym[SIZE+1];
    double fm[SIZE][SIZE+1];
    double fd[SIZE+1];

    for(int i = 0; i <= SIZE; i++)
        for(int j = 0; j <= SIZE; j++)
            fm[i][j] = 0.0;

    for(int i = 0; i <= SIZE; i++)
        //xm[i] = x_equidistant(-5.0, 5.0, SIZE, i);
        xm[i] = x_chebyshev(-5.0, 5.0, SIZE, i);
    printv(xm, SIZE+1);
    for(int i = 0; i <= SIZE; i++)
        ym[i] = fx(xm[i]);
    printv(ym, SIZE+1);

    for(int i = 0; i <= SIZE; i++)
        fm[i][0] = ym[i];

    for(int j = 1; j <= SIZE; j++)
        for(int i = j; i <= SIZE; i++)
            fm[i][j] = (fm[i][j-1]-fm[i-1][j-1])/(xm[i]-xm[i-j]);
    printm(SIZE+1, fm);
    
    for(int i = 0; i <= SIZE; i++)
        fd[i] = fm[i][i];

    FILE *f;
    if(SIZE == 5) f = fopen("out5.txt", "w");
    if(SIZE == 10) f = fopen("out10.txt", "w");
    if(SIZE == 15) f = fopen("out15.txt", "w");
    if(SIZE == 20) f = fopen("out20.txt", "w");
    double result;
    for(double g = -5.0; g <= 5.0; g+=0.01)
    {
        result = wnx(g, SIZE, xm, fd);
        //printf("%f %g\n", g, result);
        fprintf(f, "%f %g\n", g, result);
    }
    fclose(f);
    return;
}

double fx(double x)
{
    return (1.0/(1.0+x*x));
}

double wnx(double x, const int n, double xm[n+1], double fd[n+1])
{
    double sum = 0.0;
    double mult = 1.0;
    for(int j = 0; j <= n; j++)
    {
        mult = 1.0;
        for(int i = 0; i <= j-1; i++)
            mult *= (x - xm[i]);
        sum += fd[j] * mult;
    }
    return sum;
}

double x_equidistant(double xmin, double xmax, int n, int i)
{
    double h = (double)(xmax - xmin) / n;
    return xmin + i*h;
}

double x_chebyshev(double xmin, double xmax, int n, int i)
{
    return 0.5*((xmin - xmax)*cos(M_PI*(2*i + 1)/(2*n + 2))+(xmin + xmax));
}

void printv(double v[], int n)
{
    for(int i = 0; i < n; i++)
        printf("%d %lf\n", i, v[i]);
    printf("\n");
    return;
}

void printm(int n, double m[n][n])
{
    for(int i = 0; i < n; i++)
    {
        for(int j = 0; j < n; j++)
            printf("%12g ", m[i][j]);
        printf("\n");
    }
    printf("\n");
    return;
}