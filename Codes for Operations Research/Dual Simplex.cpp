/*
NAME -> Varun Gupta
ROLL NUMBER -> 18MA20050
OR LAB WEEK 6 ASSIGNMENT
This is the code to implement the Dual Simplex Algorithm by first transforming the given equations and optimality condition in all constraints as <= to get only slack varaibles and the optimality condition as
Maximization type, after this we apply our normal Simplex Algorithm and hence find our Optimal Solution.
*/

#include <bits/stdc++.h>	// including the required libraries
using namespace std;		// using namespace for the required functions

void Print_Table(double** Table,int N,int M);		// function that would print the Simplex Table Whenever required
void Evaluate_Cost(double** Table,double* Z,int* Final,int N,int M);		// the function which would evaluate the value of Cj - Zj for each particular iteration of a Simlpex Table
double** Build_Simplex_Table(double** A,double *B,double *Z,int N,int M);	// function to build the initial Simplex Table
int CheckPivot(double** Table,int** Points,int N,int M,int iteration,int* Final,int* sign,int menu);		// code to evaluate the pivot element and the minimum positive ratio, and hence returns the pivot row and column
void Evaluate_Simplex_Table(double** Table,int** Points,int N,int M,int iteration);		// code to revise the simplex table and complete one iteration, and hence print the next solution, and object value
int* Take_input(double** A,double* B,int* sign,int* signend,double* Z,int N,int M);						// function to take the input
void Menu(double** table,int** Points,int*Final,double** A,double* B,double* Z,int N,int M,int choice,int iteration,int* sign,int answer); 			// the function which defines the menu driven program
void OptimalAnswer(double** A,double* B,double* Z,double** table,int** Points,int* Final,int N,int M,int iteration,int value,int menu,int choice,int *sign);			// function that would print the Optimal Solution
void DualSimplex(double** A,double* B,double* Z, int N,int M,int** Points,int choice,int * Final,int *sign);		// main function that calls the dual simplex algorithm
double** BigMBuild_Simplex_Table(double** A,double *B,double *Z,int N,int M,int* sign,int surplus,int choice);
int BigMCheckPivot(double** Table,int** Points,int N,int M,int iteration,int* sign,int surplus,int* Final);
void BigMEvaluate_Simplex_Table(double** Table,int** Points,int N,int M,int iteration,int *sign,int surplus);
void BigMPrint_Table(double** Table,int N,int M,int surplus);
void BigMOptimalAnswer(double** A,double* B,double* Z,double** table2,int** Points,int* Final,int N,int M,int iteration,int value,int menu,int choice,int* sign,int artificial,int slack,int surplus,int* signend);
void BigMMenu(double** table,int** Points,int*Final,double** A,double* B,double* Z,int N,int M,int choice,int iteration,int* sign,int artificial,int surplus,int slack,int* signend);
void BigM(double** A,double* B,double* Z, int N,int M,int** Points,int choice,int *sign,int* signend,int surplus,int artificial,int slack,int * Final);

int main()						// main function where the program initiates
{
	cout << "\n---------------PROGRAM STARTS---------------\n";
	int n,m,choice,**Points,*sign,*signend,dual,*choices;					// initializing the required variables
	int slack = 0, artificial = 0, surplus = 0;
	double **A,*B,*Z;
	cout << "Enter the Number of Variables:\n";		// reading in the required inputs
	cin >> n;
	cout << "Enter the Number of Equations:\n";
	cin >> m;
	sign = (int*)malloc(m*sizeof(int));
	signend = (int*)malloc(m*sizeof(int));
	A = (double**)malloc(m*sizeof(double*));		// allocating space for the main matrix
	for(int i=0;i<m;i++)
		A[i] = (double*)malloc(n*sizeof(double));
	B = (double*)malloc(m*sizeof(double));			// space for the B and Z arrays
	Z = (double*)malloc((n+1)*sizeof(double));
    choices = Take_input(A,B,sign,signend,Z,n,m);					// function which would take input
    choice = choices[0];
    dual = choices[1];
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
	int* Final;
	Final = (int*)malloc(m*sizeof(int));
	for(int i=0;i<m;i++)
		Final[i] = -1;
	for(int i=0;i<m+n+surplus;i++)
		Points[i] = (int*)malloc(2*sizeof(int));
	if(dual)
		cout << "\nHence,The given problem is primal infeasible and dual feasible.\nTherefore,using Dual Simplex Method.\n";
	else
		cout << "\nHence,The given problem is primal feasible.\nTherefore,using BigM(Simplex) Method.\n";
	if(dual)
	    DualSimplex(A,B,Z,n,m,Points,choice,Final,sign);					// function which would solve the whole problem
	else
		BigM(A,B,Z,n,m,Points,choice,sign,signend,surplus,artificial,slack,Final);
	return 0;
}

