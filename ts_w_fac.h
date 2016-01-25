#ifndef TS_W_FAC_H
#define TS_W_FAC_H
#include "ts_fac.h"

#define TS_W_WIN_VAR 0
#define TS_W_LOSE_VAR 1
#define TS_W_SUM_VAR 2

#define TS_W_WIN_IDX 0
#define TS_W_LOSE_IDX 1

ts_fac_t* ts_w_fac_new(ts_fg_t *fg, ts_var_t *win_var, ts_var_t *lose_var, ts_var_t *sum_var);



#endif