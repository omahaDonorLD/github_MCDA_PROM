
#ifndef PROM_H
#define PROM_H

#include "expert.h"

/* Values for P(a,b), use of LEVEL CRITERION */
#define NO_PREF 0.
#define INDIFF 0.
#define WEAK_PREF 0.5
#define PREF 1.

/* Note  : the "PHI" array (the matrix of flows) is a 3*N matrix, for which the :
 *  - first line ( [0][i] | i in 1,..,N ) contains the outranking flow of i,
 *  - 2nd ( [1][i] | i in 1,..,N ) contains the outranked flow of i,
 *  - 3rd ( [2][i] | i in 1,..,N ) contains the net flow of i.
*/

int M, K, N;/* Parameters : M "experts", K criteria, N alternatives/actions */

/* Note : array of size [M+1][N]. The (M+1)th list contains the "global" result with all the experts :
 * for a in A, RANKS[M+1][a] = sum {l in 1 to M} (rank_{l}(a))
*/
int** RANKS;
float* COMPLETE_PREORDER_S;

/* Threshold and the with respect to level criterion shape */
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


/** level_criterion
 *
 * @param : e_l_i the list of N evaluations made by the expert l, and j the criterion currently evaluated.
 * @return : P_k_l, of value 0/0.5/1 as defined by the level criterion shape.
 *
*/
float** level_criterion(const float* e_l_i, int j);


/** compute_pref_indices
 *
 * Build the matrix of preference indices (PI).
 *
 * @param : P_l, the complete vector of K P_k_l, i.e. : P_l = (P_1_l, ... , P_K_l).
 * @return : PI, the matrix of preference indices.
 *
*/
float** compute_pref_indices(float*** P_l);


/** compute_phi
 *
 *  Computes the outranking-outranked-net flows
 *
 * @param : PI, the preference indices matrix.
 * @return : PHI, the matrix of flows (0, 1 and 2 respectively the outranking, outranked and net values.).
 *
*/
float** compute_phi(float** PI);


/** PROM_1
 *
 * Assigns preferences with respect to the PROMETHEE 1 outranking flow.
 * Note_1 : 0 is NA or not preferred, 1 is R or incomparable, 2 is I or indifference, 3 is the graal P or Preference relation
 * Note_2 : dimensions : PHI[2][N]
 *
 * @param : PHI, the flows computed from the data by the expert l.
 * @return : S_l, the table of binary (kind of) relations.
 *
*/
void PROM_1(expert* E_l, float** PHI);


/** rank_single
 *
 * ranking of the projects from just a single experts opinion
 *
 * @param : the linked list of S_l and the expert l that is doing the ranking.
 * @return : nothing
 *
*/
void rank_single(expert_pref* first, int l);


/** get_S_a
 *
 * doesn't do really much than summing the weighted ranks and store the result on the last column of "RANKS" matrix
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

/** P_i, PI, and PHI are all 2D float of size N*N, hence one can use the same method to free them **/
void free_float_n_square_matrix(float** two_d_float);

/** Free data **/
void free_remaining_data(data E);

/** Free data **/
void free_alloc_for_PROM_II();

/** Free data **/
void free_S(float** S);


#endif

