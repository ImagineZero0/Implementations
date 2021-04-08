/*
Name -> Varun Gupta
Roll Number -> 18MA20050
OR LAB WEEK 1 Question 1
This is a implementation of the Gauss Siedel Algorithm in C++
*/

#include <iostream>
#include <iomanip>

using namespace std;

int main()
{
    int num1,num2,i,j;
    cout << "Enter the number of equations to be solved\n";
    cin >> num1;
    cout << "Enter the number of variables present\n";
    cin >> num2;
    int num = max(num1,num2);
    int check;
    float A[num][num],B[num1],X[num2],Xprev[num2],Xprevprev[num2];
    if(num1 == num2)
        check = num1;
    else
    {
        if(num2 > num1)
        {
            check = num1;
            for(int k = num1;k<num2;k++)
                X[k] = 0;
        }
        else
        {
            check = num2;
        }
    }
    float I[check];
    for(i =0;i<num1;i++)
    {
        for(j=0;j<num2;j++)
        {
            cout << "A[" << i + 1 << "," << j + 1 << "]= ";
            cin >> A[i][j];
        }
    }
    cout << "\nEnter the values for the right hand side for each of the equation\n";
    for(i=0;i<num1;i++)
    {
        cout << "B[" << i + 1 << "]= ";
        cin >> B[i];
    }
   cout << "Taking initial values of X as zero.\n";
   for(i=0;i<num2;i++)
   {
       X[i] = 0;
       Xprev[i] = 0;
       Xprevprev[i] = 0;
   }

    bool flag;
    long long int iter = 0;
   while(true)
   {
       iter++;
       flag = true;
      for(i=0;i<check;i++)
      {
         I[i]=(B[i]/A[i][i]);
         for(j=0;j<check;j++)
         {
            if(j==i)
                continue;
            I[i] -= ((A[i][j] / A[i][i])*X[j]);
         }
         X[i] = I[i];
      }
      for(i=0;i<num2;i++)
      {
          if(abs((Xprev[i] - X[i])/X[i]) > 0.0001)
          {
              flag = false;
              break;
          }
      }
      if(iter > 3)
      {
          for(i=0;i<num2;i++)
          {
              if(abs(Xprevprev[i] - Xprev[i]) < abs(Xprev[i] - X[i]))
              {
                  cout << "Diverging\n";
                  return 0;
              }
          }
      }
      for(i=0;i<num2;i++)
      {
          Xprevprev[i] = Xprev[i];
          Xprev[i] = X[i];
      }
      if(flag == true)
        break;
   }
   for(i=0;i<num2;i++)
        cout<< "X"<< i + 1 << " = "<< fixed << setprecision(4) << X[i]<<" , ";
   cout << "\n";
   return 0;
}