void Print_Table(double** Table,int N,int M)		// function that would print the Simplex Table Whenever required
{
    for(int i=0;i<M+1;i++)			// hence printing the values
	{
		for(int j=0;j<N+M+1;j++)
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

void BigMPrint_Table(double** Table,int N,int M,int surplus)		// function that would print the Simplex Table Whenever required
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

int* Take_input(double** A,double* B,int* sign,int* signend,double* Z,int N,int M)						// function to take the input
{
	int *values = (int*)malloc(2*sizeof(int));
	values[0] = 0;
	values[1] = 0;
	int choice,dual=0;
	double val;
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
        if(sign[i] == 1 && B[i] > 0)
        	dual = 1;
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
	    	if(Z[i] > 0)
	    		dual = 0;
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
	    	if(Z[i] > 0)
	    		dual = 0;
	    }
	    Z[N] = 0.0;
    }
    cout << "\n";
    if(dual)
    {
    	for(int i=0;i<M;i++)
    	{
			 if(sign[i] == 1)
	        {
	        	for(int j=0;j<N;j++)
	        		A[i][j] *= (-1.0);
	        	B[i] *= (-1.0);
	        }
    	}
    }
    else
    {
    	for (int i = 0; i < N; i++)
    		Z[i] *= (-1.0);
    }
   	values[0] = choice;
   	values[1] = dual;
    return values;
}

void DualSimplex(double** A,double* B,double* Z, int N,int M,int** Points,int choice,int * Final,int* sign)		// main function that calls the dual simplex algorithm
{
	int iteration = 0,value;			// initializing the required varaibles
	bool check;
	double** table;
    table = Build_Simplex_Table(A,B,Z,N,M);			// function builds the initial Siimplex Tableau
	cout << "------------Initial Simplex Table--------------------\n";
	Print_Table(table,N,M);
    value = CheckPivot(table,Points,N,M,iteration,Final,sign,1);			// finding the first pivot element
    while(value == 1)			// hence continuing till a solution is found
    {
    	cout << "\n";
    	cout << "------------Iteration Number: " << iteration + 1 << " --------------------\n";
    	cout << "The Pivot Row is " << Points[iteration][0] + 1 << " and the Pivot Column is " << Points[iteration][1] + 1 << ".\n";
    	cout << "Hence, The Pivot Element from Iteration Number " << iteration + 1 << " is " << table[Points[iteration][0]][Points[iteration][1]] << ".\n";
    	cout << "Hence, The Updated Simplex Table is as follows:\n\n";
    	Evaluate_Simplex_Table(table,Points,N,M,iteration);		// function that would evaluate all the Pivot Points
    	Evaluate_Cost(table,Z,Final,N,M);						// function to evaluate the cost of this simplex table
    	iteration++;
		Print_Table(table,N,M);
	    value = CheckPivot(table,Points,N,M,iteration,Final,sign,1);		// checking pivot for new simplex table
    }
	cout << "\n------------Final Simplex Table(Above Table)--------------------\n";
	OptimalAnswer(A,B,Z,table,Points,Final,N,M,iteration,value,1,choice,sign);			// hence printing the Solution
    return;
}

