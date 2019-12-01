#include <omp.h>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <algorithm>
#include <chrono>

using namespace std;

const int N = 6;
const int M = 8;

int main() {
	int a[N][M];

	for (int i = 0; i < N; i++) {
		for (int j = 0; j < M; j++) {
			a[i][j] = rand() % 100;
			cout << a[i][j] << " ";
		}
		cout << endl;
	}

	cout << endl;

	int max = 0;
	int min = INT_MAX;

#pragma omp parallel for schedule(runtime)
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < M; j++) {
			if (a[i][j] > max) {
#pragma omp critical
				if (a[i][j] > max) {
					max = a[i][j];
				}
			}
			if (a[i][j] < min) {
#pragma omp critical
				if (a[i][j] < min) {
					min = a[i][j];
				}
			}
		}
	}

	printf("MIN = %d\n", min);
	printf("MAX = %d\n", max);

	return 0;
}