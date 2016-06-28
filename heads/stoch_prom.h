
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


/** EPF_level_criterion
 * 
 * The expected preference function with use of the level criterion.
 * Caution : the computing of the EPF is different from the usual and mere application of the level criterion preference function
 *
 * @param : e_l_i the list of N evaluations made by the expert l, and j the criterion currently evaluated.
 * @return : the expected preference function
 *
*/
float** EPF_level_criterion(const float* e_l_i, int j, bool sort_shift);


/** the standard deviation for each alternative on each criterion. */
float** STDs;

#endif