double** Build_Simplex_Table(double** A,double *B,double *Z,int N,int M)	// function to build the initial Simplex Table
{
	double **Table;
	int place = 0;									// initializing the variable for the Simplex Table
	Table = (double**)malloc((M+1)*sizeof(double*));	// hence, allocating space for the Simplex Table
	for(int i=0;i<M+1;i++)
		Table[i] = (double*)malloc((N+M+1)*sizeof(double));
	for(int i=0;i<M;i++)			// hence placing all the elements from the given A,B,Z matrices
	{
		for(int j=0;j<N+M+1;j++)
		{
			if(j<N)
				Table[i][j] = A[i][j];
			else if(j<N+M)
				Table[i][j] = 0.0;
			else
				Table[i][j] = B[i];
		}
		Table[i][N+place] = 1.0;
		place++;

	}
	for(int i=0;i<N;i++)				// hence evaluating cost on the basis of basic variables
		Table[M][i] = Z[i];
	for(int i=N;i<N+M;i++)				// hence evaluating cost on the basis of basic variables
		Table[M][i] = 0.0;
	return Table;		// returning the initial Simplex Table
}

int CheckPivot(double** Table,int** Points,int N,int M,int iteration,int* Final,int* sign,int menu)		// code to evaluate the pivot element and the minimum positive ratio, and hence returns the pivot row and column
{
	bool infeasible = true;					// varaible to verify if the given problem is unbounded
	double mini,mini2;							// variable to find the minimum value of in the last row,pivot column of the Simplex tableau
	int index=-1,index2=-1;							// the index for the particular column,row of the pivot element
	for(int i=0;i<M;i++)		// finding the row of the most negative element using the basic values
	{
		if(index == -1)		// if not element has been saved , saving the first element
		{
			mini = Table[i][N+M];
			index = i;
		}
		else
		{
			if(Table[i][N+M] < mini)		// else checking if any element is smaller
			{
				mini = Table[i][N+M];
				index = i;
			}
		}
	}
	if(mini < 0)				// if we have found the minimum element as negative
	{
		for(int i=0;i<N+M;i++)		// now finding the minimum positive ratio element in the pivot element row
		{
			if(Table[index][i] < 0)		// only if that element in negative, as negative/negative would give postive ratio
			{
				if(index2 == -1)				// and if no first element has been saved
				{
					infeasible = false;									// hence not unbounded
					mini2 = (Table[M][i]/Table[index][i]);		// saving the minimim ratio
					index2 = i;						// saving the index of that column
				}
				else
				{
					if((Table[M][i]/Table[index][i]) < mini2)		// for future elements, if ratio is smaller than previous ratio
					{
						mini2 =( Table[M][i]/Table[index][i]);		// saving the minimum ratio
						index2 = i;								// sasving index of the minimum ratio
					}
				}
			}
		}
		if(infeasible)						// if infeasible
			return 2;
		else
		{
			Points[iteration][0] = index;			// hence saving the indices of that pivot element
			Points[iteration][1] = index2;
			Final[index] = index2;					// saving that particular rows change with what column
			if(menu)
				sign[index] = -1;
			return 1;
		}
	}
	else if(iteration==0)			// no basic feasible solution found by this method.
	{
		cout << "Hence, The Method fails to get optimal basic feasible Solution.\n";
		return 4;
	}
	else if(mini == 0)		// for alternative optimal solution
		return 0;
	else if(mini > 0)			// for only optimal solution
		return 3;
}

void Evaluate_Simplex_Table(double** Table,int** Points,int N,int M,int iteration)		// code to revise the simplex table and complete one iteration, and hence print the next solution, and object value
{
	int row,col;										// variable for storing for the individual pivot points found in each iteration
	row = Points[iteration][0];					// evaluating one pivot point
	col = Points[iteration][1];
	double value = Table[row][col];
	for(int i=0;i<N+M+1;i++)			// hence evaluating the Simplex Tableua on the basis of Simplex Algorithm
		Table[row][i] /= value;
	for(int i=0;i<M;i++)	// now evaluating new values for the particular pivot column
	{
		if(i==row)		// for the pivot element, skipping else evaluating
			continue;
		value = Table[i][col];
		for(int j=0;j<N+M+1;j++)
			Table[i][j] -= value*Table[row][j];
	}
	return;
}

