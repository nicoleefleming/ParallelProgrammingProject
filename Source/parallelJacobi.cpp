#include <iostream>
#include <fstream>
#include <iostream>
#include <mpi.h>
#include <unistd.h>
#include <stdlib.h>
#include <math.h>

#define MPI_COMM_WORLD MCW
#define DIM 20
#define maxIters 50

using namespace std;

//Global variables
double[DIM][DIM] A;
double[DIM][DIM] L;
double[DIM][DIM] U;
double[DIM][DIM] D;
double[DIM] b;
double[DIM] x;

//initialize A to a diagonally dominant matrix
double[][] initA(double[] ad, double[] al, double[] as)
{
    for (int i = 0; i < DIM; i++)
    {
        A[i][i]   = ad[i];    //main diagonal
        A[i][i+1] = as[i];    //first super-diagonal
        A[i+1][i] = al[i];    //first sub-diagonal
    }
    
    return A;
}


//initialize RHS vector b
double[] RHSinit(double[] b)
{
    for (int i = 0; i < DIM; i++)
    {
        b[i] = 0.0;
    }
    //Do Boundary Conditions need to be taken into account?
    return b;
}

//TODO: figure out L,U,D 

//TODO: Jacobi Method
double[] jacobi(double[] subB, double[] ad, double[] al, double[] as, double xZero) 
{
    // TODO: insert code from 5620 here :)
}

//TODO: if time, test convergence :)

int main(int argc, char **argv)
{
	int rank, size;
	int data;
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MCW, &rank);
	MPI_Comm_size(MCW, &size);
    int iter = 0;
    int subB;
    double[] subArrB;
    double[] subArrX;
	
    //initialize ad, as, al
    for (int i = 0; i < DIM; i++)
    {
        ad[i] = -2.0;
    }
    for(int i = 0; i < (DIM - 1); i++)
    {
        as[i] = 1.0;
        al[i] = 1.0;
    }

    //initialize A
    A = initA(ad,al,as);

    //initialize b
    b = RHSinit(b);

    // split array b
    subB = DIM / size; // 5 for DIM 20
    MPI_Scatter(&b, subB, MPI_DOUBLE, &subArrB, subB, MPI_DOUBLE, 0, MCW);

    double xZero = 1.0;     // this is our guess
    x[0] = xZero;  

    // add an 'AND'/&& condition to the while loop, look up later--not logical
    while(iter <= maxIters) 
    {
        // call Jacobi method
        subArrX = jacobi(subArrB, ad, al, as, xZero);
        
        iter++;
    }
    //MPI_Barrier(MCW);
    MPI_Gather (&subArrX, subB, MPI_DOUBLE, x, subB, MPI_DOUBLE, 0, MCW);
    //MPI_Barrier(MCW);

    sleep(1);

    //MPI_Gather(A_local, n_bar*MAX_DIM, MPI_FLOAT, temp, n_bar*MAX_DIM, MPI_FLOAT, 0, MPI_COMM_WORLD);
    //MPI_Scatter(&startArr, (SIZE/size), MPI_INT, &subArr, (SIZE/size), MPI_INT, 0, MCW);


	MPI_Finalize();
	return 0;
}