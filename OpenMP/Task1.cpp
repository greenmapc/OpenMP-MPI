#include <omp.h>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <algorithm>
#include <chrono>

using namespace std;

int main() {
#pragma omp parallel num_threads(8)
	{
		printf("Hello world! from thread %d\n", omp_get_thread_num());
	}
	
	return 0;
}