#include "gauss.h"
#include "ts_fac.h"
#include "ts_lh_fac.h"

static void ts_lh_fac_up(ts_fac_t *self, int useless){
	ts_var_t *value_var = self->vars[TS_LH_VAL_VAR];
	ts_var_t *mean_var  = self->vars[TS_LH_MEAN_VAR];
	gauss_t old_msg = ts_fg_msg_to_var(value_var, self);
	gauss_t v = value_var->value;
	gauss_t msg = gauss_div(v, old_msg);
	double a = 1.0 / (1.0 + ts_lh_variance(self) * gauss_pi(msg));
	gauss_t new_msg = gauss_init_prec(gauss_tau(msg) * a, gauss_pi(msg) * a);
	ts_var_update_msg(mean_var, self, new_msg);
}

static void ts_lh_fac_down(ts_fac_t *self, int useless){
	ts_var_t *value_var = self->vars[TS_LH_VAL_VAR];
	ts_var_t *mean_var  = self->vars[TS_LH_MEAN_VAR];
	gauss_t old_msg = ts_fg_msg_to_var(mean_var, self);
	gauss_t v = mean_var->value;
	gauss_t msg = gauss_div(v, old_msg);
	double pi = 1.0 / ts_lh_variance(self);
	double a = pi / (pi + gauss_pi(v));
	gauss_t new_msg = gauss_init_prec(gauss_tau(msg) * a, gauss_pi(msg) * a);
	ts_var_update_msg(value_var, self, new_msg);

}

ts_fac_t* ts_lh_fac_new(ts_fg_t *fg, ts_var_t *mean_var, ts_var_t *val_var, double variance){
	ts_fac_t *self = ts_fac_new(fg);
	self->vars[TS_LH_MEAN_VAR] = mean_var;
	self->vars[TS_LH_VAL_VAR]  = val_var;
	self->up_func   = ts_lh_fac_up;
	self->down_func = ts_lh_fac_down;
	ts_lh_variance(self) = variance;
	ts_fg_msg_to_var(mean_var, self) = gauss_init_prec(0.0, 0.0);
	ts_fg_msg_to_var(val_var, self)  = gauss_init_prec(0.0, 0.0);
	return self;
}