#include <omp.h>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <algorithm>

using namespace std;

const int N = 10;

void printArray(int arr[]) {
	for (int i = 0; i < sizeof(arr); i++) {
		printf("%d ", arr[i]);
	}

	printf("\n");
}

int main() {
	int a[N];
	int b[N];

	for (int i = 0; i < N; i++) {
		a[i] = rand() % 100;
		b[i] = rand() % 100	;
	}
	
	printf("Array a : ");
	printArray(a);
	
	printf("Array b : ");
	printArray(b);

	int minValue = INT16_MAX;
	int maxValue = 0;
#pragma omp parallel num_threads(2) 
	{
		int threadNumber = omp_get_thread_num();
	#pragma omp master
		for (int i = 0; i < N; i++) {
			printf("for1 thread %d\n", threadNumber);
			minValue = min(minValue, a[i]);
		}

		if (threadNumber != 0) 
			for (int i = 0; i < N; i++) {
				printf("for2 thread %d\n", threadNumber);
				maxValue = max(maxValue, b[i]);
			}
	}

	printf("min of a : %d\n", minValue);
	printf("max of b : %d\n", maxValue);

	return 0;
}