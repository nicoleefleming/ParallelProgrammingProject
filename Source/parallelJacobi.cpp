#include <iostream>
#include <fstream>
#include <iostream>
#include <mpi.h>
#include <unistd.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#define MCW MPI_COMM_WORLD

#define maxIters 50
#define DIM 10000
#define PI 3.14159265
using namespace std;

//Global variables
double A[DIM][DIM] ;
double b[DIM];
double x[DIM];

//initialize A to a diagonally dominant matrix
void initA(double ad[], double al[], double as[])
{
    for (int i = 0; i < DIM; i++)
    {
        A[i][i]   = ad[i];    //main diagonal
        A[i][i+1] = as[i];    //first super-diagonal
        A[i+1][i] = al[i];    //first sub-diagonal
    }
    
}


//initialize RHS vector b
void RHSinit(double b[])
{
    for (int i = 0; i < DIM; i++)
    {
        b[i] = 1.0;
        // b[i] = cos(PI*i);
    }
    //Do Boundary Conditions need to be taken into account?
}

//TODO: figure out L,U,D 

//TODO: Jacobi Method
double* jacobi(double subB[], double ad[], double al[], double as[], double xZero) 
{
    //temporary storage array.
    double x1[DIM];

    //xnew to hold new found values of x
    static double xnew[DIM];

    //xold to old old values of x with guess at index 0
    double xold[DIM];
    for(int i = 0; i <DIM; i++)
    {
        xold[i] = x[i];
    }

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
    double subArrB[DIM/size];
    double *subArrX;
    double ad[DIM];
    double as[DIM - 1];
    double al[DIM - 1];

	
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
    // printf("Initialized ad, as, al");

    //initialize A
    initA(ad,al,as);
    // printf("Initialized A");

    //initialize b
    RHSinit(b);
    // printf("Initialized b");

    // split array b
    subB = DIM / size; // 5 for DIM 20
    // printf("Split b");

    double startProcessTime = MPI_Wtime();
    // double start = time();

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
    MPI_Barrier(MCW);

    
    // sleep(1);

    double endProcessTime = MPI_Wtime();
    // double end = time();

    double duration = endProcessTime - startProcessTime;

    if(!rank) 
    {
        cout << "Duration: " << duration << " seconds " << endl;
    }


	// MPI_Finalize();
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