void Evaluate_Cost(double** Table,double* Z,int* Final,int N,int M)		// the function which would evaluate the value of Cj - Zj for each particular iteration of a Simlpex Table
{
	double val;
	double *Cost = (double*)malloc((N+M+1)*sizeof(double));		// here we would take for all the decision variables
	int index;
	for(int i=0;i<N;i++)									// hence storing the cost
		Cost[i] = Z[i];
	for(int i=N;i<N+M+1;i++)									// hence storing the cost
		Cost[i] = 0.0;
	for(int i=0;i<N+M+1;i++)				// saving the coefficient of the decision variables
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
		if(i<N+M)
			Table[M][i] = Cost[i] - val;				// hence subtracting for all except the optimal value
		else
			Table[M][i] = val;
	}
	return;
}

void OptimalAnswer(double** A,double* B,double* Z,double** table,int** Points,int* Final,int N,int M,int iteration,int value,int menu,int choice,int* sign)			// function that would print the Optimal Solution
{
	bool check;
	int values = 0;
	cout << "\nThe Optimal Solution is as follows:\n";
	while(values < N)
	{
		check = true;					// checking if the variables pivot point has been found or not
		for(int i=0;i<M;i++)
		{
			if(Final[i] == values)
			{
				check = false;				// if found than false
				cout << "X[" << values + 1 << "] = " << fixed << setprecision(2) << table[i][N+M] << "\n";
				break;
			}
		}
		if(check)			// if not than that elements value is 0
			cout << "X[" << values + 1 << "] = 0.00\n";
		values++;
	}
	for(int j=0;j<M;j++)
	{
		check = true;
		for(int i=0;i<iteration;i++)
		{
			if(Points[i][0] == j)
			{
				check = false;				// if found than false
				cout << "X[" << values + 1 << "] = 0.00\n";
				break;
			}
		}
		if(check)			// if not than that elements value is 0
			cout << "X[" << values + 1 << "] = " << fixed << setprecision(2) << table[j][N+M] << "\n";
		values++;
	}
	if(value != 4)
	{
		if(choice == 1)
			cout << "\nThe Optimal value is Z = " << fixed << setprecision(2) << table[M][N+M] << "\n";		// outputting the optimal value of the function
		else
			cout << "\nThe Optimal value is Z = " << fixed << setprecision(2) << -1*table[M][N+M] << "\n";		// outputting the optimal value of the function
	}
	if(value == 0)
		cout << "\nHence,The Probelm also has Alternative Optimal Solution.\n";
	else if(value == 2)
		cout << "\nHence,The Solution to the given problem is Infeasible.\n";		// hence this is a Unbounded type of problem
	else if(value == 4)
	{
		cout << "\nHence,The Optimal Value using Dual Simplex Method comes as, Z = 0.00\n";		// hence this is a Unbounded type of problem
		cout << "\nHence,The Method fails to obtain any Initial Basic Feasible Solution.\n";		// hence this is a Unbounded type of problem
	}
	if(menu == 1)
		Menu(table,Points,Final,A,B,Z,N,M,choice,iteration,sign,value);
	return;
}

void BigMOptimalAnswer(double** A,double* B,double* Z,double** table,int** Points,int* Final,int N,int M,int iteration,int value,int menu,int choice,int* sign,int artificial,int slack,int surplus,int* signend)
{
	bool check;
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
				cout << "X[" << value + 1 << "] = " << fixed << setprecision(2) << table[i][N+surplus] << "\n";
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
	if(menu)
		BigMMenu(table,Points,Final,A,B,Z,N,M,choice,iteration,sign,artificial,surplus,slack,signend);
	return;
}

