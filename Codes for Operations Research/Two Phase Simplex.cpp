/*
NAME -> Varun Gupta
ROLL NUMBER -> 18MA20050
OR LAB WEEK 5 ASSIGNMENT
This is the code to implement the Two Phase Simplex Algorithm by first reducing the function made by artificial variables to zero which completes the Phase 1 and gives us the initial BFS to use for the basic variables
and then we use the normal Simplex method to find the final values of the basic variables which completes the Phase 2 of the Algorithm and hence we evaluate the Optimal Solution.
*/

#include <bits/stdc++.h>			// including the required libraries
using namespace std;		// using namespace for the required functions

void Print_Table(double** Table,int N,int M,int surplus)		// function that would print the Simplex Table Whenever required
{
    for(int i=0;i<M+1;i++)			// hence printing the values
	{
		for(int j=0;j<N+surplus+1;j++)
		{
			if(-100<Table[i][j] && Table[i][j]<100)
				cout << fixed << setprecision(6) << Table[i][j] << "\t";
			else
				cout << fixed << setprecision(2) <<  Table[i][j] << "\t";
		}
		cout << "\n";
	}
	return;
}

void Evaluate_Cost(double** Table,float* Z,int* Final,int N,int M,int surplus,int* sign,int phase)		// the function which would evaluate the value of Cj - Zj on the basis of required phase
{
	double val;
	if(phase == 1)															// if phase 1
	{
		double* Cost = (double*)malloc(M*sizeof(double));				// then we would evaluate Zj frorm cost of Artificial variables and take cost of other variables as 0
		for(int i=0;i<M;i++)
		{
			if(sign[i] == 3 || sign[i] == -1)						// giving cost on basis of sign
				Cost[i] = 0.0;
			else
				Cost[i] = -1.0;
		}
		for(int i=0;i<N+surplus+1;i++)							// hence evaluating the Zj
		{
			val = 0.0;
			for(int j=0;j<M;j++)
			{
				val += Cost[j]*Table[j][i];
			}
			if(val == 0.0)							// hence putting the values
				Table[M][i] = val;
			else
				Table[M][i] = (-1.0)*val;
		}
	}
	else 																// if we do for phase 2
	{
		double* Cost = (double*)malloc((N+surplus+1)*sizeof(double));		// here we would take for all the decision and surplus variables
		int index;
		double val;
		for(int i=0;i<N;i++)									// hence storing the cost
			Cost[i] = Z[i];
		for(int i=N;i<N+surplus;i++)
			Cost[i] = 0.0;									// surplus cost is 0.0
		for(int i=0;i<N+surplus+1;i++)				// saving the coefficient of the decision variables
		{
			val = 0.0;
			for(int j=0;j<M;j++)					// hence evaluting Zj
			{
				index = -1;
				if(Final[j]!=-1)						// storing the latest basic variable cost
					index = Final[j];
				if(index == -1)
					continue;
				else
					val += Cost[index]*Table[j][i];			// evaluating
			}
			if(i!=N+surplus)
				Table[M][i] = Cost[i] - val;				// hence subtracting for all except the optimal value
			else
				Table[M][i] = val;

		}
	}
	return;
}

double** Build_Phase_1_Table(float** A,float *B,float *Z,int N,int M,int* sign,int surplus,int choice)	// function to build the initial Phase 1 Simplex Table,
{
	double** Table,val;									// initializing the variable for the Simplex Table
	double* Cost = (double*)malloc(M*sizeof(double));
	Table = (double**)malloc((M+1)*sizeof(double*));
	for(int i=0;i<M+1;i++)
		Table[i] = (double*)malloc((N+1+surplus)*sizeof(double));
	int place = 0;					// cost coefficient on the basis of type of variable
	for(int i=0;i<M;i++)
	{
		if(sign[i] == 3)
			Cost[i] = 0.0;
		else
			Cost[i] = -1.0;
	}
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
	for(int i=0;i<N+surplus+1;i++)				// saving the cost values
	{
		val = 0.0;
		for(int j=0;j<M;j++)
		{
			val += Cost[j]*Table[j][i];
		}
			Table[M][i] = -val;
	}
	return Table;		// returning the Simplex Table
}

