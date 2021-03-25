#include <stdio.h>
#include <math.h>

#include <gsl/gsl_complex.h>
#include <gsl/gsl_complex_math.h>
#include <gsl/gsl_eigen.h>
#include <gsl/gsl_linalg.h>
#include <gsl/gsl_math.h>

#define L 10
#define n 200
#define RO(alfa, x) 1.0+4.0*(alfa)*(x)*(x)
#define DELTA(i, j) ((i)==(j) ? 1.0 : 0.0)
#define delX (double)(L)/((n)+1)
#define N 1

double x_i ( int i , double el, double dx );

int main(void)
{
	int alfa = 0;
	gsl_matrix *A = gsl_matrix_calloc(n, n);
	gsl_vector_complex *eval = gsl_vector_complex_calloc(n);
	gsl_matrix_complex *evec = gsl_matrix_complex_calloc(n, n);

	FILE *f = fopen("out.txt", "w");
	FILE *a0f = fopen("a0f.txt", "w");
	FILE *a100f = fopen("a100f.txt", "w");
	while(alfa <= 100)
	{
		gsl_matrix_set_zero(A);
		gsl_matrix_complex_set_zero(evec);
		gsl_vector_complex_set_zero(eval);
		for(int i=0; i<n; i++)
		{
			for(int j=0; j<n; j++)
			{
				gsl_matrix_set(A, i, j, (-DELTA(i, j+1)+2*DELTA(i, j)-DELTA(i, j-1))*N/(RO(alfa, x_i(i, L, delX))*delX*delX) );
			}
		}
		gsl_eigen_nonsymmv_workspace *w = gsl_eigen_nonsymmv_alloc(n);
		gsl_eigen_nonsymmv(A, eval, evec, w);
		gsl_eigen_nonsymmv_sort(eval, evec, GSL_EIGEN_SORT_ABS_ASC);
		gsl_eigen_nonsymmv_free(w);

		fprintf(f, "%d ", alfa);
		for(int i=0; i<6; i++)
			fprintf(f, "%f ", sqrt(GSL_REAL(gsl_vector_complex_get(eval, i))));
		fprintf(f, "\n");

		if(alfa == 0)
		{
			for(int i=0; i<n; i++)
			{
				fprintf(a0f, "%f ", x_i(i, L, delX));
				for(int j=0; j<6; j++)
				{
					fprintf(a0f, "%f ", GSL_REAL(gsl_matrix_complex_get(evec, i, j)));
				}
				fprintf(a0f, "\n");
			}
		}
		if(alfa == 100)
		{
			for(int i=0; i<n; i++)
			{
				fprintf(a100f, "%f ", x_i(i, L, delX));
				for(int j=0; j<6; j++)
				{
					fprintf(a100f, "%f ", GSL_REAL(gsl_matrix_complex_get(evec, i, j)));
				}
				fprintf(a100f, "\n");
			}
		}

		alfa += 2;
	}
	/*for(int i=0; i<n; i++)
	{
		for(int j=0; j<n; j++)
		{
			printf("%f ", gsl_matrix_get(A, i, j));
		}
		printf("\n");
	}*/
	gsl_matrix_free(A);
	gsl_matrix_complex_free(evec);
	gsl_vector_complex_free(eval);
	fclose(f);
	fclose(a0f);
	fclose(a100f);
	return 0;
}

double x_i ( int i , double el, double dx )
{
	return -el/2.0 + dx * (i+1);
}