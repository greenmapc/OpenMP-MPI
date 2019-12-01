#include<iostream>
#include <stdio.h> 
#include <stdlib.h> 
#include<mpi.h>
#include <algorithm>

using namespace std;

int main(int argc, char* argv[])
{
	const int N = 19;
	int rank, size;
	int x[N];
	int positiveNumbersGlobal = 0;


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

	int elements_for_process = max(N / size, 2);

	int* subarrLeft = new int[elements_for_process + 1];
	int* subarrRight = new int[elements_for_process + 1];
	int* countLeft = new int[size];
	int* countRight = new int[size];
	int* displsRight = new int[size];
	int* displsLeft = new int[size];

	for (int i = 0; i < size; i++) {
		countLeft[i] = elements_for_process;
		countLeft[i] /= 2;
	}

	for (int i = 0; i < size; i++) {
		countRight[i] = elements_for_process;
		countRight[i] /= 2;
	}

	int balance = N - elements_for_process * size;
	if (balance % 2 == 1) {
		balance--;
	}
	for (int i = 0; i < size; i++) {
		if (balance > 0) {
			countRight[i] ++;
			countLeft[i] ++;
			balance -= 2;
		}
	}

	displsLeft[0] = 0;
	for (int i = 1; i < size; i++) {
		displsLeft[i] = displsLeft[i - 1] + countLeft[i - 1];
	}

	displsRight[0] = N - countRight[0];
	for (int i = 1; i < size; i++) {
		displsRight[i] = displsRight[i - 1] - countRight[i];
	}

	if (rank == 0) {
		for (int i = 0; i < size; i++) {
			cout << displsLeft[i] << " ";
		}

		cout << endl;

		for (int i = 0; i < size; i++) {
			cout << displsRight[i] << " ";
		}

		cout << endl;
	}
	
	MPI_Scatterv(x, countLeft, displsLeft, MPI_INT, subarrLeft, N, MPI_INT, 0, MPI_COMM_WORLD);
	MPI_Scatterv(x, countRight, displsRight, MPI_INT, subarrRight, N, MPI_INT, 0, MPI_COMM_WORLD);

	if (rank == 0) {
		cout << "RANK left " << rank << endl;
		for (int i = 0; i < countLeft[rank]; i++) {
			cout << subarrLeft[i] << " ";
		}
		cout << endl;

		cout << "RANK right " << rank << endl;
		for (int i = 0; i < countRight[rank]; i++) {
			cout << subarrRight[i] << " ";
		}
		cout << endl;
	}

	for (int i = 0; i < countLeft[rank]; i++) {
		cout << subarrLeft[i] << " " << subarrRight[countLeft[rank] - i - 1] << endl;
		swap(subarrLeft[i], subarrRight[countLeft[rank] - i - 1]);
	}

	
	MPI_Gatherv(subarrLeft, countLeft[rank], MPI_INT, x, countLeft, displsLeft, MPI_INT, 0, MPI_COMM_WORLD);
	MPI_Gatherv(subarrRight, countRight[rank], MPI_INT, x, countRight, displsRight, MPI_INT, 0, MPI_COMM_WORLD);

	if (rank == 0) {
		for (int i = 0; i < N; i++) {
			cout << x[i] << " ";
		}
		cout << endl;
	}

	MPI_Finalize();
	return 0;
}