double** Build_Phase_2_Simplex_Table(double** table,int* Final,float** A,float *B,float *Z,int N,int M,int* sign,int surplus,int choice)	// function to build the Phase 2 Simplex Table
{
	double** Table,val;									// initializing the variable for the Simplex Table
	int index;
	double* Cost = (double*)malloc((N+surplus+1)*sizeof(double));		// array to store the variables cost
	Table = (double**)malloc((M+1)*sizeof(double*));	// hence, allocating space for the Simplex Table
	for(int i=0;i<M+1;i++)
		Table[i] = (double*)malloc((N+1+surplus)*sizeof(double));
	for(int i=0;i<M;i++)			// hence placing all the elements from the given A,B,Z matrices
	{
		for(int j=0;j<N+1+surplus;j++)
		{
			Table[i][j] = table[i][j];
		}
	}
	for(int i=0;i<N;i++)				// saving the cost of the decision variables
		Cost[i] = Z[i];
	for(int i=N;i<N+surplus;i++)				// saving the cost of the surplus variables
		Cost[i] = 0.0;
	for(int i=0;i<N+surplus;i++)				// hence evaluating cost on the basis of basic variables
	{
		val = 0.0;
		for(int j=0;j<M;j++)
		{
			index = -1;
			if(Final[j]!=-1)
				index = Final[j];
			if(index == -1)
				continue;
			else
				val += Cost[index]*Table[j][i];			// evaluating Zj
		}
		if(i!=N+surplus)
			Table[M][i] = Cost[i] - val;			// subtracting
		else
			Table[M][i] = val;
	}
	return Table;		// returning the initial Simplex Table
}

int CheckPivot(double** Table,int** Points,int N,int M,int iteration,int* sign,int surplus,int* Final)		// code to evaluate the pivot element and the minimum ratio, and hence returns the pivot row and column
{
	bool unbounded = true;				// variable to check if the solution is unbounded or not
	bool flag = true;					// a flag to check if the pivot has been found
	double maxi,min;							// variable to find the minimum value of in the last row,pivot column of the Simplex tableau
	int index,index2;							// the index for the particular column,row of the pivot element
	for(int i=0;i<N+surplus;i++)		// finding the column of the most negative element
	{
		if(flag)		// if not element has been saved , saving the first element
		{
			maxi = Table[M][i];
			index = i;
			flag = false;
		}
		else
		{
			if(maxi < Table[M][i])		// else checking if any element is smaller
			{
				maxi = Table[M][i];
				index = i;
			}
		}
	}
	if(maxi > 0)				// if we have found the minimum element as negative
	{
		for(int i=0;i<M;i++)		// now finding the minimum ratio element in the pivot element column
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
	else if(maxi < 0)					// ending the program by printing the already found optimal solution
	{
		return 0;
	}
	else 								// checking if any alternate solution would exist
	{
		return -1;
	}
}

void Evaluate_Simplex_Table(double** Table,int** Points,int N,int M,int iteration,int surplus)		// code to revise the simplex table and complete one iteration, and hence print the next solution, and object value
{
	int row,col;										// variable for storing for the individual pivot points found in each iteration
	row = Points[iteration][0];					// evaluating one pivot point
	col = Points[iteration][1];
	double value = Table[row][col];
	for(int i=0;i<N+surplus+1;i++)			// hence evaluating the Simplex Tableua on the basis of Simplex Algorithm
	{
		Table[row][i] /= value;
	}
	for(int i=0;i<M;i++)	// now evaluating new values for the particular pivot column
	{
		if(i==row)		// for the pivot element, skipping else evaluating
			continue;
		value = Table[i][col];
		for(int j=0;j<N+surplus+1;j++)
		{
			Table[i][j] -= value*Table[row][j];
		}
	}
	return;
}

int Take_input(float** A,float* B,int* sign,int* signend,float* Z,int N,int M)						// function to take the input
{
	int choice;
	float val;
	cout << "\nNow, Enter the values of A:\n";					// hence inputting values of A
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
	        Z[i] = val;							// hence inputting the values of the optimal equation
	    }
	    Z[N] = 0.0;
    }
    else
    {
    	for (int i = 0; i < N; i++)
	    {
	        cout << "Enter coefficient of X[" << i+1 <<"], i.e. Z[" << i+1 << "] = ";					// hence inputting the coefficients
	        cin >> val;
	        Z[i] = (-1.0)*val;							// hence inputting the values of the optimal equation
	    }
	    Z[N] = 0.0;
    }
    cout << "\n";
    return choice;			// returns the int which signifies which tyoe of problem this would be
}

