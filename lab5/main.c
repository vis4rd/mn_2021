#include <stdio.h>
#include <stdlib.h>
#include <math.h>

float **matAlloc(int n1, int n2);
float *vecAlloc(int n);
float *matvecMult(float **a, float *b, int n);
float **matmatMult(float **a, float **b, int n);
float **valmatMult(float value, float **a, int n);
float **matCopy(float **src, int n);
float **matSubtr(float **a, float **b, int n);
void matFill(float **a, int n, float value);
void matFree(float **a, int n);
float **matTrans(float **a, int n);
float vecvecMult(float *a, float *b, int n);
float **vecvecTensor(float *a, float *b, int n);
float vecNorm(float *a, int n);
float *vecvalDiv(float *a, float b, int n);
float *vecCopy(float *src, int n);
void fprintV(FILE *f, float *a, int n);
void fprintM(FILE *f, float **a, int n);
void printV(float *a, int n);
void printM(float **a, int n);

float **calcW(float *x, float lambda, float **Wprev, int n);
float **calcD(float **x, float **A, int n);

int main(void)
{
	//stale
    const int n = 7;
    const int itMAX = 12;
    const int Kval = n;

    //zmienne
    float **A = matAlloc(n, n);
    float **W;
    float **x = matAlloc(n, n);
    matFill(x, n, 1.f);
    float **x2 = calloc(n, sizeof(float *));
    float *lambda = vecAlloc(n);
    float **D;

    FILE *f = fopen("out.txt", "w");

    for(int i = 0; i < n; i++)
        for(int j = 0; j < n; j++)
            A[i][j] = 1/(sqrt(2 + fabs((float)i-j)));

    printf("matrix A:\n");
    printM(A, n);

    W = matCopy(A, n); //inicjalizacja macierzy iterujacej

    for(int k = 0; k < Kval; k++)
    {
    	fprintf(f, "lambda%d = ", k);
    	for(int i = 1; i <= itMAX; i++)
    	{
    		x2[k] = matvecMult(W, x[k], n);
    		lambda[k] = vecvecMult(x2[k], x[k], n) / vecvecMult(x[k], x[k], n);
    		free(x[k]);
    		x[k] = vecvalDiv(x2[k], vecNorm(x2[k], n), n);

    		fprintf(f, "%f ", lambda[k]);

    		if(k == 0)
    			printf("lambda%d = %f\n", i, lambda[k]);
    		if(k == 1 && i == 1)
    			printf("lambda1-1 = %f\n", lambda[k]);
    	}//petla iterujaca
    	fprintf(f, "\n"); //enter po wypisaniu lambdy

    	//wyznaczanie W
    	W = calcW(x[k], lambda[k], W, n);
    }//petla wektorow

    fprintf(f, "matrix X = \n");
	fprintM(f, x, n);

	//wyznaczanie D
   	D = calcD(x, A, n);

	fprintf(f, "matrix D = \n");
	fprintM(f, D, n);

	printf("matrix D = \n");
	printM(D, n);

    fclose(f);
    matFree(A, n);
    matFree(W, n);
    matFree(x, n);
    matFree(x2, n);
    free(lambda);
    matFree(D, n);
    return 0;
}

float **matAlloc(int n1, int n2)
{
    float **result = calloc(n1, sizeof(float *));
    for(int i = 0; i < n1; i++)
        result[i] = calloc(n2, sizeof(float));

    for(int i = 0; i < n1; i++)
        for(int j = 0; j < n2; j++)
            result[i][j] = 0.f;
    return result;
}

float *vecAlloc(int n)
{
	float *result = calloc(n, sizeof(float));
	for(int i = 0; i < n; i++)
		result[i] = 0.f;
	return result;
}

float *matvecMult(float **a, float *b, int n)
{
	float *result = vecAlloc(n);
    for(int i = 0; i < n; i++)
    {
        result[i] = 0.f;
        for(int j = 0; j < n; j++)
            result[i] += ( a[i][j] * b[j] );
    }
    return result;
}

