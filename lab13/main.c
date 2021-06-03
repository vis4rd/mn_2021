#include <stdio.h>
#include <math.h>

#include "nrutil.h"

//#pragma GCC diagnostic ignored "-Wimplicit-function-declaration"

#define PI 3.14159265
#define maxN 100

void exercise1(char);
void exercise2(char);
void exercise3(char);
void printv(float *vector, long start, long stop);
float f1(float x);
float f2(float x);
float f3(float x);

void gauleg(float x1, float x2, float x[], float w[], int n);
void gaulag(float x[], float w[], int n, float alf);
void gauher(float x[], float w[], int n);

int main(void)
{
    exercise1(1);//Gauss - Legandre
	printf("\n");
    exercise2(1);//Gauss - Hermit + Gauss - Legandre
	printf("\n");
    exercise3(1);//Gauss - Laguere
    return 0;
}

void exercise1(char print)
{
    //Gauss - Legandre
	for(unsigned n = 2; n <= maxN; n++)
	{
		float xmin = 1.f;
		float xmax = 2.f;
		float *x = vector(1, n);
		float *w = vector(1, n);

		gauleg(xmin, xmax, x, w, n);
		float c_1a = PI/3.f;
		float c1 = 0.f;
		for(int i = 1; i <= n; i++)
			c1 += w[i] * f1(x[i]);
		
		float e = fabsf(c1 - c_1a);
		if(print)
			printf("%-4d %-7f\n", n, e);

		free_vector(x, 1, n);
		free_vector(w, 1, n);
	}
}

void exercise2(char print)
{
    //Gauss - Hermit
	float xmin = 0.f;
	float xmax = 5.f;
	float c_2a = -0.8700577f;
	for(int n = 2; n <= maxN; n+=2)
	{
		float *x = vector(1, n);
		float *w = vector(1, n);

		gauher(x, w, n);
		
		float c2 = 0.f;
		for(int i = 1; i <= n; i++)
			c2 += w[i] * (log(fabsf(x[i]))/2.f);
		float e = fabsf(c2 - c_2a);
		if(print)
			printf("%-4d %-7f\n", n, e);

		free_vector(x, 1, n);
		free_vector(w, 1, n);
	}
	printf("\n");

	//Gauss - Legandre
	for(int n = 2; n <= maxN; n++)
	{
		float *x = vector(1, n);
		float *w = vector(1, n);

		gauleg(xmin, xmax, x, w, n);

		float c2 = 0.f;
		for(int i = 1; i <= n; i++)
			c2 += w[i] * f2(x[i]);
		float e = fabsf(c2 - c_2a);
		if(print)
			printf("%-4d %-7f\n", n, e);

		free_vector(x, 1, n);
		free_vector(w, 1, n);
	}
}

void exercise3(char print)
{
    //Gauss - Laguere
	for(int n = 2; n <= 10; n++)
	{
		float *x = vector(1, n);
		float *w = vector(1, n);

		gaulag(x, w, n, 0.f);

		float c_3a = 2.f/13.f;
		float c3 = 0.f;
		for(int i = 1; i <= n; i++)
			c3 += w[i] * f3(x[i]);
		float e = fabsf(c3 - c_3a);
		if(print)
			printf("%-4d %-7f\n", n, e);

		free_vector(x, 1, n);
		free_vector(w, 1, n);
	}
}

void printv(float *vector, long start, long stop)
{
    printf("[ ");
    for(long i = start; i <= stop; i++)
    {
        printf("%f ", vector[i]);
    }
    printf("]\n");
}

float f1(float x)
{
    return 1.f/(x*sqrt(x*x - 1.f));
}

float f2(float x)
{
    return log(x)*exp(-x*x);
}

float f3(float x)
{
    return sin(2*x)*exp(-2*x);
}
