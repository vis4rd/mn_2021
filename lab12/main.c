#include <stdio.h>
#include <math.h>

#define A 0
#define B 1

float f(float x);
float h_simp(float a, float b, unsigned w);
float h_milne(float a, float b, unsigned w);
float x(float a, unsigned i, float h);

void simpson(unsigned SIZE);
void milne(unsigned SIZE);

int main(void)
{
    simpson(8);
    milne(8);
    return 0;
}

void simpson(unsigned SIZE)
{
    float D[SIZE+1][SIZE+1];
    unsigned N;
    for(int w = 0; w < SIZE+1; w++)
    {
        D[w][0] = 0.f;
        N = pow(2, w+1);
        for(int i = 0; i <= (N/2)-1; i++)
        {
            float h = h_simp(A, B, w);
            D[w][0] += (h/3.f) * 
                    ( f(x(A, 2*i, h)) + 
                    4.f * f(x(A, 2*i+1, h)) + 
                    f(x(A, 2*i+2, h)));
        }
    }
    for(int w = 1; w < SIZE+1; w++)
    {
        for(int k = 1; k <= w; k++)
        {
            D[w][k] = (pow(4, k)*D[w][k-1]-D[w-1][k-1]) / ((pow(4, k)-1));
        }
    }

    FILE *f = fopen("simpson.dat", "w");
    for(int w = 0; w < SIZE+1; w++)
    {
        printf("D[%d][0] = %f | D[%d][%d] = %f\n", w, D[w][0], w, w, D[w][w]);
        fprintf(f, "D[%d][0] = %f | D[%d][%d] = %f\n", w, D[w][0], w, w, D[w][w]);
    }
    printf("\n");
    fprintf(f, "\n");
    fclose(f);
}

void milne(unsigned SIZE)
{
    float D[SIZE+1][SIZE+1];
    unsigned N;
    for(int w = 0; w < SIZE+1; w++)
    {
        D[w][0] = 0.f;
        N = pow(2, w+2);
        for(int i = 0; i <= (N/4)-1; i++)
        {
            float h = h_milne(A, B, w);
            D[w][0] += (4*h/90.f) * 
                ( 7.f * f(x(A, 4*i, h)) + 
                  32.f * f(x(A, 4*i+1, h)) + 
                  12.f * f(x(A, 4*i+2, h)) +
                  32.f * f(x(A, 4*i+3, h)) +
                  7.f * f(x(A, 4*i+4, h)));
        }
    }
    for(int w = 1; w < SIZE+1; w++)
    {
        for(int k = 1; k <= w; k++)
        {
            D[w][k] = (pow(4, k)*D[w][k-1]-D[w-1][k-1]) / ((pow(4, k)-1));
        }
    }

    FILE *f = fopen("milne.dat", "w");
    for(int w = 0; w < SIZE+1; w++)
    {
        printf("D[%d][0] = %f | D[%d][%d] = %f\n", w, D[w][0], w, w, D[w][w]);
        fprintf(f, "D[%d][0] = %f | D[%d][%d] = %f\n", w, D[w][0], w, w, D[w][w]);
    }
    printf("\n");
    fprintf(f, "\n");
    fclose(f);
}

float f(float x)
{
    return (log(x*x*x + 3*x*x + x + 0.1)*sin(18*x));
}

float h_simp(float a, float b, unsigned w)
{
    return (float)(b-a)/pow(2, w+1);
}

float h_milne(float a, float b, unsigned w)
{
    return (float)(b-a)/pow(2, w+2);
}

float x(float a, unsigned i, float h)
{
    return A + i*h;
}