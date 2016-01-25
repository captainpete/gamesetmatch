#include <gsl/gsl_randist.h>
#include <gsl/gsl_cdf.h>
#include <math.h>

#include "gauss.h"
#include "ts_fac.h"
#include "ts_tr_fac.h"



static double v_func(double diff){
	double denom = gsl_cdf_ugaussian_P(diff);
	double nom = gsl_ran_ugaussian_pdf(diff);
	return nom / denom;
}

static double w_func(double diff, double v){
	return (v*(v+diff));
}
static void ts_tr_fac_up(ts_fac_t *self, int useless){
	ts_var_t *var  = self->vars[TS_TR_SUM_VAR];
	gauss_t msg    = ts_fg_msg_to_var(var, self);
	gauss_t val    = gauss_div(var->value, msg);
	double sqrt_pi = sqrt(gauss_pi(val));
	double normal  = gauss_tau(val) / sqrt_pi;
	double v       = v_func(normal);
	double w       = w_func(normal, v);
	double denom   = 1.0 - w;
	double new_pi  = gauss_pi(val) / denom;
	double new_tau = (gauss_tau(val) + (sqrt_pi * v)) / denom;
	gauss_t nv     = gauss_init_prec(new_tau, new_pi);
	ts_var_update_val(var, self, nv);
}

static void ts_tr_fac_down(ts_fac_t *self, int useless){

}

ts_fac_t* ts_tr_fac_new(ts_fg_t *fg, ts_var_t *sum_var){
	ts_fac_t *self = ts_fac_new(fg);
	self->vars[TS_TR_SUM_VAR] = sum_var;
	self->up_func   = ts_tr_fac_up;
	self->down_func = ts_tr_fac_down;
	ts_fg_msg_to_var(sum_var, self) = gauss_init_prec(0.0, 0.0);

	return self;
}