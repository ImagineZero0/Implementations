/*
NAME -> Varun Gupta
ROLL NUMBER -> 18MA20050
OR LAB WEEK 4 ASSIGNMENT
This is the code to implement the BigM Simplex Algorithm by using the similar Simplex Algorithm but this also includes the case of greater than and equal to inequalities and hence
makes us make use of Artificial and Surplus Variables which modifies the Simplex Algorithm a little and hence we evaluate the Optimal Solution.
*/

#include <bits/stdc++.h>			// including the required libraries
using namespace std;		// using namespace for the required functions

void Print_Table(double** Table,int N,int M,int surplus)		// function that would print the Simplex Table Whenever required
{
    for(int i=0;i<M+1;i++)			// hence printing the values
	{
		for(int j=0;j<N+surplus+1;j++)
		{
			if(-10<Table[i][j] && Table[i][j]<10)
				cout << fixed << setprecision(6) << Table[i][j] << "\t";
			else
				cout << fixed << setprecision(2) <<  Table[i][j] << "\t";
		}
		cout << "\n";
	}
	return;
}

double** Build_Simplex_Table(float** A,float *B,float *Z,int N,int M,int* sign,int surplus,int choice)	// function to build the initial Simplex Table, and code to store the Zj - Cj values
{
	double** Table;									// initializing the variable for the Simplex Table
	double* coefficient = (double*)malloc((N+surplus+1)*sizeof(double));		// array to store the variables coefficients multiplied with large M value
	long long int BM = 1e6;
	Table = (double**)malloc((M+1)*sizeof(double*));	// hence, allocating space for the Simplex Table
	for(int i=0;i<M+1;i++)
		Table[i] = (double*)malloc((N+1+surplus)*sizeof(double));
	int place = 0;
	for(int i=0;i<M;i++)			// hence placing all the elements from the given A,B,Z matrices
	{
		for(int j=0;j<N+1+surplus;j++)
		{
			if(j<N)
				Table[i][j] = A[i][j];
			else if (j < N+surplus)			// initially given zeroes for all columns of Surplus variables
				Table[i][j] = 0.0;
			else
				Table[i][j] = B[i];
		}
		if(sign[i] == 1)				// giving the respective columns with value of -1.0 for surplus varaibles
		{
			Table[i][N+place] = -1.0;
			place++;
		}
	}
	for(int i=0;i<N;i++)				// saving the coefficient of the decision variables
		Table[M][i] = Z[i];
	for(int i=N;i<N+surplus+1;i++)		// saving the coefficients of surplus variables in Z
		Table[M][i] = 0.0;
	for(int i=0;i<N+surplus+1;i++)		// now finding the terms which would have Big M as coefficients in Z
		coefficient[i] = 0.0;
	for(int i=0;i<M;i++)
	{
		if(sign[i] == 1 || sign[i] == 2)		// hence if artificial variables is present
		{
			for(int j=0;j<N+surplus+1;j++)
			{
				coefficient[j] += Table[i][j];			// hence summing the coefficients
			}
		}
	}
	for(int i=0;i<N+surplus+1;i++)
		Table[M][i] -= coefficient[i]*BM;				// hence taking the effect of coefficients in the Simplex Table
	return Table;		// returning the initial Simplex Table
}

