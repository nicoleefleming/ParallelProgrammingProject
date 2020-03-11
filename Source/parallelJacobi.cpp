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
        //b[i] = cos(pi*i);
    }
    //Do Boundary Conditions need to be taken into account?
    return b;
}

//TODO: figure out L,U,D 

//TODO: Jacobi Method
double[] jacobi(double[] subB, double[] ad, double[] al, double[] as, double xZero) 
{
    //temporary storage array.
    double[DIM] x1;

    //xnew to hold new found values of x
    double[] xnew = x1;

    //xold to old old values of x with guess at index 0
    double[] xold = x;

    //Iterative process for Jacobi Method. In other examples there is a use of Swap, 
    //I don't think it is needed here. 

    //Using form:: xnew = (1/D)(b - (L+U)*xold)
    //where (b - (L+U)*xold) == C, compute C
    for (int i = 0; i < DIM; i++)
    {
        xnew[i] = subB[i];
    }
    for(int i = 0; i < (DIM - 1); i++)
    {
        xnew[i] = xnew[i] - al[i]*xold[i];
        xnew[i] = xnew[i] - as[i]*xold[i+1];
    }

    //compute (1/D)*C
    for (int i = 0; i < DIM; i++)
    {
        xnew[i] = xnew[i]/ad[i];
    }
    
    //return approximated xValues
    return xnew;
    
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

/* Jacobi notes. 
do {
        iter_num++;
        
        // Interchange x_old and x_new 
        Swap(x_old, x_new);
        
        for (i_local = 0; i_local < n_bar; i_local++)
        {
            i_global = i_local + my_rank*n_bar;
            x_local[i_local] = b_local[i_local];
           
            for (j = 0; j < i_global; j++)
            {
                x_local[i_local] = x_local[i_local] - A_local[i_local][j]*x_old[j];
            }
            for (j = i_global+1; j < n; j++)
            {
                x_local[i_local] = x_local[i_local] -  A_local[i_local][j]*x_old[j];
            }
            x_local[i_local] = x_local[i_local]/A_local[i_local][i_global];
        }
*/
