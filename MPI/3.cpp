#include<iostream>
#include <stdio.h> 
#include <stdlib.h> 
#include<mpi.h>
#include<algorithm>

using namespace std;

double circle(double x) {
	double y = 1 - x * x;
	return y;
}

int main(int argc, char* argv[])
{
	const int N = 32;
	int rank, size;
	double x[N]; //точка i с координатой x a[i]
	double y[N]; //точка i с координатой y a[i]

	int globalInCircle;

	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	int elements_for_process = N / size;
	int localInCircle;
	
	if (rank == 0) {
		for (int i = 0; i < N; i++) {
			x[i] = rand() / (RAND_MAX + 1.0);
			y[i] = rand() / (RAND_MAX + 1.0);

			cout << "Dot " << i << ": " << x[i] << " " << y[i] << endl;
		}
	}

	MPI_Bcast(&x, N, MPI_DOUBLE, 0, MPI_COMM_WORLD);
	MPI_Bcast(&y, N, MPI_DOUBLE, 0, MPI_COMM_WORLD);

	int begin = elements_for_process * rank;
	int end = elements_for_process * (rank + 1);
	
	localInCircle = 0;
	for (int i = begin; i < end; i++) {
		if (y[i] * y[i] <= circle(x[i])) {
			localInCircle++;
		}
	}
	
	MPI_Reduce(&localInCircle, &globalInCircle, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
	if (rank == 0) {
		double ans = 4.0 * globalInCircle / N;
		cout << "pi " << ans << endl;
	}
	
	MPI_Finalize();
	return 0;
}

