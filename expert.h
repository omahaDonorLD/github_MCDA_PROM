
#ifndef EXPERT_H
#define EXPERT_H

#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include <stdbool.h>

/**
#define PTR_NULL(PTR,LINE,FROM_FILE)	( ( (PTR_NULL) == NULL){
**/
/* memory allocation failure (here should be comments ending : *star*slash) */
/**
printf("MEMO_ALLOC_FAILURE line %d in file %s\n", (LINE), (FROM_FILE )); })
**/


typedef struct expert_pref
{
	unsigned short int p;/* value of the expert prerences. Added only when P R or I. NA or NOPREF is skipped. */
	int a;/* a*b, where * is either  P, R or I */
	int b;/* a*b, where * is either  P, R or I */
	struct expert_pref* NEXT_S_l;/* Linked list */
	/* Note : values of p for R, I, P, are respectively 1, 2, 3. */
}expert_pref;


/* The "object" expert is defined as follow : */
typedef struct expert
{
	float p_l;/* The weight of the expert. */
	float** e_ij;/* e_ij, size K*N, gives the evaluation (the score) of project j on the criterion i by the current expert. */
	expert_pref* S_l;/* binary preference relation (size N*N). */
	int* ranks;/* when S_l computation is done, rank */
}expert;


typedef expert* data;/* "data" is the set of M "opinions" */


expert_pref* add_S_l(expert_pref* current, unsigned short int new_p, int new_a, int new_b);

#endif
