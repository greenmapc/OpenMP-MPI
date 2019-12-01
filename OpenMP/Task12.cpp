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
		a[i] = rand() % 100 + 1;
		printf("%d ", a[i]);
	}

	printf("\n");

	int maxMod7 = -1;
#pragma omp parallel for schedule(dynamic, 3)  num_threads(10)
	for (int i = 0; i < N; i++) {
		if (a[i] % 7 == 0 && a[i] > maxMod7) {
#pragma omp critical
			if (a[i] > maxMod7) {
				maxMod7 = a[i];
			}
		}
	}

	if (maxMod7 == -1) {
		printf("нет таких");
	}
	else {
		printf("%d", maxMod7);
	}

	return 0;
}