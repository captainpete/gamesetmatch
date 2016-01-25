#include <stdlib.h>
#include <string.h>
#include "ts_error.h"
#include "ts_fg.h"
#include "ts_fac.h"
#include "ts_var.h"
#include "ts_lh_fac.h"
#include "ts_w_fac.h"
#include "ts_tr_fac.h"


ts_fg_t* ts_fg_new(){
	return malloc(sizeof(ts_fg_t));
}

void ts_fg_run(ts_fg_t *self, gauss_t *skills, int n, double beta, double tau){
	int i;
	int j;
	self->var_n = 0;
	self->factor_n = 0;
	self->msg_n = 0;
	self->n = n;
	self->beta = beta;
	if(n > TS_MAX_PLAYERS) ts_exit_error("Too many players: %d > %d\n", n, TS_MAX_PLAYERS);
	double beta_sq = beta * beta;
	gauss_t gauss_tau = gauss_init_std(0, tau);
	for(i = 0; i < n; i++){
		self->skill_vars[i] = ts_var_new(self);
		ts_var_set(self->skill_vars[i], gauss_add(skills[i], gauss_tau));
		self->perf_vars[i] = ts_var_new(self);
		self->likelyhood_layer[i] = ts_lh_fac_new(self, self->skill_vars[i], self->perf_vars[i], beta_sq);
		ts_fac_eval_down(self->likelyhood_layer[i]);
	}
	for(i = 0; i < (n - 1); i++){
		ts_var_t* diff_var = ts_var_new(self);
		self->diff_layer[i] = ts_w_fac_new(self, self->perf_vars[i], self->perf_vars[i+1], diff_var);
		self->trunc_layer[i] = ts_tr_fac_new(self, diff_var);
	}
	for(j = 0; j < ITERATIONS; j++){
		for(i = 0; i < (n - 1); i++){
			ts_fac_eval_down (self->diff_layer[i]);
			ts_fac_eval_up   (self->trunc_layer[i], TS_USELESS);
			ts_fac_eval_up   (self->diff_layer[i],  TS_W_LOSE_IDX);
		}
		for(i = (n - 2); i >= 0; i--){
			ts_fac_eval_down (self->diff_layer[i]);
			ts_fac_eval_up   (self->trunc_layer[i], TS_USELESS);
			ts_fac_eval_up   (self->diff_layer[i],  TS_W_WIN_IDX);
		}
	}
	for(i = 0; i < n; i++){
		ts_fac_eval_up(self->likelyhood_layer[i], TS_USELESS);
		skills[i] = self->skill_vars[i]->value;
	}
}