int CheckPivot(double** Table,int** Points,int N,int M,int iteration,int* sign,int surplus,int* Final)		// code to evaluate the pivot element and the minimum ratio, and hence returns the pivot row and column
{
	bool unbounded = true;				// variable to check if the solution is unbounded or not
	bool flag = true;					// a flag to check if the pivot has been found
	float min;							// variable to find the minimum value of in the last row,pivot column of the Simplex tableau
	int index,index2;							// the index for the particular column,row of the pivot element
	for(int i=0;i<N+surplus;i++)		// finding the column of the most negative element
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
					min = Table[i][N+surplus]/Table[i][index];		// saving the minimim ratio
					index2 = i;						// saving the index of that row
					unbounded = false;				// false for flag when one element has been saved
				}
				else
				{
					if(min > Table[i][N+surplus]/Table[i][index])		// for future elements, if ratio is smaller than previous ratio
					{
						min = Table[i][N+surplus]/Table[i][index];		// saving the minimum ratio
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
			Final[index2] = index;
			sign[index2] = -1;
			return 1;
		}
	}
	else if(iteration==0)			// if minimum element is not negative and no negative minimum element has been found in the last row
	{
		for(int i=0;i<M+N+surplus;i++)
		{
			cout << "X[" << i+1 << "] = 0 ";			// hence printing the values of X
		}
		cout << "The Optimal value of Z is 0.00.\n";		// then the maximum value is the constant term and all values of variables are 0
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

void Evaluate_Simplex_Table(double** Table,int** Points,int N,int M,int iteration,int *sign,int surplus)		// code to revise the simplex table and complete one iteration, and hence print the next solution, and object value
{
	int Point[2];										// variable for storing for the individual pivot points found in each iteration
	Point[0] = Points[iteration][0];					// evaluating one pivot point
	Point[1] = Points[iteration][1];
	for(int i=0;i<M+1;i++)			// hence evaluating the Simplex Tableua on the basis of Simplex Algorithm
	{
		if(i==Point[0])				// if the pivot row, then skipping
			continue;
		for(int j=0;j<N+1+surplus;j++)		// else finding the new value of the point
		{
			if(j==Point[1])			// if that particular column then skipping
				continue;
			Table[i][j] = (Table[i][j]*Table[Point[0]][Point[1]] - Table[i][Point[1]]*Table[Point[0]][j]);
			Table[i][j] /= Table[Point[0]][Point[1]];
		}
	}

	for(int i=0;i<N+1+surplus;i++)		// now evaluating new values for the particular pivot row
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

int Take_input(float** A,float* B,int* sign,int* signend,float* Z,int N,int M)						// function to take the input
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
        cout << "Now, Enter the number of the Inequality Sign on the basis of:\n1)>=\n2)=\n3)<=\n";
        cout << "A[" << i+1 << "][Sign] = ";
        cin >> sign[i];
        signend[i] = sign[i];
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

void BigM(float** A,float* B,float* Z, int N,int M,int** Points,int choice,int *sign,int* signend,int surplus,int artificial,int slack,int * Final)
{
	int iteration = 0,value;
	double** table;
    table = Build_Simplex_Table(A,B,Z,N,M,sign,surplus,choice);			// function builds the initial Siimplex Tableau
	cout << "------------Initial Simplex Table: " << iteration + 1 << " --------------------\n";
	Print_Table(table,N,M,surplus);
    value = CheckPivot(table,Points,N,M,iteration,sign,surplus,Final);
    while(value == 1)
    {
    	cout << "\n";
    	cout << "------------Iteration Number: " << iteration + 1 << " --------------------\n";
    	cout << "The Pivot Row is " << Points[iteration][0] + 1 << " and the Pivot Column is " << Points[iteration][1] + 1 << ".\n";
    	cout << "Hence, The Pivot Element from Iteration Number " << iteration + 1 << " is " << table[Points[iteration][0]][Points[iteration][1]] << ".\n";
    	Evaluate_Simplex_Table(table,Points,N,M,iteration,sign,surplus);		// function that would evaluate all the Pivot Points
    	iteration++;
		Print_Table(table,N,M,surplus);
	    value = CheckPivot(table,Points,N,M,iteration,sign,surplus,Final);
    }
	cout << "\nThe Optimal Solution is as follows:\n";
	bool check;
	value = 0;
	while(value < N)
	{
		check = true;					// checking if the variables pivot point has been found or not
		// for(int i=max(0,iteration-artificial-slack);i<iteration;i++)
		for(int i=0;i<slack+artificial;i++)
		{
			if(Final[i] == value)
			{
				check = false;				// if found than false
				cout << "X[" << value + 1 << "] = " << fixed << setprecision(2) << table[Points[i][0]][N+surplus] << "\n";
				break;
			}
		}
		if(check)			// if not than that elements value is 0
			cout << "X[" << value + 1 << "] = 0\n";
		value++;
	}
	int value2 = N;
	for(int j=0;j<M;j++)
	{
		if(signend[j] == 3)
		{
			check = true;
			// for(int i=max(0,iteration-artificial-slack);i<iteration;i++)
			for(int i=0;i<iteration;i++)
			{
				if(Points[i][0] == j)
				{
					check = false;				// if found than false
					cout << "X[" << value + 1 << "] = 0\tSlack Variable.\n";
					break;
				}
			}
			if(check)			// if not than that elements value is 0
				cout << "X[" << value + 1 << "] = " << fixed << setprecision(2) << table[j][N+surplus] << "\tSlack Variable.\n";
			value++;
			continue;
		}
		else if(signend[j] == 2)
		{
			check = true;
			// for(int i=max(0,iteration-artificial-slack);i<iteration;i++)
			for(int i=0;i<iteration;i++)
			{
				if(Points[i][0] == j)
				{
					check = false;				// if found than false
					cout << "X[" << value + 1 << "] = 0\tArtificial Variable.\n";
					break;
				}
			}
			if(check)			// if not than that elements value is 0
				cout << "X[" << value + 1 << "] = " << fixed << setprecision(2) << table[j][N+surplus] << "\tArtificial Variable.\n";
			value++;
			continue;
		}
		else if(signend[j] == 1)
		{
			check = true;					// checking if the variables pivot point has been found or not
			for(int i=max(0,iteration-artificial-slack);i<iteration;i++)
			{
				if(Points[i][1] == value2)
				{
					check = false;				// if found than false
					cout << "X[" << value + 1 << "] = " << fixed << setprecision(2) << table[Points[i][0]][N+surplus] << "\tSurplus Variable.\n";
					break;
				}
			}
			if(check)			// if not than that elements value is 0
				cout << "X[" << value + 1 << "] = 0\tSurplus Variable.\n";
			value++;
			value2++;
			check = true;
			// for(int i=max(0,iteration-artificial-slack);i<iteration;i++)
			for(int i=0;i<iteration;i++)
			{
				if(Points[i][0] == j)
				{
					check = false;				// if found than false
					cout << "X[" << value + 1 << "] = 0\tArtificial Variable.\n";
					break;
				}
			}
			if(check)			// if not than that elements value is 0
				cout << "X[" << value + 1 << "] = " << fixed << setprecision(2) << table[j][N+surplus] << "\tArtificial Variable.\n";
			value++;
			continue;
		}
	}
	if(choice == 1)
		cout << "\nThe Optimal value is Z = " << fixed << setprecision(2) << table[M][N+surplus] << "\n";		// outputting the optimal value of the function
	else
		cout << "\nThe Optimal value is Z = " << fixed << setprecision(2) << -1*table[M][N+surplus] << "\n";		// outputting the optimal value of the function
	check = false;
	for(int i=0;i<M;i++)
	{
		if(sign[i] == 1 || sign[i] == 2)
		{
			check = true;
			break;
		}
	}
	if(check)
		cout << "The problem has an infeasible Solution.\n";
    return;
}

int main()						// main function where the program initiates
{
	int n,m,choice,** Points,* sign,*signend;					// initializing the required variables
	int slack = 0, artificial = 0, surplus = 0;
	float **A,*B,*Z;
	cout << "Enter the Number of Variables:\n";		// reading in the required inputs
	cin >> n;
	cout << "Enter the Number of Equations:\n";
	cin >> m;
	sign = (int*)malloc(m*sizeof(int));
	signend = (int*)malloc(m*sizeof(int));
	A = (float**)malloc(m*sizeof(float*));		// allocating space for the main matrix
	for(int i=0;i<m;i++)
		A[i] = (float*)malloc(n*sizeof(float));
	B = (float*)malloc(m*sizeof(float));			// space for the B and Z arrays
	Z = (float*)malloc((n+1)*sizeof(float));
    choice = Take_input(A,B,sign,signend,Z,n,m);					// function which would take input
    for(int i=0;i<m;i++)
    {
    	if(sign[i] == 1)
    	{
    		artificial++;
    		surplus++;
    	}
    	else if(sign[i] == 2)
    		artificial++;
    	else
    		slack++;
    }
	Points = (int**)malloc((m+n+surplus)*sizeof(int*));		// hence allocating space
	int Final[artificial+slack];
	for(int i=0;i<artificial+slack;i++)
		Final[i] = -1;
	for(int i=0;i<m+n+surplus;i++)
		Points[i] = (int*)malloc(2*sizeof(int));
    BigM(A,B,Z,n,m,Points,choice,sign,signend,surplus,artificial,slack,Final);					// function which would solve the whole problem
	return 0;
}
