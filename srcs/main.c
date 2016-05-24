
/** For a better documentation see file main.h **/

#ifndef MAIN_C
#define MAIN_C

#include "../heads/main.h"


/* To activate PROM_II just uncomment line below. Otherwise PROM I used. */
/* #define COMPLETE_PREORDER */

/* define if printing out needed */
#define PRINT_STUFFS

/* =========================================================== */
/* Portion of code to use only if one wants to display results */
/* =========================================================== */

	void print_data(const data E)
	{
		int l=0, i=0, j=0;

		printf("In print_data\n");

		printf("M:%d, K:%d (respective weights:",M,K);

		for(i=0;i<K;i++)
		{
			printf("%f,",criterion_weights[i]);
		}

		printf("), N:%d, q:%f, p:%f\n",N,q,p);

		for(l=0;l<M;l++)
		{
			printf("Expert:%d weighted :%f-------------\n",l+1,E[l].p_l);

			for(i=0;i<K;i++)
			{
				for(j=0;j<N;j++)
					printf("%f\t",E[l].e_ij[i][j]);
				printf("\n");
			}
		}
		printf("End print_data\n");
	}


	void print_P_l(float** P_l)
	{
		int a=0,b=0;

		printf("In print_P_l\n");

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


	void print_PI(float** PI)
	{
		int a=0,b=0;

		printf("In print_PI\n");

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


	void print_PHI(float** PHI)
	{
		int a=0;

		printf("In print_PHI\n");

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
		expert_pref* current_iterator=first;

		printf("In print_S_l\n");

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
		int a=0,l=0;

		printf("In print_ranks\n");

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


	void print_aggregated_S_l(float** S)
	{
		int a=0,b=0;

		printf("In print_aggregated_S_l\n");

		for(a=0;a<N;a++)
		{
			for(b=0;b<N;b++)
				printf("%f\t",S[a][b]);
			printf("\n");
		}

		printf("OUT S - IN OUT\n");
		for(a=0;a<N;a++)
			printf("%f\t%f\n",S[a][N],S[N][a]);
		printf("\n");

		printf("End print_aggregated_S_l\n");
	}

/*==================== End of printing stuffs ========================*/



int qsort_comparator(const void* pa, const void* pb)
{
	return -( (*(const int **)pa)[1] - (*(const int **)pb)[1] ) ;	
}


data read_data(char** argv)
{	
	int i=0,j=0,k=0;
	float buff=0;
	data E;
	FILE* fp;

	fp=fopen(argv[1],"r");

	if( fscanf(fp,"%d %d %d", &M, &K, &N) < 0 ){printf("EXIT_FAILURE line %d in file %s\n", __LINE__, __FILE__);}

	if( fscanf(fp,"%f %f", &q, &p) < 0 ){printf("EXIT_FAILURE line %d in file %s\n", __LINE__, __FILE__);}
	
	/* assign values for thresholds and grades of level criteria */
	THRESHOLDS=malloc(3*sizeof(float));
	if(THRESHOLDS == NULL){ /* memory allocation failure */ printf("MEMO_ALLOC_FAILURE line %d in file %s\n", __LINE__, __FILE__); }

	THRESHOLDS[0]=-q; THRESHOLDS[1]=q; THRESHOLDS[2]=p;

	LEV_CRIT_GRADS=malloc(4*sizeof(float));
	if(LEV_CRIT_GRADS == NULL){ /* memory allocation failure */ printf("MEMO_ALLOC_FAILURE line %d in file %s\n", __LINE__, __FILE__); }

	LEV_CRIT_GRADS[0]=NO_PREF; LEV_CRIT_GRADS[1]=INDIFF; LEV_CRIT_GRADS[2]=WEAK_PREF; LEV_CRIT_GRADS[3]=PREF;

	E=malloc(M*sizeof(expert));
	if(E == NULL){ /* memory allocation failure */ printf("MEMO_ALLOC_FAILURE line %d in file %s\n", __LINE__, __FILE__); }

#ifdef COMPLETE_PREORDER

	/* allocate memory for the ranking and "S" when complete preorder PROM II is used */
	RANKS=malloc(M*sizeof(int*));
	if(RANKS == NULL){ /* memory allocation failure */ printf("MEMO_ALLOC_FAILURE line %d in file %s\n", __LINE__, __FILE__); }

	COMPLETE_PREORDER_S=malloc(N*sizeof(float));
	if(COMPLETE_PREORDER_S == NULL){ /* memory allocation failure */ printf("MEMO_ALLOC_FAILURE line %d in file %s\n", __LINE__, __FILE__); }

	for(i=0;i<M;i++)
	{
		RANKS[i]=calloc(N,sizeof(int));
		if(RANKS[i] == NULL){ /* memory allocation failure */ printf("MEMO_ALLOC_FAILURE line %d in file %s\n", __LINE__, __FILE__); }
	}

#endif

	/* alloc memory for each expert */
	for(i=0;i<M;i++)
	{
		E[i].p_l=0.;

		E[i].e_ij=malloc(K*sizeof(float*));
		if(E[i].e_ij == NULL){ /* memory allocation failure */ printf("MEMO_ALLOC_FAILURE line %d in file %s\n", __LINE__, __FILE__); }

		E[i].S_l=NULL;

		/* alloc memory for each criteria */
		for(j=0;j<K;j++)
		{
			E[i].e_ij[j]=calloc(N,sizeof(float));
			if(E[i].e_ij[j] == NULL){ /* memory allocation failure */ printf("MEMO_ALLOC_FAILURE line %d in file %s\n", __LINE__, __FILE__); }
		}
	}	

	/* alloc memory for the criterion weights */
	criterion_weights=calloc(K,sizeof(float));
	if(criterion_weights == NULL){ /* memory allocation failure */ printf("MEMO_ALLOC_FAILURE line %d in file %s\n", __LINE__, __FILE__); }

	buff=0;

	/* get criterion weights */
	for(i=0;i<K;i++)
	{
		if( fscanf(fp,"%f", &buff) < 0 ){printf("EXIT_FAILURE line %d in file %s\n", __LINE__, __FILE__);};
		criterion_weights[i]=buff;
	}

	/* gather infos from experts */
	for(i=0;i<M;i++)
	{
		if( fscanf(fp,"%f", &buff) < 0 ){printf("EXIT_FAILURE line %d in file %s\n", __LINE__, __FILE__);};
		E[i].p_l=buff;

		/* for each criteria... */
		for(j=0;j<K;j++)
		{
			/* get the evaluation done by expert l to action k with respect to criterion j */
			for(k=0;k<N;k++)
			{
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

	for(i=0;i<N;i++)
	{
		free(two_d_float[i]);
		two_d_float[i]=NULL;
	}

	free(two_d_float);
	two_d_float=NULL;

	return;
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
			E[i].e_ij[j]=NULL;
		}
		free(E[i].e_ij);
		E[i].e_ij=NULL;
	}
	free(E);
	E=NULL;

	/* free memory allocated for the criterion weights */
	free(criterion_weights);
	criterion_weights=NULL;
}


void free_alloc_for_PROM_II(void)
{
	int i=0;

	free(COMPLETE_PREORDER_S);

	for(i=0;i<M;i++)
	{
		free(RANKS[i]);
		RANKS[i]=NULL;
	}
	free(RANKS);
	RANKS=NULL;
}


void free_S(float** S)
{
	int i=0;

	for(i=0;i<N;i++)
	{
		free(S[i]);
		S[i]=NULL;
	}
	free(S);
	S=NULL;
}


float** level_criterion(const float* e_l_i)
{
	int a=0,b=0,best=-1,worst=-1,i=0;
	float d=0., buff=0.;
	bool assigned=false;
	float** P_i;

	P_i=malloc(	N*sizeof(float*));
	if(P_i == NULL){ /* memory allocation failure */ printf("MEMO_ALLOC_FAILURE line %d in file %s\n", __LINE__, __FILE__); }

	for(i=0;i<N;i++)
	{
		P_i[i]=calloc(N,sizeof(float));
		if(P_i[i] == NULL){ /* memory allocation failure */ printf("MEMO_ALLOC_FAILURE line %d in file %s\n", __LINE__, __FILE__); }
	}

	assigned=false;
	
	for(a=0;a<N-1;a++)
	{
		for(b=a+1;b<N;b++)
		{
			d=e_l_i[a]-e_l_i[b];

			best=a;worst=b;/* supposition */
			if(d<0)/* then bPa, thus a isn't the best, switch best-worst. */
			{best=b;worst=a;}

			assigned=false;
			i=0;
			buff=(d < 0 ? -d : d);/* absolute value since P_i[worst][best] has the worst value anyway, remains the best which has always d > 0 */

			while(!assigned)
			{
				if( buff < THRESHOLDS[i] )
				{
					P_i[best][worst]=LEV_CRIT_GRADS[i];
					assigned=true;
					continue;
				}

				i++;

				if( i==3 )
				{/* End of the threshold array => strict preferrence */
					P_i[best][worst]=LEV_CRIT_GRADS[i];
					assigned=true;
				}
			}
		}
	}

	return P_i;
}


float** compute_pref_indices(float*** P_l)
{
	int j=0, a=0, b=0;
	float num_1=0., num_2=0., denom=0., buff=0.;

	float** PI=malloc(N*sizeof(float*));
	if(PI == NULL){ /* memory allocation failure */ printf("MEMO_ALLOC_FAILURE line %d in file %s\n", __LINE__, __FILE__); }

	for(j=0;j<N;j++)
	{
		PI[j]=calloc(N,sizeof(float));
		if(PI[j] == NULL){ /* memory allocation failure */ printf("MEMO_ALLOC_FAILURE line %d in file %s\n", __LINE__, __FILE__); }
	}

	/* the denominator is : sum{j in 1 to K}(criterion_weights[j]) */
	for(j=0;j<K;j++){denom+=(criterion_weights[j]);}

	for(a=0;a<N-1;a++)
	{
		for(b=a+1;b<N;b++)
		{
			num_1=num_2=0.;
			for(j=0;j<K;j++)
			{
				buff=(P_l[j][a][b]*criterion_weights[j]);
				num_1+=buff;
				buff=(P_l[j][b][a]*criterion_weights[j]);
				num_2+=buff;
			}
			buff=num_1/denom;
			PI[a][b]+=buff;
			buff=num_2/denom;
			PI[b][a]+=buff;
		}
	}

	return PI;
}


float** compute_phi(float** PI)
{
	int a=0, b=0;
	
	float** PHI=malloc(N*sizeof(float*));
	if(PHI == NULL){ /* memory allocation failure */ printf("MEMO_ALLOC_FAILURE line %d in file %s\n", __LINE__, __FILE__); }

	for(a=0;a<3;a++)
	{
		PHI[a]=calloc(N,sizeof(float));
		if(PHI[a] == NULL){ /* memory allocation failure */ printf("MEMO_ALLOC_FAILURE line %d in file %s\n", __LINE__, __FILE__); }
	}

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


void PROM_1(expert* E_l, float** PHI)
{
	int a=0, b=0, greater=-1, smaller=-1;

	for(a=0;a<N;a++)
	{/* count (a,a) too as a binary preference, the indifference preference.
		Note : this step can't be done on the next loop since on the latter "a" goes from 1 to N-1. And in the both case Cxity is still O(n+n*n) */
		E_l->S_l=add_S_l(E_l->S_l, 2, a, a);
	}

	/* b=a+1 helps saving time by skipping performing an O(N^2) loop */
	for(a=0;a<N-1;a++)
	{
		for(b=a+1;b<N;b++)
		{
			/* Default */
			greater=a;
			smaller=b;

			/* Change if opposite */
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
			{/* keeps the incomparability relation by adding both relations aIb, bIa with value 1 */
				E_l->S_l=add_S_l(E_l->S_l, 1, greater, smaller);
				E_l->S_l=add_S_l(E_l->S_l, 1, smaller, greater);
			}
		}
	}
}


void PROM_2(expert* E_l, float** PHI)
{
	int a=0, b=0, greater=-1, smaller=-1;

	for(a=0;a<N;a++)
	{/* count (a,a) too as a binary preference, the indifference preference.
		Note : this step can't be done on the next loop since on the latter "a" goes from 1 to N-1. And in the both case Cxity is still O(n+n*n) */
		E_l->S_l=add_S_l(E_l->S_l, 2, a, a);
	}
	
	/* b=a+1 helps saving time by skipping performing an O(N^2) loop */
	for(a=0;a<N-1;a++)
	{
		for(b=a+1;b<N;b++)
		{
			/* Default */
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


void rank_single(expert_pref* first, int l)
{
	int i=0,rank=0;
	float previous_sln=-1.;/* needed for handling equals ranks */
	int **buff_scores;
	expert_pref* current_iterator;

	/* allocate memory for the ranking */
	/* note : for a more convenient way to use the qsort comparator, buff_scores is
	 * structured as #lines=N and #columns=2 (index of project, its score) */
	buff_scores=malloc(N*sizeof(int*));
	if(buff_scores == NULL){ /* memory allocation failure */ printf("MEMO_ALLOC_FAILURE line %d in file %s\n", __LINE__, __FILE__); }

	for(i=0;i<N;i++)
	{
		buff_scores[i]=calloc(2,sizeof(int));
		if(buff_scores[i] == NULL){ /* memory allocation failure */ printf("MEMO_ALLOC_FAILURE line %d in file %s\n", __LINE__, __FILE__); }

		buff_scores[i][0]=i;/* the first column of each line contains the indices of "projects" to rank. */
	}

	current_iterator=first;

	while(current_iterator!=NULL)
	{
		/* compute scores */
		buff_scores[current_iterator->a][1]+=current_iterator->p;
		current_iterator=current_iterator->NEXT_S_l;
	}

	current_iterator=NULL;
	
	qsort(buff_scores, N, sizeof(int*), qsort_comparator);

	previous_sln=-1.;
	rank=0;
	for(i=0;i<N;i++)
	{
		if( buff_scores[i][1] != previous_sln )
		{
			previous_sln=buff_scores[i][1];
			rank++;
		}
		RANKS[l][buff_scores[i][0]]=rank;
	}
	
	for(i=0;i<N;i++)
		free(buff_scores[i]);

	free(buff_scores);
}


void get_S_a(const data E)
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
	int l=0, a=0, b=0;
	expert_pref* current_iterator;
	expert_pref* tmp;
	
	/* Note : S contains S(a,b) AND S_in(a) (as a supplementary column), S_out(a) (as a supplementary row).
	 * S[N,N] can stay empty. */

	/* Init binary preference relations */
	float** S=malloc((N+1)*sizeof(float*));
	if(S == NULL){ /* memory allocation failure */ printf("MEMO_ALLOC_FAILURE line %d in file %s\n", __LINE__, __FILE__); }

	for(l=0;l<N+1;l++)
	{
		S[l]=calloc((N+1),sizeof(float));
		if(S[l] == NULL){ /* memory allocation failure */ printf("MEMO_ALLOC_FAILURE line %d in file %s\n", __LINE__, __FILE__); }
	}

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

	/* Get the sum of scores */
	for(a=0;a<N-1;a++)
	{
		for(b=a;b<N;b++)
		{
			S[a][N]+=S[a][b];
			S[N][a]+=S[b][a];
		}
	}

	return S;
}


int main(int argc, char** argv)
{
	data E;
	int l=0, i=0;

	float*** P_l=NULL;
	float** PI=NULL;
	float** PHI=NULL;

#ifndef COMPLETE_PREORDER
	float** S;
#endif

	if( argc < 2 ){printf("Please add more parameters\n");};

	E=read_data(argv);

#ifdef PRINT_STUFFS
print_data(E);/* Since data is of type "expert*" one can send directly the object that is a pointer, no copies will be created. */
#endif

	for(l=0;l<M;l++)
	{

#ifdef PRINT_STUFFS
printf("Expert %d\n",l);
#endif		

		P_l=malloc(K*sizeof(float**));
		if(P_l == NULL){ /* memory allocation failure */ printf("MEMO_ALLOC_FAILURE line %d in file %s\n", __LINE__, __FILE__); }


		for(i=0;i<K;i++)
		{/* computes P_1_l,..., P_K_l and gathers them into P_l */
			P_l[i]=level_criterion(E[l].e_ij[i]);
		}

#ifdef PRINT_STUFFS
for(i=0;i<K;i++)
print_P_l(P_l[i]);
#endif

		/* Reminder : PI(a,b) = [ ( sum{j in 1 to K}(w[j]*P[j](a,b)) )/( sum{i in 1 to K} (w[j]) ) ] and is of size n*n.
			PI(a,b) ~ P_j(a,b) considering simultaneously all the criteria */
		PI=compute_pref_indices(P_l);

		/* dealloc memory provided for P_l[i] */
		for(i=0;i<K;i++)
		{
			free_2D_float(P_l[i]);
		}

		/* dealloc memory allocated to P_l, caution it has to be deallocated after deallocating all P_l[i] */
		free(P_l);
		P_l=NULL;

		PHI=compute_phi(PI);

#ifdef PRINT_STUFFS
print_PI(PI);
#endif

		/* dealloc memory for PI */
		free_2D_float(PI);
		PI=NULL;

		/* applying PROMETHEE */
#ifdef COMPLETE_PREORDER
		PROM_2((E+l), PHI);
#else
		PROM_1((E+l), PHI);
#endif

#ifdef PRINT_STUFFS
print_PHI(PHI);
#endif

		/* dealloc PHI */
		free_2D_float(PHI);
		PHI=NULL;

#ifdef PRINT_STUFFS
print_S_l(E[l].S_l);
#endif

#ifdef COMPLETE_PREORDER
		rank_single(E[l].S_l, l);/* ranks the projects with respect to opinion of l and store the ranking into "RANKS" list. */
#endif

	}

#ifdef COMPLETE_PREORDER
	/* PROMETHEE II */
	get_S_a(E);/* if the ranking is a complete preorder, the computing can be done based on the rankings. */

#ifdef PRINT_STUFFS
printf("complete preorder (PROM II) :\n");
print_ranks();
#endif

	free_alloc_for_PROM_II();
#else
	/* PROMETHEE I */
	S=aggregate_S_l(E);/* deallocating E[l].S_l memory is done while aggregating */

#ifdef PRINT_STUFFS
printf("aggregated result :\n");
print_aggregated_S_l(S);
#endif

	free_S(S);
#endif /* ends the first #ifdef */

	/* free_remaining_data ;P */
	free_remaining_data(E);

	return EXIT_SUCCESS;
}

#endif