float **matmatMult(float **a, float **b, int n)
{
	float **result = matAlloc(n, n);
    for(int i = 0; i < n; i++)
    {
        for(int j = 0; j < n; j++)
        {
            result[i][j] = 0.f;
            for(int k = 0; k < n; k++)
                result[i][j] += (a[i][k]*b[k][j]);
        }
    }
    return result;
}

float **valmatMult(float value, float **a, int n)
{
	float **result = matAlloc(n, n);
	for(int i = 0; i < n; i++)
		for(int j = 0; j < n; j++)
			result[i][j] = value*a[i][j];
	return result;
}

float **matCopy(float **src, int n)
{
	float **result = matAlloc(n, n);
    for(int i = 0; i < n; i++)
        for(int j = 0; j < n; j++)
            result[i][j] = src[i][j];
    return result;
}

float **matSubtr(float **a, float **b, int n)
{
	float **result = matAlloc(n, n);
    for(int i = 0; i < n; i++)
        for(int j = 0; j < n; j++)
            result[i][j] = a[i][j] - b[i][j];
    return result;
}

void matFill(float **a, int n, float value)
{
	for(int i = 0; i < n; i++)
		for(int j = 0; j < n; j++)
			a[i][j] = value;
	return;
}

void matFree(float **a, int n)
{
	for(int i = 0; i < n; i++)
		free(a[i]);
	free(a);
	return;
}

float **matTrans(float **a, int n)
{
	float **result = matAlloc(n, n);
	for(int i = 0; i < n; i++)
		for(int j = 0; j < n; j++)
			result[j][i] = a[i][j];
	return result;
}

float vecvecMult(float *a, float *b, int n)
{
    float result = 0.f;
    for(int i = 0; i < n; i++)
        result += (a[i]*b[i]);
    return result;
}

float **vecvecTensor(float *a, float *b, int n)
{
	float **result = matAlloc(n, n);
    for(int i = 0; i < n; i++)
        for(int j = 0; j < n; j++)
            result[i][j] = a[i]*b[j];
    return result;
}

float vecNorm(float *a, int n)
{
	// Euclidean norm
    float result = 0.f;
    for(int i = 0; i < n; i++)
        result += (a[i] * a[i]);
    return sqrt(result);
}

float *vecvalDiv(float *a, float b, int n)
{
	float *result = vecAlloc(n);
	for(int i = 0; i < n; i++)
		result[i] = a[i] / b;
	return result;
}

float *vecCopy(float *src, int n)
{
	float *result = vecAlloc(n);
	for(int i = 0; i < n; i++)
		result[i] = src[i];
	return result;
}

void fprintV(FILE *f, float *a, int n)
{
    for(int i = 0; i < n; i++)
        fprintf(f, "%f ", a[i]);
    fprintf(f, "\n");
    return;
}

void fprintM(FILE *f, float **a, int n)
{
	for(int i = 0; i < n; i++)
	{
		for(int j = 0; j < n; j++)
			fprintf(f, "%g ", a[i][j]);
		fprintf(f, "\n");
	}
	fprintf(f, "\n");
	return;
}

void printV(float *a, int n)
{
	for(int i = 0; i < n; i++)
		printf("%f ", a[i]);
	printf("\n");
	return;
}

void printM(float **a, int n)
{
	for(int i = 0; i < n; i++)
	{
		for(int j = 0; j < n; j++)
			printf("%g ", a[i][j]);
		printf("\n");
	}
	printf("\n");
	return;
}

float **calcW(float *x, float lambda, float **Wprev, int n)
{
	float **xxt = vecvecTensor(x, x, n);
	float **lamxxt = valmatMult(lambda, xxt, n);
	float **W = matSubtr(Wprev, lamxxt, n);

	matFree(xxt, n);
	matFree(lamxxt, n);
	matFree(Wprev, n);
	return W;
}

float **calcD(float **x, float **A, int n)
{
	float **xttemp = matTrans(x, n);
	float **dtemp = matmatMult(x, A, n);
	float **D = matmatMult(dtemp, xttemp, n);
	matFree(xttemp, n);
	matFree(dtemp, n);
	return D;
}