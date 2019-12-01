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
	int b[N];
	
	int ans[N];

	int answer;

	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	
	//читаем наоборот
	if (rank == 0) {
		for (int i = 0; i < N; i++) {
			for (int j = 0; j < N; j++) {
				a[j][i] = rand() % 10 + 1;
			}
		}

		for (int i = 0; i < N; i++) {
			for (int j = 0; j < N; j++) {
				cout << a[j][i] << " ";
			}
			cout << endl;
		}
		cout << endl;
		

		for (int i = 0; i < N; i++) {
			ans[i] = 0;
			b[i] = rand() % 10 + 1;
			cout << b[i] << " ";
		}
		cout << endl;
		cout << endl;
	}

	int elements_for_process = N / size;

	int* subarrMatrix = new int[elements_for_process + 1 * N];
	int* subarrVector = new int[elements_for_process + 1];
	int* countForMatrix = new int[size];
	int* countForVector = new int[size];
	int* displsForMatrix = new int[size];
	int* displsForVector = new int[size];

	int balance = N - elements_for_process * size;
	
	for (int i = 0; i < size; i++) {
		countForMatrix[i] = elements_for_process;
		if (balance > 0) {
			balance--;
			countForMatrix[i]++;
		}
		countForMatrix[i] *= N;
	}

	displsForMatrix[0] = 0;
	for (int i = 1; i < size; i++) {
		displsForMatrix[i] = displsForMatrix[i - 1] + countForMatrix[i - 1];
	}

	balance = N - elements_for_process * size;
	for (int i = 0; i < size; i++) {
		countForVector[i] = elements_for_process;
		if (balance > 0) {
			balance--;
			countForVector[i]++;
		}
	}

	displsForVector[0] = 0;
	for (int i = 1; i < size; i++) {
		displsForVector[i] = displsForVector[i - 1] + countForVector[i - 1];
	}

	/*if (rank == 0) {
		for (int i = 0; i < size; i++) {
			cout << countForMatrix[i] << " ";
		}
		cout << endl;

		for (int i = 0; i < size; i++) {
			cout << displsForMatrix[i] << " ";
		}
		cout << endl;

		for (int i = 0; i < size; i++) {
			cout << countForVector[i] << " ";
		}
		cout << endl;

		for (int i = 0; i < size; i++) {
			cout << displsForVector[i] << " ";
		}
		cout << endl;
	}
	*/



	int localSumArr[N];
	int localSum;
	int vectorNum;

	//get matrix
	MPI_Scatterv(a, countForMatrix, displsForMatrix, MPI_INT, subarrMatrix, N * N, MPI_INT, 0, MPI_COMM_WORLD);

	//get vector
	MPI_Scatterv(b, countForVector, displsForVector, MPI_INT, subarrVector, N, MPI_INT, 0, MPI_COMM_WORLD);

	for (int i = 0; i < N; i++) {
		localSum = 0;
		vectorNum = 0;
		for (int j = i; j < countForMatrix[rank]; j += N) {
			localSum += subarrMatrix[j] * subarrVector[vectorNum];
			vectorNum++;
		}
		localSumArr[i % N] = localSum;
	}


	for (int i = 0; i < N; i++) {
		MPI_Reduce(&localSumArr[i], &ans[i], 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
	}

	if (rank == 0) {
		cout << "result vector:" << endl;
		for (int i = 0; i < N; i++) {
			cout << ans[i] << endl;
		}
		cout << endl;
	}



	

	MPI_Finalize();
	return 0;
}