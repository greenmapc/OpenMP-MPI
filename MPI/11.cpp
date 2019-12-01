#include<iostream>
#include <stdio.h> 
#include <stdlib.h> 
#include<mpi.h>
#include <algorithm>

using namespace std;

int main(int argc, char* argv[])
{
	int rank;
	int size;


	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	int message = 1;
	int dest = rank + 1 == size ? 0 : rank + 1;
	int source = rank - 1 < 0 ? size - 1 : rank - 1;

	if (rank == 0 && size > 1) {
		message *= 2;
		MPI_Send(&message, 1, MPI_INT, dest, 0, MPI_COMM_WORLD);
		cout << rank << " sent " << message << " to " << dest << endl;
	}
	else  if (size > 1) {
		MPI_Recv(&message, 1, MPI_INT, source, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		cout << rank << " got " << message << " from " << source << endl;
		message *= 2;
		MPI_Send(&message, 1, MPI_INT, dest, 0, MPI_COMM_WORLD);
		cout << rank << " sent " << message << " to " << dest << endl;
	}

	if (rank == 0 && size > 1) {
		MPI_Recv(&message, 1, MPI_INT, source, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		cout << rank << " got " << message << " from " << source << endl;
	}



	MPI_Finalize();
	return 0;
}