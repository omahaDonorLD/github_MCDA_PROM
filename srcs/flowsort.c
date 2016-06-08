
#ifndef FLOWSORT_C
#define FLOWSORT_C

#include "../heads/flowsort.h"

/* display the results */
#define PRINT_STUFFS

/* =========================================================== */
/* Portion of code to use only if one wants to display results */
/* =========================================================== */
#ifdef PRINT_STUFFS

	void print_categories()
	{
		int c=0,i=0;
		categorie* ite;

		printf("The Categories (non printed categories have no actions within) :");

		for(c=0;c<3;c++)
		{
			if(c<2)
				printf("\nC[%s] : ", (c==1?"negative":"positive"));
			else
				printf("\nC[net] :      ");

			for(i=0;i<(N_CAT-1);i++)
			{
				ite=C[c][i];
				while(ite!=NULL)
				{
					if(ite->alternative==C[c][i]->alternative)
						printf("[C_%d :", i+1);

					printf(" action_%d ", ite->alternative+1);
					ite=ite->next_alternative;

					if(ite==NULL)	printf("]\t");
				}
			}
		}

		printf("\nEnd print_categories\n");
	}


#endif
/*==================== End of printing stuffs ========================*/



ptr_cat add_a_categorie(ptr_cat head, int to_add)
{
	/* LIFO : Last In First Out */
	categorie* new_categorie_ll;
	new_categorie_ll=malloc(sizeof(categorie));

	if(new_categorie_ll == NULL){ /* memory allocation failure */ printf("MEMO_ALLOC_FAILURE line %d in file %s\n", __LINE__, __FILE__); }

	new_categorie_ll->alternative=to_add;
	new_categorie_ll->next_alternative=head;
	return new_categorie_ll;
}

void flowsort(float** PHI)
{
	int a=0,b=0,i=0;

	for(i=0;i<3;i++)
		for(a=0;a<N;a++)
			for(b=1;b<N_CAT;b++)
			{
				if( i != 1 )
				{/* The inequality slightly changes when one wants to assigne an action to a category with its negative flow value  */
					if( PHI[i][((N_CAT+1)*a)+N_CAT] >= PHI[i][((N_CAT+1)*a)+b] || b==N_CAT-1 )
					{
						C[i][b-1]=add_a_categorie(C[i][b-1], a);
						break;
					}
				}
				else
				{
					if( PHI[i][((N_CAT+1)*a)+N_CAT] < PHI[i][((N_CAT+1)*a)+b] || b==N_CAT-1 )
					{
						C[i][b-1]=add_a_categorie(C[i][b-1], a);
						break;
					}
				}
			}
}
	

#endif
