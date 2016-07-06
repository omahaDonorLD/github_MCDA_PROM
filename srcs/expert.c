
#ifndef EXPERT_C
#define EXPERT_C

#include "../heads/expert.h"

bool COMPLETE_PREORDER=false;
bool STOCH=false;/* tells if a stochastic problem or the opposite */

/** The 2 following functions are made just to track the memory allocation failure and others failures.
*/
void PRINT_MEM_FAIL(int LINE, char* THE_FILE)
{
	printf("MEMO_ALLOC_FAILURE line %d in file %s\n", LINE, THE_FILE);
}


void LEAVE_FAIL(int LINE, char* THE_FILE)
{
	printf("EXIT_FAILURE line %d in file %s\n", LINE, THE_FILE);
}


expert_pref* add_S_l(expert_pref* current, unsigned short int new_p, int new_a, int new_b)
{
	/* LIFO : Last In First Out */
	expert_pref* new_S_l;
	new_S_l=malloc(sizeof(expert_pref));

	if(new_S_l == NULL){ /* memory allocation failure */ PRINT_MEM_FAIL(__LINE__, __FILE__); }

	new_S_l->p=new_p;
	new_S_l->a=new_a;
	new_S_l->b=new_b;
	new_S_l->NEXT_S_l=current;

	return new_S_l;
}


#endif