void Menu(double** table,int** Points,int*Final,double** A,double* B,double* Z,int N,int M,int choice,int iteration,int* sign,int answer) 			// the function which defines the menu driven program
{
	int value= -1;
	cout << "\nHence, Select from the given options.\n";
	while(value != 5)
	{
		int *Last = (int*)malloc(M*sizeof(int));
		for(int i=0;i<M;i++)
			Last[i] = -1;
		int **Pointer = (int**)malloc((M+N)*sizeof(int*));
		for(int i=0;i<N+M;i++)
			Pointer[i] = (int*)malloc(2*sizeof(int));
		double** Initial = (double**)malloc((M+1)*sizeof(double*));
		for(int i=0;i<M;i++)
			Initial[i] = (double*)malloc((N+M+1)*sizeof(double));
		Initial = Build_Simplex_Table(A,B,Z,N,M);
		cout << "\n----------------MENU STARTS---------------\n";
		cout << "1.)Initial Simplex Table.\n";
		cout << "2.)List of All Basic and Non Basic Variables for the ith Iteration.\n";
		cout << "3.)Simplex Table at the ith Iteration.\n";
		cout << "4.)Optimal Solution(also if Alternative Optimum Exists),if Optimal Solution doesn't exist then the condition for non existence, i.e. Infeasible or Unbounded.\n";
		cout << "5.)Exit.\n";
		cout << "----------------MENU ENDS---------------\n";
		cin >> value;
		if(value==1)
		{
			cout << "Hence, The Initial Simplex Table is as follows:\n\n";
			Print_Table(Initial,N,M);
			cout << "\n";
		}
		else if(value == 2 || value == 3)
		{
			int iter,itercheck=0,number;
			if(value == 2)
				cout << "Enter the Iteration Number i for which we need to find out the Values of Basic and Non Basic Variables.\n";
			else
				cout << "Enter the Iteration Number i for which we need to find out the Simplex Table.\n";
			cin >> iter;
			while(iter>iteration)
			{
				cout << "The Total Number of Iteration to find the Solution is " << iteration << "\n";
				cout << "Please, Enter a Valid Iteration Number.\n";
				cin >> iter;
			}
			while(itercheck<iter)
			{
			    number = CheckPivot(Initial,Pointer,N,M,itercheck,Last,sign,0);			// finding the first pivot element
				Evaluate_Simplex_Table(Initial,Pointer,N,M,itercheck);		// function that would evaluate all the Pivot Points
				Evaluate_Cost(Initial,Z,Last,N,M);						// function to evaluate the cost of this simplex table
				itercheck++;
			}
			if(value == 2)
			{
				int values = 0;
				bool check;
				while(values < N)
				{
					check = true;					// checking if the variables pivot point has been found or not
					for(int i=0;i<M;i++)
					{
						if(Last[i] == values)
						{
							check = false;				// if found than false
							cout << "X[" << values + 1 << "] = " << fixed << setprecision(2) << Initial[i][N+M] << "\n";
							break;
						}
					}
					if(check)			// if not than that elements value is 0
						cout << "X[" << values + 1 << "] = 0.00\n";
					values++;
				}
				for(int j=0;j<M;j++)
				{
					check = true;
					for(int i=0;i<iter;i++)
					{
						if(Pointer[i][0] == j)
						{
							check = false;				// if found than false
							cout << "X[" << values + 1 << "] = 0.00\n";
							break;
						}
					}
					if(check)			// if not than that elements value is 0
						cout << "X[" << values + 1 << "] = " << fixed << setprecision(2) << Initial[j][N+M] << "\n";
					values++;
				}
			}
			else
			{
				cout << "\nHence,The Simplex Table at the Iteration " << iter << " is as follows:\n";
				Print_Table(Initial,N,M);
			}

		}
		else if(value == 4)
		{
			OptimalAnswer(A,B,Z,table,Points,Final,N,M,iteration,answer,0,choice,sign);			// hence printing the Solution
			cout << "\n";
		}
	}
	cout << "\n---------------PROGRAM ENDS---------\n";
	return;
}

double** BigMBuild_Simplex_Table(double** A,double *B,double *Z,int N,int M,int* sign,int surplus,int choice)	// function to build the initial Simplex Table, and code to store the Zj - Cj values
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

