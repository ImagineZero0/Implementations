/*
NAME -> Varun Gupta
ROLL NUMBER -> 18MA20050
OR LAB WEEK 6 ASSIGNMENT
This is the code to implement the Revised Simplex Algorithm by first developing a basic variable identity matrix and hence evaluating their solutions and cost values and hence finding the respective values of
Zj - Cj from the Simplex Table and hence updating the basic matrix in each iteration and hence finding the new basic inverse matrix and cost values, we continue this until we get our required solution
when the Simplex Table satisfies the Optimal Condition and hence we find our optimal answer.
*/

#include <bits/stdc++.h>			// including the required libraries
using namespace std;		// using namespace for the required functions

double** MatrixMult(double** Mat1,double** Mat2,int size1,int size2,int size3)				// A function to multiply two matrices as vector multiplication
{
    int i, j, k;
    double** Table;												// allocating space for the new vector multiplied matrix
    Table = (double**)malloc(size1*sizeof(double*));
    for(int i=0;i<size1;i++)
    	Table[i] = (double*)malloc(size2*sizeof(double));
    for(i = 0; i < size1; i++)
    {
        for(j = 0; j < size2; j++)
        {
            Table[i][j] = 0.0;
            for(k = 0; k < size3; k++)
                Table[i][j] += (Mat1[i][k]*Mat2[k][j]);					// hence evaluating the matrix
        }
    }
    return Table;
}

double** InverseOfMatrix(double** matrix, int order)			// function that would evaluate the inverse of a matrix
{
	double **inverse,temp,**return_inverse;
	inverse = (double**)malloc(order*sizeof(double*));			// allocating memory for the evaluated inverse of the matrix
	return_inverse = (double**)malloc(order*sizeof(double*));
	for(int i=0;i<order;i++)
	{
		inverse[i] = (double*)malloc((2*order)*sizeof(double));
		return_inverse[i] = (double*)malloc((order)*sizeof(double));
	}
	for(int i=0;i<order;i++)
	{
		for(int j=0;j<order;j++)
		{
			inverse[i][j] = matrix[i][j];							// copying the matrix
		}
	}
    for (int i = 0; i < order; i++)
    {

        for (int j = 0; j < 2 * order; j++)
        {
            if (j == (i + order))								// first storing the augmented matrix with the matrix only
                inverse[i][j] = 1;
        }
    }
    for (int i = 0; i < order-1; i++) 			// hence using the inverse algorithm
    {
        if (inverse[i][i] == 0)
        {
        	for(int j=i+1;j<order;j++)
        	{
        		if(inverse[j][i] != 0)
				{
		            double* temp = inverse[j];
		            inverse[j] = inverse[i];
		            inverse[i] = temp;
				}
        	}
        }
    }
    for (int i = 0; i < order; i++)
    {

        for (int j = 0; j < order; j++)
        {
            if (j != i)
            {
                temp = inverse[j][i] / inverse[i][i];
                for (int k = 0; k < 2 * order; k++)
                    inverse[j][k] -= inverse[i][k] * temp;
            }
        }
    }
    for (int i = 0; i < order; i++)
    {

        temp = inverse[i][i];
        for (int j = 0; j < 2 * order; j++)
            inverse[i][j] = inverse[i][j] / temp;
    }
	for(int i=0;i<order;i++)
	{
		for(int j=order;j<2*order;j++)
		{
			return_inverse[i][j-order] = inverse[i][j];
			if(abs(inverse[i][j]) == 0.0)
				return_inverse[i][j-order] = 0.0;
		}
	}
    return return_inverse;				// returning the inverted matrix
}

double** Identity(int size)												// a function that would develop an Identity Matrix of dimension sizexsize
{
	double** table;
	table = (double**)malloc(size*sizeof(double*));
	for(int i=0;i<size;i++)
		table[i] = (double*)malloc(size*sizeof(double));
	for(int i=0;i<size;i++)
	{
		for(int j=0;j<size;j++)
		{
			if(i==j)
				table[i][j] = 1.0;
			else
				table[i][j] = 0.0;
		}
	}
	return table;
}

