
#ifndef FLOWSORT_H
#define FLOWSORT_H

#include "../heads/stoch_prom.h"


/** a "categorie" is a container of different alternatives => a linked-list of alternatives (integers) */
typedef struct categorie
{
    int alternative;
    struct categorie* next_alternative;
}categorie;


/** the head of the linked-list containing the alternatives
*/
typedef categorie* ptr_cat;


/** C is a set of 3*N_CAT (positive, negative and net flows) pointers to "category" types (to the heads ot the linked-lists).
*/
ptr_cat** C;

/** C is the set of 3*N_CAT ptr to categories : comparing to the positive, negative and net flows.
*/
int** act_to_cat;


/** add_a_categorie
 * 
 * add a "category" to the linked-list of actions so as the action to add becomes the head and the previous head comes next.
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

