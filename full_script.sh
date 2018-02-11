#!/bin/bash

# Deterministic part
./MCDA 0 0 0 data/uop_dtrm data/uop_dtrm_ref_prof
find ./outputs/* ! -name a -exec mv -t UoP_results/uop_results_dtrmnistic/flowsort/promI/Level_criterion/ {} +

./MCDA 1 0 0 data/uop_dtrm data/uop_dtrm_ref_prof
find ./outputs/* ! -name a -exec mv -t UoP_results/uop_results_dtrmnistic/flowsort/promI/Linear_criterion/ {} +

./MCDA 0 0 1 data/uop_dtrm data/uop_dtrm_ref_prof
find ./outputs/* ! -name a -exec mv -t UoP_results/uop_results_dtrmnistic/flowsort/promII/Level_criterion/ {} +

./MCDA 1 0 1 data/uop_dtrm data/uop_dtrm_ref_prof
find ./outputs/* ! -name a -exec mv -t UoP_results/uop_results_dtrmnistic/flowsort/promII/Linear_criterion/ {} +

# Stochastic part sig=4
./MCDA 0 1 0 data/uop_stoch_sig4 data/uop_stoch_ref_prof_sig4
find ./outputs/* ! -name a -exec mv -t UoP_results/uop_results_stoch/sig_4/flowsort/promI/Level_criterion/ {} +

./MCDA 1 1 0 data/uop_stoch_sig4 data/uop_stoch_ref_prof_sig4
find ./outputs/* ! -name a -exec mv -t UoP_results/uop_results_stoch/sig_4/flowsort/promI/Linear_criterion/ {} +

./MCDA 0 1 1 data/uop_stoch_sig4 data/uop_stoch_ref_prof_sig4
find ./outputs/* ! -name a -exec mv -t UoP_results/uop_results_stoch/sig_4/flowsort/promII/Level_criterion/ {} +

./MCDA 1 1 1 data/uop_stoch_sig4 data/uop_stoch_ref_prof_sig4
find ./outputs/* ! -name a -exec mv -t UoP_results/uop_results_stoch/sig_4/flowsort/promII/Linear_criterion/ {} +

# Stochastic part sig=1
./MCDA 0 1 0 data/uop_stoch_sig1 data/uop_stoch_ref_prof_sig1
find ./outputs/* ! -name a -exec mv -t UoP_results/uop_results_stoch/sig_1/flowsort/promI/Level_criterion/ {} +

./MCDA 1 1 0 data/uop_stoch_sig1 data/uop_stoch_ref_prof_sig1
find ./outputs/* ! -name a -exec mv -t UoP_results/uop_results_stoch/sig_1/flowsort/promI/Linear_criterion/ {} +

./MCDA 0 1 1 data/uop_stoch_sig1 data/uop_stoch_ref_prof_sig1
find ./outputs/* ! -name a -exec mv -t UoP_results/uop_results_stoch/sig_1/flowsort/promII/Level_criterion/ {} +

./MCDA 1 1 1 data/uop_stoch_sig1 data/uop_stoch_ref_prof_sig1
find ./outputs/* ! -name a -exec mv -t UoP_results/uop_results_stoch/sig_1/flowsort/promII/Linear_criterion/ {} +

# Stochastic part sig=.5
./MCDA 0 1 0 data/uop_stoch_sigNor5 data/uop_stoch_ref_prof_sigNor5
find ./outputs/* ! -name a -exec mv -t UoP_results/uop_results_stoch/sig_nor5/flowsort/promI/Level_criterion/ {} +

./MCDA 1 1 0 data/uop_stoch_sigNor5 data/uop_stoch_ref_prof_sigNor5
find ./outputs/* ! -name a -exec mv -t UoP_results/uop_results_stoch/sig_nor5/flowsort/promI/Linear_criterion/ {} +

./MCDA 0 1 1 data/uop_stoch_sigNor5 data/uop_stoch_ref_prof_sigNor5
find ./outputs/* ! -name a -exec mv -t UoP_results/uop_results_stoch/sig_nor5/flowsort/promII/Level_criterion/ {} +

./MCDA 1 1 1 data/uop_stoch_sigNor5 data/uop_stoch_ref_prof_sigNor5
find ./outputs/* ! -name a -exec mv -t UoP_results/uop_results_stoch/sig_nor5/flowsort/promII/Linear_criterion/ {} +

# Stochastic part sig=.2
./MCDA 0 1 0 data/uop_stoch_sigNor2 data/uop_stoch_ref_prof_sigNor2
find ./outputs/* ! -name a -exec mv -t UoP_results/uop_results_stoch/sig_nor2/flowsort/promI/Level_criterion/ {} +

./MCDA 1 1 0 data/uop_stoch_sigNor2 data/uop_stoch_ref_prof_sigNor2
find ./outputs/* ! -name a -exec mv -t UoP_results/uop_results_stoch/sig_nor2/flowsort/promI/Linear_criterion/ {} +

./MCDA 0 1 1 data/uop_stoch_sigNor2 data/uop_stoch_ref_prof_sigNor2
find ./outputs/* ! -name a -exec mv -t UoP_results/uop_results_stoch/sig_nor2/flowsort/promII/Level_criterion/ {} +

./MCDA 1 1 1 data/uop_stoch_sigNor2 data/uop_stoch_ref_prof_sigNor2
find ./outputs/* ! -name a -exec mv -t UoP_results/uop_results_stoch/sig_nor2/flowsort/promII/Linear_criterion/ {} +

