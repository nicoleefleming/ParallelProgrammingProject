#include <iostream>
#include <fstream>
#include <iostream>
#include <mpi.h>
#include <unistd.h>
#include <stdlib.h>

#define MPI_COMM_WORLD MCW
#define DIM 20

using namespace std;

int main(int argc, char **argv)
{
	int rank, size;
	int data;
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MCW, &rank);
	MPI_Comm_size(MCW, &size);
	
	srand(rank);
	int order;
	
	//MPI_Send(data, 1, MPI_INT, (rank+1)%size, 0, MCW);
	//MPI_Recv(data, 1, MPI_INT, MPI_ANY_SOURCE, 0, MCW, MPI_STATUS_IGNORE);
	
	
	MPI_Finalize();
	return 0;
}