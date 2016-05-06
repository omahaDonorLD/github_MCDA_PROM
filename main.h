
#ifndef MAIN_H
#define MAIN_H

#include "expert.c"
/*
#define M 6//Number of "experts"
#define K 3//Number of criteria
#define N 4//Number of alternatives/actions
*/

// Values for P(a,b), use of LEVEL CRITERION
#define NO_PREF 0.
#define INDIFF 0.
#define WEAK_PREF 0.5
#define PREF 1.


int M=0, K=0, N=0;// Parameters : M "experts", K criteria, N alternatives/actions
float q=0., p=0.;// Thresholds for indifference-weak-strict preferences.

// Threshold and  the with respect to level criterion shape
float LEV_CRIT_GRADS[4] = {NO_PREF,INDIFF,WEAK_PREF,PREF};
float THRESHOLDS[3] = {-q,q,p};

float* criterion_weights;// The criterion weights :)

// Note  : the PHI_l array is a 3*N matrix, for which the :
// 		- first line ( [0][i] | i in 1,..,N ) contains the outranking flow of i,
// 		- 2nd ( [1][i] | i in 1,..,N ) contains the outranked flow of i,
//		- 3rd ( [2][i] | i in 1,..,N ) contains the net flow of i.


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
float** compute_pref_indices(const float*** P_l);


/** compute_phi
 * 
 *  Computes the outranking-outranked-net flows
 * 
 * @param : PI, the preference indices matrix.
 * @return : PHI, the matrix of flows (0, 1 and 2 respectively the outranking, outranked and net values.).
 * 
*/
float** compute_phi(float const* const* PI_l);


/** PROM_1
 * 
 * Assigns preferences with respect to the PROMETHEE 1 outranking flow.
 * Note_1 : 0 is NA or not preferred, 1 is R or incomparable, 2 is I or indifference, 3 is the graal P or Preference relation
 * Note_2 : dimensions : PHI_l[2][N]
 * 
 * @param : PHI_l, the flows computed from the data by the expert l.
 * @return : S_l, the table of binary (kind of) relations.
 * 
*/
void PROM_1(expert* E_l, float const* const* PHI_l);


/** aggregate_S_l
 * 
 * With respect to the formula 
 * 
 * @param : 
 * @return : 
 * 
*/
float** aggregate_S_l(const data* E);

#endif
