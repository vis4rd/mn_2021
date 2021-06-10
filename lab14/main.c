#include <stdio.h>
#include <math.h>

//funkcje do rozkladu jednorodnego
long homo_dist(long prev_x, long a, long c, long m);
float norm_hd(long hd, long m);

float average(float *values, const unsigned n);
float std_deviation(float *values, const unsigned n, const float avg);

void calc_intervals(float xmin, float xmax, unsigned k, float *x, const unsigned n, unsigned *returned);

//funkcje do rozkladu normalnego
float prob_density(float sigma, float y, float e_val);
float cumul_distr(float x, float e_val, float sigma);
float test_stat(unsigned k, const unsigned n, unsigned *nj, float *x, float eval, float sigma, float xmin, float xmax);

//numerical recipes
float erff(float x);
float gammp(float a, float x);

int main(void)
{

	//rozklad jednorodny
	const unsigned N = 10e4;
	long X1[N]; X1[0] = 10;
	long X2[N]; X2[0] = 10;
	float x1[N], x2[N]; //znormalizowane

	for(int i = 1; i < N; i++)
	{
		X1[i] = homo_dist(X1[i-1], 123, 1, pow(2, 15));
		X2[i] = homo_dist(X2[i-1], 69069, 1, pow(2, 32));
		x1[i] = norm_hd(X1[i], pow(2, 15));
		x2[i] = norm_hd(X2[i], pow(2, 32));
	}
	
	FILE *file1 = fopen("U.dat", "w");
	for(unsigned i = 0; i < N-1; i++)
		fprintf(file1, "%f %f\n", x1[i], x1[i+1]);
	fprintf(file1, "\n\n");
	for(unsigned i = 0; i < N-1; i++)
		fprintf(file1, "%f %f\n", x2[i], x2[i+1]);
	fclose(file1);

	// for(int i = 1; i <= 5; i++)
	// 	printf("%d %f\n", i, x1[i]);
	float avg1 = average(x1, N);
	float sdev1 = std_deviation(x1, N, avg1);
	printf("avg1 = %f\n", avg1);
	printf("std_dev1 = %f\n", sdev1);
	printf("\n");

	// for(int i = 1; i <= 5; i++)
	// 	printf("%d %f\n", i, x2[i]);
	float avg2 = average(x1, N);
	float sdev2 = std_deviation(x2, N, avg2);
	printf("avg2 = %f\n", avg2);
	printf("std_dev2 = %f\n", sdev2);
	printf("\n");

	float xmin = 0.f;
	float xmax = 1.f;
	unsigned k = 12;

	unsigned intervals1[k];
	unsigned intervals2[k];
	calc_intervals(xmin, xmax, k, x1, N, intervals1);
	calc_intervals(xmin, xmax, k, x2, N, intervals2);

	float dk = (xmax - xmin) / (float)k;
	FILE *file2 = fopen("U_hist.dat", "w");
	for(unsigned i = 0; i < k; i++)
		fprintf(file2, "%f %f\n", ((xmin + (i+1)*dk) + (xmin +i*dk))/2.f, intervals1[i]/(float)N);
	fprintf(file2, "\n\n");
	for(unsigned i = 0; i < k; i++)
		fprintf(file2, "%f %f\n", ((xmin + (i+1)*dk) + (xmin +i*dk))/2.f, intervals2[i]/(float)N);
	fclose(file2);

	// for(unsigned i = 0; i < k; i++)
	// 	printf("%u\n", intervals1[i]);
	// printf("\n");
	// for(unsigned i = 0; i < k; i++)
	// 	printf("%u\n", intervals2[i]);
	// printf("\n");

	//rozklad normalny

	const float eval = 0.2f;
	const float sigma = 0.5f;

	float xnmin = eval - 3*sigma;
	float xnmax = eval + 3*sigma;
	float xn[N];
	long U1, U2, U_start = 10;
	float u1, u2;
	for(unsigned i = 1; i < N; i++)
	{
		do
		{
			U1 = homo_dist(U_start, 69069, 1, pow(2, 32));
			U2 = homo_dist(U1, 69069, 1, pow(2, 32));
			U_start = U2;
			u1 = norm_hd(U1, pow(2, 32)) * (xnmax - xnmin) + xnmin;
			u2 = norm_hd(U2, pow(2, 32));
		}while(u2 > prob_density(sigma, u1, eval));
		xn[i] = u1;
	}

	float avgn = average(xn, N);
	float sdevn = std_deviation(xn, N, avgn);
	printf("avgn = %f\n", avgn);
	printf("std_devn = %f\n", sdevn);
	printf("\n");

	unsigned kn = 12;
	unsigned intervalsn[kn];
	calc_intervals(xnmin, xnmax, kn, xn, N, intervalsn);

	float dkn = (xnmax - xnmin) / (float)kn;
	FILE *file3 = fopen("N_hist.dat", "w");
	for(unsigned i = 0; i < kn; i++)
		fprintf(file3, "%f %f\n", ((xnmin + (i+1)*dkn) + (xnmin +i*dkn))/2.f, intervalsn[i]/(float)N);
	fclose(file3);

	float st = test_stat(kn, N, intervalsn, xn, eval, sigma, xnmin, xnmax);
	printf("test_stat = %f\n\n", st);

	printf("testowanie hipotezy:\n");
	printf("wartosc tablicowa dla istotnosci alfa = 0.05:\n");
	int vi = kn - 2 - 1;
	printf("vi = %d\n", vi);
	printf("test_stat(tabl) = 16.91\n\n");
	float eps = 16.91f;
	if(st < eps)
		printf("%f < %f => hipoteza potwierdzona!\nn", st, eps);
	else
		printf("%f </ %f => hipoteza odrzucona!\nn", st ,eps);

	//jesli hipoteza jest odrzucona, to moze to wynikac 
	//  z niedokladnosci typow zmiennoprzecinkowych

	float trust_indicator = gammp(vi/2.f, st/2.f);
	float significance_indicator = 1 - trust_indicator;
	printf("poziom_ufnosci = %f\n", trust_indicator);
	printf("poziom_istotnosci = %f\n", significance_indicator);

	return 0;
}

