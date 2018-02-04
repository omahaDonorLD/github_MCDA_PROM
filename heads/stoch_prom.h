
#ifndef STOCH_PROM_H
#define STOCH_PROM_H

#include "promethee.h"

/** DELTA
 * 
 * Computes the cumulative distribution function (CDF) of the standard normal distribution.
 * 
 * @param : MU and STD for N(MU,STD) where here MU is the mean w.r.t. the difference  e_{i_{1}}_{j} - e_{i_{2}}_{j} and STD its standard deviation
 * @return : the CDF of the standard normal distribution of a random variable evaluated at x.
 *
*/
float DELTA(float x, float d_mu, float d_sigma);


/** EPF
 * 
 * The expected preference function when either the level criterion or the linear criterion is used.
 * Note : the use of the EPF method is different from the mere application of the level criterion preference function
 *
 * @param : e_l_i the list of N evaluations made by the expert l, and j the criterion currently evaluated. SORTING says whether a flowsort is also applied
 * @return : the expected preference function
 *
*/
float** EPF(const float* e_l_i, int j, bool SORTING);


/** EPF_level_criterion
 * 
 * The expected preference function when the level criterion is used.
 *
*/
float EPF_level_criterion(int j, float d_MU, float d_SIGMA);


/** EPF_linear_criterion
 * 
 * The expected preference function when the linear criterion is used.
 *
*/
float EPF_linear_criterion(int j, float d_MU, float d_SIGMA);


/** the standard deviation for each alternative on each criterion. */
float** STDs;
/** same for reference profiles. */
float** R_STDs;
/** Table of EPF to use : EPF with type Level criterion or Linear criterion */
float (*TBL_PREF_FUNC_EPF[2])(int j, float d_MU, float d_SIGMA);

#endif

