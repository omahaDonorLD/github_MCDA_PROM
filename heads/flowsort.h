
#ifndef FLOWSORT_H
#define FLOWSORT_H

#include "../heads/promethee.h"

typedef struct categorie
{
    int alternative;
    struct categorie* next_alternative;
}categorie;/* a "categorie" is a container of different alternatives <=> a linked-list of alternatives */

typedef categorie* ptr_cat;

ptr_cat** C;/* C is the set of 3*N_CAT ptr to categories : comparing to the positive, negative and net flows. */


/** add_a_categorie
 * 
 * an object "categorie" is a linked-list of actions. This function adds an action to the current category.
 * 
 * @param : the action to_add to head of the category
 * @return : the head of the category with the new added action
 *  
 */
ptr_cat add_a_categorie(ptr_cat head, int to_add);

/** flowsort
 * 
 * The main function that performs the sorting : flowsort
 *
 * @param : the flows.
 * 
 */
void flowsort(float** PHI);


#endif

