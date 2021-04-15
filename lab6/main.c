#include <stdio.h>
#include <math.h>

#define N 5
#define IT_MAX 30

double licz_r(double a[N+1], double b[N+1], int n, double x0);

int main(void)
{
    double a[N+1] = {240.0, -196.0, -92.0, 33.0, 14.0, 1.0};
    double b[N+1];
    double c[N];
    double x0 = 0.0, x1;
    double rj, rjp;
    FILE *f = fopen("out.txt", "w");
    
    int L = 1;
    for(; L <= N; L++)
    {
        x0 = 0.0;
        const int n = N - L + 1;
        int it = 1;
        while(it <= IT_MAX)
        {
            rj = licz_r(a, b, n, x0);
            rjp = licz_r(b, c, n-1, x0);
            x1 = x0 - rj/rjp;

            if(fabs(x0-x1) < 1e-07)
                break;
            
            x0 = x1;
            fprintf(f, "%-3d %-3d %-10f %-12g %-10g\n", L, it, x0, rj, rjp);
            it++;
        }
        fprintf(f, "%-3d %-3d %-10g %-12g %-10g\n\n", L, it, x0, rj, rjp);
        printf("x%d = %f\n", L, x0);
        for(int i = 0; i < n; i++)
            a[i] = b[i];
    }

    fclose(f);
    return 0;
}

double licz_r(double a[N+1], double b[N+1], int n, double x0)
{
    b[n] = 0.0;
    for(int k = n-1; k >= 0; k--)
        b[k] = a[k+1] + x0 * b[k+1];
    
    return a[0]+x0*b[0];
}