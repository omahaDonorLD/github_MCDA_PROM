
#ifndef PROM_H
#define PROM_H

#include "expert.h"

/** Values for P(a,b), use of LEVEL CRITERION */
#define NO_PREF 0.
#define INDIFF 0.
#define WEAK_PREF 0.5
#define PREF 1.

/** Note  : the "PHI" array (the matrix of flows) is a 3*N matrix, for which the :
 *  - first dimension ( [0][i] | i in 1,..,N ) contains the outranking flow of i,
 *  - 2nd ( [1][i] | i in 1,..,N ) contains the outranked flow of i,
 *  - 3rd ( [2][i] | i in 1,..,N ) contains the net flow of i.
*/

/** Parameters : M experts, K criteria, N alternatives/actions. */
int M, K, N;

/** the two next variables are only used when a sorting is to be performed. */
int N_CAT;/* the number of categories */
float** R;/* the reference profiles matrix : size N_CAT*K, where K is the number of criteria */

/** Note : array of size [M+1][N]. The (M+1)th list contains the "global" result with all the experts :
 * for a in A, RANKS[M+1][a] = sum {l in 1 to M} (rank_{l}(a))
*/
int** RANKS;
float* COMPLETE_PREORDER_S;

/** Thresholds and their preference values with respect to level criterion shape */
float* LEV_CRIT_GRADS;
float** THRESHOLDS;

float* criterion_weights;/* The criterion weights :) */


/** qsort_comparator
 *
 * used to give the sorting criteria for the ranking of projects :
 * 		"score" is a 2D list, where the first dimension contains the integers 1 to N that gives the index of
 *
 * @param : the two scores to compare
 * @return : true/false (short way to present it.)
 *
*/
int qsort_comparator(const void* pa, const void* pb);


/** Preference function
 * 
 * @param : e_l_i the list of N evaluations made by the expert l, and j the criterion currently evaluated.
 * @return : P_k_l, of value 0/0.5/1 as defined by the level criterion shape.
 *
*/
float** pref_func(const float* e_l_i, int j, bool SORTING);


/** level_criterion
 *
 * @param : j the criterion currently evaluated, d the distance between the preference values of the two alternatives to compare, and buff the absolute value of d needed for computing purposes
 * @return : the computation of according and the level criterion preference function shape
*/
float level_criterion(int j, float d, float buff);


/** linear_criterion
 *
 * similar to the function level_criterion except that for this one the computation is made according to the linear criterion preference function
 * 
*/
float linear_criterion(int j, float d, float buff);


/** compute_pref_indices
 *
 * Build the matrix of preference indices (PI).
 * 
 * Note : The structure of PI table is singular when the preference indices function is dedicated to the sorting (Sorry, this is due to a mistake done from the beginning, but a change would have required a lot of others changes, and the due date is really close, and that is it, just deal with it, and :P ...).
 * 			It is presented as follow :
 * 
 *			PI		|	r_{1}					r_{2}				...				r_{K}
 * 		========================================================================================
 * 	OUT		a_{1}	|	PI[a_{1},r_{1}]			PI[a_{1},r_{2}]		...				PI[a_{1},r_{K}]
 *			a_{2}	|	PI[a_{2},r_{1}]			PI[a_{2},r_{2}]		...				PI[a_{2},r_{K}]
 * 			...		|	...						...					...				...
 * 			a_{N}	|	PI[a_{N},r_{1}]			PI[a_{N},r_{2}]		...				PI[a_{N},r_{K}]
 * 
 * 	IN		a_{1}	|	PI[r_{1},a_{1}]			PI[r_{2},a_{1}]		...				PI[r_{K},a_{1}]
 *			a_{2}	|	PI[r_{1},a_{2}]			PI[r_{2},a_{2}]		...				PI[r_{K},a_{2}]
 * 			...		|	...						...					...				...
 * 			a_{N}	|	PI[r_{1},a_{N}]			PI[r_{2},a_{N}]		...				PI[r_{K},a_{N}]
 * 
 * @param : P_l, the complete vector of K P_k_l, i.e. : P_l = (P_1_l, ... , P_K_l).
 * @return : PI, the matrix of preference indices.
 *
*/
float** compute_pref_indices(float*** P_l, bool SORTING);


/** compute_phi
 *
 *  Computes the outranking-outranked-net flows
 *
 * @param : PI, the preference indices matrix.
 * @return : PHI, the matrix of flows (0, 1 and 2 respectively the outranking, outranked and net values.).
 *
*/
float** compute_phi(float** PI, bool SORTING);


/** PROM_1
 *
 * Assigns the preferences binary relations with respect to the PROMETHEE 1 outranking flow.
 * Note_1 : 0 is NA or not preferred, 1 is R or incomparable, 2 is I or indifference, 3 is the graal P or Preference relation
 * Note_2 : dimensions : PHI[2][N]
 *
 * @param : PHI, the flows computed from the data by the expert l.
 * @return : S_l, the table of binary (kind of) relations.
 *
*/
void PROM_1(expert* E_l, float** PHI);


/** PROM_2
 * 
 * Assigns the preferences binary relations with respect to the PROMETHEE 2 outranking flow.
 * 
 * @param : PHI, the flows computed from the data by the expert l.
 * @return : S_l, the table of binary (kind of) relations.
 * 
*/
void PROM_2(expert* E_l, float** PHI);


/** rank_single
 *
 * ranking of the projects from just one expert
 *
 * @param : the linked list of S_l and the expert l that is doing the ranking.
 * @return : nothing
 *
*/
void rank_single(expert_pref* first, int l);


/** get_S_a
 *
 * doesn't do really much other than summing the weighted ranks and store the result on the last column : "RANKS" matrix
 *
 * @param : the collected data from all experts, though only need and use their ranks
 * @return : nothing
 *
*/
void get_S_a(const data E);


/** aggregate_S_l
 *
 * With respect to the formula S(a,b)=sum{l|(a,b) in S_l}(p_l)
 * Note : frees the memory of S_l at the same time.
 *
 * @param : data from all the experts to extract their S_l
 * @return : S the aggregated S_l
 *
*/
float** aggregate_S_l(const data E);


/** Dealloc memory **/
/** Free data **/
void free_PHI(float** PHI);

/** Free data **/
void free_alloc_for_PROM_II();

void free_S(float** S);

void free_data(data E);

/** free_float_matrix
 * 
 * Frees the allocated memory to the 2d array with n_lines lines
 * 
 * @param : float_matrix the matrix to free and n_lines its number of lines
 * 
*/
void free_float_matrix(float** float_matrix, int n_lines);


/** print into file the results of promethee **/
void write_prom_results(int l, float **PI, float **PHI, expert_pref* first, char **argv);
void write_prom_global_results(char **argv, float** S);


/** Table of preference functions */
float (*TBL_PREF_FUNC[2])(int j, float d, float buff);

int TYPE_CRITERION;/** 0 : Level criterion, 1 : Linear criterion */

#endif

