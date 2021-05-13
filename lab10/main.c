#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#define N 200
#define x_min -10
#define x_max 10
#define y_min -10
#define y_max 10
#define x_0 5
#define y_0 5
#define steps 100

double f(double x, double y);
double T(double it);
double randxy(double start, double end);

int main(void)
{
    srand(time(NULL));

    FILE *w0 = fopen("w0.dat", "w");
    FILE *Tf = fopen("T.dat", "w");

    double x[N];
    double y[N];
    for(int i = 0; i < N; i++)
    {
        x[i] = x_0;
        y[i] = y_0;
    }
    
    double Temp;
    double dx, dy;
    for(int it = 0; it <= 20; it++)
    {
        Temp=T(it);
        for(int k = 0; k < steps; k++)
        {
            for(int i = 0; i < N; i++)
            {
                dx = randxy(-1, 1);
                dy = randxy(-1, 1);
                if(f(x[i]+dx, y[i]+dy) < f(x[i], y[i]))
                {
                    x[i] = x[i] + dx;
                    y[i] = y[i] + dy;
                    if(x[i] > x_max) x[i] = x_max;
                    if(x[i] < x_min) x[i] = x_min;
                    if(y[i] > y_max) y[i] = y_max;
                    if(y[i] < y_min) y[i] = y_min;
                }
                else if(randxy(0, 1) < exp(-((f(x[i]+dx, y[i]+dy) - f(x[i], y[i]))/Temp)))
                {
                    x[i] = x[i] + dx;
                    y[i] = y[i] + dy;
                    if(x[i] > x_max) x[i] = x_max;
                    if(x[i] < x_min) x[i] = x_min;
                    if(y[i] > y_max) y[i] = y_max;
                    if(y[i] < y_min) y[i] = y_min;
                }
            } 
            fprintf(w0, "%f\n", f(x[0], y[0]));
        }
        if(it == 0 || it == 7 || it == 20)
        {
            for(int p = 0; p < N; p++)
                fprintf(Tf, "%f %f\n", x[p], y[p]);
            fprintf(Tf, "\n\n");
        }
    }

    double fmin=999.0, fx, fy;
    for(int i = 0; i < N; i++)
    {
        if(f(x[i], y[i]) < fmin)
        {
            fmin = f(x[i], y[i]);
            fx = x[i];
            fy = y[i];
        }
    }
    printf("f(%f, %f) = %f\n", fx, fy, fmin);
    fclose(w0);
    fclose(Tf);
    return 0;
}

double f(double x, double y)
{
    return sin(x)*sin(y)-exp(-pow(x+(M_PI/2), 2) - pow(y-(M_PI/2), 2));
}

double T(double it)
{
    return (10/pow(2, it));
}

double randxy(double start, double end)
{
    return (double) rand() / RAND_MAX * (end - start) + start;
}