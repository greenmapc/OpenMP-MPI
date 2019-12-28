#include<iostream>
#include<stdio.h> 
#include<stdlib.h> 
#include<mpi.h>
#include<algorithm>

using namespace std;

int powerOfTwo(int size) {
	int ans = 0;
	while (size > 1) {
		ans++;
		size /= 2;
	}

	return ans;
}

int main(int argc, char* argv[])
{
	const int N = 16;
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
	int* subarr = new int[elements_for_process];
	int gettingElement;
	bool swapElements;
	bool swapWithPrevProcess;
	bool sorted = true;
	

	MPI_Scatter(a, elements_for_process, MPI_INT, subarr, elements_for_process, MPI_INT, 0, MPI_COMM_WORLD);

	int firstInter = powerOfTwo(size);

	for (int i = 0; i < firstInter; i++) {
		int bit = firstInter - i - 1;
		int swapProcess = rank ^ (1 << bit);

		for (int j = 0; j < elements_for_process; j++) {
			if (swapProcess > rank) {
				MPI_Send(&subarr[j], 1, MPI_INT, swapProcess, 0, MPI_COMM_WORLD);
				MPI_Recv(&subarr[j], 1, MPI_INT, swapProcess, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
			}
			else {
				MPI_Recv(&gettingElement, 1, MPI_INT, swapProcess, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
				if (subarr[j] < gettingElement) {
					swap(subarr[j], gettingElement);
				}
				MPI_Send(&gettingElement, 1, MPI_INT, swapProcess, 0, MPI_COMM_WORLD);
			}
		}
	}

	for (int i = 0; !globalSorted && i < size; i++) {

		sorted = true;

		int start = i % 2;

		if (size > 1 && rank == 0) {
			swapElements = (elements_for_process - start) % 2 != 0;
			MPI_Send(&swapElements, 1, MPI_INT, rank + 1, rank, MPI_COMM_WORLD);
		}

		if (rank != 0) {
			MPI_Recv(&swapWithPrevProcess, 1, MPI_INT, rank - 1, rank - 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
			start = swapWithPrevProcess ? 1 : 0;
		}

		if (rank != 0 && rank != size - 1) {
			swapElements = (elements_for_process - start) % 2 != 0;
			MPI_Send(&swapElements, 1, MPI_INT, rank + 1, rank, MPI_COMM_WORLD);
		}

		for (int j = start; j < elements_for_process - 1; j += 2) {
			if (subarr[j] > subarr[j + 1]) {
				sorted = false;
				swap(subarr[j], subarr[j + 1]);
			}
		}

		if (rank != size - 1) {
			if ((elements_for_process - start) % 2 != 0) {
				MPI_Send(&subarr[elements_for_process - 1], 1, MPI_INT, rank + 1, rank, MPI_COMM_WORLD);
				MPI_Recv(&subarr[elements_for_process - 1], 1, MPI_INT, rank + 1, rank + 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
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



	MPI_Gather(subarr, elements_for_process, MPI_INT, a, elements_for_process, MPI_INT, 0, MPI_COMM_WORLD);

	if (rank == 0) {
		for (int i = 0; i < N; i++) {
			cout << a[i] << " ";
		}
		cout << endl;
	}

	MPI_Finalize();
	return 0;
}