/*
NAME -> Varun Gupta
ROLL NUMBER -> 18MA20050
OR LAB WEEK 3 ASSIGNMENT
This is the code to implement the Simplex Algorithm by evaluating the required Pivot Elements and hence iterating through the Simplex Tableau
and with the required methods that would be used in the algorithm
*/

#include <bits/stdc++.h>			// including the required libraries
using namespace std;		// using namespace for the required functions

void LPP_in_Standard(float** A,float* B,float* Z,int N,int M,int choice)									//	code for outputting the given question in the Standard Form
{
	cout << "__________________________________SOLUTION BEGINS__________________________________________\n";
	cout << "Hence, We can the write the provided Linear Programming Problem in Standard Form as follows:";
	cout << "\nSince,Here we have " << M << " Constraint Equation, therefore we would have " << M << " Slack Variables which would be named as follows:\n";
	for(int i=N+1;i<=M+N;i++)																// which are the variables that are used as slack varaibles
		cout << "X[" << i << "] ";
	cout << "\nThe representation of matrix A is as follows:\n\n";
	for(int i=0;i<M;i++)												// the format of the equation is written
	{
		for(int j=0;j<N+M;j++)
		{
			cout << "X[" << i+1 << "][" << j+1 << "] ";
		}
		cout <<"\n";
	}
	cout << "\nThe Matrix A is as follows:\n";
	for(int i=0;i<M;i++)											// the standard matrix is outputted
	{
		for(int j=0;j<N+M;j++)
		{
			if(j<N)
				cout << A[i][j] << "\t";
			else if(j-N==i)
				cout << "1\t";
			else
				cout <<"0\t";
		}
		cout <<"\n";
	}
	cout << "\nThe values of Column Vector B, containing the Right Hand Side of the Constraint Equations are as follows:\n";
	for(int i=0;i<M;i++)																			// the B vector is outputted
		cout << "B[" << i+1 << "] = " << B[i] << "\n";
	cout << "\nThe values of Column Vector C,containing the coefficients of X(i) in the Optimal Equation are as follows:\n";
	for(int i=0;i<N;i++)														// the coefficients of the varirables in the optimal equation
	{
		if(choice == 1)
			cout << "Z[" << i+1 << "] = " << (-1)*Z[i] << "\n";
		else
			cout << "Z[" << i+1 << "] = " << Z[i] << "\n";

	}
	return;
}

float** Build_Simplex_Table(float** A,float *B,float *Z,int N,int M)	// function to build the initial Simplex Table, and code to store the Zj - Cj values
{
	float** Table;									// initializing the variable for the Simplex Table
	Table = (float**)malloc((M+1)*sizeof(float*));	// hence, allocating space for the Simplex Table
	for(int i=0;i<M+1;i++)
		Table[i] = (float*)malloc((N+1)*sizeof(float));
	for(int i=0;i<M;i++)			// hence placing all the elements from the given A,B,Z matrices
	{
		for(int j=0;j<N+1;j++)
		{
			if(j<N)
				Table[i][j] = A[i][j];
			else
				Table[i][j] = B[i];
		}
	}
	for(int i=0;i<N+1;i++)
		Table[M][i] = Z[i];
	return Table;		// returning the initial Simplex Table
}

