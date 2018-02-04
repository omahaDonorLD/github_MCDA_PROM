#!/bin/bash

# Deterministic part
./MCDA 0 0 0 data/uop_eval_dtrmnistic data/uop_eval_dtrmnistic_reference_profiles
./MCDA 1 0 0 data/uop_eval_dtrmnistic data/uop_eval_dtrmnistic_reference_profiles
./MCDA 0 0 1 data/uop_eval_dtrmnistic data/uop_eval_dtrmnistic_reference_profiles
./MCDA 1 0 1 data/uop_eval_dtrmnistic data/uop_eval_dtrmnistic_reference_profiles

# Stochastic part sig=4
./MCDA 0 1 0 data/uop_eval_stoch_sig_4 data/uop_eval_stoch_ref_profil_sig_4
./MCDA 1 1 0 data/uop_eval_stoch_sig_4 data/uop_eval_stoch_ref_profil_sig_4
./MCDA 0 1 1 data/uop_eval_stoch_sig_4 data/uop_eval_stoch_ref_profil_sig_4
./MCDA 1 1 1 data/uop_eval_stoch_sig_4 data/uop_eval_stoch_ref_profil_sig_4

# Stochastic part sig=1
./MCDA 0 1 0 data/uop_eval_stoch_sig_1 data/uop_eval_stoch_ref_profil_sig_1
./MCDA 1 1 0 data/uop_eval_stoch_sig_1 data/uop_eval_stoch_ref_profil_sig_1
./MCDA 0 1 1 data/uop_eval_stoch_sig_1 data/uop_eval_stoch_ref_profil_sig_1
./MCDA 1 1 1 data/uop_eval_stoch_sig_1 data/uop_eval_stoch_ref_profil_sig_1

# Stochastic part sig=.5
./MCDA 0 1 0 data/uop_eval_stoch_sig_nor5 data/uop_eval_stoch_ref_profil_sig_nor5
./MCDA 1 1 0 data/uop_eval_stoch_sig_nor5 data/uop_eval_stoch_ref_profil_sig_nor5
./MCDA 0 1 1 data/uop_eval_stoch_sig_nor5 data/uop_eval_stoch_ref_profil_sig_nor5
./MCDA 1 1 1 data/uop_eval_stoch_sig_nor5 data/uop_eval_stoch_ref_profil_sig_nor5

# Stochastic part sig=.5
./MCDA 0 1 0 data/uop_eval_stoch_sig_nor2 data/uop_eval_stoch_ref_profil_sig_nor2
./MCDA 1 1 0 data/uop_eval_stoch_sig_nor2 data/uop_eval_stoch_ref_profil_sig_nor2
./MCDA 0 1 1 data/uop_eval_stoch_sig_nor2 data/uop_eval_stoch_ref_profil_sig_nor2
./MCDA 1 1 1 data/uop_eval_stoch_sig_nor2 data/uop_eval_stoch_ref_profil_sig_nor2

