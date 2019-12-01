#include <omp.h>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <algorithm>

using namespace std;

const int N = 16000;

int main() {
	int a[N];
	double b[N];


	for (int i = 0; i < N; i++) {
		a[i] = i + 1;
	}

	double start = omp_get_wtime();
#pragma omp parallel for schedule(static, 200) num_threads(8)
	for (int i = 1; i < N - 1; i++) {
		b[i] = (a[i - 1] + a[i] + a[i + 1]) / 3.0;
	}
	double end = omp_get_wtime();
	printf("Time = %f\n", end - start);

	start = omp_get_wtime();
#pragma omp parallel for schedule(dynamic, 8) num_threads(8)
	for (int i = 1; i < N - 1; i++) {
		b[i] = (a[i - 1] + a[i] + a[i + 1]) / 3.0;
	}
	end = omp_get_wtime();
	printf("Time = %f\n", end - start);

	start = omp_get_wtime();
#pragma omp parallel for schedule(guided, 8) num_threads(8)
	for (int i = 1; i < N - 1; i++) {
		b[i] = (a[i - 1] + a[i] + a[i + 1]) / 3.0;
	}
	end = start = omp_get_wtime();
	printf("Time = %f\n", end - start);

	start = omp_get_wtime();
#pragma omp parallel for schedule(runtime) num_threads(8)
	for (int i = 1; i < N - 1; i++) {
		b[i] = (a[i - 1] + a[i] + a[i + 1]) / 3.0;
	}
	end = omp_get_wtime();
	printf("Time = %f\n", end - start);



	return 0;
}