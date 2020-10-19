/****
This program solves the LLG Equation(ODE) in terms of theta and phi and hence finds the components of the Magnetic Dipole vector <m> : <mx,my,mz>
****/

#include<stdio.h>
#include<stdlib.h>
#include<math.h>

#define gamma -1.76e11
#define Hz 7.96e4 //100e-3*(1/Mu0)
#define omega (gamma*(100e-3)*-1)  //*-1 to get positive T0(time for one precession) omega = Mu0*Hz*gamma
#define T0 ((2*M_PI)/omega)
#define step_size 1e-15


double alpha;
double lambda=0.0;
double* theta;
double* phi;
double* mx;
double* my;
double* mz;
double tk1=0;
double tk2=0;
double pk1=0;
double pk2=0;
double t_half_step=0.0; //half step in theta
double p_half_step=0.0; //halfstep in phi
double mod = 0.0;
double tvalue=0.0;
double pvalue=0.0;
double step=0.0;
int flg = 1;
double val = M_PI / 180.0;
int n=0; //to scale theta between 0 and 360
double temp=0.0; //double to deal with negative angles

double dthetadt(double theta1,double phi1)
{
    tvalue=lambda*Hz*sin(theta1*val);
    return tvalue;
}

double dphidt(double theta2,double phi2)
{
    pvalue=dthetadt(theta2,phi2)*-1*(1/(alpha*sin(theta2*val)));
    return pvalue;
}

int main(int argc,char* argv[])
{
    alpha =atof(argv[1]);
    lambda=alpha*(gamma/(1+pow(alpha,2)));
    theta = malloc(1*(sizeof(double)));
    phi = malloc(1*(sizeof(double)));
    mx = malloc(1*(sizeof(double)));
    my = malloc(1*(sizeof(double)));
    mz = malloc(1*(sizeof(double)));

    int theta_initial= 179 ; //in degrees
    int phi_initial = 1 ; //in degrees

    theta[0]=theta_initial;
    phi[0]=phi_initial;

    mx[0]=(sin(theta[0]*val))*(cos(phi[0]*val));
    my[0]=(sin(theta[0]*val))*(sin(phi[0]*val));
    mz[0]=cos(theta[0]*val);

    //printf("0    %le     %le\n",theta[0],phi[0]);
    //printf("0       %le\n",mz[0]);
    printf("0    %le     %le     %le\n",mx[0],my[0],mz[0]);

    long long unsigned int size=2;
    int i=0;
    while(flg>0)
    {
        theta=realloc(theta,(size)*sizeof(double));
        phi=realloc(phi,(size)*sizeof(double));
        mx=realloc(mx,(size)*sizeof(double));
        my=realloc(my,(size)*sizeof(double));
        mz=realloc(mz,(size)*sizeof(double));

        tk1=dthetadt(theta[i],phi[i]);
        pk1=dphidt(theta[i],phi[i]);

        t_half_step=(0.5)*tk1*step_size;
        p_half_step=(0.5)*pk1*step_size;

        tk2=dthetadt(theta[i]+t_half_step, phi[i]+p_half_step); 
        pk2=dphidt(theta[i]+t_half_step, phi[i]+p_half_step);               

        theta[i+1]=theta[i]+(tk2*step_size);
        phi[i+1]=phi[i]+(pk2*step_size);


        //scaling values down of theta from 0 to 360
        if(phi[i+1]>0&&phi[i+1]>360)
        {
            n=floor(phi[i+1]/360);
            phi[i+1]=phi[i+1]-(n*360);
        }
        if(phi[i+1]<0&&abs(phi[i+1]>360)) 
        {   
            temp=abs(phi[i+1]);
            n=floor(temp/360);
            temp=temp-(n*360);
            phi[i+1]=360-temp;
        }

        mx[i+1]=(sin(theta[i+1]*val))*(cos(phi[i+1]*val));
        my[i+1]=(sin(theta[i+1]*val))*(sin(phi[i+1]*val));
        mz[i+1]=cos(theta[i+1]*val);

       


        //printf("%d    %le     %le\n",(i+1),theta[i+1],phi[i+1]);
        //printf("%d      %le\n",(i+1),mz[i+1]);
        printf("%le     %le     %le     %le\n",(((i+1)*step_size)/pow(10,-15)),mx[i+1],my[i+1],mz[i+1]);

        //time taken = (i+1)*step_size
    
       if(theta[i+1]<=1)
            {flg=0;}

       if(theta[i+1]>1)
       {
            i++;
            size++;
       }

    }

    free(theta);
    free(phi);
    free(mx);
    free(my);
    free(mz);
 

return 0;
}