void TwoPhase(float** A,float* B,float* Z, int N,int M,int** Points,int choice,int *sign,int* signend,int surplus,int artificial,int slack,int * Final)
{
	int iteration = 0,value;
	double** table;
    table = Build_Phase_1_Table(A,B,Z,N,M,sign,surplus,choice);			// function builds the First Phase Simplex Table
	cout << "------------Phase 1 Starts--------------------\n";
	cout << "\n------------Initial Simplex Table--------------------\n";
	Print_Table(table,N,M,surplus);
    value = CheckPivot(table,Points,N,M,iteration,sign,surplus,Final);
    while(value == 1)
    {
    	cout << "\n";
    	cout << "------------Iteration Number: " << iteration + 1 << " --------------------\n";
    	cout << "The Pivot Row is " << Points[iteration][0] + 1 << " and the Pivot Column is " << Points[iteration][1] + 1 << ".\n";
    	cout << "Hence, The Pivot Element from Iteration Number " << iteration + 1 << " is " << table[Points[iteration][0]][Points[iteration][1]] << ".\n";
    	Evaluate_Simplex_Table(table,Points,N,M,iteration,surplus);		// function that would evaluate all the Pivot Points
    	Evaluate_Cost(table,Z,Final,N,M,surplus,sign,1);
    	iteration++;
		Print_Table(table,N,M,surplus);
	    value = CheckPivot(table,Points,N,M,iteration,sign,surplus,Final);
    }
    cout << "\n------------Phase 1 Ends--------------------\n";
	bool check = false;
	for(int i=0;i<M;i++)
	{
		if(sign[i] == 1 || sign[i] == 2)
		{
			check = true;
			break;
		}
	}
	if(check)
	{
		cout << "All the Artificial Variables Value haven't been reduced to zero.\n";
		cout << "Hence,The problem has an Infeasible Solution.\n";
	}
	else
	{
		int iteration2 = 0;
		value;
		double** table2;
	    table2 = Build_Phase_2_Simplex_Table(table,Final,A,B,Z,N,M,sign,surplus,choice);			// function builds the initial Siimplex Tableau
		cout << "\n------------Phase 2 Starts--------------------\n";
		Print_Table(table2,N,M,surplus);
	    value = CheckPivot(table2,Points,N,M,iteration,sign,surplus,Final);
	    if(value != 1)
	    {
	    	cout << "Hence, There were no negative values in the Last Row of the Simplex Table.\n";
	    	cout << "Thereforce, No other Iteration would be done in Phase 2.\n";
	    	Evaluate_Cost(table2,Z,Final,N,M,surplus,sign,2);
	    }
		while(value == 1)
		{
			cout << "\n";
	    	cout << "------------Iteration Number: " << iteration2 + 1 << " --------------------\n";
	    	cout << "The Pivot Row is " << Points[iteration][0] + 1 << " and the Pivot Column is " << Points[iteration][1] + 1 << ".\n";
	    	cout << "Hence, The Pivot Element from Iteration Number " << iteration2 + 1 << " is " << table[Points[iteration][0]][Points[iteration][1]] << ".\n";
	    	Evaluate_Simplex_Table(table2,Points,N,M,iteration,surplus);		// function that would evaluate all the Pivot Points
	    	Evaluate_Cost(table2,Z,Final,N,M,surplus,sign,2);
	    	iteration++;
	    	iteration2++;
			Print_Table(table2,N,M,surplus);
		    value = CheckPivot(table2,Points,N,M,iteration,sign,surplus,Final);
		}
	    cout << "\n------------Phase 2 Ends--------------------\n";
		cout << "\nThe Optimal Solution is as follows:\n";
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
					cout << "X[" << value + 1 << "] = " << fixed << setprecision(2) << table2[i][N+surplus] << "\n";
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
					cout << "X[" << value + 1 << "] = " << fixed << setprecision(2) << table2[j][N+surplus] << "\tSlack Variable.\n";
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
					cout << "X[" << value + 1 << "] = " << fixed << setprecision(2) << table2[j][N+surplus] << "\tArtificial Variable.\n";
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
						cout << "X[" << value + 1 << "] = " << fixed << setprecision(2) << table2[Points[i][0]][N+surplus] << "\tSurplus Variable.\n";
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
					cout << "X[" << value + 1 << "] = " << fixed << setprecision(2) << table2[j][N+surplus] << "\tArtificial Variable.\n";
				value++;
				continue;
			}
		}
		if(choice == 1)
			cout << "\nThe Optimal value is Z = " << fixed << setprecision(2) << table2[M][N+surplus] << "\n";		// outputting the optimal value of the function
		else
			cout << "\nThe Optimal value is Z = " << fixed << setprecision(2) << -1*table2[M][N+surplus] << "\n";		// outputting the optimal value of the function
    }
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
    TwoPhase(A,B,Z,n,m,Points,choice,sign,signend,surplus,artificial,slack,Final);					// function which would solve the whole problem
	return 0;
}
