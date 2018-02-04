
/** For a better documentation see file main.h **/

#ifndef PROM_C
#define PROM_C

#include "../heads/stoch_prom.h"


float DELTA(float x, float d_mu, float d_sigma)
{
	return 0.5*(1+erf((x-d_mu)/(sqrt(2.)*d_sigma)));
}


float** EPF(const float* e_l_i, int j, bool SORTING)
{

	int a=0, b=0, n_cols=0, n_ites=0;
	float d_MU=0., d_SIGMA=0.;
	float** P_i;

	/* the number of columns and number of iterations change given the purpose of the calculation :
			are we just applying PROMETHEE or a sorting will also be performed ? The boolean "SORTING" tells it. */
	if(SORTING)
	{
		n_cols=N_CAT;
		n_ites=N;
	}
	else
	{
		n_cols=N;
		n_ites=N-1;
	}

	P_i=malloc(	N*sizeof(float*));

	if(P_i == NULL){ /* memory allocation failure */ printf("MEMO_ALLOC_FAILURE line %d in file %s\n", __LINE__, __FILE__); }

	for(a=0;a<N;a++)
	{
		P_i[a]=calloc(n_cols,sizeof(float));
		if(P_i[a] == NULL){ /* memory allocation failure */ printf("MEMO_ALLOC_FAILURE line %d in file %s\n", __LINE__, __FILE__); }
	}

	for(a=0;a<n_ites;a++)
	{
		if(SORTING)
			b=0;
		else
			b=a+1;

		for(;b<n_cols;b++)
		{
			if(SORTING)
			{
				d_MU=e_l_i[a]-R[b][j];
				d_SIGMA=sqrt(pow(STDs[j][a],2.)+pow(R_STDs[b][j],2.));/* Reminder : R_STDs : N_CAT*K */
			}
			else
			{/* if nonsorting <=> if ranking */
				d_MU=e_l_i[a]-e_l_i[b];
				d_SIGMA=sqrt(pow(STDs[j][a],2.)+pow(STDs[j][b],2.));
			}

			P_i[a][b]=(*TBL_PREF_FUNC_EPF[TYPE_CRITERION])(j, d_MU, d_SIGMA);
		}
	}
	return P_i;
}


float EPF_level_criterion(int j, float d_MU, float d_SIGMA)
{
	float buff=0., buff2=0., res=0.;
	buff=(d_MU < 0 ? -d_MU : d_MU);
	buff2=( DELTA(THRESHOLDS[j][1],buff,d_SIGMA) + DELTA(THRESHOLDS[j][2],buff,d_SIGMA) );
	res=(1-buff2/2);
	return( d_MU >= 0 ? res : -res );
}


float EPF_linear_criterion(int j, float d_MU, float d_SIGMA)
{
	float PI=0.;
	PI=acos(-1);
	float buff=0., buff1=0., buff2=0., buff3=0., buff4=0., buff5=0., res=0., up_bound=0;
	buff=(d_MU < 0 ? -d_MU : d_MU);
	buff1=(d_SIGMA/(sqrt(2.*PI)*(THRESHOLDS[j][2]-THRESHOLDS[j][1])));
	buff2=exp(-(pow(THRESHOLDS[j][1]-buff,2.)/(2*pow(d_SIGMA,2.))));
	buff3=exp(-(pow(THRESHOLDS[j][2]-buff,2.)/(2*pow(d_SIGMA,2.))));
	buff4=DELTA(THRESHOLDS[j][2],buff,d_SIGMA)*(((buff-THRESHOLDS[j][1])/(THRESHOLDS[j][2]-THRESHOLDS[j][1]))-1);
	buff5=DELTA(THRESHOLDS[j][1],buff,d_SIGMA)*((buff-THRESHOLDS[j][1])/(THRESHOLDS[j][2]-THRESHOLDS[j][1]));
	res=1+(buff1*(buff2-buff3))+buff4+buff5;
	if(res>1)	res=1;/* The EPF has to stay in the limit [0,1] */
	return( d_MU >= 0 ? res : -res );
}

#endif

