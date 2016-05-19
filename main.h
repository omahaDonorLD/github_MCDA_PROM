
#ifndef MAIN_H
#define MAIN_H

#include "expert.h"

/* Values for P(a,b), use of LEVEL CRITERION */
#define NO_PREF 0.
#define INDIFF 0.
#define WEAK_PREF 0.5
#define PREF 1.

/*
	Note  : the PHI array is a 3*N matrix, for which the :
		- first line ( [0][i] | i in 1,..,N ) contains the outranking flow of i,
		- 2nd ( [1][i] | i in 1,..,N ) contains the outranked flow of i,
		- 3rd ( [2][i] | i in 1,..,N ) contains the net flow of i.
*/


/** comparator
 * 
 * used to give the sorting criteria for the ranking of projects :
 * 		"score" is a 2D list, where the first dimension contains the integers 1 to N that gives the index of 
 * 
 * @param : the two scores to compare
 * @return : true/false (short way to present it.)
 * 
*/
int comparator(const void* score_1, const void* score_2);


/** read_data
 * 	
 * Read instances given as arguments and return the right form to be used by the application.
 * 
 * @param : the text file (see ./data/README for more.) depicting the data for the instance.
 * @return : a usable form of data for PROMETHEE : a vector of matrices of size M. Each matrix is of size (K,N) and represents the preference values given by the current expert l in 1..M to the project i in 1..N on the criteria i in 1..K. Such matrix is denoted E_l, and the vector of all such matrices is denoted E.
 *
*/
data read_data(char** argv);


/** level_criterion
 * 
 * @param : e_l_i or e_i[j]_l, the 1D array of N evaluations made by the expert l.
 * @return : P_k_l, of value 0/0.5/1 as defined by the level criterion shape.
 * 
*/
float** level_criterion(const float* e_l_i);


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


/** S_project
 * 
 * doesn't do really much than summing the weighted ranks and store the result on the last column of "RANKS" matrix
 * 
 * @param : nothing
 * @return : nothing
 * 
*/
void S_project(const data E);


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

/** P_i, PI, and PHI are all 2D float, thus one can use the same method to free them **/
void free_2D_float(float** two_d_float);

/** Free data **/
void free_remaining_data(data E);

/** Free data **/
void free_alloc_for_PROM_II();

/** Free data **/
void free_S(float** S);


#endif
