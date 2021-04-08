/*
NAME -> Varun Gupta
ROLL NUMBER -> 18MA20050
OR LAB WEEK 8 ASSIGNMENT
This is the code to implement the Integer Programmin - Gomory Cutting Plane Method by first solving the question as a normal LPP and hence obtaining the decision varaibles
values if the values are integers we then stop, if they are not then we find the all the decision variables which are not integers and hence add new constraints to remove
those fractional parts, and hence solve the question using Dual Simplex Method, and find the final integer answers.
*/

#include <bits/stdc++.h>                // including the libraries required

using namespace std;                    // namespace for different developed functions

#define BM 100000000

int Take_input(double** A,double* B,int* sign,double* Z,int N,int M)                       // function to take the input
{
    int choice;
    cout << "\nNow, Enter the values of A:\n";                  // hence inputting values of A
    for (int i = 0; i < M; i++)
    {
        for (int j = 0; j <N; j++)
        {
            cout << "Enter A[" << i+1 << "][" << j+1 << "] = ";         // hence inputting the matrix
            cin >> A[i][j];
        }
        cout << "Now, Enter the number of the Inequality Sign on the basis of:\n1)>=\n2)=\n3)<=\n";
        cout << "A[" << i+1 << "][Sign] = ";
        cin >> sign[i];
        cout << "\n";
    }
    cout << "\nNow, enter the values of B:\n";          // hence inputting values of B
    for (int i = 0; i < M; i++)
    {
        cout << "Enter B[" << i+1 << "] = ";
        cin >> B[i];                        // hence inputting the values of B
    }
    cout << "\nEnter which kind of Optimality do we need to evaluate:\n";
    cout << "1)Maximizing.\n2)Minimizing.\n";
    cin >> choice;
    choice -= 1;
    cout  << "\nNow enter the coefficients of the optimality equation of Z:\n";
    for (int i = 0; i < N; i++)
    {
        cout << "Enter coefficient of X[" << i+1 <<"], i.e. Z[" << i+1 << "] = ";                   // hence inputting the coefficients
        cin >> Z[i];
    }
    cout << "\n";
    return choice;          // returns the int which signifies which tyoe of problem this would be
}

pair<int, double> decompose(double x)
{
    if (x == 0.0)
        return {0.0, 0.0};
    double sign = x / abs(x);
    x = abs(x);
    double FracPart = x - (double)((int)(x));
    double IntPart = (int)(x);
    if (abs(FracPart - 1) < 1e-8)
        IntPart += 1, FracPart = 0;
    return {sign * IntPart, sign * FracPart};
}

