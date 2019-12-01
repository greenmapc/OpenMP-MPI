#include <omp.h>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <algorithm>

using namespace std;

const int N = 30;

int main() {
	int a[N];

	for (int i = 0; i < N; i++) {
		a[i] = rand() % 100;
		printf("%d ", a[i]);
	}

	printf("\n");

	int mod9 = 0;
#pragma omp parallel for schedule(static, 3) num_threads(10)
	for (int i = 0; i < N; i++) {
		if (a[i] % 9 == 0) {
#pragma omp atomic
			mod9++;
		}
	}

	printf("%d", mod9);

	return 0;
}