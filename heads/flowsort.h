
#ifndef FLOWSORT_H
#define FLOWSORT_H

#include "../heads/promethee.h"

int N_CAT;/* the number of categories */

typedef struct categorie
{
    int alternative;
    struct categorie* next_alternative;
}categorie;/* a "categorie" is a container of different alternatives <=> a linked-list of alternatives */

float** R;/* the reference profiles matrix : size N_CAT*K, where K is the number of criteria */
categorie* C;/* C is the set of N_CAT categories */

float** sorting_level_criterion(const float* e_l_i, int j);

/** the size of the performance degrees between the actions and reference profiles is : 2*(N*sizeof(R)),
 *   2* since for entering and leaving performance for each alternative
*/
float** in_pref_deg_profil_actions(data E, float** R);
float** out_pref_deg_profil_actions(data E, float** R);


/** flow_sort
 *
 * applies the flowsort method
 *
 * @param : the data to get the evaluation of the actions, R is known and is a global variable
 *
*/
void flow_sort(data E);

#endif

