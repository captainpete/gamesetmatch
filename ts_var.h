#ifndef TS_VAR_H
#define TS_VAR_H 
#include "gauss.h"

struct ts_fg;
struct ts_fac;
typedef struct ts_var {
	struct ts_fg *fg;
	int index;
	gauss_t value;

} ts_var_t;

#include "ts_fg.h"
ts_var_t* ts_var_new        (struct ts_fg *fg);
void      ts_var_set        (ts_var_t *self, gauss_t val); 
void      ts_var_update_msg (ts_var_t *self, struct ts_fac *factor, gauss_t msg);
void      ts_var_update_val (ts_var_t *self, struct ts_fac *factor, gauss_t val);

#endif