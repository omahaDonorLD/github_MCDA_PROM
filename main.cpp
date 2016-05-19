
/** For a better documentation see file main.h **/


#include "main.h"

//#define PRINT_STUFFS

//#define PARTIAL_PREORDER /* PROM_I */
#define COMPLETE_PREORDER /* PROM_II */

/* =========================================================== */
/* Portion of code to use only if one wants to display results */
/* =========================================================== */

	#ifdef PRINT_STUFFS

	void print_data(const data E)
	{

		printf("In print_data\n");

		int l=0, i=0, j=0;

		printf("M:%d, K:%d (respective weights:",M,K);
		for(i=0;i<K;i++)
		{
			printf("%f,",criterion_weights[i]);
		}

		printf("), N:%d, q:%f, p:%f\n",N,q,p);

		for(l=0;l<M;l++)
		{printf("Expert:%d of weight:%f-------------\n",l+1,E[l].p_l);

			for(i=0;i<K;i++)
			{
				for(j=0;j<N;j++)
					printf("%f\t",E[l].e_ij[i][j]);
				printf("\n");
			}
		}
		printf("End print_data\n");
	}


	void print_P_l(float const* const* P_l)
	{

		printf("In print_P_l\n");

		int a=0,b=0;
		for(a=0;a<N;a++)
		{
			printf("%d\t:",a);
			for(b=0;b<N;b++)
			{
				printf("%f\t",P_l[a][b]);
			}
			printf("\n");
		}
		printf("End print_P_l\n");
	}


	void print_PI(float const* const* PI)
	{
		printf("In print_PI\n");

		int a=0,b=0;
		for(a=0;a<N;a++)
		{
			printf("%d\t:",a);
			for(b=0;b<N;b++)
			{
				printf("%f\t",PI[a][b]);
			}
			printf("\n");
		}
		printf("End print_PI\n");
	}
	#endif



	void print_PHI(float const* const* PHI)
	{
		printf("In print_PHI\n");

		int a=0;
		printf("a|\tphi_plus\t|\tphi_minus\t|\tnet\n");
		printf("=====================================================================\n");
		for(a=0;a<N;a++)
		{
			printf("%d:\t%f\t\t%f\t\t%f\n",a,PHI[0][a],PHI[1][a],PHI[2][a]);
		}
		printf("End print_PHI\n");
	}


	void print_S_l(expert_pref* first)
	{
		printf("In print_S_l\n");

		expert_pref* current_iterator=first;

		while(current_iterator!=NULL)
		{
			printf("(%d,%d,%d)\t",current_iterator->a,current_iterator->b,current_iterator->p);
			current_iterator=current_iterator->NEXT_S_l;
		}
		printf("\n");
		printf("End print_S_l\n");
	}


	void print_ranks()
	{
		printf("In print_ranks\n");
		int a=0,l=0;
		for(l=0;l<M;l++)
		{
			printf("%d : ", l);
			for(a=0;a<N;a++)
				printf("%d\t",RANKS[l][a]);
			printf("\n");
		}

		printf("S(a) : ");
		for(a=0;a<N;a++)
			printf("%f\t",COMPLETE_PREORDER_S[a]);
		printf("\n");

		printf("End print_ranks\n");
	}


	void print_aggregated_S_l(float const* const* S)
	{
		printf("In print_aggregated_S_l\n");

		int a=0,b=0;
		for(a=0;a<N;a++)
		{
			for(b=0;b<N;b++)
				printf("%f\t",S[a][b]);
			printf("\n");
		}

		printf("End print_aggregated_S_l\n");
	}

//	#endif

/*============================================*/

int qsort_comparator(const void* pa, const void* pb)
{
	return -( (*(const int **)pa)[1] - (*(const int **)pb)[1] ) ;	
}


