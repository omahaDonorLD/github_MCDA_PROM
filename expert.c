
#include "expert.h"

expert_pref* add_S_l(expert_pref* current, unsigned short int new_p, int new_a, int new_b)
{
	/* LIFO : Last In First Out */
	expert_pref* new_S_l;
	new_S_l=(expert_pref*) calloc(1,sizeof(expert_pref));
/*
	PTR_NULL(new_S_l,__LINE__,__FILE__);
*/
	if(new_S_l == NULL){ /* memory allocation failure */ printf("MEMO_ALLOC_FAILURE line %d in file %s\n", __LINE__, __FILE__); }

	new_S_l->p=new_p;
	new_S_l->a=new_a;
	new_S_l->b=new_b;
	new_S_l->NEXT_S_l=current;
	
	return new_S_l;
}