void BigM(double **A, double *B, double *C, int n, int m, int var, int *sign, int choice) // the function which solves the LPP using simpmlex method
{
    double sum;
    int i, j, k = n + var;
    int num_equations = m;
    int num_var = n + var;
    int doublecheck = 0;
    double **sim = (double **)malloc(100 * sizeof(double *));
    double *cb = (double *)malloc(((100) * sizeof(double)));
    for (i = 0; i < 100; i++)
        sim[i] = (double *)malloc(100 * sizeof(double));
    k = 0;
    for (i = 0; i < m; i++)
    {
        for (j = 0; j < n; j++)
            sim[i][j] = A[i][j];
        for (j = n; j < (n + var); j++)
            sim[i][j] = 0.0;
        if (sign[i] == 1)
        {
            sim[i][n + k] = -1.0;
            k++;
            sim[i][n + k] = 1.0;
            k++;
        }
        else
        {
            sim[i][n + k] = 1.0;
            k++;
        }
    }
    for (i = 0; i < n; i++)
        cb[i] = C[i];
    k = 0;
    for (i = 0; i < m; i++)
    {
        if (sign[i] == 0)
        {
            cb[n + k] = 0.0;
            k++;
        }
        else if (sign[i] == 1)
        {
            cb[n + k] = 0.0;
            k++;
            if (choice == 0)
                cb[n + k] = -BM;
            else
                cb[n + k] = BM;
            k++;
        }
        else
        {
            if (choice == 0)
                cb[n + k] = -BM;
            else
                cb[n + k] = BM;
            k++;
        }
    }
    int keycol, keyrow;
    double maxincmz = 0.0;                                          // this helps to find the minimum Z_i - C_i in each iteration
    double mininsol = 999999.0;                                     // this helps to find the minimum ratio in each iteration
    int *basv = (int *)malloc(100 * sizeof(int));                   // stores the basic variables values
    double *Z = (double *)malloc(((100) * sizeof(double)));         //stores the values of Z = \sum (CB_i)*(a_i_j)
    double *CminusZ = (double *)malloc(((100) * sizeof(double)));   // calculates the Z_i - C_i
    double *sol = (double *)malloc(100 * sizeof(double));           // stores the solutions of each iteration
    double *ratio = (double *)malloc(100 * sizeof(double));         // calculates the ratios of each iteration
    double *keyrowval = (double *)malloc(((100) * sizeof(double))); // stores the key row in a separate array
    double *keycolval = (double *)malloc(100 * sizeof(double));     // stores the key column in a separate array
    k = 0;
    for (i = 0; i < m; i++)
    {
        if (sign[i] == 0)
        {
            basv[i] = k + n;
            k++;
        }
        else if (sign[i] == 1)
        {
            k++;
            basv[i] = k + n;
            k++;
        }
        else
        {
            basv[i] = k + n;
            k++;
        }
        sol[i] = B[i];
    }
    int check = 1;
    int iter = 0;
    double solkey;
    double zsol;
    printf("Now, after modifying the equations, we get : \n");
    if (choice)
        cout << "Minimize: Z =\n";
    else
        cout << "Maximize: Z =\n";
    for (i = 0; i < (n + var); i++)
        cout << cb[i] << "*X[" << i+1 << "] +";
    cout << "\nsubject to\n";
    for (i = 0; i < m; i++)
    {
        for (j = 0; j < (n + var); j++)
            cout << sim[i][j] << " X[" << j+1 << "] +";
        cout << " 0 = " << sol[i] << "\n";
    }
    cout << "and \n";
    for (i = 0; i < (n + var - 1); i++)
        cout << " X[" << i+1 << "],\n";
    cout << " X[" << i+1 << "] >= 0\n";
    iter = 0;
    while (check)
    {
        if (iter >= 10) // checker for inifite iterations
        {
            printf("Hence, Infeasible Solution.\n");
            break;
        }
        if (iter != 0) // apart from the first iteration, we don't have to change the columns
        {
            basv[keyrow] = keycol; //entering variable is put in the basic variable column
            for (i = 0; i < m; i++)
            {
                if (i == keyrow)
                    sol[i] = sol[i] / keyrowval[keycol]; // for the pivot row
                else
                    sol[i] = sol[i] - (keycolval[i] * solkey) / keyrowval[keycol]; // for other rows
                for (j = 0; j < (n + var); j++)
                {
                    if (i == keyrow)
                        sim[i][j] = sim[i][j] / keyrowval[keycol]; // for pivot row
                    else
                        sim[i][j] = sim[i][j] - (keycolval[i] * keyrowval[j]) / keyrowval[keycol]; // for other rows
                }
            }
        }
        check = 0;
        maxincmz = -90000.0;
        for (i = 0; i < (n + var); i++)
        {
            sum = 0;
            for (j = 0; j < m; j++)
                sum += cb[basv[j]] * sim[j][i];
            Z[i] = sum;
            if (choice == 0)
                CminusZ[i] = cb[i] - Z[i]; // Z_i - C_i values for each variable
            else
                CminusZ[i] = Z[i] - cb[i];
            if (CminusZ[i] > 0)
                check = 1;
            if (CminusZ[i] > maxincmz) // finds the key column
            {
                keycol = i;
                maxincmz = CminusZ[i];
            }
        }
        sum = 0;
        for (i = 0; i < m; i++)
            sum += cb[basv[i]] * sol[i]; // calculates the sum for the solution
        zsol = sum;
        mininsol = 999999.0;
        for (i = 0; i < m; i++)
            keycolval[i] = sim[i][keycol];
        for (i = 0; i < m; i++)
        {
            ratio[i] = sol[i] / keycolval[i]; // finds the ratio in each iteration
            if ((sol[i] == 0.000) && (keycolval[i] < 0))
                continue;
            if (ratio[i] < 0.0000000)
                continue;
            if (ratio[i] <= mininsol)
            {
                mininsol = ratio[i];
                keyrow = i;
            }
        }
        for (i = 0; i < (n + var); i++)
            keyrowval[i] = sim[keyrow][i];
        solkey = sol[keyrow];
        cout << "\nIteration: " << iter + 1 << "\n";
        cout << "\t Cb \t Cj ";
        for (i = 0; i < (n + var); i++)
            cout << "\t" << cb[i];
        cout << "\n \t \t BV. ";
        for (i = 0; i < (var + n); i++)
            cout << "\t X[" << i+1 << "]";
        cout << "\t Solution\n";
        for (i = 0; i < (22 * (m + n) + 20); i++)
            cout << "-";
        cout << "\n";
        for (i = 0; i < m; i++)
        {
            cout << "\t " << fixed << setprecision(2) << cb[basv[i]] << basv[i] + 1;
            for (j = 0; j < (n + var); j++)
                cout << "\t " << sim[i][j];
            cout << "\t " << sol[i] << "\n";
        }
        for (i = 0; i < (22 * (m + n) + 20); i++)
            cout << "-";
        cout << "\n\t Zj \t";
        for (i = 0; i < (n + var); i++)
            cout << "\t " << Z[i];
        cout << "\t " << zsol;
        cout << "\n \t Cj - Zj ";
        for (i = 0; i < (n + var); i++)
        {
            if (choice == 0)
                cout << "\t " << (-1)*CminusZ[i];
            else
                cout << "\t " << CminusZ[i];
        }
        cout << "\n";
        for (i = 0; i < (22 * (m + n) + 20); i++)
            cout << "-";
        cout << "\n";
        cout << "\nMininum Ratio is: " << mininsol << "at Pivot Row: " << keyrow + 1<< "\n";
        if (choice == 0)
            cout << "Minimum Zi - Ci is: " << (-1)*maxincmz << "at Pivot Column: " << keycol + 1;
        else
            cout << "Maximum Zi - Ci is: " << maxincmz << "at Pivot Column: " << keycol + 1;
        cout << "Value pf Z is: " << zsol << "\n";
        iter++;
    }
    for (i = 0; i < m; i++)
    {
        if (cb[basv[i]] < -1000000.0)
        {
            cout << "All Artificial Varibles haven't been reduced to 0, hence Infeasible Solution.\n";
            return;
        }
    }
    int *integer_sol = (int *)malloc(100 * sizeof(int));
    double *fractional_part = (double *)malloc(100 * sizeof(double));
    double maxfrac = -1;
    double *varibls = (double *)malloc((100) * sizeof(double));
    int maxfrac_ind;
    double frac_indicator;
    double intPart, fractPart;
    double epsilon = 1e-9;
    int flag = 1;
    bool flagger;
    cout << "\nHence, The Optimal Values are:\n";
    for(i = 0; i < n; i++)
    {
        flagger = true;
        for(j=0;j<m;j++)
        {
            if(basv[j] == i)
            {
                cout << "X[" << basv[j]+1 << "]=" << sol[j] << "\n";
                flagger = false;
            }
        }
        if(flagger)
            cout << "X[" << i+1 << "]= 0.00\n";
    }
    cout << "Z : " << zsol << "\n";
    iter = 0;
    while (1)
    {
        if (iter++ > 6)
            break;
        cout << "\n";
        maxfrac = -1.0;
        for (i = 0; i < num_equations; i++)
        {
            integer_sol[i] = decompose(sol[i]).first;
            fractional_part[i] = decompose(sol[i]).second;
            if (maxfrac < fractional_part[i])
            {
                maxfrac = fractional_part[i];
                maxfrac_ind = i;
            }
        }
        for (i = 0; i < num_equations; i++)
            cout << "For X[ " << basv[i] + 1 << " Integer is " << integer_sol[i] << " Fractional Part is " << fractional_part[i] << "\n";
        for (i = 0; i < num_equations; i++)
            sim[i][num_var] = 0.0;
        cout << "Hence, The Maximum fractional part is coming out at X[" << maxfrac_ind+1 << " which is " << maxfrac << "\n";
        num_var++;
        num_equations++;
        k = 0;
        for (i = 0; i < num_var; i++)
        {
            if (i < (num_var - 1))
            {
                fractPart = decompose(sim[maxfrac_ind][i]).second;
                if (abs(fractPart) > 0.9999)
                {
                    fractPart = 0.0;
                    varibls[i] = fractPart;
                    cout << varibls[i] << "*X[" << i+1 << "]";
                    continue;
                }
                if (fractPart >= 0.0)
                {
                    varibls[i] = -fractPart;
                    cout << varibls[i] << "*X[" << i+1 << "] + ";
                }
                else
                {
                    varibls[i] = -1.0 - fractPart;
                    cout << varibls[i] << "*X[" << i+1 << "]";
                }
            }
            else
            {
                varibls[i] = 1.0;
            }
            sim[num_equations - 1][i] = varibls[i];
        }
        k = 0;
        cout << " X[" << num_var << "]\n";
        cout << "Hence, Printing table now we get:";
        printf("The number of variables now is : %d\t And the number of equations is %d\nSo, printing the table after adding the extra variable, we get :\n", num_var, num_equations);
        sol[num_equations - 1] = -maxfrac;
        cb[num_var - 1] = 0.0;
        basv[num_equations - 1] = (num_var - 1);
        cout << "\n\t Cb \t Cj ";
        for (i = 0; i < (num_var); i++)
            cout << cb[i];
        cout << "\n \t \t BV. ";
        for (i = 0; i < (num_var); i++)
            cout << "\t     X[" << i+1 << "]";
        cout << "Solution\n";
        for (i = 0; i < (25 * (num_var) + 20); i++)
            cout << "-";

        cout << "\n";
        for (i = 0; i < num_equations; i++)
        {
            cout << "\t " << cb[basv[i]] << " X[" << basv[i] + 1 << "]";
            for (j = 0; j < (num_var); j++)
                cout << "\t " << sim[i][j];
            cout << "\t " << sol[i] << "\n";
        }
        for (i = 0; i < (25 * (num_var) + 20); i++)
            cout << "-";
        cout << "\n";
        for (i = 0; i < num_var; i++)
        {
            sum = 0;
            for (j = 0; j < m; j++) // now we will calculate the Z values for each variable
                sum += cb[basv[j]] * sim[j][i];
            Z[i] = sum;
            CminusZ[i] = cb[i] - Z[i]; // Z_i - C_i values for each variable
        }
        cout << "\n\t Zj \t";
        for (i = 0; i < (num_var); i++)
            cout << "\t " << Z[i];
        sum = 0;
        for (i = 0; i < num_equations; i++)
            sum += cb[basv[i]] * sol[i]; // calculates the sum for the solution
        zsol = sum;
        cout << "\t " << zsol;
        cout << "\n \t Cj - Zj ";
        for (i = 0; i < (num_var); i++)
            cout << "\t " << CminusZ[i];
        cout << "\n";
        for (i = 0; i < (22 * (num_var) + 20); i++)
            cout << "-";
        cout << "\n";
        int baniter = 0;
        while (1)
        {
            if (baniter++ > 5)
                break;
            mininsol = 0;
            for (i = 0; i < num_equations; i++)
            {
                if (sol[i] < mininsol)
                {
                    mininsol = sol[i];
                    keyrow = i;
                }
                if (sol[i] < 0)
                {
                    check = 1;
                }
            }
            cout << "Most Negative Value if at row " << keyrow + 1 << "which is " << sol[keyrow] << ",and hence the leaving variable is:" << basv[keyrow] + 1;
            for (i = 0; i < (20 * (num_var) + 5); i++)
                cout << "-";
            cout << "\n";
            cout << "Variables ";
            for (i = 0; i < (num_var); i++)
                cout << "\t  X[" << i+1;
            cout << "\n";
            for (i = 0; i < (20 * (num_var) + 5); i++)
                cout << "-";
            cout << "\n Zj - Cj";
            for (i = 0; i < (num_var); i++)
                cout << "\t " << (-1)*CminusZ[i];
            cout << "\n X[" << keyrow + 1 << "]\t";
            for (i = 0; i < (num_var); i++)
                cout << "\t " << sim[keyrow][i];
            double minratio = -100000;
            double ratio2;
            for (i = 0; i < (num_var); i++)
            {
                if (sim[keyrow][i] < -0.0000001)
                {
                    ratio2 = (-1 * CminusZ[i]) / sim[keyrow][i];
                    cout << "\t " << ratio2;
                    if (ratio2 > minratio)
                    {
                        minratio = ratio2;
                        keycol = i;
                    }
                }
                else
                    cout << "\t --      ";
            }
            cout << "\n";
            for (i = 0; i < (20 * (num_var) + 5); i++)
                cout << "-";
            cout << "The Maximum Value is " << minratio << " hence the entering variable is X[" << keycol+1 << "]";
            for (i = 0; i < (num_var); i++)
                keyrowval[i] = sim[keyrow][i];
            solkey = sol[keyrow];
            for (i = 0; i < num_equations; i++)
                keycolval[i] = sim[i][keycol]; // now it evaluates the key colvalues
            basv[keyrow] = keycol; //entering variable is put in the basic variable column
            cout << "Key Row and column are: " << keyrow << " " << keycol << " respectively, hence the element is " << keyrowval[keycol];
            for (i = 0; i < num_equations; i++)
            {
                if (i == keyrow)
                    sol[i] = sol[i] / keyrowval[keycol]; // for the pivot row
                else
                    sol[i] = sol[i] - (keycolval[i] * solkey) / keyrowval[keycol]; // for other rows
                for (j = 0; j < (num_var); j++)
                {
                    if (i == keyrow)
                        sim[i][j] = sim[i][j] / keyrowval[keycol]; // for pivot row
                    else
                        sim[i][j] = sim[i][j] - (keycolval[i] * keyrowval[j]) / keyrowval[keycol]; // for other rows
                }
            }
            cout << "\n\t Cb \t Cj ";
            for (i = 0; i < (num_var); i++)
                cout << "\t " << cb[i];
            cout << "\n \t \t BV.";
            for (i = 0; i < (num_var); i++)
                cout << "\t     X[" << i+1 << "]";
            cout << "Solution\n";
            for (i = 0; i < (25 * (num_var) + 20); i++)
                cout << "-";
            cout << "\n";
            for (i = 0; i < num_equations; i++)
            {
                cout << "\t " << cb[basv[i]] << "   X[" << basv[i] + 1 << "]";
                for (j = 0; j < (num_var); j++)
                    cout << "\t " << sim[i][j];
                cout << "\t " << sol[i] << "\n";
            }
            for (i = 0; i < (25 * (num_var) + 20); i++)
                cout << "-";
            cout << "\n";
            for (i = 0; i < num_var; i++)
            {
                sum = 0;
                for (j = 0; j < m; j++) // now we will calculate the Z values for each variable
                    sum += cb[basv[j]] * sim[j][i];
                Z[i] = sum;
                CminusZ[i] = cb[i] - Z[i]; // Z_i - C_i values for each variable
            }
            cout << "\n\t Zj << \t ";
            for (i = 0; i < (num_var); i++)
                cout << "\t " << Z[i];
            sum = 0;
            for (i = 0; i < num_equations; i++)
                sum += cb[basv[i]] * sol[i]; // calculates the sum for the solution
            zsol = sum;
            cout << "\t " << zsol;
            cout << "\n \t Cj-Zj ";
            for (i = 0; i < (num_var); i++)
                cout << "\t " << CminusZ[i];
            cout << "\n";
            for (i = 0; i < (22 * (num_var) + 20); i++)
                cout << "-";
            cout << "\n";
            frac_indicator = 0.0001;
            doublecheck = 1;
            for (i = 0; i < num_equations; i++)
            {
                if (sol[i] < 0)
                    doublecheck = 0;
            }
            if (doublecheck)
                break;
        }
        flag = 1;
        for (i = 0; i < num_equations; i++)
        {
            if (basv[i] >= n)
                continue;
            fractPart = decompose(sol[i]).second;
            integer_sol[i] = decompose(sol[i]).first;
            fractional_part[i] = fractPart;
            printf("So, x_%d = %lf + %d\n", basv[i] + 1, fractional_part[i], decompose(sol[i]).first);
            if (fractional_part[i] > frac_indicator)
                flag = 0;
        }
        if (flag)
            break;
        else
            cout << "Optimal Solution not reached.\n";
    }
    if (iter > 5)
        cout << "No Integer Solutions\n";
    else
    {
        cout << "Optimal Answer\n";
        for(j=0;j<n;j++)
        {
            flagger = true;
            for (i = 0; i < num_equations; i++)
            {
                if(j==basv[i])
                {
                    cout << "X[" << basv[i] + 1 << "] = " << sol[i] << "\n";
                    flagger = false;
                }
            }
            if(flagger)
                cout << "X[" << j+1 << "] = 0.00\n";
        }
        cout << "Z = " << zsol << "\n";
    }
    return;
}
int main() // menu driven program
{
    int n, m, i, j, k,*sign,choice,var=0;
    double **A, *B,*Z;
    cout << "Enter the Number of Variables:\n";     // reading in the required inputs
    cin >> n;
    cout << "Enter the Number of Equations:\n";
    cin >> m;
    sign = (int *)malloc(m * sizeof(int));
    A = (double **)malloc(m * sizeof(double *));
    for (i = 0; i < m; i++)
        A[i] = (double *)malloc(n * sizeof(double));
    B = (double *)malloc(m * sizeof(double));
    Z = (double *)malloc(n * sizeof(double));
    choice = Take_input(A,B,sign,Z,n,m);                    // function which would take input
    for (i = 0; i < m; i++)
    {
        if (sign[i] == 1)
            var += 2;
        else
            var++;
    }
    BigM(A, B, Z, n, m, var, sign, choice);
    return 0;
}