data read_data(char** argv)
{	
	FILE* fp;
	fp=fopen(argv[1],"r");

	if( fscanf(fp,"%d %d %d", &M, &K, &N) < 0 ){printf("EXIT_FAILURE line %d in file %s\n", __LINE__, __FILE__);}

	if( fscanf(fp,"%f %f", &q, &p) < 0 ){printf("EXIT_FAILURE line %d in file %s\n", __LINE__, __FILE__);}
	
	/* assign values for thresholds and grades of level criteria */
	THRESHOLDS=(float*) malloc(3*sizeof(float));
	THRESHOLDS[0]=-q; THRESHOLDS[1]=q; THRESHOLDS[2]=p;

	LEV_CRIT_GRADS=(float*) malloc(4*sizeof(float));
	LEV_CRIT_GRADS[0]=NO_PREF; LEV_CRIT_GRADS[1]=INDIFF; LEV_CRIT_GRADS[2]=WEAK_PREF; LEV_CRIT_GRADS[3]=PREF;

	data E=(expert*) malloc(M*sizeof(expert));
	if(E == NULL){ /* memory allocation failure */ printf("MEMO_ALLOC_FAILURE line %d in file %s\n", __LINE__, __FILE__); }

	int i=0,j=0,k=0;

#ifdef COMPLETE_PREORDER
	/* allocate memory for the ranking and "S" when complete preorder PROM II is used */
	RANKS=(int**) malloc(M*sizeof(int*));
	COMPLETE_PREORDER_S=(float*) malloc(N*sizeof(float));
	for(i=0;i<M;i++)	RANKS[i]=(int*) calloc(N,sizeof(int));
#endif

	for(i=0;i<M;i++)
	{/* alloc memory for each expert */
		E[i].p_l=0.;
		E[i].e_ij=(float**) malloc(K*sizeof(float*));
		E[i].S_l=NULL;

		for(j=0;j<K;j++)
		{/* alloc memory for each criteria */
			E[i].e_ij[j]=(float*) calloc(N,sizeof(float));
		}
	}	

	/* alloc memory for the criterion weights */
	criterion_weights=(float*) calloc(K,sizeof(float));

	float buff=0;
	
	for(i=0;i<K;i++)
	{/* criterion weights */
		if( fscanf(fp,"%f", &buff) < 0 ){printf("EXIT_FAILURE line %d in file %s\n", __LINE__, __FILE__);};
		criterion_weights[i]=buff;
	}

	for(i=0;i<M;i++)
	{/* experts */
		if( fscanf(fp,"%f", &buff) < 0 ){printf("EXIT_FAILURE line %d in file %s\n", __LINE__, __FILE__);};
		E[i].p_l=buff;
		for(j=0;j<K;j++)
		{/* criteria */
			for(k=0;k<N;k++)
			{/* actions */
				if( fscanf(fp,"%f", &buff) < 0 ){printf("EXIT_FAILURE line %d in file %s\n", __LINE__, __FILE__);};
				E[i].e_ij[j][k]=buff;
			}
		}
	}

	fclose(fp);

	return E;
}


void free_2D_float(float** two_d_float)
{
	int i=0;

	for(i=0;i<N;i++)	free(two_d_float[i]);
	free(two_d_float);
}


void free_remaining_data(data E)
{

	int i=0,j=0;
	/* free memory allocated for "data" */
	for(i=0;i<M;i++)
	{
		for(j=0;j<K;j++)
		{
			free(E[i].e_ij[j]);
		}
		free(E[i].e_ij);
	}
	free(E);

	/* free memory allocated for the criterion weights */
	free(criterion_weights);
}


void free_alloc_for_PROM_II(void)
{
	int i=0;
	/* and the ranking lists */
	free(COMPLETE_PREORDER_S);
	for(i=0;i<M;i++)	free(RANKS[i]);
	free(RANKS);
}


void free_S(float** S)
{
	int i=0;
	/* free S */
	for(i=0;i<N;i++)	free(S[i]);
	free(S);
}


