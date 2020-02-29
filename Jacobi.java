import java.io.*;
import java.util.*;
import java.lang.*;

public class Jacobi {

    static int test_convergence(double []x, double []oldx, int n)
    {
        double maxvalue, tempvalue;
        int i;
        double epsilon = 0.000000001;

        maxvalue = 0.0;
        for(i=0; i < n; i++)
        {
            tempvalue = Math.abs(x[i] - oldx[i]);
            if (tempvalue > maxvalue)
                maxvalue = tempvalue;

        } // for
        if (maxvalue > epsilon)
            return 0;
        else
            return 1;

    } // test_convergence

    static void jacobi(double A[][], double b[], int n, double x[])
    {

        int i, j, k, p;
        double []oldx;

        oldx =  new double [n];

        do {
            for (i=0; i < n; i++)
                oldx[i] = x[i];

            for (i=0; i < n; i++)
            {
                x[i] = b[i];
                for(j=0; j < n; j++)
                    if ( i != j)
                        x[i] = x[i] - A[i][j] * oldx[j];
                x[i] = x[i]/A[i][i];

            } // for

        } while(test_convergence(x, oldx,n) == 0);

    } //  jacobi 
}
