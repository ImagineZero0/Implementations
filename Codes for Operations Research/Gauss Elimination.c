/*
NAME -> Varun Gupta
ROLL NUMBER -> 18MA20050
OR LAB WEEK 2 ASSIGNMENT
This is the code to find all the basic solutions and then evaluating all the basic feasible solutions, and finally from that find the optimal solution, by using the Gauss Elimination Algorithm
*/

#include <stdio.h>		// including all the required libraries
#include <math.h>
#include <stdlib.h>

double minimum;                                         // hence storing the values for the minimum and maximum solutions for the getting the optimal solution
double maximum;                                         // defining these values globally to get the answers after running all the iterations with all the combinations
int check=-1;                                              // for checking if the value of optimal solution has been stored or not
double* max_optimal;                                // defining an dynamic array for storing the max optimal answer
double* min_optimal;                                // defining an dynamic array for storing the min optimal answer

void feasible_solutions(int permutation_n_c_m[],double **A,double *B,int initial,int values,int m,int n,double err,double* Z);  // using predefined functions here and hence defining later
double *Gauss_Elimination(double **A, int n, float err);

int main()
{
    int choice,n,m,i,j,k;									// defining all the required varibles which would be used
	double error = 0.001;								// including defining the pointer for the variable sized matrices
	double **A, *B;
    printf("Enter the number of variables present\n");		// inputting the variables
    scanf("%d", &n);
    printf("Enter the number of equations to be solved\n");
    scanf("%d", &m);
    printf("\n");
    max_optimal = (double *)malloc(n * sizeof(double));                                 // global array to store the max optimal solution
    min_optimal = (double *)malloc(n * sizeof(double));                                 // global array to store the min optimal solution
    A = (double **)malloc(m * sizeof(double *));				// allocating the reqruired space for the rows of the matrix
    for (i = 0; i < m; i++)
    {
        A[i] = (double *)malloc(n * sizeof(double));			// hence allocating size for each row of the matrix
    }

    B = (double *)malloc(m * sizeof(double));
    for (i = 0; i < m; i++)
    {
        for (j = 0; j < n; j++)
        {
            printf("Enter A[%d][%d] :",i+1,j+1);				// hence inputting the matrix
            scanf("%lf", &A[i][j]);
        }
    }

    printf("\nNow, enter the values of B\n");					// hence inputting values of B
    for (i = 0; i < m; i++)
    {
        printf("Enter B[%d] :",i+1);
        scanf("%lf", &B[i]);
    }
    printf("\nNow enter the coefficients of the optimality equation of Z,which we would need to solve:\n");
    double *Z;
    Z =  (double *)malloc(n * sizeof(double ));					// allocating size for the coefficients of the optimality equation
    for ( i = 0; i < n; i++)
    {
        printf("Enter coefficient of X[%d] : ",i+1);					// hence inputting the coefficients
        scanf("%lf" , &Z[i]);
    }
    printf("\nHence, All the basic Solutions with their feasibility and not feasibility are as follws:\n");
    int * possible_permutes;										// allocating space for the index of the possible number of combinations that we could make
    possible_permutes = (int *)malloc(m*sizeof(int));
    feasible_solutions(possible_permutes,A,B,0,0,m,n,error,Z);			// hence getting all the solutions

    printf("\nEnter which optimality condition do we need for Z:\n1)Maximize.\n2)Minimize.\n");                             // and hence printing that value
    scanf("%d", &choice);                                           // hence getting choice for optimality
    if(choice == 1)                                                 // hence printing the optimal answer, as per the choice
    {
        printf("\nHence, The value of all variables and Maximum Z is, this being the optimal soltion:\n");
        printf("Z = %lf\n", maximum);
        for (j=0;j<n;j++)
        {
            printf("x[%d]  = %.3lf ",j+1,max_optimal[j]);      // now printing the optimal soltion
        }
        printf("\n");
    }
    else
    {
        printf("\nHence, The value of all variables and Minimum Z is, this being the optimal solution:\n");
        printf("Z = %lf", minimum);
        for (j=0;j<n;j++)
        {
            printf("x[%d]  = %.3lf ",j+1,min_optimal[j]);      // now printing the optimal soltion
        }
        printf("\n");
    }
    return 0;
}

