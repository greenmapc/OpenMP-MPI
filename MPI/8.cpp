#include<iostream>
#include <stdio.h> 
#include <stdlib.h> 
#include<mpi.h>

using namespace std;

int main(int argc, char* argv[])
{
	const int N = 16 ;
	int rank, size;
	int x[N];

	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	if (rank == 0) {
		for (int i = 0; i < N; i++) {
			int sign = rand() % 10;
			x[i] = rand() % 100;
			if (sign % 2 == 0) {
				x[i] *= -1;
			}
			cout << x[i] << " ";
		}
		cout << endl;
	}

	int elements_for_process = N / size;
	int* subarr = new int[elements_for_process];

	if (rank == 0) {
		MPI_Sendrecv(x, elements_for_process, MPI_INT, 0, 0, subarr, elements_for_process, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		for (int i = elements_for_process; i < N; i += elements_for_process) {
			MPI_Send(x + i, elements_for_process, MPI_INT, i / elements_for_process, 0, MPI_COMM_WORLD);
		}
	}
	else {
		MPI_Recv(subarr, elements_for_process, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
	}

	for (int i = 0; i < elements_for_process; i++) {
		cout << subarr[i] << " ";
	}
	cout << "from rank " << rank << endl;

	
	if (rank == 0) {
		MPI_Sendrecv(subarr, elements_for_process, MPI_INT, 0, 0, x, elements_for_process, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
	}
	else {
		MPI_Send(subarr, elements_for_process, MPI_INT, 0, 0, MPI_COMM_WORLD);
	}

	if (rank == 0) {
		for (int i = elements_for_process; i < N; i += elements_for_process) {
			MPI_Recv(x + i, elements_for_process, MPI_INT, i / elements_for_process, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		}
		for (int i = 0; i < N; i++) {
			cout << x[i] << " ";
		}
		cout << endl;
	}
	
	MPI_Finalize();

	return 0;
}