float** level_criterion(const float* e_l_i)
{
	int a=0,b=0,best=-1,worst=-1,i=0;
	float d=0.;

	float** P_i=malloc(N*sizeof(*P_i));

	for(a=0;a<N;a++){P_i[a]=(float*)malloc((N+1)*sizeof(float*));}

//for(a=0;a<N;a++){printf("%p\n",P_i[a]);}

//printf("\n");
	
	bool assigned=false;
	
	for(a=0;a<N-1;a++)
	{
		for(b=a+1;b<N;b++)
		{
			d=e_l_i[a]-e_l_i[b];

			best=a;worst=b;/* supposition */
			if(d<0)/* bPa and a isn't the best, thus switch best-worst. */
			{best=b;worst=a;}

			//P_i[worst][best]=NO_PREF;/* indeed, the worst CAN'T be preferred anyway. */

			assigned=false;
			i=0;
			while(!assigned)
			{
				/* absolute value since P_i[worst][best] is already assigned the worst value, remains the best wich has always d > 0 */
				if( fabsf(d) < THRESHOLDS[i] )
				{
//					P_i[best][worst]=LEV_CRIT_GRADS[i];
					*(*(P_i+best)+worst)=*(LEV_CRIT_GRADS+i);
//					memcpy((*(P_i+best)+worst), (LEV_CRIT_GRADS+i), sizeof(float)+1);
					assigned=true;
					continue;
				}

				i++;

				if( i==3 )
				{/* End of the threshold array => strict preferrence */
//					P_i[best][worst]=LEV_CRIT_GRADS[i];
//					(*(P_i+best))[worst]=LEV_CRIT_GRADS[i];
//					memcpy((*(P_i+best)+worst), (LEV_CRIT_GRADS+i), sizeof(float)+1);
					*(*(P_i+best)+worst)=*(LEV_CRIT_GRADS+i);
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
	for(j=0;j<N;j++){PI[j]=(float*) calloc(N,sizeof(float));}

	/* denom is sum{j in 1 to K}(criterion_weights[j]) */
	for(j=0;j<K;j++)	denom+=(criterion_weights[j]);

	for(a=0;a<N-1;a++)
		for(b=a+1;b<N;b++)
		{
			num_1=0;
			num_2=0;
			for(j=0;j<K;j++)
			{
				num_1+=(P_l[j][a][b]*criterion_weights[j]);
				num_2+=(P_l[j][b][a]*criterion_weights[j]);
			}
			PI[a][b]+=num_1/denom;
			PI[b][a]+=num_2/denom;
		}

	return PI;
}


float** compute_phi(float const* const* PI)
{
	int a=0, b=0;
	
	float** PHI=(float**) malloc(N*sizeof(float*));
	for(a=0;a<3;a++)	PHI[a]=(float*) calloc(N,sizeof(float));

	/* Summing the scores on the prefs inds */
	for(a=0;a<N-1;a++)
	{
		for(b=a+1;b<N;b++)
		{
			PHI[0][a]+=PI[a][b];/* outranking */
			PHI[1][a]+=PI[b][a];/* outranked */

			/* Fill for b at the same time */
			PHI[0][b]+=PI[b][a];
			PHI[1][b]+=PI[a][b];
		}
	}

	for(a=0;a<N;a++)
	{
		PHI[0][a]/=(N-1);
		PHI[1][a]/=(N-1);
		PHI[2][a]=PHI[0][a]-PHI[1][a];/* Net flow */
	}

	return PHI;
}


void PROM_1(expert* E_l, float const* const* PHI)
{
	int a=0, b=0, greater=-1, smaller=-1;

	for(a=0;a<N;a++)
	{/* (a,a) preference is I */
		E_l->S_l=add_S_l(E_l->S_l, 2, a, a);
	}
	
	/* May save time and skip performing an O(N^2) loop */
	for(a=0;a<N-1;a++)
	{
		for(b=a+1;b<N;b++)
		{
			/* Default : */
			greater=a;
			smaller=b;

			/* Change if the opposite */
			if ( PHI[0][smaller] > PHI[0][greater] )
			{
				greater=b;
				smaller=a;
			}

			/* Preference */
			if ( ( PHI[0][greater] > PHI[0][smaller] && PHI[1][greater] <= PHI[1][smaller] )
					|| ( PHI[0][greater] == PHI[0][smaller] && PHI[1][greater] < PHI[1][smaller] ) )
			{
				E_l->S_l=add_S_l(E_l->S_l, 3, greater, smaller);
			}

			/* Indifference */
			if ( PHI[0][greater] == PHI[0][smaller] && PHI[1][greater] == PHI[1][smaller] )
			{/* Since indifference => add both with value 2 */
				E_l->S_l=add_S_l(E_l->S_l, 2, greater, smaller);
				E_l->S_l=add_S_l(E_l->S_l, 2, smaller, greater);
			}


			/* Incomparability */
			if ( ( PHI[0][greater] > PHI[0][smaller] && PHI[1][greater] > PHI[1][smaller] )
					|| ( PHI[0][greater] < PHI[0][smaller] && PHI[1][greater] < PHI[1][smaller] ) )
			{/* Since incomparability => add both with value 1 */
				E_l->S_l=add_S_l(E_l->S_l, 1, greater, smaller);
				E_l->S_l=add_S_l(E_l->S_l, 1, smaller, greater);
			}
		}
	}
}


void PROM_2(expert* E_l, float const* const* PHI)
{
	int a=0, b=0, greater=-1, smaller=-1;

	for(a=0;a<N;a++)
	{/* (a,a) preference is I */
		E_l->S_l=add_S_l(E_l->S_l, 2, a, a);
	}
	
	/* May save time and skip performing an O(N^2) loop */
	for(a=0;a<N-1;a++)
	{
		for(b=a+1;b<N;b++)
		{
			/* Default : */
			greater=a;
			smaller=b;

			/* Change if the opposite */
			if ( PHI[2][smaller] > PHI[2][greater] )
			{
				greater=b;
				smaller=a;
			}

			/* Preference */
			if ( PHI[2][greater] > PHI[2][smaller] )
			{
				E_l->S_l=add_S_l(E_l->S_l, 3, greater, smaller);
			}

			/* Indifference */
			if ( PHI[2][greater] == PHI[2][smaller] )
			{/* Since indifference => add both with value 2 */
				E_l->S_l=add_S_l(E_l->S_l, 2, greater, smaller);
				E_l->S_l=add_S_l(E_l->S_l, 2, smaller, greater);
			}
		}
	}
}


void rank_single(struct expert_pref* first, int l)
{
	int i=0;

	/* allocate memory for the ranking */
	/* note : for a more convenient way to use the qsort comparator, buff_scores is structured as #lines=N and #columns=2 (index of project, its score) */
	int **buff_scores=(int**) malloc(N*sizeof(int*));
	for(i=0;i<N;i++)
	{
		buff_scores[i]=(int*) calloc(2,sizeof(int));
		buff_scores[i][0]=i;/* the first column of each line contains the indices of "projects" to rank. */
	}

	expert_pref* current_iterator=first;

	while(current_iterator!=NULL)
	{
		/* compute scores */
		buff_scores[current_iterator->a][1]+=current_iterator->p;
		current_iterator=current_iterator->NEXT_S_l;
	}

	current_iterator=NULL;
	
	qsort(buff_scores, N, sizeof(int*), qsort_comparator);

	for(i=0;i<N;i++)	RANKS[l][buff_scores[i][0]]=i+1;
	
	for(i=0;i<N;i++)
		free(buff_scores[i]);
	free(buff_scores);
}


void S_project(const data E)
{
	int a=0, l=0;
	
	for(l=0;l<M;l++)
	{
		for(a=0;a<N;a++)
		{
			COMPLETE_PREORDER_S[a]+=(E[l].p_l*RANKS[l][a]);
		}
	}
}


float** aggregate_S_l(const data E)
{
	int l=0;

	/* Init binary preference relations */
	float** S=(float**) malloc(N*sizeof(float*));
	for(l=0;l<N;l++)	S[l]=(float*) calloc(N,sizeof(float));

	expert_pref* current_iterator;
	expert_pref* tmp;

	for(l=0;l<M;l++)
	{
		current_iterator=E[l].S_l;
		while(current_iterator!=NULL)
		{
			S[current_iterator->a][current_iterator->b]+=E[l].p_l;
			/* frees at the same time */
			tmp=current_iterator;
			current_iterator=current_iterator->NEXT_S_l;
			free(tmp);
		}
	}

	current_iterator=NULL;
	return S;
}


int main(int argc, char** argv)
{
	if( argc < 2 ){printf("Please add more parameters\n");};

	data E=read_data(argv);

	int l=0, i=0;

	float*** P_l=(float***) malloc(M*sizeof(float**));
	float** PI=NULL;
	float** PHI=NULL;

#ifdef PRINT_STUFFS
print_data(E);/* Since data is of type expert* can send directly the object that is a pointer and won't create copies. */
#endif

//	for(l=0;l<2;l++)
	for(l=0;l<M;l++)
	{
#ifdef PRINT_STUFFS
printf("Expert %d\n",l);
#endif		

		for(i=0;i<K;i++)
		{/* Gathers P_1_l,..., P_K_l and keep into P_l */
			P_l[i]=level_criterion(E[l].e_ij[i]);
		}

#ifdef PRINT_STUFFS
for(i=0;i<K;i++)
print_P_l(P_l[i]);
#endif

		/* Reminder : PI = [ ( sum{j in 1 to K}(w[j]*P[j](a,b)) )/( sum{i in 1 to K} (w[j]) ) ]_n*n (<=> PI is of size n*n).
			PI <=> P_j(a,b) simultaneously on all the criteria */
		PI=compute_pref_indices(P_l);
		/* dealloc memory for P_l[i] */
		for(i=0;i<K;i++)	free_2D_float(P_l[i]);

		PHI=compute_phi(PI);
#ifdef PRINT_STUFFS
print_PI(PI);
#endif

		/* dealloc memory for PI */
		free_2D_float(PI);

#ifdef COMPLETE_PREORDER
		PROM_2(&E[l], PHI);
#else
		PROM_1(&E[l], PHI);
#endif

//#ifdef PRINT_STUFFS
print_PHI(PHI);
//#endif		
		/* dealloc memory for PHI */
		free_2D_float(PHI);

#ifdef PRINT_STUFFS
print_S_l(E[l].S_l);
#endif

#ifdef COMPLETE_PREORDER
		rank_single(E[l].S_l, l);/* ranks the projects with respect to opinion of l and store the ranking into "RANKS" list. */
#endif
	}
	
	/* when all the computing is done, dealloc memory allocated to P_l */
	free(P_l);

#ifdef COMPLETE_PREORDER
	S_project(E);/* if the ranking is a complete preorder, the computing can be done based on the rankings. */
#ifdef PRINT_STUFFS
printf("complete preorder (PROM II) :\n");
print_ranks();
#endif
	free_alloc_for_PROM_II();
#else
	float** S=aggregate_S_l(E);/* deallocating E[l].S_l memory is done while aggregating */
#ifdef PRINT_STUFFS
printf("aggregated result :\n");
print_aggregated_S_l(S);
#endif
#endif

	/* free_remaining_data ;P */
	free_remaining_data(E);

	/* free S if it has been used */
#ifndef COMPLETE_PREORDER
	free_S(S);
#endif

	return EXIT_SUCCESS;
}
