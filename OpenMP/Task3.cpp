#include <omp.h>
#include <stdio.h>

int main() {
	int a = 10;
	int b = 20;

	printf("Before parallel area 1: a = %d, b = %d\n", a, b);

#pragma omp parallel num_threads(2) private(a) firstprivate(b)
	{
		a = 10;
		int threadNumber = omp_get_thread_num();
		a += threadNumber;
		b += threadNumber;

		printf("Inside parallel area 1: a = %d, b = %d, thread number = %d\n", a, b, threadNumber);
	}
	printf("After parallel area 1: a = %d, b = %d\n", a, b);

	
	printf("Before parallel area 2: a = %d, b = %d\n", a, b);
	
#pragma omp parallel num_threads(4) private(b)
	{
		b = 20;
		int threadNumber = omp_get_thread_num();
		a -= threadNumber;
		b -= threadNumber;

		printf("Inside parallel area 2: a = %d, b = %d, thread number = %d\n", a, b, threadNumber);
	}
	printf("Before parallel area 2: a = %d, b = %d\n", a, b);

	return 0;
}