
#include "gauss.h"
#include "ts_fac.h"
#include "ts_w_fac.h"

void ts_w_fac_up(ts_fac_t *self, int index){
	ts_var_t *win_var  = self->vars[TS_W_WIN_VAR];
	ts_var_t *lose_var = self->vars[TS_W_LOSE_VAR];
	ts_var_t *sum_var  = self->vars[TS_W_SUM_VAR];
	gauss_t w = gauss_div(win_var->value,  ts_fg_msg_to_var(win_var,  self));
	gauss_t l = gauss_div(lose_var->value, ts_fg_msg_to_var(lose_var, self));
	gauss_t s = gauss_div(sum_var->value,  ts_fg_msg_to_var(sum_var, self));
	gauss_t msg;
	ts_var_t *var;
	switch(index){
		case TS_W_WIN_IDX:
		var = win_var;
		// msg = gauss_init_var(s.mean + l.mean, s.var - l.var);
		msg = gauss_add(s, l);
		break;
		case TS_W_LOSE_IDX:
		var = lose_var;
		// msg = gauss_init_var(s.mean - w.mean, s.var - w.var);
		msg = gauss_sub(w, s);
	}
	ts_var_update_msg(var, self, msg);
}

void ts_w_fac_down(ts_fac_t *self, int useless){
	ts_var_t *win_var  = self->vars[TS_W_WIN_VAR];
	ts_var_t *lose_var = self->vars[TS_W_LOSE_VAR];
	ts_var_t *sum_var  = self->vars[TS_W_SUM_VAR];
	gauss_t w = gauss_div(win_var->value,  ts_fg_msg_to_var(win_var,  self));
	gauss_t l = gauss_div(lose_var->value, ts_fg_msg_to_var(lose_var, self));
	gauss_t diff = gauss_sub(w, l);
	ts_var_update_msg(sum_var, self, diff);
}

ts_fac_t* ts_w_fac_new(ts_fg_t *fg, ts_var_t *win_var, ts_var_t *lose_var, ts_var_t *sum_var){
	ts_fac_t *self = ts_fac_new(fg);
	self->vars[TS_W_LOSE_VAR] = lose_var;
	self->vars[TS_W_WIN_VAR]  = win_var;
	self->vars[TS_W_SUM_VAR]  = sum_var;
	self->up_func   = ts_w_fac_up;
	self->down_func = ts_w_fac_down;
	ts_fg_msg_to_var(win_var, self)  = gauss_init_prec(0.0, 0.0);
	ts_fg_msg_to_var(lose_var, self) = gauss_init_prec(0.0, 0.0);
	ts_fg_msg_to_var(sum_var, self)  = gauss_init_prec(0.0, 0.0);

	return self;
}