double** Build_Simplex_Table(double** A,double *B,double *Z,int N,int M,int surplus,int choice)	// function to build the initial Simplex Table, and code to store the Zj - Cj values
{
	double** Table;									// initializing the variable for the Simplex Table
	int loc = 0;
	Table = (double**)malloc((M+1)*sizeof(double*));	// hence, allocating space for the Simplex Table
	for(int i=0;i<M+1;i++)
		Table[i] = (double*)malloc((N+1+M)*sizeof(double));
	for(int i=0;i<M;i++)			// hence placing all the elements from the given A,B,Z matrices
	{
		for(int j=0;j<N+1+M;j++)
		{
			if(j<N)
				Table[i][j] = A[i][j];
			else if(j < N+M)
				Table[i][j] = 0.0;
			else
				Table[i][j] = B[i];
		}
		if(loc<M)
		{
			Table[i][N+loc] = 1.0;
			loc++;
		}
	}
	for(int i=0;i<N;i++)				// saving the coefficient of the decision variables
		Table[M][i] = Z[i];
	Table[M][N+M] = 0.0;
	return Table;		// returning the initial Simplex Table
}

void Print_Table(double** Table,int N,int M)		// function that would print the Simplex Table Whenever required
{
	cout << "\tCv\t";
	for(int i=0;i<N+M;i++)
	{
		if(i<N)
			cout << Table[M][i] << "\t";
		else
			cout << "0.00\t";
	}
	cout << "\nCb\tB,V\t";
	for(int i=0;i<N+M;i++)
		cout << "X[" << i+1 << "]\t";
	cout << "b\n";
    for(int i=0;i<M+1;i++)			// hence printing the values
	{
		if(i<M)
			cout << "0.00\tX[" << N+i+1 << "]\t";
		else
			cout << "\t\t";
		for(int j=0;j<N+M+1;j++)
		{
			if(i<M)
				cout << fixed << setprecision(2) <<  Table[i][j] << "\t";
			else
			{
				if(Table[i][j]!=0)
					cout << fixed << setprecision(2) <<  (-1.0)*Table[i][j] << "\t";
				else
					cout << fixed << setprecision(2) <<  Table[i][j] << "\t";
			}
		}
		cout << "\n";
	}
	return;
}

int CheckPivot(double** Table,int* NB,int** Points,int N,int M,int iteration,int* sign,int surplus,int* Final,double** Y,double** basic_inverse_matrix)		// code to evaluate the pivot element and the minimum ratio
{
	bool unbounded = true;				// variable to check if the solution is unbounded or not
	bool flag = true;					// a flag to check if the pivot has been found
	double mini,val,**XB,**values,**value;							// initializing the required varaibles
	int index,index2;							// the index for the particular column,row of the pivot element
	for(int i=0;i<N;i++)		// evaluating the Cj - Zj using the matrix
	{
		val = 0.0;
		for(int j=0;j<M;j++)
			val += (Y[0][j]*Table[j][NB[i]]);
		val -= Table[M][NB[i]];
		if(flag)		// if not element has been saved , saving the first element
		{
			mini = val;
			index = NB[i];
			flag = false;
		}
		else
		{
			if(mini > val)		// else checking if any element is smaller
			{
				mini = val;
				index = NB[i];
			}
		}
	}
	value = (double**)malloc(M*sizeof(double*));			// storing extra varaibles in space for further requirement
	for(int i=0;i<M;i++)
		value[i] = (double*)malloc(sizeof(double));
	for(int i=0;i<M;i++)
		value[i][0] = Table[i][N+M];				// storing the B values
	if(mini < 0)				// if we have found the minimum element as negative
	{
		XB = MatrixMult(basic_inverse_matrix,value,M,1,M);			// hence evaluating XB
		for(int i=0;i<M;i++)
			value[i][0] = Table[i][index];							// storing the basic variables coefficients
		values = MatrixMult(basic_inverse_matrix,value,M,1,M);			// hence evaluating the matrix mult from the basic variable coefficient
		for(int i=0;i<M;i++)		// now finding the minimum ratio element in the pivot element column
		{
			if(values[i][0] > 0)		// only if that element in positve
			{
				if(unbounded)				// and if no first element has been saved
				{
					mini = XB[i][0]/values[i][0];		// saving the minimim ratio
					index2 = i;						// saving the index of that row
					unbounded = false;				// false for flag when one element has been saved
				}
				else
				{
					if(mini > XB[i][0]/values[i][0])		// for future elements, if ratio is smaller than previous ratio
					{
						mini = XB[i][0]/values[i][0];		// saving the minimum ratio
						index2 = i;								// sasving index of the minimum ratio
					}
				}
			}
		}
		if(unbounded)			// if no positve element has been found even when negative element exist in the last row
			return 2;				// hence, exiting the program
		else 				// if we have found the most negative element in the last row, and minimum ratio in that elements column, hence found the pivot element
		{
			Points[iteration][0] = index2;			// hence saving the indices of that pivot element
			Points[iteration][1] = index;
			Final[index2] = index;
			sign[index2] = -1;
			for(int k=0;k<N;k++)
			{
				if(NB[k] == index)
				{
					NB[k] = index2+N;
					break;
				}
			}
			return 1;
		}
	}
	else if(iteration==0)			// if minimum element is not negative and no negative minimum element has been found in the last row
		return 4;		// exiting from the program
	else if(mini > 0)					// ending the program by printing the already found optimal solution
		return 0;
	else 								// checking if any alternate solution would exist
		return 3;
}