/////////////////////////
//////// funkcje ////////
/////////////////////////

long homo_dist(long prev_x, long a, long c, long m)
{
	return (a * prev_x + c) % m;
}

float norm_hd(long hd, long m)
{
	return hd / (m + 1.f);
}

float average(float *values, const unsigned n)
{
	float sum = 0.f;
	for(unsigned i = 0; i < n; i++)
		sum += values[i];
	return sum/(float)n;
}

float std_deviation(float *values, const unsigned n, const float avg)
{
	float sum = 0.f;
	for(unsigned i = 0; i < n; i++)
		sum += pow(values[i] - avg, 2);
	return sqrt(sum / n);
}

void calc_intervals(float xmin, float xmax, unsigned k, float *x, const unsigned n, unsigned *returned)
{
	for(unsigned i = 0; i < k; i++)
		returned[i] = 0;
	float dk = (xmax - xmin) / (float)k; //odleglosc miedzy przedzialami
	for(unsigned i = 0; i < n; i++)
	{
		int j1 = (x[i] - xmin) / dk;
		returned[j1]++;
	}
	return;
}

float prob_density(float sigma, float y, float e_val)
{
	return (1.f / (sigma*sqrt(2*M_PI)) * 
			exp(-(pow(y-e_val, 2) / (2.f * sigma * sigma))));
}

float cumul_distr(float x, float e_val, float sigma)
{
	return ( 1.f + 
			erff((x-e_val) / (sigma * sqrt(2.f))) ) /
			 2.f;
}

float test_stat(unsigned k,
				const unsigned n,
				unsigned *nj,
				float *x,
				float eval,
				float sigma,
				float xmin,
				float xmax)
{
	float dk = (xmax - xmin) / (float)k;
	float result = 0.f;
	float pj = 0.f;
	for(unsigned j = 0; j < k - 1; j++)
	{
		pj = cumul_distr(xmin + dk*(j+1), eval, sigma) - 
			cumul_distr(xmin + dk*j, eval, sigma);
		//printf("pj%u = %f\n", j, pj);
		//printf("nj - N*pj = %f\n", nj[j] - n * pj);
		result += (pow(nj[j] - (n * pj), 2) / (n*pj));
	}
	return result;
}