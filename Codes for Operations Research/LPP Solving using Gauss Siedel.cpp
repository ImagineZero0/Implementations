/*
Name -> Varun Gupta
Roll Number -> 18MA20050
OR LAB WEEK 1 Question 2
This is a implementation of the Gauss Siedel Algorithm in C++
*/

#include <stdio.h>
#include <math.h>
#include <stdlib.h>

float *Gauss_Seidel(float **A, float *B, int n, float err)
{
    int i, j;
    bool flag = true;
    float *X, *x_answer, total;
    float allErrors;
    X = (float *)malloc(n * sizeof(double));
    x_answer = (float *)malloc(n * sizeof(double));
    for (i = 0; i < n; i++)
    {
        X[i] = 0;
    }
    int counter = 0;
    while (flag == true)
    {
        counter ++;
        for (i = 0; i < n; i++)
        {
            total = B[i];
            for (j = 0; j < n; j++)
            {
                if (i != j)
                {
                    total -= A[i][j] * X[j];
                }
            }
            x_answer[i] = total / A[i][i];
            allErrors = (x_answer[i] - X[i]) / x_answer[i];
            if (allErrors < 0)
                allErrors *= (-1);
            if (allErrors > err)
            {
                flag = true;
                X[i] = x_answer[i];
            }
            else
            {
                flag = false;
            }
        }
        if(counter > 20)
        {
            printf("Diverging\n");
            break;
        }
    }
    return x_answer;
}
void generatefinal_answer(int permutation_n_c_m[],float **A,float *B,int initial,int values,int m,int n,float err)
{
    int j=0,k=0,i;
    float *x;
    float *final_answer;
    x=(float *)malloc(m*sizeof(double));
    final_answer=(float *)malloc(n*sizeof(double));

    if (values==m)
    {
        float **A_square;
        A_square=(float ** )malloc(m*sizeof(float * ));
        for (j=0;j<m;j++)
            {
                A_square[j]=(float *)malloc(m*sizeof(double));
            }
        for (j=0;j<m;j++)
        {
            for (k=0;k<m;k++)
            {

                A_square[j][k]=A[j][permutation_n_c_m[k]];
            }
        }

        x = Gauss_Seidel(A_square,B,m,err);

        for(j=0;j<n;j++)
        {
            final_answer[j]=0;
        }
        for (k=0;k<m;k++)
        {
            final_answer[permutation_n_c_m[k]]=x[k];
        }
        for (j=0;j<n;j++)
        {
            printf("x[%d]  = %.3lf ",j,final_answer[j]);
        }
        printf("\n");
        return;
    }

    for (i=initial; i<=n-1 && n-i >= m-values; i++)
    {
        permutation_n_c_m[values] = i;
        generatefinal_answer(permutation_n_c_m,A,B,i+1,values+1,m,n,err);
    }
}
int main()
{
    int n,m, i, j, k;
    printf("Enter the number of variables present\n");
    scanf("%d", &n);
    printf("Enter the number of equations to be solved\n");
    scanf("%d", &m);
    float error = 0.001;
    float **A, *B;
    printf("\nEnter the values for the right hand side for each of the equation\n");
    A = (float **)malloc(m * sizeof(float *));
    for (i = 0; i < m; i++)
    {
        A[i] = (float *)malloc(n * sizeof(double));
    }

    B = (float *)malloc(m * sizeof(double));
    for (i = 0; i < m; i++)
    {
        for (j = 0; j < n; j++)
        {
            printf("A[%d,%d]= ",i,j);
            scanf("%f", &A[i][j]);
        }
    }
    printf("\nEnter the values for the right hand side for each of the equation\n");
    for (i = 0; i < m; i++)
    {
        printf("Enter B[%d]=",i);
        scanf("%f", &B[i]);
    }
    printf("Taking initial values of X as zero.\n");
    int * permutation_n_c_m;
    permutation_n_c_m = (int *)malloc(m*sizeof(int));
    generatefinal_answer(permutation_n_c_m,A,B,0,0,m,n,error);

    return 0;
}
