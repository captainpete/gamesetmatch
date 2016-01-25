#ifndef TS_FG_H
#define TS_FG_H 
#define TS_USELESS -1
#define ITERATIONS 5
#define TS_MAX_PLAYERS 64
#define TS_FACTORS 3
#define TS_VARS 4
#define TS_MAX_VARS TS_VARS * TS_MAX_PLAYERS
#define TS_MAX_FACTORS TS_FACTORS * TS_MAX_PLAYERS

#include "gauss.h"
#include "ts_fac.h"
#include "ts_var.h"

struct ts_fac;
struct ts_var;

typedef struct ts_fg {
	int var_n;
	int factor_n;
	int msg_n;
	int n;
	double beta;
	struct ts_var vars[TS_MAX_VARS];
	struct ts_fac factors[TS_MAX_FACTORS];
	struct ts_fac* likelyhood_layer[TS_MAX_PLAYERS];
	struct ts_fac* diff_layer[TS_MAX_PLAYERS];
	struct ts_fac* trunc_layer[TS_MAX_PLAYERS];
	gauss_t factor_to_var_msg[TS_MAX_VARS][TS_MAX_FACTORS];
	struct ts_var* skill_vars[TS_MAX_PLAYERS];
	struct ts_var* perf_vars[TS_MAX_PLAYERS];
} ts_fg_t;

#define ts_fg_msg_to_var(var, fac) var->fg->factor_to_var_msg[var->index][fac->index]


ts_fg_t* ts_fg_new ();
void     ts_fg_run (ts_fg_t *self, gauss_t *skills, int n, double beta, double tau);



#endif