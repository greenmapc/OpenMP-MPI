#include <omp.h>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <algorithm>

using namespace std;

const int N = 12;

int main() {
	int a[N];
	int b[N];
	int c[N];

#pragma omp parallel for schedule(static, 4) num_threads(3)
	for (int i = 0; i < N; i++) {
		a[i] = rand() % 100;
		b[i] = rand() % 100;
		printf("Thread %d\nSet a[%d] = %d, b[%d] = %d\n", omp_get_thread_num(), i, a[i], i, b[i]);
	}
	printf("\n");
	printf("parallel section 2 : \n");
	printf("\n");
#pragma omp parallel for schedule(dynamic, 3) num_threads(4)
	for (int i = 0; i < N; i++) {
		c[i] = a[i] + b[i];
		printf("Thread %d\nSet c[%d] = %d\n", omp_get_thread_num(), i, c[i]);
	}
	return 0;
}