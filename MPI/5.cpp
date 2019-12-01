#include<iostream>
#include <stdio.h> 
#include <stdlib.h> 
#include<mpi.h>

using namespace std;

int main(int argc, char* argv[])
{
	const int N = 10;
	int rank, size;
	int a[N];
	int b[N];

	int globalAns;

	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	if (rank == 0) {
		for (int i = 0; i < N; i++) {
			a[i] = rand() % 100;
			cout << a[i] << " ";
		}
		cout << endl;

		for (int i = 0; i < N; i++) {
			b[i] = rand() % 100;
			cout << b[i] << " ";
		}
		cout << endl;
	}

	int elements_for_process = N / size;

	int* subarrA = new int[elements_for_process + 1];
	int* subarrB = new int[elements_for_process + 1];
	int* count = new int[size];
	int* displs = new int[size];

	int balance = N - elements_for_process * size;
	for (int i = 0; i < size; i++) {
		count[i] = elements_for_process;
		if (balance > 0) {
			balance--;
			count[i]++;
		}
	}

	displs[0] = 0;
	for (int i = 1; i < size; i++) {
		displs[i] = displs[i - 1] + count[i - 1];
	}


	int localSum;

	MPI_Scatterv(a, count, displs, MPI_INT, subarrA, N, MPI_INT, 0, MPI_COMM_WORLD);
	MPI_Scatterv(b, count, displs, MPI_INT, subarrB, N, MPI_INT, 0, MPI_COMM_WORLD);

	localSum = 0;
	for (int i = 0; i < count[rank]; i++) {
		cout << "a = " << subarrA[i] << " b = " << subarrB[i] << " ";
		localSum += subarrA[i] * subarrB[i];
	}
	cout << endl;

	MPI_Reduce(&localSum, &globalAns, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

	if (rank == 0) {
		cout << "Vector's scalar = " << globalAns << endl;
	}

	MPI_Finalize();
	return 0;
}