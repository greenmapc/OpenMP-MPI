#include<iostream>
#include <stdio.h> 
#include <stdlib.h> 
#include<mpi.h>
#include<algorithm>

using namespace std;

int main(int argc, char* argv[])
{
	const int N = 10;
	int rank, size;
	int a[N][N];

	int globalMaxMin;
	int globalMax;

	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	if (rank == 0) {
		for (int i = 0; i < N; i++) {
			for (int j = 0; j < N; j++) {
				a[i][j] = rand() % 100 + 1;
				cout << a[i][j] << " ";
			}
			cout << endl;
		}

		cout << endl;
	}

	int elements_for_process = N / size;

	int* subarr = new int[elements_for_process + 1 * N];
	int* count = new int[size];
	int* displs = new int[size];

	int balance = N - elements_for_process * size;
	for (int i = 0; i < size; i++) {
		count[i] = elements_for_process;
		if (balance > 0) {
			balance--;
			count[i]++;
		}
		count[i] *= N;
	}

	displs[0] = 0;
	for (int i = 1; i < size; i++) {
		displs[i] = displs[i - 1] + count[i - 1];
	}

	int localMaxMin;
	int localMax;
	int stringMin;

	MPI_Scatterv(a, count, displs, MPI_INT, subarr, N * N, MPI_INT, 0, MPI_COMM_WORLD);

	localMaxMin = 0;
	localMax = 0;
	for (int i = 0; i < count[rank] / N; i++) {
		stringMin = 101;
		for (int j = i * N; j < (i + 1) * N; j++) {
			localMax = max(localMax, subarr[j]);
			stringMin = min(subarr[j], stringMin);
		}
		cout << endl << stringMin << endl;
		localMaxMin = max(localMaxMin, stringMin);
	}
	cout << endl;
	
	MPI_Reduce(&localMax, &globalMax, 1, MPI_INT, MPI_MAX, 0, MPI_COMM_WORLD);
	MPI_Reduce(&localMaxMin, &globalMaxMin, 1, MPI_INT, MPI_MAX, 0, MPI_COMM_WORLD);

	if (rank == 0) {
		cout << "Max = " << globalMax << endl 
			 << "MaxMin = " << globalMaxMin << endl;
	}

	MPI_Finalize();
	return 0;
}