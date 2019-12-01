#include <omp.h>
#include <stdio.h>

int main() {
	omp_set_num_threads(3);

#pragma omp parallel if(omp_get_max_threads() > 2)
	{
		printf("Hello from thread %d, threads num %d\n", omp_get_thread_num(), omp_get_num_threads());
	}

	omp_set_num_threads(2);

#pragma omp parallel if(omp_get_max_threads() > 2)
	{
		printf("Hello from thread %d, threads num %d\n", omp_get_thread_num(), omp_get_num_threads());
	}
}