void Print_Table(float** Table,int** Points,int N,int M,int iteration,int choice,bool final,int* Exchange)		// function that would print the Simplex Table Whenever required
{
	int value = 0;
	bool check;
	if(final == false)
	{
		if(iteration == 0)
			cout << "\nHence, The Initial Simplex Table is as follows:\n";
		else
			cout << "\nThe Simplex Table at Iteration " << iteration << " is as follows:\n";
		for(int i=0;i<N;i++)
		{
			if(Exchange[i] < N+1)
				cout << "-X[" << Exchange[i] << "]\t";
			else
				cout << "X[" << Exchange[i] << "]\t";

		}
		cout << "1\n";
	    for(int i=0;i<M+1;i++)			// hence printing the values
		{
			cout << " ";
			for(int j=0;j<N+2;j++)
			{
				if(j<N+1)
					cout << fixed << setprecision(2) << Table[i][j] << "\t";
				else if(i<M)
				{
					if(Exchange[N+i] < N+1)
						cout << "X[" << Exchange[N+i] << "]\tBasic Variables\n";
					else
						cout << "X[" << Exchange[N+i] << "]\tSlack Variables\n";
				}
				else if(i==M && j==N+1)
					cout << "Z\tOptimal Value\n";
			}
			cout << "\n";
		}
	}
	while(value < N+M)						// hence outputting the found optimal values on the basis of the pivot points found
	{
		if(value < N)
		{
			check = true;					// checking if the variables pivot point has been found or not
			for(int i=0;i<iteration;i++)
			{
				if(Points[i][1] == value)
				{
					check = false;				// if found than false
					cout << "X[" << value + 1 << "] = " << Table[Points[i][0]][N] << "\n";
					value++;				// finding for further variables
				}
			}
			if(check)			// if not than that elements value is 0
			{
				cout << "X[" << value + 1 << "] = 0\n";
				value++;
			}
		}
		else
		{
			check = true;
			int value2 = value - N;
			for(int i=0;i<iteration;i++)
			{
				if(Points[i][0] == value2)
				{
					check = false;				// if found than false
					cout << "X[" << value + 1 << "] = 0\tSlack Variable\n";
					value++;				// finding for further variables
				}
			}
			if(check)			// if not than that elements value is 0
			{
				cout << "X[" << value + 1 << "] = " << Table[value2][N] << "\tSlack Variable\n";
				value++;
			}
		}
	}
	if(final)
	{
		if(choice == 1)
			cout << "The Optimal value is Z = " << Table[M][N];		// outputting the optimal value of the function
		else
			cout << "The Optimal value is Z = " << -1*Table[M][N];		// outputting the optimal value of the function
	}
	else if(iteration > 0)
	{
		if(choice == 1)
			cout << "Hence, The Optimal Value at Iteration " << iteration << " is " << Table[M][N] << "\n";
		else
			cout << "Hence, The Optimal Value at Iteration " << iteration << " is " << -1*Table[M][N] << "\n";
	}
	cout << "\n";
	return;
}

int CheckPivot(float** Table,int** Points,int N,int M,int iteration)		// code to evaluate the pivot element and the minimum ratio, and hence returns the pivot row and column
{
	bool unbounded = true;				// variable to check if the solution is unbounded or not
	bool flag = true;					// a flag to check if the pivot has been found
	float min;							// variable to find the minimum value of in the last row,pivot column of the Simplex tableau
	int index,index2;							// the index for the particular column,row of the pivot element
	for(int i=0;i<N+1;i++)		// finding the column of the most negative element
	{
		if(flag)		// if not element has been saved , saving the first element
		{
			min = Table[M][i];
			index = i;
			flag = false;
		}
		else
		{
			if(min > Table[M][i])		// else checking if any element is smaller
			{
				min = Table[M][i];
				index = i;
			}
		}
	}
	if(min < 0)				// if we have found the minimum element as negative
	{
		for(int i=0;i<M+1;i++)		// now finding the minimum ratio element in the pivot element column
		{
			if(Table[i][index] > 0)		// only if that element in positve
			{
				if(unbounded)				// and if no first element has been saved
				{
					min = Table[i][N]/Table[i][index];		// saving the minimim ratio
					index2 = i;						// saving the index of that row
					unbounded = false;				// false for flag when one element has been saved
				}
				else
				{
					if(min > Table[i][N]/Table[i][index])		// for future elements, if ratio is smaller than previous ratio
					{
						min = Table[i][N]/Table[i][index];		// saving the minimum ratio
						index2 = i;								// sasving index of the minimum ratio
					}
				}
			}
		}
		if(unbounded)			// if no positve element has been found even when negative element exist in the last row
		{
			cout << "The Solution to the given problem is Unbounded.\n";		// hence this is a Unbounded type of problem
			exit(0);				// hence, exiting the program
		}
		else 				// if we have found the most negative element in the last row, and minimum ratio in that elements column, hence found the pivot element
		{
			Points[iteration][0] = index2;			// hence saving the indices of that pivot element
			Points[iteration][1] = index;
			return 1;
		}
	}
	else if(iteration==0)			// if minimum element is not negative and no negative minimum element has been found in the last row
	{
		cout << "The Maximum value of Z is 0.0000000.\nThe Optimizing Point are all 0 i.e.\n";		// then the maximum value is the constant term and all values of variables are 0
		for(int i=0;i<N;i++)
		{
			cout << "X[" << i+1 << "]=0.00000 ";			// hence printing the values of X
		}
		cout << "\n";
		exit(0);		// exiting from the program
	}
	else if(min > 0)					// ending the program by printing the already found optimal solution
	{
		return 0;
	}
	else 								// checking if any alternate solution would exist
	{
		return -1;
	}
}

