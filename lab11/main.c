#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>

#include "nrutil.h"

#pragma GCC diagnostic ignored "-Wimplicit-function-declaration"

double randD (double a, double b);
double fun(float T, float ti);
double gun(float sigma, float ti);

int main(void)
{
    srand(time(NULL));
    int N;
    float T, tMax, deltaT, sigma, *f, *g1, *g2;
    float ti;
    float a1, b1, a2, b2;
    char *files[3] = {"k8.dat", "k10.dat", "k12.dat"};

    for(int k = 8; k <= 12; k += 2)
    {
        FILE *file = fopen(files[(k-8)/2], "w");
        N = pow(2, k);
        T = 1.0;
        tMax = 3*T;
        deltaT = tMax/N;
        sigma = T/20;

        f = vector(1, 2*N);
        //fstart = vector(1, 2*N);
        g1 = vector(1, 2*N);
        g2 = vector(1, 2*N);

        for(int i = 1; i <= N; i++)
        {
            f[i] = 0;
            //fstart[i] = 0.f;
            g1[i] = 0.f;
            g2[i] = 0.f;
        }

        for(int i = 1; i <= N; i++)
        {
            ti = deltaT * (i-1);
            f[2*i - 1] = fun(T, ti) + randD(-0.5, 0.5);
            //fstart[2*i - 1] = fun(T, ti) + randD(-0.5, 0.5);

            g1[2*i - 1] = gun(sigma, ti);
            g2[2*i - 1] = gun(sigma, ti);
            fprintf(file, "%f %f\n", ti, f[2*i - 1]);
        }

        four1(f, N, 1);
        four1(g1, N, 1);
        four1(g2, N, 1);

        for(int i = 1; i <= N; i++)
        {
            a1 = f[2*i - 1];
            b1 = f[2*i];
            a2 = g1[2*i - 1] + g2[2*i - 1];
            b2 = g1[2*i] + g2[2*i];
            f[2*i - 1] = a1*a2 - b1*b2;
            f[2*i] = a1*b2 + a2*b1;
        }

        four1(f, N, -1);

        float maxval = fabs(f[1]);
        for(int i = 1; i <= N; i++)
            maxval = (fabs(maxval) > fabs(f[2*i - 1])) ? fabs(maxval) : fabs(f[2*i - 1]);


        fprintf(file, "\n\n");
        for(int i = 1; i <= N; i++)
        {
            ti = deltaT * (i-1);
            fprintf(file, "%f %f\n", ti-0.03f, f[2*i - 1]/maxval*2.5f);
        }

        free_vector(f, 1, 2*N);
        //free_vector(fstart, 1, 2*N);
        free_vector(g1, 1, 2*N);
        free_vector(g2, 1, 2*N);
        fclose(file);
    }
    return 0;
}

double randD (double a, double b)
{
 	return (a + (rand()/(1.0 * RAND_MAX)) *(b-a));
}

double fun(float T, float ti)
{
    return sin((2*M_PI/T) * ti) + sin(2 * (2*M_PI) * ti) + sin(3 * (2*M_PI/T) * ti);
}

double gun(float sigma, float ti)
{
    return (1.f / (sigma * sqrt(2*M_PI))) * exp(-(ti*ti) / (2*sigma*sigma));
}