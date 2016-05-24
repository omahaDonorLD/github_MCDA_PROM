
#ifndef EXPERT_H
#define EXPERT_H

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

/**
#define PTR_NULL(PTR,LINE,FROM_FILE)	( ( (PTR_NULL) == NULL){
**/
/* above create a memory allocation failure (here should be comments ending : *star*slash) */
/**
printf("MEMO_ALLOC_FAILURE line %d in file %s\n", (LINE), (FROM_FILE )); })
**/


typedef struct expert_pref
{
	unsigned short int p;/* value of the expert prerences. */
	int a;/* a and b belongs to A; a~b, where ~ is p, either preferred (P), can't compare (R) or indifferent (I). */
	int b;
	struct expert_pref* NEXT_S_l;/* Linked list, provides the next relation preference of expert l. */
	/* Note : values of p for R, I, P, are respectively 1, 2, 3. */
}expert_pref;


/* An expert is defined as follow : */
typedef struct expert
{
	float p_l;/* The weight of the expert. */
	float** e_ij;/* e_ij, size K*N, gives the evaluation (the score) of project j on the criterion i by the current expert. */
	expert_pref* S_l;/* binary preference relation (size N*N). */
	int* ranks;/* correct only after the S_l computation and ranking. */
}expert;

typedef expert* data;/* "data" is the set of M "opinions" */

/* add a new preference relation to the S_l linked list. */
expert_pref* add_S_l(expert_pref* current, unsigned short int new_p, int new_a, int new_b);

#endif