void Evaluate_Simplex_Table(float** Table,int** Points,int N,int M,int iteration)		// code to revise the simplex table and complete one iteration, and hence print the next solution, and object value
{
	int Point[2];										// variable for storing for the individual pivot points found in each iteration
	Point[0] = Points[iteration][0];					// evaluating one pivot point
	Point[1] = Points[iteration][1];
	for(int i=0;i<M+1;i++)			// hence evaluating the Simplex Tableua on the basis of Simplex Algorithm
	{
		if(i==Point[0])				// if the pivot row, then skipping
			continue;
		for(int j=0;j<N+1;j++)		// else finding the new value of the point
		{
			if(j==Point[1])			// if that particular column then skipping
				continue;
			Table[i][j] = (Table[i][j]*Table[Point[0]][Point[1]] - Table[i][Point[1]]*Table[Point[0]][j]);
			Table[i][j] /= Table[Point[0]][Point[1]];
		}
	}

	for(int i=0;i<N+1;i++)		// now evaluating new values for the particular pivot row
	{
		if(i==Point[1])			// for the pivot element, skipping else evaluating
			continue;
		Table[Point[0]][i] /= Table[Point[0]][Point[1]];
	}

	for(int i=0;i<M+1;i++)	// now evaluating new values for the particular pivot column
	{
		if(i==Point[0])		// for the pivot element, skipping else evaluating
			continue;
		Table[i][Point[1]] /= Table[Point[0]][Point[1]];
		Table[i][Point[1]] *= (-1.0);
	}
	Table[Point[0]][Point[1]] = 1/Table[Point[0]][Point[1]];		// hence changing the pivot element
	return;
}

int Take_input(float** A,float* B,float* Z,int N,int M)						// function to take the input
{
	int choice;
	float val;
	cout << "\nNow, enter the values of A:\n";					// hence inputting values of A
	for (int i = 0; i < M; i++)
    {
        for (int j = 0; j <N; j++)
        {
            cout << "Enter A[" << i+1 << "][" << j+1 << "] = ";			// hence inputting the matrix
            cin >> A[i][j];
        }
        cout << "\n";
    }
    cout << "\nNow, enter the values of B:\n";			// hence inputting values of B
    for (int i = 0; i < M; i++)
    {
        cout << "Enter B[" << i+1 << "] = ";
        cin >> B[i];						// hence inputting the values of B
    }
    cout << "\nEnter which kind of Optimality do we need to evaluate:\n";
    cout << "1)Maximizing.\n2)Minimizing.\n";
    cin >> choice;
    cout  << "\nNow enter the coefficients of the optimality equation of Z:\n";
    if(choice == 1)
    {
	    for (int i = 0; i < N; i++)
	    {
	        cout << "Enter coefficient of X[" << i+1 <<"], i.e. Z[" << i+1 << "] = ";					// hence inputting the coefficients
	        cin >> val;
	        Z[i] = (-1.0)*val;							// hence inputting the values of the optimal equation
	    }
	    Z[N] = 0.0;
    }
    else
    {
    	for (int i = 0; i < N; i++)
	    {
	        cout << "Enter coefficient of X[" << i+1 <<"], i.e. Z[" << i+1 << "] = ";					// hence inputting the coefficients
	        cin >> val;
	        Z[i] = val;							// hence inputting the values of the optimal equation
	    }
	    Z[N] = 0.0;
    }
    cout << "\n";
    return choice;
}

