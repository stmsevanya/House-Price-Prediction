#include <bits/stdc++.h>
#include <stdio.h>
#include <iostream>
#include <string.h>
#include <fstream>

using namespace std;


int main()
{
    int i=0,m=17290,a,j,k,n=4,M=21613;
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


    float learning_rate = 0.05;
    float regularization,rmse,h;
    int d;
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
    regularization=0;
    cout<<endl<<"Learning Rate = "<<learning_rate<<endl;
    while(1)
    {
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
            costfunction+=theta[j]*theta[j];
            theta[j]*=(1-learning_rate*regularization/m);
            theta[j]-=learning_rate/(1.0*m)*dervCost[j];
        }

        costfunction/=(2.0*m);


        if(flag)converge=(((costfunction-prev_cf)>0)?(costfunction-prev_cf):(prev_cf-costfunction));
        if(flag++>1 && converge<0.005)
        {
                //testin Data
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
                cout<<"\tRegularization = "<<regularization<<", RMSE = "<<sqrt(rmse)<<endl;


            regularization+=10;
            theta[0]=100;theta[1]=100;theta[2]=100;theta[3]=100;theta[4]=100;

            if(regularization>100)break;
        }
                prev_cf=costfunction;
    }
    ///////////////////////////////////////////////////////////////////////////////////

    cout<<"Done"<<endl;

    return 0;
}
