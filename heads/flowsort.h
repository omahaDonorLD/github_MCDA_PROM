
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
 * C is used from the categories point of view : " what are the actions in category i ? "
*/
ptr_cat** C;

/** R is the list of categories on which the differents actions belong : size N*3 (for each action : the positive, negative and net flows to which it belongs).
 * R is the "set of categories" from the actions point of view : " what are the categories the action i belongs to ? ".
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


/** flowsort // Check into file promethee.h the format of preference indices when it comes for reference profiles.
 * 
 * The main function that performs the sorting : flowsort
 *
 * @param : the flows.
 * 
 */
void flowsort(float** PHI);


/** print into file the results of promethee **/
void write_flowsort_results(float** PI, float** PHI, char** argv);

#endif

