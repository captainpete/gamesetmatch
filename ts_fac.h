#ifndef TS_FAC_H
#define TS_FAC_H 


struct ts_fac;
typedef void (*ts_fac_func_t) (struct ts_fac *, int);

struct ts_fg;
struct ts_var;

typedef struct ts_fac {
	struct ts_var* vars[3];
	struct ts_fg* fg;
	ts_fac_func_t up_func;
	ts_fac_func_t down_func;
	int index;
	double data1;
} ts_fac_t;
#include "ts_var.h"
#include "ts_fg.h"

ts_fac_t* ts_fac_new       (struct ts_fg *fg);
void      ts_fac_eval_down (struct ts_fac *self);
void      ts_fac_eval_up   (struct ts_fac *self, int index);


#endif