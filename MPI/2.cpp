#include<iostream>
#include <stdio.h> 
#include <stdlib.h> 
#include<mpi.h>

using namespace std;

int main(int argc, char* argv[])
{
	const int N = 16;
	int rank, size;
	int x[N];
	int result = 0;


	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	if (rank == 0) {
		for (int i = 0; i < N; i++) {
			x[i] = rand() % 100;
			cout << x[i] << " ";
		}
		cout << endl;
	}

	int elements_for_process = N / size;


	MPI_Bcast(&x, N, MPI_INT, 0, MPI_COMM_WORLD);

	int begin = elements_for_process * rank;
	int end = elements_for_process * (rank + 1);
	int localResult = 0;

	for (int i = begin; i < end; i++) {
		if (x[i] > localResult) {
			localResult = x[i];
		}
	}
	

	MPI_Reduce(&localResult, &result, 1, MPI_INT, MPI_MAX, 0, MPI_COMM_WORLD);
	if (rank == 0) {
		cout << "max element " << result << endl;
	}

	MPI_Finalize();
	return 0;
}