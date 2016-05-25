
#ifndef EXPERT_H
#define EXPERT_H

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

/* A linked list denoting the binary preference set of an expert. */
typedef struct expert_pref
{
	int a;/* a and b belongs to A */
	int b;
	unsigned short int p;/* value of the expert prerences <=> a~b, where ~ is p, either preferred (P), can't compare (R) or indifferent (I).. */
	struct expert_pref* NEXT_S_l;/* Linked list, provides the next relation preference of expert l. */
	/* Note : values of p for R, I, P, are respectively 1, 2, 3. */
}expert_pref;


/* The structure "expert" : */
typedef struct expert
{
	float p_l;/* The weight of the expert. */
	float** e_ij;/* e_ij, size K*N, gives the evaluation (the score) of project j on the criterion i by the current expert. */
	expert_pref* S_l;/* binary preference relation (size N*N). */
	int* ranks;/* correct only after the S_l computation and ranking. */
}expert;


typedef expert* data;/* "data" is the list of M "opinions" */

/* adds a new preference relation to the S_l linked list. */
expert_pref* add_S_l(expert_pref* current, unsigned short int new_p, int new_a, int new_b);

#endif