void Solve(float** A,float* B,float* Z, int N,int M,int** Points,int choice,int* Exchange)
{
	int iteration = 0,value;
	float** table;
	bool final = false;
    LPP_in_Standard(A,B,Z,N,M,choice);				// prints the whole problem in the Standard form
    table = Build_Simplex_Table(A,B,Z,N,M);			// function builds the initial Siimplex Tableau
   	Print_Table(table,Points,N,M,iteration,choice,final,Exchange);
	cout << "Hence, The Initial Optimal Value is " << table[M][N] << "\n";
    value = CheckPivot(table,Points,N,M,iteration);
    while(value == 1)
    {
    	swap(Exchange[Points[iteration][1]],Exchange[N+Points[iteration][0]]);
    	cout << "\n";
    	cout << "------------Iteration Number: " << iteration + 1 << " --------------------\n";
    	cout << "The Pivot Row is " << Points[iteration][0] + 1 << " and the Pivot Column is " << Points[iteration][1] + 1 << ".\n";
    	cout << "Hence, The Pivot Element from Iteration Number " << iteration + 1 << " is " << table[Points[iteration][0]][Points[iteration][1]] << ".\n";
    	Evaluate_Simplex_Table(table,Points,N,M,iteration);		// function that would evaluate all the Pivot Points
    	iteration++;
    	Print_Table(table,Points,N,M,iteration,choice,final,Exchange);
	    value = CheckPivot(table,Points,N,M,iteration);
    }
    cout << "The Total Number of Iterations to get the Final Optimal Solution is " << iteration << ".\n";
	cout << "Therefore, The Final Optimal Solution is as follows:\n";
	final = true;
	Print_Table(table,Points,N,M,iteration,choice,final,Exchange);
    if (value == -1)
    	cout << "\nAlternate Basic Solution Exists.";
    return;
}

int main()						// main function where the program initiates
{
	int n,m,choice,** Points;					// initializing the required variables
	int *Exchange;								// array to store the indices which have been exchanged
	float **A,*B,*Z;
	cout << "Enter the Number of Variables:\n";		// reading in the required inputs
	cin >> n;
	cout << "Enter the Number of Equations:\n";
	cin >> m;
	Points = (int**)malloc((m+n)*sizeof(int*));		// hence allocating space
	Exchange = (int*)malloc((m+n)*sizeof(int));
	for(int i=0;i<m+n;i++)
		Points[i] = (int*)malloc(2*sizeof(int));
	A = (float**)malloc(m*sizeof(float*));		// allocating space for the main matrix
	for(int i=0;i<m;i++)
		A[i] = (float*)malloc(n*sizeof(float));
	for(int i=0;i<n+m;i++)
		Exchange[i] = i+1;
	B = (float*)malloc(m*sizeof(float));			// space for the B and Z arrays
	Z = (float*)malloc((n+1)*sizeof(float));
    choice = Take_input(A,B,Z,n,m);					// function which would take input
    Solve(A,B,Z,n,m,Points,choice,Exchange);					// function which would solve the whole problem
    cout << "__________________________________SOLUTION ENDS__________________________________________\n";
	return 0;
}