double *Gauss_Elimination(double **A, int n, float err) 			// defining the required gauss elimination process, which evaluates all the solutions
{
    double c;                       // defining the required variables
    double *X, total,sum;
    int i,j,k;
    X = (double *)malloc(n * sizeof(double));       // allocating space for variable which takes in the value for iterating variables
    for(j=1; j<=n; j++)                 // hence defining the algorithm
    {
        for(i=1; i<=n; i++)
        {
            if(i>j)
            {
                c=A[i][j]/A[j][j];                      // dividing the B element from the diagonal element
                for(k=1; k<=n+1; k++)
                {
                    A[i][k]=A[i][k]-c*A[j][k];          // hence subtracting the other values
                }
            }
        }
    }
    X[n-1]=A[n][n+1]/A[n][n];               // storing those iteration values
    for(i=n-1; i>=1; i--)               // hence here evaluating the error for that particular iteration
    {
        sum=0;
        for(j=i+1; j<=n; j++)
        {
            sum=sum+A[i][j]*X[j-1];
        }
        X[i-1]=(A[i][n+1]-sum)/A[i][i];         // hence finally evaluating the error
    }
    return X;           // hence returning the iterated variables
}

void feasible_solutions(int permutation_n_c_m[],double **A,double *B,int initial,int values,int m,int n,double err,double* Z)   		// this is the function which wouold iterate over all the possible solutions
{
    int j=0,k=0,i;                  // defining the required variables
    double *x;
    double *final_answer;
    x=(double *)malloc(m*sizeof(double));               // allocating space for the variables to store the answer
    final_answer=(double *)malloc(n*sizeof(double));

    if (values==m)                                          // iterating till every different value
    {
        double **A_square;                                      // defining the other variable for storing the value of the proper square matrix
        A_square=(double ** )malloc((m+1)*sizeof(double * ));   // allocating the respective space as required for the size
        for (j=0;j<=m;j++)                                              // allocating memory in each row
            {
                A_square[j]=(double *)malloc((m+2)*sizeof(double));
            }
        for (j=0;j<m;j++)                                   // hence storing that particular combination in the Square matrix for which we would be checking the feasibility of the solution
        {
            for (k=0;k<m;k++)
            {

                A_square[j+1][k+1]=A[j][permutation_n_c_m[k]];          // hence storing
            }
        }
        for ( i = 1; i <= m; i++)
        {
            A_square[i][m+1]  = B[i-1];                 // respectively storing the B values for computation
        }
        x  = Gauss_Elimination(A_square,m,0.1);        // now evaluating the answers using the gauss elimination algorithm
        for(j=0;j<n;j++)
        {
            final_answer[j]=0;                      // now initialzing an array for the answer
        }
        for (k=0;k<m;k++)
        {
            final_answer[permutation_n_c_m[k]]=x[k];        // hence storing for that particular answer
        }
        for (j=0;j<n;j++)
        {
            printf("x[%d]  = %.3lf ",j+1,final_answer[j]);      // now printing the solutions
        }
        int check = 1;
        for ( i = 0; i < n; i++)
        {
            if(final_answer[i] <0 )             // condition for checking feasibility, here every variable is greater than 0
            {
                check = 0;
                break;
            }
        }
        if(check)
        {
            double z_value = 0.0;                                       // hence evaluating the value of the optimal condition at this basic feasible solution
            int min_flag = 0;                                          // variable to check if a better solution came or not
            int max_flag = 0;                                          // for each type optimal solution
            printf("; Hence,Feasable solution. Z = ");
            for ( i = 0; i < n; i++)
            {
                z_value+= final_answer[i]*Z[i];                     // evaluating the value of Z at this point
            }
            if(check == -1)          // checking if we ever stored a value of maximum or minimum
            {
                maximum = z_value;
                minimum = z_value;
                check = 1;
                min_flag = 1;
                max_flag = 1;
            }
            else                            // if did, then checking if the new value is better or not, and hence saving that value and the index and the variables value
            {
                if(z_value > maximum)           // for maximum
                {
                    max_flag = 1;
                    maximum = z_value;
                }
                if(z_value < minimum)           // for minimum
                {
                    min_flag = 1;
                    minimum = z_value;
                }
            }
            if(max_flag)                // hence storing the max optimal values of the variables
            {
                for ( i = 0; i < n; i++)
                {
                    max_optimal[i] = final_answer[i];
                }
            }
            if(min_flag)                // hence storing the min optimal values of the variables
            {
                for ( i = 0; i < n; i++)
                {
                    min_optimal[i] = final_answer[i];
                }
            }
            printf("%lf ",z_value);                             // and hence printing that value
        }
        else
        {
            printf(";Not a Feasable Solution.Hence, Z value not evaluated.");            // else printing that this is the non feasible solution
        }
        printf("\n");
        return;                                                 // hence returning from the recursion
    }

    for (i=initial; i<=n-1 && n-i >= m-values; i++)             // hence running the loop for every diffrent combination
    {
        permutation_n_c_m[values] = i;                                      // storing index for every different combination
        feasible_solutions(permutation_n_c_m,A,B,i+1,values+1,m,n,err,Z);           // evaluating the solution at this combination
    }
}
