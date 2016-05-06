
/** For a better documentation see file main.h **/

#include "main.h"

data read_data(char** argv)
{
	FILE* fp;
	fp=fopen(argv[1],"r");

	if( fscanf(fp,"%d %d %d", &M, &K, &N) < 0 ){printf("EXIT_FAILURE line %d in file %s\n", __LINE__, __FILE__);}

	if( fscanf(fp,"%f %f", &q, &p) < 0 ){printf("EXIT_FAILURE line %d in file %s\n", __LINE__, __FILE__);}

	data E= (expert*) malloc(M*sizeof(expert));
	int i=0,j=0,k=0;
	for(i=0;i<M;i++)
	{
		E[i].p_l=0.;
		E[i].e_ij=(float**) malloc(K*sizeof(float*));
		E[i].S_l=NULL;

		for(j=0;j<K;j++)
		{
			E[i].e_ij[j]=(float*) calloc(N,sizeof(float));
		}
	}	

	// alloc memory for the criterion weights
	criterion_weights=(float*) calloc(K,sizeof(float));

	float buff=0;
	
	for(i=0;i<K;i++)
	{// criterion weights
		if( fscanf(fp,"%f", &buff) < 0 ){printf("EXIT_FAILURE line %d in file %s\n", __LINE__, __FILE__);};
		criterion_weights[i]=buff;
	}

	for(i=0;i<M;i++)
	{// experts
		if( fscanf(fp,"%f", &buff) < 0 ){printf("EXIT_FAILURE line %d in file %s\n", __LINE__, __FILE__);};
		E[i].p_l=buff;
		for(j=0;j<K;j++)
		{// criteria
			for(k=0;k<N;k++)
			{// actions
				if( fscanf(fp,"%f", &buff) < 0 ){printf("EXIT_FAILURE line %d in file %s\n", __LINE__, __FILE__);};
				E[i].e_ij[j][k]=buff;
			}
		}
	}

	fclose(fp);

	return E;
}


float** level_criterion(const float* e_l_i)
{
	int a=0,b=0,best=-1,worst=-1,i=0;
	float d=0.;

	float** P_i=(float**) malloc(N*sizeof(float*));
	for(a=0;a<N;a++)	P_i[a]=(float*) calloc(N,sizeof(float));
	
	bool assigned=false;
	
	for(a=0;a<N-1;a++)
	{
		for(b=a+1;b<N;b++)
		{
			d=e_l_i[a]-e_l_i[b];

			best=a,worst=b;// supposition
			if(d<0)// bPa and a isn't the best, thus switch best-worst.
			{best=b,worst=a;}
			
			P_i[worst][best]=NO_PREF;// indeed, the worst CAN'T be preferred anyway.

			assigned=false;
			i=0;
			while(!assigned)
			{
				if( fabsf(d) < THRESHOLDS[i] )
				{
					P_i[best][worst]=LEV_CRIT_GRADS[i];
					assigned=true;
				}

				i++;

				if( i==3 )
				{// End of the threshold array => strict preferrence
					P_i[best][worst]=LEV_CRIT_GRADS[i];
					assigned=true;
				}
			}
		}
	}

	return P_i;
}


float** compute_pref_indices(float const* const* const* P_l)
{
	int j=0, a=0, b=0;
	float num_1=0.,num_2=0.,denom=0.;

	float** PI=(float**) malloc(N*sizeof(float*));
	for(j=0;j<N;j++)	PI[j]=(float*) calloc(N,sizeof(float));

	for(a=0;a<N-1;a++)
		for(b=a+1;b<N;b++)
		{
			for(j=0;j<K;j++)
			{
				num_1+=(P_l[j][a][b]*criterion_weights[j]);
				num_2+=(P_l[j][b][a]*criterion_weights[j]);
				denom+=(P_l[j][a][b]);
			}
			PI[a][b]+=num_1/denom;
			PI[b][a]+=num_2/denom;
		}

	return PI;
}


