#include<iostream>
#include<stdio.h> 
#include<stdlib.h> 
#include<mpi.h>
#include<algorithm>

using namespace std;

int main(int argc, char* argv[])
{
	const int N = 24;
	int rank, size;
	int a[N];

	int globalSorted = false;

	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);


	if (rank == 0) {
		for (int i = 0; i < N; i++) {
			a[i]= rand() % 10;
			cout << a[i] << " ";
		}
		cout << endl;
	}
	

	int elements_for_process = N / size;
	int* subarr = new int[elements_for_process + 1];
	int* count = new int[size];
	int* displs = new int[size];
	int gettingElement = 0;
	bool sorted = true;
	bool swapWithPrevProcess = false;
	bool swapElements;

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

	MPI_Scatterv(a, count, displs, MPI_INT, subarr, N, MPI_INT, 0, MPI_COMM_WORLD);
	
	for (int i = 0; i < N; i++) {
		if (globalSorted) {
			break;
		}

		sorted = true;

		int start = i % 2;
		if (size > 1 && rank == 0) {
			swapElements = (count[rank] - start) % 2 != 0;
			MPI_Send(&swapElements, 1, MPI_INT, rank + 1, rank, MPI_COMM_WORLD);
		}
		
		if (rank != 0) {
			MPI_Recv(&swapWithPrevProcess, 1, MPI_INT, rank - 1, rank - 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
			start = swapWithPrevProcess ? 1 : 0;
		}

		if (rank != 0 && rank != size - 1) {
			swapElements = (count[rank] - start) % 2 != 0;
			MPI_Send(&swapElements, 1, MPI_INT, rank + 1, rank, MPI_COMM_WORLD);
		}

		for (int j =  start; j < count[rank] - 1; j += 2) {
			if (subarr[j] > subarr[j + 1]) {
				sorted = false;
				swap(subarr[j], subarr[j + 1]);
			}
		}
	
		if (rank != size - 1) {
			if ((count[rank] - start) % 2 != 0) {
				MPI_Send(&subarr[count[rank] - 1], 1, MPI_INT, rank + 1, rank, MPI_COMM_WORLD);
				MPI_Recv(&subarr[count[rank] - 1], 1, MPI_INT, rank + 1, rank + 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
			}
		}
		if (rank != 0) {
			if (swapWithPrevProcess) {
				MPI_Recv(&gettingElement, 1, MPI_INT, rank - 1, rank - 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
				if (gettingElement > subarr[0]) {
					sorted = false;
					swap(subarr[0], gettingElement);
				}
				MPI_Send(&gettingElement, 1, MPI_INT, rank - 1, rank, MPI_COMM_WORLD);
			}
		}	
		MPI_Allreduce(&sorted, &globalSorted, 1, MPI_C_BOOL, MPI_LAND, MPI_COMM_WORLD);
	}


	MPI_Gatherv(subarr, count[rank], MPI_INT, a, count, displs, MPI_INT, 0, MPI_COMM_WORLD);
	if (rank == 0) {
		for (int i = 0; i < N; i++) {
			cout << a[i] << " ";
		}
		cout << endl;
	}


	MPI_Finalize();
	return 0;
}