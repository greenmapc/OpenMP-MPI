#include<iostream>
#include <stdio.h> 
#include <stdlib.h> 
#include<mpi.h>
#include <algorithm>

using namespace std;

int main(int argc, char* argv[])
{
	const int N = 500;
	int rank;
	int size = 2;
	int x[N];
	int positiveNumbersGlobal = 0;


	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	double start = 0;
	double end = 0;
	int* arr = new int[N];

	if (rank == 0) {
		for (int i = 0; i < N; i++) {
			x[i] = rand() % 100;
		}

		start = MPI_Wtime();
		if (size > 1) {
			//MPI_Send(x, N, MPI_INT, 1, 0, MPI_COMM_WORLD);
			//MPI_Ssend(x, N, MPI_INT, 1, 0, MPI_COMM_WORLD);
			MPI_Rsend(x, N, MPI_INT, 1, 0, MPI_COMM_WORLD);
		}
	}
	else {
		MPI_Recv(arr, N, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		//MPI_Send(arr, N, MPI_INT, 0, 0, MPI_COMM_WORLD);
		//MPI_Ssend(arr, N, MPI_INT, 0, 0, MPI_COMM_WORLD);
		MPI_Rsend(arr, N, MPI_INT, 0, 0, MPI_COMM_WORLD);
	}

	if (rank == 0) {
		if (size > 1) {
			MPI_Recv(arr, N, MPI_INT, 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		}
		end = MPI_Wtime();

		cout << "time :" << end - start << endl;
	}


	MPI_Finalize();
	return 0;
}