int BigMCheckPivot(double** Table,int** Points,int N,int M,int iteration,int* sign,int surplus,int* Final)		// code to evaluate the pivot element and the minimum ratio, and hence returns the pivot row and column
{
	bool unbounded = true;				// variable to check if the solution is unbounded or not
	bool flag = true;					// a flag to check if the pivot has been found
	double min;							// variable to find the minimum value of in the last row,pivot column of the Simplex tableau
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
			return 2;
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
		cout << "Hence, The Method fails to get optimal basic feasible Solution.\n";
		return 4;		// exiting from the program
	}
	else if(min > 0)					// ending the program by printing the already found optimal solution
		return 3;
	else 								// checking if any alternate solution would exist
		return 0;
}

void BigMEvaluate_Simplex_Table(double** Table,int** Points,int N,int M,int iteration,int *sign,int surplus)		// code to revise the simplex table and complete one iteration, and hence print the next solution, and object value
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

void BigM(double** A,double* B,double* Z, int N,int M,int** Points,int choice,int *sign,int* signend,int surplus,int artificial,int slack,int * Final)
{
	int iteration = 0,value;
	double** table;
    table = BigMBuild_Simplex_Table(A,B,Z,N,M,sign,surplus,choice);			// function builds the initial Siimplex Tableau
	cout << "------------Initial Simplex Table: " << iteration + 1 << " --------------------\n";
	BigMPrint_Table(table,N,M,surplus);
    value = BigMCheckPivot(table,Points,N,M,iteration,sign,surplus,Final);
    while(value == 1)
    {
    	cout << "\n";
    	cout << "------------Iteration Number: " << iteration + 1 << " --------------------\n";
    	cout << "The Pivot Row is " << Points[iteration][0] + 1 << " and the Pivot Column is " << Points[iteration][1] + 1 << ".\n";
    	cout << "Hence, The Pivot Element from Iteration Number " << iteration + 1 << " is " << table[Points[iteration][0]][Points[iteration][1]] << ".\n";
    	BigMEvaluate_Simplex_Table(table,Points,N,M,iteration,sign,surplus);		// function that would evaluate all the Pivot Points
    	iteration++;
		BigMPrint_Table(table,N,M,surplus);
	    value = BigMCheckPivot(table,Points,N,M,iteration,sign,surplus,Final);
    }
    BigMOptimalAnswer(A,B,Z,table,Points,Final,N,M,iteration,value,1,choice,sign,artificial,slack,surplus,signend);
    return;
}