float** compute_phi(float const* const* PI_l)
{
	int a=0, b=0;
	
	float** PHI_l=(float**) malloc(N*sizeof(float*));
	for(a=0;a<3;a++)	PHI_l[a]=(float*) calloc(N,sizeof(float));

	// Summing the scores on the prefs inds
	for(a=0;a<N-1;a++)
	{
		for(b=a+1;b<N;b++)
		{
			PHI_l[0][a]+=PI_l[a][b];// outranking
			PHI_l[1][a]+=PI_l[b][a];// outranked

			// Fill for b at the same time
			PHI_l[0][b]+=PHI_l[1][a];// PHI_PLUS_of_b_TO_ONLY_a(b,a)=PHI_MINUS_of_a_FROM_ONLY_b(b,a)
			PHI_l[1][b]+=PHI_l[0][a];// PHI_MINUS_of_b_FROM_ONLY_a(a,b)=PHI_PLUS_of_a_TO_ONLY_b(b,a)
		}
	}

	for(a=0;a<N;a++)	PHI_l[2][a]=PHI_l[0][a]-PHI_l[1][a];// Net flow

	return PHI_l;
}


void PROM_1(expert* E_l, float const* const* PHI_l)
{
	int a=0, b=0, greater=-1, smaller=-1;
	
	/* May save time and skip performing an O(N^2) loop */
	for(a=0;a<N-1;a++)
	{
		for(b=a+1;b<N;b++)
		{
			// Default :
			greater=a;
			smaller=b;

			// Change if the opposite
			if ( PHI_l[0][smaller] > PHI_l[0][greater] )
			{
				greater=b;
				smaller=a;
			}

			// Preference
			if ( ( PHI_l[0][greater] > PHI_l[0][smaller] && PHI_l[1][greater] <= PHI_l[1][smaller] )
					|| ( PHI_l[0][greater] == PHI_l[0][smaller] && PHI_l[1][greater] < PHI_l[1][smaller] ) )
			{
				E_l->S_l=add_S_l(E_l->S_l, 3, greater, smaller);
			}

			// Indifference
			if ( PHI_l[0][greater] == PHI_l[0][smaller] && PHI_l[1][greater] == PHI_l[1][smaller] )
			{// Since indifference => add both with value 2
				E_l->S_l=add_S_l(E_l->S_l, 2, greater, smaller);
				E_l->S_l=add_S_l(E_l->S_l, 2, smaller, greater);
			}


			// Incomparability
			if ( ( PHI_l[0][greater] > PHI_l[0][smaller] && PHI_l[1][greater] > PHI_l[1][smaller] )
					|| ( PHI_l[0][greater] < PHI_l[0][smaller] && PHI_l[1][greater] < PHI_l[1][smaller] ) )
			{// Since incomparability => add both with value 1
				E_l->S_l=add_S_l(E_l->S_l, 1, greater, smaller);
				E_l->S_l=add_S_l(E_l->S_l, 1, smaller, greater);
			}
		}
	}
}


float** aggregate_S_l(const data* E)
{
	int l=0;

	// Init binary preference relations
	float** S=(float**) malloc(N*sizeof(float*));
	for(l=0;l<N;l++)	S[l]=(float*) calloc(N,sizeof(float));

	expert_pref* current_iterator;

	for(l=0;l<M;l++)
	{
		current_iterator=E[l]->S_l;
		while(current_iterator!=NULL)
		{
			S[current_iterator->a][current_iterator->b]+=E[l]->p_l;
			current_iterator=current_iterator->NEXT_S_l;
		}
	}

	return S;
}


int main(int argc, char** argv)
{
	if( argc < 2 ){printf("Please add more parameters\n");};

	data E=read_data(argv);

	int l=0, i=0;

	float*** P_l=NULL;
	float** PI_l=NULL;
	float** PHI_l=NULL;

	for(l=0;l<M;l++)
	{
		for(i=0;i<K;i++)
		{// Gather P_1_l,..., P_K_l and keep into P_l
			P_l[i]=level_criterion(E[l].e_ij[i]);
		}

		PI_l=compute_pref_indices(P_l);// PI = [ ( sum{j in 1 to K}(w[j]*P[j](a,b)) )/( sum{i in 1 to K} (w[j]) ) ]_n*n (<=> PI is of size n*n). PI <=> P_j(a,b) simultaneously on all the criteria
		PHI_l=compute_phi(PI_l);
		PROM_1(&E[l], PHI_l);
	}
	
	float** S=aggregate_S_l(&E);


/*	
	for(l=0;l<M;l++)
	for(i=0;i<K;i++)
		printf("%f %f %f %f\n",E[l].e_ij[i][0],E[l].e_ij[i][1],E[l].e_ij[i][2],E[l].e_ij[i][3]);
*/

	
	return EXIT_SUCCESS;
}
