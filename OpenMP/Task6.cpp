#include <omp.h>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <algorithm>

using namespace std;

const int N = 100;

int main() {
	int a[N];

	for (int i = 0; i < N; i++) {
		a[i] = rand() % 100 + 100;
		printf("%d ", a[i]);
	}

	printf("\n");
	
	int avg = 0;
	int avgReduction = 0;

#pragma omp parallel for
	for (int i = 0; i < N; i++) {
		printf("from thread %d\n avg = %d\n", omp_get_thread_num(), avg);
		avg += a[i];
	}

#pragma omp parallel for reduction(+ : avgReduction)
	for (int i = 0; i < N; i++) {
		printf("from thread %d\n avg = %d\n", omp_get_thread_num(), avg);
		avgReduction += a[i];
	}

	avg /= N;
	avgReduction /= N;
	printf("AVG %d\n", avg);
	printf("AVG REDUCTION %d\n", avgReduction);
	
	return 0;
}