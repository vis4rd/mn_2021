#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "nrutil.h"

#define SIZE 3

void printM(float **matrix, int size);
float norm(float **matrix, int size);
float **mmult(float **m, float **n, int size);
 
int main(void)
{
    float **A = matrix(1, SIZE, 1, SIZE);
    float **Ar = matrix(1, SIZE, 1, SIZE);
     float **B = matrix(1, SIZE, 1, SIZE);
    float **Br = matrix(1, SIZE, 1, SIZE);
    for(int i=1; i<=SIZE; i++)
    {
        for(int j=1; j<=SIZE; j++)
        {
            A[i][j] = 3*(i-1) + j;
            Ar[i][j] = 3*(i-1) + j;
            B[i][j] = 3*(i-1) + j;
            Br[i][j] = 3*(i-1) + j;
        }
    }
    B[1][1] = 1.1f;
    Br[1][1] = 1.1f;
    printM(A, SIZE);

    int *vecA = ivector(1, SIZE);
    int *vecB = ivector(1, SIZE);
    float permA = 0.f;
    float permB = 0.f;

    ludcmp(A, SIZE, vecA, &permA);
    ludcmp(B, SIZE, vecB, &permB);

    printf("\n");
    printM(A, SIZE);

    float **b = matrix(1, SIZE, 1, SIZE);
    float **b2 = matrix(1, SIZE, 1, SIZE);
    for(int i=1; i<=SIZE; i++)
    {
        for(int j=1; j<=SIZE; j++)
        {
            if(i==j)
            {
                b[i][j] = 1.f;
                b2[i][j] = 1.f;
            }
            else
            {
                b[i][j] = 0.f;
                b2[i][j] = 0.f;
            }
        }
    }

    for(int i=1; i<=SIZE; i++)
        lubksb(A, SIZE, vecA, b[i]);

    for(int i=1; i<=SIZE; i++)
        lubksb(B, SIZE, vecB, b2[i]);

    printf("\n");
    printM(b, SIZE);
    printM(b2, SIZE);

    float normA = norm(Ar, SIZE);
    float normAmin1 = norm(b, SIZE);
    printf("normA = %g\n", normA);
    printf("normAmin1 = %g\n", normAmin1);

    float condp = normA * normAmin1;
    printf("condp = %g\n", condp);

    float normB = norm(Br, SIZE);
    float normBmin1 = norm(b2, SIZE);
    printf("normB = %g\n", normB);
    printf("normBmin1 = %g\n", normBmin1);

    float condp2 = normB * normBmin1;
    printf("condp = %g\n", condp2);

    float **rm1 = mmult(Ar, b, SIZE);
    float **rm2 = mmult(Br, b2, SIZE);

    printM(rm1, SIZE);
    printM(rm2, SIZE);

    FILE *f = fopen("out.txt", "w");
    if(NULL == f)
        exit(1);

    fprintf(f, " condp = %g\n", condp);
    fprintf(f, " condp2 = %g\n\n", condp2);
    for(int i=1; i<=SIZE; i++)
    {
        for(int j=1; j<=SIZE; j++)
        {
            fprintf(f, "%g ", rm1[i][j]);
        }
        fprintf(f, "\n");
    }
    fprintf(f, "\n");
    for(int i=1; i<=SIZE; i++)
    {
        for(int j=1; j<=SIZE; j++)
        {
            fprintf(f, "%g ", rm2[i][j]);
        }
        fprintf(f, "\n");
    }

    fclose(f);

    free_matrix(A, 1, SIZE, 1, SIZE);
    free_matrix(Ar, 1, SIZE, 1, SIZE);
    free_ivector(vecA, 1, SIZE);
    free_matrix(b, 1, SIZE, 1, SIZE);
    free_matrix(rm1, 1, SIZE, 1, SIZE);

    free_matrix(B, 1, SIZE, 1, SIZE);
    free_matrix(Br, 1, SIZE, 1, SIZE);
    free_ivector(vecB, 1, SIZE);
    free_matrix(b2, 1, SIZE, 1, SIZE);
    free_matrix(rm2, 1, SIZE, 1, SIZE);

    return 0;
}

void printM(float **matrix, int size)
{
    printf("matrix = [\n");
    for(int i=1; i<=size; i++)
    {
        for(int j=1; j<=size; j++)
        {
            printf("%g ", matrix[i][j]);
        }
        printf("\n");
    }
    printf("]\n");
    return;
}

float norm(float **matrix, int size)
{
    float max = 0.f;
    for(int i=1; i<=size; i++)
    {
        for(int j=1; j<=size; j++)
        {
            if(fabs(matrix[i][j]) > max)
            max = fabs(matrix[i][j]);
        }
    }
    return max;
}

float **mmult(float **m, float **n, int size)
{
    float **result = matrix(1, size, 1, size);

    for(int a = 1; a <=size; a++)
    {
        for(int b = 1; b <= size; b++)
        {
            result[a][b] = 0.f;
            for(int c = 1; c <= size; c++)
                result[a][b] += m[a][c]*n[c][b];
        }
    }
    return result;
}