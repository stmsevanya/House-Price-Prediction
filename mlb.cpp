#include <bits/stdc++.h>
#include <stdio.h>
#include <iostream>
#include <string.h>
#include <fstream>

using namespace std;


#define m 17290
#define M 21613
#define N 5

void multiplication(double A[][m] , int rA, int cA, double B[][N], int rB, int cB,double C[][N]);
bool inverse(double A[N][N], double inverse[N][N]);



int main()
{
    int i=0,a,j,k,n=4;
    ifstream file ( "MLinput.csv" );
    string value, val;
    double x[M+1][n+1];
    double y[M+1];


////////////////////////READING FROM INPUT FILE////////////////////////////////////
    getline ( file, value);
    while ( file.good() )
    {
        getline ( file, value);
        //cout<<value<<endl;
        a=0;j=0;k=0;
        x[i][0]=1;
        while(j<n)
        {
            if(value[k]==',')
            {
                val=value.substr(a,k-a);
                x[i][++j]=stod(val);
                a=k+1;
            }

            if(j==n)
            {
                val=value.substr(a);
                y[i++]=stod(val);
            }
            k++;
        }

        if(i>M)break;

    }
//////////////////////////////////////////INPUT TAKEN////////////////////////////////////////////////////
    cout<<"Woah"<<endl;


    double learning_rate = 0.05;
    double rmse,h;
    int d, flag2;
    double theta[n+1]={100,100,100,100,100};
    double htheta[m];
    double sumx[n+1]={0,0,0,0,0};
    double sumy=0;
    double max[n+1]={0,0,0,0,0};
    double min[n+1]={1000000,1000000,1000000,1000000,1000000};
    double dervCost[n+1];
    double costfunction=0,prev_cf=0,converge;
    double miny=1000000, maxy=0;


//////////////////////////////////////SCALING FEATURES/////////////////////////////////////////////
    for(i=0;i<m;i++)
    {
        for(j=1;j<=n;j++)
        {
            sumx[j]+=x[i][j];
            if(x[i][j]>max[j])max[j]=x[i][j];
            if(x[i][j]<min[j])min[j]=x[i][j];
        }
        sumy+=y[i];
        if(y[i]>maxy)maxy=y[i];
        if(y[i]<miny)miny=y[i];
    }
    sumx[1]/=1.0*m;sumx[2]/=1.0*m;sumx[3]/=1.0*m;sumx[4]/=1.0*m;
    sumy/=1.0*m;

    for(i=0;i<M;i++)
    {
        for(j=1;j<=n;j++)
        {
            x[i][j]=(x[i][j]-sumx[j])/(max[j]-min[j]);
            //if(j==1)x[i][j]*=100;
        }
        if(i>=m)continue;
        y[i]=(y[i]-sumy)/(maxy-miny);
    }

    //////////////////////////////////////SCALING DONE//////////////////////////////////////////////


    ////////////////////////////////////////////////////////////////////////////////
    int flag=0;
    flag2=5;
    cout<<endl<<"By Gradient Descent with Learning Rate = "<<learning_rate<<endl;
    while(1)
    {
                //if(flag)converge=(((costfunction-prev_cf)>0)?(costfunction-prev_cf):(prev_cf-costfunction));
                if(flag%1000==0 || flag%flag2==0)
                {
                        if(flag && flag%flag2==0)flag2*=10;
                        //testing Data
                        rmse=0.0;
                        j=0;
                        for(d=m;d<M;d++)
                        {
                            h=theta[0];

                            j=1;
                                for(a=1;a<=n;a++)
                                {
                                    h+=theta[j]*x[d][a];
                                    j++;
                                }

                            h*=(maxy-miny);
                            h+=sumy;
                            rmse+=(h-y[d])*(h-y[d])/(1.0*(M-m));
                        }
                        //cout<<theta[0]<<" "<<theta[1]<<" "<<theta[2]<<" "<<theta[3]<<" "<<theta[4]<<endl;
                        cout<<"\tNumber of iterations = "<<flag<<", RMSE = "<<sqrt(rmse)<<endl;


                    if(flag==5000)break;
                }


        costfunction=0;
        for(j=0;j<=n;j++)
            dervCost[j]=0;

        for(i=0;i<m;i++)
            htheta[i]=0;


        for(i=0;i<m;i++)
        {
            //for each i

            for(j=0;j<=n;j++)
            {// calculating h0(xi)
                htheta[i]+=theta[j]*x[i][j];
            }

            for(j=0;j<=n;j++)
            {
                dervCost[j]+=(htheta[i]-y[i])*x[i][j];
            }
            costfunction+=(htheta[i]-y[i])*(htheta[i]-y[i]);

        }


        for(j=0;j<=n;j++)
        {
            theta[j]-=learning_rate/(1.0*m)*dervCost[j];
        }

        costfunction/=(2.0*m);
        prev_cf=costfunction;
        flag++;
    }
    ///////////////////////////////////////////////////////////////////////////////////

    cout<<endl<<"By Iterative Reweighted Least Square Method"<<endl;

    double A[n+1][m];
    double C[n+1][N];
    double D[n+1][1];

    //transpose
    for(i=0;i<m;i++)
    {
        for(j=0;j<=n;j++)
        {
            A[j][i]=x[i][j];
        }
    }

    //multiplication(A,n+1,m,x,m,n+1,C);
    for (i = 0; i < N; i++)
    {
        for (j = 0; j < N; j++)
        {
            C[i][j] = 0;
            for (k = 0; k < m; k++)
            {
                C[i][j] += A[i][k]*x[k][j];
            }
        }
    }

    inverse(C,C);

    //multiplication(A,n+1,m,y,m,1,D);
    for (i = 0; i < N; i++)
    {
        for (j = 0; j < 1; j++)
        {
            D[i][j] = 0;
            for (k = 0; k < m; k++)
            {
                D[i][j] += A[i][k]*y[k];
            }
        }
    }

    //multiplication(C,N,N,D,N,1,C);
    for (i = 0; i < N; i++)
    {
        for (j = 0; j < 1; j++)
        {
            theta[i] = 0;
            for (k = 0; k < N; k++)
            {
                theta[i] += C[i][k]*D[k][j];

            }
        }
        //cout<<theta[i]<< "  " ;
    }



    rmse=0.0;
    j=0;
    for(d=m;d<M;d++)
    {
        h=theta[0];

        j=1;
            for(a=1;a<=n;a++)
            {
                h+=theta[j]*x[d][a];
                j++;
            }

        h*=(maxy-miny);
        h+=sumy;
        rmse+=(h-y[d])*(h-y[d])/(1.0*(M-m));
    }
    cout<<endl<<"\tNumber of iterations = 0, RMSE = "<<sqrt(rmse)<<endl;

    cout<<"Done"<<endl;

    return 0;
}


