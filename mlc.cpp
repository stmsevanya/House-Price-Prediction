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
    int combination,b,c,d,flag;
    double rmse, h;
    double theta[40]={100};
    double htheta[M];
    double sumx[n+1]={0,0,0,0,0};
    double sumy=0;
    double max[n+1]={0,0,0,0,0};
    double min[n+1]={1000000,1000000,1000000,1000000,1000000};
    double dervCost[40];
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
    int flag2=0;
    combination=0;
    for(;;)
    {
        flag=0;

        for(i=0;i<40;i++)theta[i]=100;

        if(flag2%6==0)
        {
            cout<<endl;
            if(++combination>3)break;
            if(combination==1)
            cout<<"Combination : Linear"<<endl;
            if(combination==2)
            cout<<"Combination : Quadratic"<<endl;
            if(combination==3)
            cout<<"Combination : Cubic"<<endl;

        }
            while(1)
            {
                costfunction=0;

                for(j=0;j<=40;j++)
                    dervCost[j]=0;

                for(i=0;i<m;i++)
                    htheta[i]=theta[0];


                for(i=0;i<m;i++)
                {
                    //for each i

                    //calculating h0(xi)
                    j=1;
                        for(a=1;a<=n;a++)
                        {
                            htheta[i]+=theta[j]*x[i][a];
                            j++;
                            if(combination>1)
                            {
                                for(b=a;b<=n;b++)
                                {
                                    htheta[i]+=theta[j]*x[i][a]*x[i][b];
                                    j++;
                                    if(combination==3)
                                    {
                                        for(c=b;c<=n;c++)
                                        {
                                            htheta[i]+=theta[j]*x[i][a]*x[i][b]*x[i][c];
                                            j++;
                                        }
                                    }

                                }
                            }
                        }

                    j=0;
                    dervCost[j++]+=(htheta[i]-y[i]);
                    for(a=1;a<=n;a++)
                    {
                        dervCost[j]+=(htheta[i]-y[i])*x[i][a];
                        j++;
                        if(combination>1)
                        {
                            for(b=a;b<=n;b++)
                            {
                                dervCost[j]+=(htheta[i]-y[i])*x[i][a]*x[i][b];
                                j++;
                                if(combination==3)
                                {
                                    for(c=b;c<=n;c++)
                                        {
                                            dervCost[j]+=(htheta[i]-y[i])*x[i][a]*x[i][b]*x[i][c];
                                            j++;
                                        }
                                }
                            }
                        }
                    }
                    costfunction+=(htheta[i]-y[i])*(htheta[i]-y[i]);
                }


                for(a=0;a<j;a++)
                {
                    theta[a]-=learning_rate/(1.0*m)*dervCost[a];
                }

                costfunction/=(2.0*m);

                //cout<<costfunction<<"  ";
                if(flag)converge=(((costfunction-prev_cf)>0)?(costfunction-prev_cf):(prev_cf-costfunction));
                if(flag++>1 && converge<0.005)
                {

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
                                if(combination>1)
                                {
                                    for(b=a;b<=n;b++)
                                    {
                                        h+=theta[j]*x[d][a]*x[d][b];
                                        j++;
                                        if(combination==3)
                                        {
                                            for(c=b;c<=n;c++)
                                            {
                                                h+=theta[j]*x[d][a]*x[d][b]*x[d][c];
                                                j++;
                                            }
                                        }

                                    }
                                }
                            }

                        h*=(maxy-miny);
                        h+=sumy;
                        rmse+=(h-y[d])*(h-y[d])/(1.0*(M-m));
                    }

                    cout<<"\tLearning Rate = "<<learning_rate<<", RMSE = "<<sqrt(rmse)<<endl;


                    if(flag2%6==0)learning_rate=0.1;
                    else if(flag2%6==1)learning_rate=0.3;
                    else if(flag2%6==2)learning_rate=0.7;
                    else if(flag2%6==3)learning_rate=1.0;
                    else if(flag2%6==4)learning_rate=1.5;
                    else if(flag2%6==5)learning_rate=0.05;

                    flag2++;
                    break;

                }

                prev_cf=costfunction;
            }
    ///////////////////////////////////////////////////////////////////////////////////
    }
    cout<<"Done"<<endl;

    return 0;
}