void Revised_Simplex(double** A,double* B,double* Z, int N,int M,int choice,int *sign,int* signend,int surplus)		// the main revised simplex method which carries out the whole algorithm
{
	int checker = -1,iter,itercheck,iteration = 0,value=0,*Final,**Points;													// initializing the required varaibles
    bool check;
    int* NB = (int*)malloc(N*sizeof(int));
    for(int i=0;i<N;i++)
    	NB[i] = i;
	double **table,**basic_matrix,**basic_inverse_matrix,temp,**cost,**Y,**values,**XB,**answer;
	while(checker!=4)
	{
		if(checker != -1)
		{
			cout << "\n----------------MENU STARTS---------------\n";
			cout << "1.)Initial Simplex Table.\n";
			cout << "2.)List of All Basic and Non Basic Variables for the ith Iteration.\n";
			cout << "3.)Optimal Solution(also if Alternative Optimum Exists),if Optimal Solution doesn't exist then the condition for non existence, i.e. Infeasible or Unbounded.\n";
			cout << "4.)Exit.\n";
			cout << "----------------MENU ENDS---------------\n";
			cin >> checker;
			if(checker == 2)
			{
				itercheck=0;
				cout << "Enter the Iteration Number i for which we need to find out the Values of Basic and Non Basic Variables.\n";
				cin >> iter;
				while(iter>iteration)
				{
					cout << "The Total Number of Iteration to find the Solution is " << iteration << "\n";
					cout << "Please, Enter a Valid Iteration Number.\n";
					cin >> iter;
				}
				cout << "\n";
			}
			for(int i=0;i<N;i++)
		    	NB[i] = i;
		}
	    cost = (double**)malloc(sizeof(double*));
	    for(int i=0;i<M;i++)											// allocating space for the initial cost
	    	cost[i] = (double*)malloc(M*sizeof(double));
	    Points = (int**)malloc((M+N+surplus)*sizeof(int*));		// hence allocating space
		for(int i=0;i<M+N+surplus;i++)
			Points[i] = (int*)malloc(2*sizeof(int));
		Final = (int*)malloc(M*sizeof(int));
		values = (double**)malloc(M*sizeof(double*));			// allocating space for extra variables
		for(int i=0;i<M;i++)
			values[i] = (double*)malloc(sizeof(double));
		answer = (double**)malloc(sizeof(double*));				// allocating space for the answer
		answer[0] = (double*)malloc(sizeof(double));
	    for(int i=0;i<M;i++)
	    	cost[0][i] = 0.0;
		for(int i=0;i<M;i++)
			Final[i] = -1;
	    table = Build_Simplex_Table(A,B,Z,N,M,surplus,choice);			// function builds the Simplex Table
		if(checker == -1 || checker == 1)
		{
			cout << "Hence, The Initial Table is as follows:\n";
		    Print_Table(table,N,M);
		}
	    basic_matrix = Identity(M);													// initializing the first basic matrix from the basic variables as the Identity Matrix
	    basic_inverse_matrix = InverseOfMatrix(basic_matrix,M);				// the first inverse matrix from the basic matrix
	    Y = MatrixMult(cost,basic_inverse_matrix,1,M,M);					// hence evaluating the initial value of function from the inverse basic matrix
		for(int i=0;i<M;i++)
			values[i][0] = table[i][N+M];
	    XB = MatrixMult(basic_inverse_matrix,values,M,1,M);
	    if(checker == -1 || (checker == 2 && iter == 0))
	    {
	    	cout << "\nHence, The Initial values basic variables and non basic varaibles are:\n";
		    for(int i=0;i<N+M;i++)
		    {
		    	if(i<N)
		    		cout << "X[" << i+1 << "] = 0.00\n";
		    	else
		    	{
		    		cout << "X[" << i+1 << "] = " << fixed << setprecision(2) << XB[i-N][0] << "\t";
		    		if(signend[i-N] == 3)
		    			cout << "Slack Variable\n";
		    		else
		    			cout << "Artificial Variable\n";
		    	}
		    }
	    }
	    if(checker == -1 || checker == 3)
	    	value = CheckPivot(table,NB,Points,N,M,iteration,sign,surplus,Final,Y,basic_inverse_matrix);			// hence now finding the value of a pivot
	    else if(checker == 2)
	    	value = CheckPivot(table,NB,Points,N,M,itercheck,sign,surplus,Final,Y,basic_inverse_matrix);			// hence now finding the value of a pivot
	    while(value == 1)												// until no pivot exists
	    {
	    	if(checker == 2)
	    	{
	    		if(Points[itercheck][1] >= N)
		    		cost[0][Points[itercheck][0]] = 0.0;
		    	else
		    		cost[0][Points[itercheck][0]] = Z[Points[itercheck][1]];
	    		for(int i=0;i<M;i++)
		    		basic_matrix[i][Points[itercheck][0]] = table[i][Points[itercheck][1]];
				table[M][Points[itercheck][1]] = 0.0;						// hence the cost in the simplex table is now 0.0 for that basic variable
	    	}
	    	else
	    	{
	    		if(Points[iteration][1] >= N)
		    		cost[0][Points[iteration][0]] = 0.0;
		    	else
			    	cost[0][Points[iteration][0]] = Z[Points[iteration][1]];			// hence updating the cost of the basic variable
		    	for(int i=0;i<M;i++)
		    		basic_matrix[i][Points[iteration][0]] = table[i][Points[iteration][1]];		// swapping the values of the basic variable from the Simpex table and the basic matrix
				table[M][Points[iteration][1]] = 0.0;						// hence the cost in the simplex table is now 0.0 for that basic variable
	    	}
			basic_inverse_matrix = InverseOfMatrix(basic_matrix,M);		// hence finding the new inverse matrix
			Y = MatrixMult(cost,basic_inverse_matrix,1,M,M);				// Now again evaluaing the new value of Y after the new cost and inverse values
			if(checker == -1)
			{
			    cout << "\n---------------Iteration " << iteration + 1 << "------------\n";
			    cout << "Hence, The basic variables and non basic varaibles values are:\n";
			}
		    for(int i=0;i<M;i++)
				values[i][0] = table[i][N+M];
		    XB = MatrixMult(basic_inverse_matrix,values,M,1,M);
		    if(checker == -1)
		    {
		    	for(int i=0;i<N+M;i++)
			    {
			    	if(i<N)
			    	{
			    		check = true;
			    		for(int j=0;j<M;j++)
			    		{
				    		if(Final[j] == i)
				    		{
				    			cout << "X[" << i + 1 << "] = " << fixed << setprecision(2) << XB[j][0] << "\n";
				    			check = false;
				    			break;
				    		}
			    		}
			    		if(check)
			    			cout << "X[" << i + 1 << "] = 0.00\n";
			    	}
			    	else
			    	{
			    		if(Final[i-N] != -1)
			    			cout << "X[" << i + 1 << "] = 0.00\t";
						else
			    			cout << "X[" << i + 1 << "] = " << fixed << setprecision(2) << XB[i-N][0] << "\t";
			    		if(signend[i-N] == 3)
			    			cout << "Slack Variable\n";
			    		else
			    			cout << "Artificial Variable\n";
			    	}
			    }
		    }
		    if(checker == 2 && itercheck == iter-1)
	    	{
	    		for(int i=0;i<N+M;i++)
			    {
			    	if(i<N)
			    	{
			    		check = true;
			    		for(int j=0;j<M;j++)
			    		{
				    		if(Final[j] == i)
				    		{
				    			cout << "X[" << i + 1 << "] = " << fixed << setprecision(2) << XB[j][0] << "\n";
				    			check = false;
				    			break;
				    		}
			    		}
			    		if(check)
			    			cout << "X[" << i + 1 << "] = 0.00\n";
			    	}
			    	else
			    	{
			    		if(Final[i-N] != -1)
			    			cout << "X[" << i + 1 << "] = 0.00\t";
						else
			    			cout << "X[" << i + 1 << "] = " << fixed << setprecision(2) << XB[i-N][0] << "\t";
			    		if(signend[i-N] == 3)
			    			cout << "Slack Variable\n";
			    		else
			    			cout << "Artificial Variable\n";
			    	}
			    }
	    		break;
	    	}
		    if(checker == -1 || checker == 3)
		    {
		    	iteration++;		// next iteration
			    value = CheckPivot(table,NB,Points,N,M,iteration,sign,surplus,Final,Y,basic_inverse_matrix);		// hence checking if new pivot exists or not
		    }
	    	else if(checker == 2)
	    	{
	    		itercheck++;
			    value = CheckPivot(table,NB,Points,N,M,itercheck,sign,surplus,Final,Y,basic_inverse_matrix);		// hence checking if new pivot exists or not
	    	}
	    }
	    if(checker == -1  || checker == 3)
	    {
	    	cout << "\nThe Optimal Solution is as follows:\n";
	    	for(int i=0;i<M;i++)
				values[i][0] = table[i][N+M];
			XB = MatrixMult(basic_inverse_matrix,values,M,1,M);			// hence evaluating the basic variables values
			value = 0;
			for(int i=0;i<N+M;i++)
		    {
		    	if(i<N)
		    	{
		    		check = true;
		    		for(int j=0;j<M;j++)
		    		{
			    		if(Final[j] == i)
			    		{
			    			cout << "X[" << i + 1 << "] = " << fixed << setprecision(2) << XB[j][0] << "\n";
			    			check = false;
			    			break;
			    		}
		    		}
		    		if(check)
		    			cout << "X[" << i + 1 << "] = 0.00\n";
		    	}
		    	else
		    	{
		    		if(Final[i-N] != -1)
		    			cout << "X[" << i + 1 << "] = 0.00\t";
					else
		    			cout << "X[" << i + 1 << "] = " << fixed << setprecision(2) << XB[i-N][0] << "\t";
		    		if(signend[i-N] == 3)
		    			cout << "Slack Variable\n";
		    		else
		    			cout << "Artificial Variable\n";
		    	}
		    }
			answer = MatrixMult(cost,XB,1,1,M);			// hence evaluating the answer
			if(choice == 1)
				cout << "\nThe Optimal value is Z = " << fixed << setprecision(2) << answer[0][0] << "\n";		// outputting the optimal value of the function
			else
				cout << "\nThe Optimal value is Z = " << fixed << setprecision(2) << -1*answer[0][0] << "\n";		// outputting the optimal value of the function
			check = false;			// now evaluating if the solution is feasible or not
			for(int i=0;i<M;i++)
			{
				if(signend[i] == 1 || signend[i] == 2)
				{
					check = true;
					break;
				}
			}
			if(check)			// if infeasible
				cout << "Hence,The problem has an Infeasible Solution.\n";
	    }
	    if(value == 2)
			cout << "The Solution to the given problem is Unbounded.\n";		// hence this is a Unbounded type of problem
	    else if(value == 3)
			cout << "Hence, An Alternative Optimal Solution also exists to the given problem.\n";		// hence this is a Unbounded type of problem
		else if(value == 4)
		{
			for(int i=0;i<M+N+surplus;i++)
				cout << "X[" << i+1 << "] = 0 ";			// hence printing the values of X
		}
		if(checker != 4)
			checker = 0;
	}
	cout << "\n---------------PROGRAM ENDS------------------------\n";
    return;
}

int Take_input(double** A,double* B,int* sign,int* signend,double* Z,int N,int M)						// function to take the input
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
    for (int i = 0; i < N; i++)
    {
        cout << "Enter coefficient of X[" << i+1 <<"], i.e. Z[" << i+1 << "] = ";					// hence inputting the coefficients
        cin >> val;
	    if(choice == 1)
	        Z[i] = val;							// hence inputting the values of the optimal equation
    	else
	        Z[i] = (-1.0)*val;							// hence inputting the values of the optimal equation

    }
    cout << "\n";
    return choice;			// returns the int which signifies which tyoe of problem this would be
}

int main()						// main function where the program initiates
{
	cout << "\n---------------PROGRAM STARTS------------------------\n";
	int n,m,choice,* sign,*signend;					// initializing the required variables
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
	Z = (double*)malloc(n*sizeof(double));
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
    Revised_Simplex(A,B,Z,n,m,choice,sign,signend,surplus);					// function which would solve the whole problem
	return 0;
}
