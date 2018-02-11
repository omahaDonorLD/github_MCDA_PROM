
/** For a better documentation see file main.h **/

#ifndef PROM_C
#define PROM_C

#include "../heads/promethee.h"

/* display the results */
#define PRINT_STUFFS

/* =========================================================== */
/* Portion of code to use only if one wants to display results */
/* =========================================================== */
#ifdef PRINT_STUFFS
	
	void print_data(const data E)
	{
		int l=0, i=0, j=0;

		printf("The data:\n");

		printf("M:%d, N:%d, K:%d (weight,-q,q,p:",M,N,K);

		for(i=0;i<K;i++)
		{
			printf("\tcriterion %d : %f, %f, %f, %f\n", i+1,criterion_weights[i], THRESHOLDS[i][0], THRESHOLDS[i][1], THRESHOLDS[i][2]);
		}
		printf(")\n");

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

		printf("The unicriterion preference function\n");

		for(a=0;a<N-1;a++)
		{
			printf("%d\t:",a);

			for(b=0;b<a;b++)	printf("\t\t");

			for(b=a+1;b<N;b++)
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

		printf("The multicriteria preference indices\n");

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


	void print_PI_FlowSort(float** PI)
	{
		int a=0,b=0;

		printf("The multicriteria preference indices FOR FlowSort\n");

		for(a=0;a<N;a++)
		{
			printf("%d (Out)\t:",a);
			for(b=0;b<N_CAT;b++)
			{
				printf("%f\t",PI[a][b]);
			}
			printf("\n");
			printf("%d (In)\t:",a);
			for(b=0;b<N_CAT;b++)
			{
				printf("%f\t",PI[N+a][b]);
			}
			printf("\n");
		}
		printf("End print_PI_FS\n");
	}


	void print_PHI(float** PHI)
	{
		int a=0;

		printf("The flows\n");

		printf("a|\toutranking flows\t|\toutranked flows\t|\tnet flows\n");
		printf("=====================================================================\n");

		for(a=0;a<N;a++)
		{
			printf("%d:\t%f\t\t%f\t\t%f\n",a,PHI[0][a],PHI[1][a],PHI[2][a]);
		}
		printf("End print_PHI\n");
	}


	void print_PHI_FlowSort(float** PHI)
	{
		int a=0,b=0;

		printf("The FlowSort flows\n");

		printf("R|\t Ri+,Ri-,Ri\t|\t results for NCAT categories \t|\t res for alt_i\n");
		printf("=====================================================================\n");

		for(a=0;a<N;a++)
		{
			printf("R%d+ :\t",a+1);
			for(b=0;b<=N_CAT;b++)
				printf("%f ",PHI[0][((N_CAT+1)*a)+b]);
			printf("\n");
			printf("R%d- :\t",a+1);
			for(b=0;b<=N_CAT;b++)
				printf("%f ",PHI[1][((N_CAT+1)*a)+b]);
			printf("\n");
			printf("R%d  :\t",a+1);
			for(b=0;b<=N_CAT;b++)
				printf("%f ",PHI[2][((N_CAT+1)*a)+b]);
			printf("\n");
		}
		printf("End print_PHI\n");
	}


	void print_S_l(expert_pref* first)
	{
		expert_pref* current_iterator=first;

		printf("The binary preference relations\n");

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

		printf("The ranking (complete preorder/PROMETHEE II)\n");

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

		printf("The aggregated binary preference relations (partial preorder/PROMETHEE I)\n");

		for(a=0;a<N;a++)
		{
			for(b=0;b<N;b++)
				printf("%f\t",S[a][b]);
			printf("\n");
		}

		printf("S+ | S-\n");
		for(a=0;a<N;a++)
			printf("%f\t%f\n",S[a][N],S[N][a]);
		printf("\n");

		printf("End print_aggregated_S_l\n");
	}
	
	void write_prom_results(int l, float **PI, float **PHI, expert_pref* first, char **argv)
	{
		int a=0,b=0;
		FILE *f;
		char path[30];
		char buff[30];
		expert_pref* current_iterator;

		strcpy(path,"./outputs/prom_");
		if(COMPLETE_PREORDER)
			strcat(path,"2_");
		else
			strcat(path,"1_");

		strcpy(buff,argv[4]);
		strtok(buff,"/");
		strcat(path,strtok(NULL,"/"));

		if(l==0)
			f=fopen(path,"w");
		else
			f=fopen(path,"a");

		/* Write the results obtained for PI */
		for(a=0;a<N;a++)
		{
			for(b=0;b<N;b++)
				fprintf(f,"%f ", PI[a][b]);
			fprintf(f,"\n");
		}
		fprintf(f,"\n");

		/* Write the results obtained for PHI. Reminder : 0,1,2 respectively the positive, negative and net flows. */
		for(a=0;a<N;a++)
			fprintf(f,"%f %f %f\n", PHI[0][a],PHI[1][a],PHI[2][a]);
		
		/* Write the binary preference relation for a given expert */
		current_iterator=first;
		while(current_iterator!=NULL)
		{
			fprintf(f,"(%d,%d,%d)\t",current_iterator->a,current_iterator->b,current_iterator->p);
			current_iterator=current_iterator->NEXT_S_l;
		}
		fprintf(f,"\n");

		fclose(f);
	}
	
	void write_prom_global_results(char **argv, float** S)
	{
		int a=0,b=0;
		FILE *f;
		char path[30];
		char buff[30];
		int **glob_rank;/* contains the final ranking considering all the experts' opinions */

		if(COMPLETE_PREORDER)
		{
			strcpy(path,"./outputs/ranks_");
			glob_rank=malloc(N*sizeof(int*));/* allocate memory to contain the final ranking considering all the experts' opinions */
			if(glob_rank == NULL){ /* memory allocation failure */ PRINT_MEM_FAIL( __LINE__, __FILE__); }
			for(a=0;a<N;a++)
			{
				glob_rank[a]=calloc(2,sizeof(int));
				if(glob_rank[a] == NULL){ /* memory allocation failure */ PRINT_MEM_FAIL( __LINE__, __FILE__); }
				glob_rank[a][0]=a+1;/* the first column of each line contains the indices of "projects" to rank. */
				glob_rank[a][1]=COMPLETE_PREORDER_S[a];/* the 2nd column of each line contains the "overall" rank of the "project". */
			}

			qsort(glob_rank, N, sizeof(int*), qsort_comparator);
		}
		else
		{
			strcpy(path,"./outputs/aggr_pref_");
		}

		strcpy(buff,argv[4]);
		strtok(buff,"/");
		strcat(path,strtok(NULL,""));
		f=fopen(path,"w");

		if(COMPLETE_PREORDER)
		{
			for(b=0;b<M;b++)
			{
				for(a=0;a<N;a++)
					fprintf(f,"%d ",RANKS[b][a]);
				fprintf(f,"\n");
			}

			for(a=0;a<N;a++)
				fprintf(f,"%f ",COMPLETE_PREORDER_S[a]);
			fprintf(f,"\n");

			/* don't mind, just a trick to print the rankings in a clearer manner */
			fprintf(f,"%d",glob_rank[N-1][0]);
			for(a=N-2;a>-1;a--)
				fprintf(f,( glob_rank[a][1]==glob_rank[a+1][1] ? ",%d" : " %d" ),glob_rank[a][0]);
			fprintf(f,"\n");
		}
		else
		{
			for(b=0;b<N;b++)
			{
				for(a=0;a<N;a++)
					fprintf(f,"%f ",S[b][a]);
				fprintf(f,"\n");
			}

			/* Print In and Out sum of flows */
			for(a=0;a<N;a++)
				fprintf(f, "%f %f\n",S[a][N],S[N][a]);
			fprintf(f,"\n");
		}

		fclose(f);
	}

#endif
/*==================== End of printing stuffs ========================*/



int qsort_comparator(const void* pa, const void* pb)
{
	return -( (*(const int **)pa)[1] - (*(const int **)pb)[1] ) ;
}


void free_float_matrix(float** float_matrix, int n_lines)
{
	int i=0;

	for(i=0;i<n_lines;i++)
	{
		free(float_matrix[i]);
		float_matrix[i]=NULL;
	}

	free(float_matrix);
	float_matrix=NULL;

	return;
}


void free_data(data E)
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

	/* free memory allocated for the thresholds */
	for(j=0;j<K;j++)
	{
		free(THRESHOLDS[j]);
		THRESHOLDS[j]=NULL;
	}
	free(THRESHOLDS);
	THRESHOLDS=NULL;
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


float** pref_func(const float* e_l_i, int j, bool SORTING)
{
	int a=0,b=0,n_cols=0, n_ites=0;
	float d=0., buff=0.;
	float** P_i;

	/* the number of columns and number of iterations change given the purpose of the calculation :
			is it needed for the ranking or sorting method (given by the boolean "SORTING"). */
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

	if(P_i == NULL){ /* memory allocation failure */ PRINT_MEM_FAIL( __LINE__, __FILE__); }

	for(a=0;a<N;a++)
	{
		P_i[a]=calloc(n_cols,sizeof(float));
		if(P_i[a] == NULL){ /* memory allocation failure */ PRINT_MEM_FAIL( __LINE__, __FILE__); }
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
				d=e_l_i[a]-R[b][j];
			else
			{/* ranking */
				d=e_l_i[a]-e_l_i[b];
			}

			buff=(d < 0 ? -d : d);/* absolute value since P_i[worst][best] has the worst value anyway, remains the best which has always d > 0 */

			/* Either P_i[a][b]=linear_criterion(j,d,buff) or P_i[a][b]=level_criterion(j,d,buff);
			 it depends on the value of TYPE_CRITERION, that says which function in array of functions "TBL_PREF_FUNC" we use (either level or linear criterion) */
			P_i[a][b]=(*TBL_PREF_FUNC[TYPE_CRITERION])(j,d,buff);
		}
	}

	return P_i;
}


float level_criterion(int j, float d, float buff)
{
	int i=0;

	for(;i<3;i++)
	{
		if( buff < THRESHOLDS[j][i] )
			return ( i < 2 ? 0. : ( d >= 0 ? LEV_CRIT_GRADS[i] : -(LEV_CRIT_GRADS[i]) ) );
		if( i==2 )
			return (d >= 0 ? 1.:-1.);
	}

	printf("This is not supposed to appear...");

	return -111111.;
}


float linear_criterion(int j, float d, float buff)
{
	int i=0;

	for(;i<3;i++)
	{
		if( buff < THRESHOLDS[j][i] )
			return ( i < 2 ? 0. : (d>=0 ?
									((buff-THRESHOLDS[j][1])/(THRESHOLDS[j][2]-THRESHOLDS[j][1]))
									: -((buff-THRESHOLDS[j][1])/(THRESHOLDS[j][2]-THRESHOLDS[j][1])) ) );
		if( i==2 )
			return (d >= 0 ? 1.:-1.);
	}

	printf("This is not supposed to appear...(Linear criterion)");

	return -222222.;
}


float** compute_pref_indices(float*** P_l, bool SORTING)
{
	int j=0, a=0, b=0, n_rows=0, n_cols=0, n_ites=0;
	float num_1=0., num_2=0., denom=0., buff=0.;
	float** PI;

	if(SORTING)
	{
		n_cols=N_CAT;
		n_ites=N;
		n_rows=(2*N);
	}
	else
	{
		n_cols=N;
		n_ites=N-1;
		n_rows=N;
	}

	PI=malloc(n_rows*sizeof(float*));

	if(PI == NULL){ /* memory allocation failure */ PRINT_MEM_FAIL( __LINE__, __FILE__); }

	for(j=0;j<n_rows;j++)
	{
		PI[j]=calloc(n_cols,sizeof(float));
		if(PI[j] == NULL){ /* memory allocation failure */ PRINT_MEM_FAIL( __LINE__, __FILE__); }
	}

	/* the denominator is : sum{j in 1 to K}(criterion_weights[j]) */
	for(j=0;j<K;j++){denom+=(criterion_weights[j]);}

	for(a=0;a<n_ites;a++)
	{
		if(SORTING)
			b=0;
		else
			b=a+1;

		for(;b<n_cols;b++)
		{
			num_1=num_2=0.;
			for(j=0;j<K;j++)
			{
				if(P_l[j][a][b] < 0)
				{
					buff=( -(P_l[j][a][b])*criterion_weights[j] );
					num_2+=buff;
				}
				else
				{
					buff=( P_l[j][a][b]*criterion_weights[j] );
					num_1+=buff;
				}
			}

			buff=num_1/denom;
			PI[a][b]+=buff;
			buff=num_2/denom;

			/* the leaving preference indices are indexed from action_1 to action_n 
				and the entering are indexed from N+act_1 to N+act_n */
			if(SORTING)
				PI[N+a][b]+=buff;

			else
				PI[b][a]+=buff;
		}
	}

	return PI;
}


float** compute_phi(float** PI, bool SORTING)
{
	int a=0, b=0, n_cols=0, n_ites=0, denom=0;
	float** PHI;

	if(SORTING)
	{
		/** Caution : the matrix is organised as follow :
		 * 		The number of rows are still 3, for in-out-net flows
		 * 		!!!BUT!!! the number of columns are (N_CAT+1)*N ordered as follows :
		 * 			r{1}_R{1}, r{2}_R{1}, r{3}_R{1}, ..., r{N_CAT}_R{1}, a{1}_R{1}, r{1}_R{2}, r{2}_R{2}, ..., r{N_CAT}_R{2}, a{2}_R{2},
		 * 				..., r{N_CAT}_R{N-1}, a{N-1}_R{N-1}, r{1}_R{N}, r{2}_R{N}, r2_R2, ..., r{N_CAT}_R{N}, a{N}_R{N}
		 */
		n_cols=N_CAT+1;
		/* since R_{i}^{*} = R^{*} U a_{i}, then |R_{i}^{*}|-1 = |R^{*}|
		 * (the new set is extended with one element, withraw one and the numbers
		 * of elements in the new set is equal to the number of elements in the previous set) */
		denom=N_CAT;
		n_ites=N;
	}
	else
	{
		n_cols=N;
		denom=N-1;
		n_ites=N-1;
	}

	PHI=malloc(3*sizeof(float*));
	if(PHI == NULL){ /* memory allocation failure */ PRINT_MEM_FAIL( __LINE__, __FILE__); }

	for(a=0;a<3;a++)
	{
		if(SORTING)
			PHI[a]=calloc(n_cols*N,sizeof(float));
		else
			PHI[a]=calloc(n_cols,sizeof(float));
		if(PHI[a] == NULL){ /* memory allocation failure */ PRINT_MEM_FAIL( __LINE__, __FILE__); }
	}

	/* in case of sorting, set strong preference degrees between limiting/centroids profiles */
	if(SORTING)
		for(a=0;a<N;a++)
		{
			for(b=0;b<N_CAT;b++)
			{
				PHI[0][((N_CAT+1)*a)+b]=N_CAT-1-b;
				PHI[1][((N_CAT+1)*a)+b]=b;
			}
		}

	/* Summing the scores on the prefs inds */
	for(a=0;a<n_ites;a++)
	{
		if(SORTING)
		{
			PHI[0][((N_CAT+1)*a)+N_CAT]=0;
			PHI[1][((N_CAT+1)*a)+N_CAT]=0;
			b=0;
		}
		else
			b=a+1;

		for(;b<n_cols;b++)
		{
			if(SORTING)
			{
				/* CAUTION :
				 * =========================
				 *  read the structure of PI at the definition of the function "compute_pref_indices", file : promethee.h
				 */
				PHI[0][((N_CAT+1)*a)+N_CAT]+=PI[a][b];
				PHI[1][((N_CAT+1)*a)+N_CAT]+=PI[N+a][b];

				PHI[0][((N_CAT+1)*a)+b]+=PI[N+a][b];
				PHI[1][((N_CAT+1)*a)+b]+=PI[a][b];
			}
			else
			{
				PHI[0][a]+=PI[a][b];/* outranking */
				PHI[1][a]+=PI[b][a];/* outranked */

				/* Fills b at the same time */
				PHI[0][b]+=PI[b][a];
				PHI[1][b]+=PI[a][b];
			}
		}
	}

	if(SORTING)
		n_cols=(N_CAT+1)*N+1;
	else
		n_cols=N;

	for(a=0;a<n_cols;a++)
	{
		PHI[0][a]/=denom;
		PHI[1][a]/=denom;
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
	if(buff_scores == NULL){ /* memory allocation failure */ PRINT_MEM_FAIL( __LINE__, __FILE__); }

	for(i=0;i<N;i++)
	{
		buff_scores[i]=calloc(2,sizeof(int));
		if(buff_scores[i] == NULL){ /* memory allocation failure */ PRINT_MEM_FAIL( __LINE__, __FILE__); }

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
		if( buff_scores[i][1] != previous_sln )/* if the two indices have not equals scores */
		{
			previous_sln=buff_scores[i][1];
			rank++;
		}
		RANKS[l][buff_scores[i][0]]=rank;/* => same scores, same rank (no "average" in case of ties) */
	}

	for(i=0;i<N;i++)
	{
		free(buff_scores[i]);
		buff_scores[i]=NULL;
	}

	free(buff_scores);
	buff_scores=NULL;
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
	if(S == NULL){ /* memory allocation failure */ PRINT_MEM_FAIL( __LINE__, __FILE__); }

	for(l=0;l<N+1;l++)
	{
		S[l]=calloc((N+1),sizeof(float));
		if(S[l] == NULL){ /* memory allocation failure */ PRINT_MEM_FAIL( __LINE__, __FILE__); }
	}

	for(l=0;l<M;l++)
	{
		current_iterator=E[l].S_l;

		while(current_iterator!=NULL)
		{
			// Only take into account the binary relation if it is a strong binary preference (not indifference nor incomparable)
			if(current_iterator->a==current_iterator->b || current_iterator->p >= 3)
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
		for(b=a+1;b<N;b++)
		{
			S[a][N]+=S[a][b];
			S[N][a]+=S[b][a];

			S[b][N]+=S[b][a];
			S[N][b]+=S[a][b];
		}
	}

	for(a=0;a<N;a++)
	{
		S[a][N]+=S[a][a];
		S[N][a]+=S[a][a];
	}

	return S;
}

#endif

