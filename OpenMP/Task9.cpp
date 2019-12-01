#include <omp.h>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <algorithm>

using namespace std;

const int N = 500;

int main() {
	int a[N][N];
	int b[N];
	int resultStatic[N];
	int resultParallel[N];

	for (int i = 0; i < N; i++) {
		b[i] = i + 2;
		for (int j = 0; j < N; j++) {
			a[i][j] = (i + 1) * (j + 1);
		}
	}


	double start = omp_get_wtime();
	for (int i = 0; i < N; i++) {
		int cur = 0;
		for (int j = 0; j < N; j++) {
			cur += a[i][j] * b[j];
		}
		resultStatic[i] = cur;
	}
	double end = omp_get_wtime();
	printf("Time = %f\n", end - start);

	start = omp_get_wtime();
#pragma omp parallel for schedule(guided, 30) num_threads(3)
	for (int i = 0; i < N; i++) {
		int cur = 0;
		for (int j = 0; j < N; j++) {
			cur += a[i][j] * b[j];
		}
		resultParallel[i] = cur;
	}
	end = omp_get_wtime();
	printf("Time = %f\n", end - start);

	return 0;
}