#include <omp.h>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <algorithm>

using namespace std;

const int N = 6;
const int M = 8;

int main() {
	int a[N][M];

	for (int i = 0; i < N; i++) {
		for (int j = 0; j < M; j++) {
			a[i][j] = rand() % 100;
			printf("%d ", a[i][j]);
		}
		printf("\n");
	}
	
	int minValue = INT_MAX;
	int avg = 0;
	int maxValue = 0;
	int countMod3 = 0;

#pragma omp parallel sections num_threads(3)
	{
#pragma omp section
		{
			printf("AVG section: thread %d\n", omp_get_thread_num());
			for (int i = 0; i < N; i++)
				for (int j = 0; j < M; j++) {
					avg += a[i][j];
				}
			avg /= M * N;
		}

#pragma omp section
		{
			printf("MIN MAX seуction: thread %d\n", omp_get_thread_num());
			for (int i = 0; i < N; i++)
				for (int j = 0; j < M; j++) {
					minValue = min(minValue, a[i][j]);
					maxValue = max(maxValue, a[i][j]);
				}
		}

#pragma omp section 
		{
			printf("mod 3 section: thread %d\n", omp_get_thread_num());
			for (int i = 0; i < N; i++)
				for (int j = 0; j < M; j++) {
					if (a[i][j] % 3 == 0)
						countMod3++;
				}
		}
	}


	printf("AVG %d\n", avg);
	printf("MIN %d\n", minValue);
	printf("MAX %d\n", maxValue);
	printf("MOD 3 %d\n", countMod3);

	return 0;
}