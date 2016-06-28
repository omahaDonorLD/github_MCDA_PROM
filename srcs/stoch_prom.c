
/** For a better documentation see file main.h **/

#ifndef PROM_C
#define PROM_C

#include "../heads/stoch_prom.h"


float DELTA(float x, float d_mu, float d_sigma)
{
	return 0.5*(1+erf((x-d_mu)/(sqrt(2.)*d_sigma)));
}


float** EPF_level_criterion(const float* e_l_i, int j, bool sort_shift)
{

	int a=0,b=0,i=0, n_cols=0, n_ites=0;
	float d_MU=0., d_SIGMA=0., buff=0., buff2=0.;
	bool assigned=false;
	float** P_i;

	/* the number of columns and number of iterations change given the purpose of the calculation :
			are we just applying PROMETHEE or a sorting will also be performed. The boolean "sort_shift" tells it. */
	if(sort_shift)
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

	for(i=0;i<N;i++)
	{
		P_i[i]=calloc(n_cols,sizeof(float));
		if(P_i[i] == NULL){ /* memory allocation failure */ printf("MEMO_ALLOC_FAILURE line %d in file %s\n", __LINE__, __FILE__); }
	}

	assigned=false;

	for(a=0;a<n_ites;a++)
	{
		if(sort_shift)
			b=0;
		else
			b=a+1;

		for(;b<n_cols;b++)
		{
			if(sort_shift)
			{
				d_MU=e_l_i[a]-R[b][j];
				d_SIGMA=sqrt(pow(STDs[j][a],2.)+pow(R[b][j],2.));
			}
			else
			{/* if nonsorting <=> if ranking */
				d_MU=e_l_i[a]-e_l_i[b];
				d_SIGMA=sqrt(pow(STDs[j][a],2.)+pow(STDs[j][b],2.));
			}

			assigned=false;
			i=0;
			buff=(d_MU < 0 ? -d_MU : d_MU);/* absolute value since P_i[worst][best] has the worst value anyway, remains the best which has always d > 0 */

			while(!assigned)
			{
				if( buff < THRESHOLDS[j][i] )
				{
					buff2=( DELTA(THRESHOLDS[j][1],buff,d_SIGMA) + DELTA(THRESHOLDS[j][2],buff,d_SIGMA) );
					P_i[a][b]=( d_MU >= 0 ? (1-buff2/2) : -(1-buff2/2));
					assigned=true;
					continue;
				}

				i++;

				if( i==3 )
				{/* End of the threshold array => strict preferrence */
					buff2=( DELTA(THRESHOLDS[j][1],buff,d_SIGMA) + DELTA(THRESHOLDS[j][2],buff,d_SIGMA) );
					P_i[a][b]=( d_MU >= 0 ? (1-buff2/2) : -(1-buff2/2));
					assigned=true;
				}
			}
		}
	}

	return P_i;
}


#endif

