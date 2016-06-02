

#ifndef MAIN_C
#define MAIN_C

#include "../heads/flowsort.h"

/* Uncomment the line below to use PROM_II. Otherwise PROM I is the default PROMETHEE used. */
/* #define COMPLETE_PREORDER */


/** read_data
 *
 * Read instances given as arguments and return the right form to be used by the application.
 *
 * @param : the text file (see ./data/README for more.) depicting the data for the instance.
 * @return : a usable form of data for PROMETHEE : a vector of matrices of size M. Each matrix is of size (K,N) and represents the preference values given by the current expert l in 1..M to the project i in 1..N on the criteria i in 1..K. Such matrix is denoted E_l, and the vector of all such matrices is denoted E.
 *
*/
data read_data(char** argv)
{
	int i=0,j=0,k=0;
	float buff=0;
	data E;
	FILE* fp;

	fp=fopen(argv[1],"r");

	if( fscanf(fp,"%d %d %d", &M, &K, &N) < 0 ){printf("EXIT_FAILURE line %d in file %s\n", __LINE__, __FILE__);}

	/* assign values for thresholds and grades of level criteria */
	THRESHOLDS=malloc(K*sizeof(float*));
	if(THRESHOLDS == NULL){ /* memory allocation failure */ printf("MEMO_ALLOC_FAILURE line %d in file %s\n", __LINE__, __FILE__); }

	for(i=0;i<K;i++)
	{/* get the thresholds for each criterion */
		THRESHOLDS[i]=calloc(3,sizeof(float));/* -q, q and p for each criteria */
		if(THRESHOLDS[i] == NULL){ /* memory allocation failure */ printf("MEMO_ALLOC_FAILURE line %d in file %s\n", __LINE__, __FILE__); }
		if( fscanf(fp,"%f %f", &THRESHOLDS[i][1], &THRESHOLDS[i][2]) < 0 ){printf("EXIT_FAILURE line %d in file %s\n", __LINE__, __FILE__);}
		THRESHOLDS[i][0]= -(THRESHOLDS[i][1]);
	}

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


/** read_sorting_data
 *
 * Read the reference profiles
 *
 * @param : the text file (see ./data/README for more.) depicting the data for the instance.
 * @return : a usable form of data for PROMETHEE : a vector of matrices of size M. Each matrix is of size (K,N) and represents the preference values given by the current expert l in 1..M to the project i in 1..N on the criteria i in 1..K. Such matrix is denoted E_l, and the vector of all such matrices is denoted E.
 *
*/
void read_sorting_data(char** argv)
{

	int i=0,j=0;
	FILE* fp;

	fp=fopen(argv[2],"r");

	if( fscanf(fp,"%d", &N_CAT) < 0 ){printf("EXIT_FAILURE line %d in file %s\n", __LINE__, __FILE__);}

	/* assign values for the reference profiles */
	R=malloc(N_CAT*sizeof(float*));
	C=malloc(N_CAT*sizeof(categorie));

    for(i=0; i<N_CAT; i++)
    {
        C[i].alternative=-1;/* alternative < 0 denotes an empty category */
        R[i]=calloc(K,sizeof(float));
    }

    for(i=0; i<N_CAT; i++)
        for(j=0; j<K; j++)  if( fscanf(fp,"%f", &R[i][j]) < 0 ){printf("EXIT_FAILURE line %d in file %s\n", __LINE__, __FILE__);}

	fclose(fp);
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
			P_l[i]=level_criterion(E[l].e_ij[i], i);
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
            free_float_n_square_matrix(P_l[i]);
		}

		/* dealloc memory allocated to P_l, caution it has to be deallocated after deallocating all P_l[i] */
		free(P_l);
		P_l=NULL;

		PHI=compute_phi(PI);

#ifdef PRINT_STUFFS
print_PI(PI);
#endif

		/* dealloc memory for PI */
		free_float_n_square_matrix(PI);
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
		free_float_n_square_matrix(PHI);
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
print_aggregated_S_l(S);
#endif

	free_S(S);
#endif /* ends the first #ifdef PRINT_STUFFS */

	/* free_remaining_data ;P */
	free_remaining_data(E);

    read_sorting_data(argv);

	return EXIT_SUCCESS;
}

#endif