///////////////////////////////////////////////////////////////////////////////////
// C++ program to find adjoint and inverse of a matrix


// Function to get cofactor of A[p][q] in temp[][]. n is current
// dimension of A[][]
void getCofactor(double A[N][N], double temp[N][N], int p, int q, int n)
{
    int i = 0, j = 0;

    // Looping for each element of the matrix
    for (int row = 0; row < n; row++)
    {
        for (int col = 0; col < n; col++)
        {
            //  Copying into temporary matrix only those element
            //  which are not in given row and column
            if (row != p && col != q)
            {
                temp[i][j++] = A[row][col];

                // Row is filled, so increase row index and
                // reset col index
                if (j == n - 1)
                {
                    j = 0;
                    i++;
                }
            }
        }
    }
}

/* Recursive function for finding determinant of matrix.
   n is current dimension of A[][]. */
double determinant(double A[N][N], int n)
{
    double D = 0; // Initialize result

    //  Base case : if matrix contains single element
    if (n == 1)
        return A[0][0];

    double temp[N][N]; // To store cofactors

    double sign = 1.0;  // To store sign multiplier

     // Iterate for each element of first row
    for (int f = 0; f < n; f++)
    {
        // Getting Cofactor of A[0][f]
        getCofactor(A, temp, 0, f, n);
        D += sign * A[0][f] * determinant(temp, n - 1);

        // terms are to be added with alternate sign
        sign = -sign;
    }

    return D;
}

// Function to get adjoint of A[N][N] in adj[N][N].
void adjoint(double A[N][N],double adj[N][N])
{
    if (N == 1)
    {
        adj[0][0] = 1;
        return;
    }

    // temp is used to store cofactors of A[][]
    double sign = 1, temp[N][N];

    for (int i=0; i<N; i++)
    {
        for (int j=0; j<N; j++)
        {
            // Get cofactor of A[i][j]
            getCofactor(A, temp, i, j, N);

            // sign of adj[j][i] positive if sum of row
            // and column indexes is even.
            sign = ((i+j)%2==0)? 1: -1;

            // Interchanging rows and columns to get the
            // transpose of the cofactor matrix
            adj[j][i] = (sign)*(determinant(temp, N-1));
        }
    }
}

// Function to calculate and store inverse, returns false if
// matrix is singular
bool inverse(double A[N][N], double inverse[N][N])
{
    // Find determinant of A[][]
    double det = determinant(A, N);
    if (det == 0)
    {
        cout << "Singular matrix, can't find its inverse";
        return false;
    }

    // Find adjoint
    double adj[N][N];
    adjoint(A, adj);

    // Find Inverse using formula "inverse(A) = adj(A)/det(A)"
    for (int i=0; i<N; i++)
        for (int j=0; j<N; j++)
            inverse[i][j] = adj[i][j]/double(det);

    return true;
}

////////////////////////////////////////////////////////////////////////////////////////
/*
void multiplication(double** A , int rA, int cA, double** B, int rB, int cB,double** C)
{
    int i,j,k;
    //double C[rA][cB];
    for (i = 0; i < rA; i++)
    {
        for (j = 0; j < cB; j++)
        {
            C[i][j] = 0;
            for (k = 0; k < rB; k++)
            {
                C[i][j] += A[i][k]*B[k][j];
            }
        }
    }
    //return &C[0][0];
}
*/
