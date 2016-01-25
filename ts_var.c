#include <stdio.h>
#include "ts_error.h"
#include "gauss.h"
#include "ts_fg.h"
#include "ts_fac.h"
#include "ts_var.h"



ts_var_t* ts_var_new(ts_fg_t *fg){
	if(fg->var_n >= TS_MAX_VARS) ts_exit_error("Max vars reached: %d\n", TS_MAX_VARS);
	ts_var_t *self = &(fg->vars[fg->var_n]);
	self->index = fg->var_n;
	self->fg    = fg;
	self->value = gauss_init_prec(0.0, 0.0);
	fg->var_n++; 
	return self;
}


void ts_var_set(ts_var_t *self, gauss_t val){
	self->value = val;
}

void ts_var_update_msg(ts_var_t *self, ts_fac_t *factor, gauss_t msg){
	gauss_t old_msg = ts_fg_msg_to_var(self, factor);
	ts_fg_msg_to_var(self, factor) = msg;
	self->value = gauss_mul(gauss_div(self->value, old_msg), msg); 
}

void ts_var_update_val(ts_var_t *self, ts_fac_t *factor, gauss_t val){
	gauss_t old_msg = ts_fg_msg_to_var(self, factor);
	ts_fg_msg_to_var(self, factor) = gauss_div(gauss_mul(val, old_msg), self->value);
	self->value = val;
}
/*
double ts_var_update_msg_delta(ts_var_t *self, ts_fac_t *factor, gauss_t msg){
	old_val = self->value;
	ts_var_update_msg(self, factor, msg);
	return gauss_abs_diff(old_val, self->value);
}

double ts_var_update_val_delta(ts_var_t *self, ts_fac_t *factor, gauss_t val){
	old_val = self->value;
	ts_var_update_msg(self, factor, val);
	return gauss_abs_diff(old_val, self->value);
}
*/