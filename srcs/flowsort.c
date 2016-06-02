
#ifndef FLOWSORT_C
#define FLOWSORT_C

#include "../heads/flowsort.h"

categorie* add_to_categorie(categorie* head, int to_add)
{
	/* LIFO : Last In First Out */
	categorie* new_categorie_ll;
	new_categorie_ll=malloc(sizeof(categorie));

	if(new_categorie_ll == NULL){ /* memory allocation failure */ printf("MEMO_ALLOC_FAILURE line %d in file %s\n", __LINE__, __FILE__); }

	new_categorie_ll->alternative=to_add;
	new_categorie_ll->next_alternative=head;

	return new_categorie_ll;
}


float** sorting_level_criterion(const float* e_l_i, int j)
{
	int a=0,b=0,i=0;
	float d=0., buff=0.;
	bool assigned=false;
	float** P_i;

	P_i=malloc(	N*sizeof(float*));
	if(P_i == NULL){ /* memory allocation failure */ printf("MEMO_ALLOC_FAILURE line %d in file %s\n", __LINE__, __FILE__); }

	for(i=0;i<N;i++)
	{
		P_i[i]=calloc(N_CAT,sizeof(float));
		if(P_i[i] == NULL){ /* memory allocation failure */ printf("MEMO_ALLOC_FAILURE line %d in file %s\n", __LINE__, __FILE__); }
	}

	assigned=false;

	for(a=0;a<N;a++)
	{
		for(b=0;b<N_CAT;b++)
		{
			d=e_l_i[a]-R[b][j];

			assigned=false;
			i=0;
			buff=(d < 0 ? -d : d);/* absolute value since P_i[worst][best] has the worst value anyway, remains the best which has always d > 0 */

			while(!assigned)
			{
				if( buff < THRESHOLDS[j][i] )
				{
					P_i[a][b]=( d >= 0 ? LEV_CRIT_GRADS[i] : -(LEV_CRIT_GRADS[i]) );
					assigned=true;
					continue;
				}

				i++;

				if( i==3 )
				{/* End of the threshold array => strict preferrence */
					P_i[a][b]=( d >= 0 ? LEV_CRIT_GRADS[i] : -(LEV_CRIT_GRADS[i]) );
					assigned=true;
				}
			}
		}
	}

	return P_i;
}



#endif
