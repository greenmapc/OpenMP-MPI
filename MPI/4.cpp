#include<iostream>
#include <stdio.h> 
#include <stdlib.h> 
#include<mpi.h>

using namespace std;

int main(int argc, char* argv[])
{
	const int N = 18;
	int rank, size;
	int x[N];
	int positiveNumbersGlobal = 0;
	int globalData[2];

	globalData[0] = 0;
	globalData[1] = 0;


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

	int* subarr = new int[elements_for_process + 1];
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

	int localData[2];

	MPI_Scatterv(x, count, displs, MPI_INT, subarr, N, MPI_INT, 0, MPI_COMM_WORLD);

	localData[0] = 0;
	localData[1] = 0;
	for (int i = 0; i < count[rank]; i++) {
		if (subarr[i] > 0) {
			localData[0] += subarr[i];
			localData[1] ++;
		}
	}

	MPI_Reduce(&localData, &globalData, 2, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

	if (rank == 0) {

		cout << "Avg of positive numbers = " << (double)globalData[0] / globalData[1] << endl;
	}

	MPI_Finalize();
	return 0;
}