void BigMMenu(double** table,int** Points,int*Final,double** A,double* B,double* Z,int N,int M,int choice,int iteration,int* sign,int artificial,int surplus,int slack,int* signend) 			// the function which defines the menu driven program
{
	int value= -1;
	cout << "\nHence, Select from the given options.\n";
	while(value != 5)
	{
		int *Last = (int*)malloc(M*sizeof(int));
		for(int i=0;i<M;i++)
			Last[i] = -1;
		int **Pointer = (int**)malloc((M+N+surplus)*sizeof(int*));
		for(int i=0;i<N+M+surplus;i++)
			Pointer[i] = (int*)malloc(2*sizeof(int));
		double** Initial = (double**)malloc((M+1)*sizeof(double*));
		for(int i=0;i<M;i++)
			Initial[i] = (double*)malloc((N+surplus+1)*sizeof(double));
		Initial = BigMBuild_Simplex_Table(A,B,Z,N,M,signend,surplus,choice);
		cout << "\n----------------MENU STARTS---------------\n";
		cout << "1.)Initial Simplex Table.\n";
		cout << "2.)List of All Basic and Non Basic Variables for the ith Iteration.\n";
		cout << "3.)Simplex Table at the ith Iteration.\n";
		cout << "4.)Optimal Solution(also if Alternative Optimum Exists),if Optimal Solution doesn't exist then the condition for non existence, i.e. Infeasible or Unbounded.\n";
		cout << "5.)Exit.\n";
		cout << "----------------MENU ENDS---------------\n";
		cin >> value;
		if(value==1)
		{
			cout << "Hence, The Initial Simplex Table is as follows:\n\n";
			BigMPrint_Table(Initial,N,M,surplus);
			cout << "\n";
		}
		else if(value == 2 || value == 3)
		{
			int iter,itercheck=0,number;
			if(value == 2)
				cout << "Enter the Iteration Number i for which we need to find out the Values of Basic and Non Basic Variables.\n";
			else
				cout << "Enter the Iteration Number i for which we need to find out the Simplex Table.\n";
			cin >> iter;
			while(iter>iteration)
			{
				cout << "The Total Number of Iteration to find the Solution is " << iteration << "\n";
				cout << "Please, Enter a Valid Iteration Number.\n";
				cin >> iter;
			}
			while(itercheck<iter)
			{
			    number = BigMCheckPivot(Initial,Pointer,N,M,itercheck,sign,surplus,Last);			// finding the first pivot element
				BigMEvaluate_Simplex_Table(Initial,Pointer,N,M,itercheck,sign,surplus);		// function that would evaluate all the Pivot Points
				itercheck++;
			}
			if(value == 2)
			{
				bool check;
				cout << "\nThe Optimal Solution is as follows:\n";
				int values = 0;
				while(values < N)
				{
					check = true;					// checking if the variables pivot point has been found or not
					// for(int i=max(0,iteration-artificial-slack);i<iteration;i++)
					for(int i=0;i<slack+artificial;i++)
					{
						if(Last[i] == values)
						{
							check = false;				// if found than false
							cout << "X[" << values + 1 << "] = " << fixed << setprecision(2) << Initial[i][N+surplus] << "\n";
							break;
						}
					}
					if(check)			// if not than that elements value is 0
						cout << "X[" << values + 1 << "] = 0\n";
					values++;
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
							if(Pointer[i][0] == j)
							{
								check = false;				// if found than false
								cout << "X[" << values + 1 << "] = 0\tSlack Variable.\n";
								break;
							}
						}
						if(check)			// if not than that elements value is 0
							cout << "X[" << values + 1 << "] = " << fixed << setprecision(2) << Initial[j][N+surplus] << "\tSlack Variable.\n";
						values++;
						continue;
					}
					else if(signend[j] == 2)
					{
						check = true;
						// for(int i=max(0,iteration-artificial-slack);i<iteration;i++)
						for(int i=0;i<iteration;i++)
						{
							if(Pointer[i][0] == j)
							{
								check = false;				// if found than false
								cout << "X[" << values + 1 << "] = 0\tArtificial Variable.\n";
								break;
							}
						}
						if(check)			// if not than that elements value is 0
							cout << "X[" << values + 1 << "] = " << fixed << setprecision(2) << Initial[j][N+surplus] << "\tArtificial Variable.\n";
						values++;
						continue;
					}
					else if(signend[j] == 1)
					{
						check = true;					// checking if the variables pivot point has been found or not
						for(int i=max(0,iteration-artificial-slack);i<iteration;i++)
						{
							if(Pointer[i][1] == value2)
							{
								check = false;				// if found than false
								cout << "X[" << values + 1 << "] = " << fixed << setprecision(2) << Initial[Pointer[i][0]][N+surplus] << "\tSurplus Variable.\n";
								break;
							}
						}
						if(check)			// if not than that elements value is 0
							cout << "X[" << values + 1 << "] = 0\tSurplus Variable.\n";
						values++;
						value2++;
						check = true;
						// for(int i=max(0,iteration-artificial-slack);i<iteration;i++)
						for(int i=0;i<iteration;i++)
						{
							if(Pointer[i][0] == j)
							{
								check = false;				// if found than false
								cout << "X[" << values + 1 << "] = 0\tArtificial Variable.\n";
								break;
							}
						}
						if(check)			// if not than that elements value is 0
							cout << "X[" << values + 1 << "] = " << fixed << setprecision(2) << Initial[j][N+surplus] << "\tArtificial Variable.\n";
						values++;
						continue;
					}
				}
			}
			else
			{
				cout << "\nHence,The Simplex Table at the Iteration " << iter << " is as follows:\n";
				BigMPrint_Table(Initial,N,M,surplus);
			}
		}
		else if(value == 4)
		{
			BigMOptimalAnswer(A,B,Z,table,Points,Final,N,M,iteration,value,0,choice,sign,artificial,slack,surplus,signend);			// hence printing the Solution
			cout << "\n";
		}
	}
	cout << "\n---------------PROGRAM ENDS---------\n";
	return;
}
