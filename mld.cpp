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
    double rmse,h,e;
    int d,error;
    double theta[n+1]={10,10,10,10,10};
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

        }
        if(i>=m)continue;
        y[i]=(y[i]-sumy)/(maxy-miny);
    }

    //////////////////////////////////////SCALING DONE//////////////////////////////////////////////


    ////////////////////////////////////////////////////////////////////////////////

    int flag2=0;
    for(;;)
    {
        if(flag2==0)
        {
            error=1;
            cout<<endl<<"Running for Mean Absolute Error on test data:"<<endl;
            learning_rate = 0.05;
        }
        else if(flag2==5)
        {
            error=2;
            cout<<endl<<"Running for Mean Squared Error on test data:"<<endl;
            learning_rate = 0.05;
        }
        else if(flag2==10)
        {
            error=3;
            cout<<endl<<"Running for Mean Cubic Error on test data:"<<endl;
            learning_rate = 0.001;
        }
        else if(flag2==1||flag2==6)learning_rate = 0.2;
        else if(flag2==2||flag2==7)learning_rate = 0.5;
        else if(flag2==3||flag2==8)learning_rate = 1.0;
        else if(flag2==4||flag2==9)learning_rate = 1.5;
        else if(flag2==11)learning_rate = 0.003;
        else if(flag2==12)learning_rate = 0.005;
        else if(flag2==13)learning_rate = 0.007;
        else break;

        theta[0]=100;theta[1]=100;theta[2]=100;theta[3]=100;theta[4]=100;
        int flag=0;

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
                        e=htheta[i]-y[i];
                        if(error==1)
                        {
                            dervCost[j]+=((e>0)?x[i][j]:-x[i][j]);
                        }
                        else if(error==2)
                        {
                            dervCost[j]+=(e*x[i][j]);
                        }
                        else
                        {
                            dervCost[j]+=((e>0)?(e*e*x[i][j]):(-e*e*x[i][j]));
                        }
                    }

                    if(e<0)e=-e;

                    if(error==1)costfunction+=e;
                    else if(error==2)costfunction+=e*e;
                    else costfunction+=e*e*e;

                }


                for(j=0;j<=n;j++)
                {
                    if(error==1)theta[j]-=learning_rate/(2.0*m)*dervCost[j];
                    else if(error==2)theta[j]-=learning_rate/(1.0*m)*dervCost[j];
                    else if(error==3)theta[j]-=learning_rate/(2.0*m)*dervCost[j]*3.0;
                }

                costfunction/=(2.0*m);

                if(flag)converge=(costfunction-prev_cf)>0?(costfunction-prev_cf):(prev_cf-costfunction);
                if(flag++>1 && converge<0.001)
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
                        cout<<"\tLearning Rate = "<<learning_rate<<", CostErrorFunction = "<<costfunction<<", RMSE = "<<sqrt(rmse)<<endl;



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

