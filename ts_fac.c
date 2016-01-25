#include "ts_error.h"
#include "ts_fg.h"
#include "ts_fac.h"

ts_fac_t *ts_fac_new(ts_fg_t *fg){
	if(fg->factor_n >= TS_MAX_FACTORS) ts_exit_error("Max factors reached: %d\n", TS_MAX_FACTORS);
	ts_fac_t *self = &(fg->factors[fg->factor_n]);
	self->index = fg->factor_n;
	self->fg    = fg;
	fg->factor_n++; 
	return self;
}

void ts_fac_eval_down(ts_fac_t *self){
	self->down_func(self, -1);
}

void ts_fac_eval_up(ts_fac_t *self, int index){
	self->up_func(self, index);
}
