#ifndef TS_LH_FAC_H
#define TS_LH_FAC_H
#include "ts_fac.h"

#define TS_LH_MEAN_VAR 0
#define TS_LH_VAL_VAR 1

#define ts_lh_variance(_self) (_self)->data1

ts_fac_t* ts_lh_fac_new(ts_fg_t *fg, ts_var_t *mean_var, ts_var_t *val_var, double variance);

#endif