#ifndef TS_GAUSS_H
#define TS_GAUSS_H 

typedef struct {
	double stddev;
	double var;
	double mean;
	double prec;
	double prec_mean;
} gauss_t;

#define gauss_pi(_self)  (_self).prec
#define gauss_tau(_self) (_self).prec_mean

gauss_t gauss_init_prec          (double prec_mean, double prec);
gauss_t gauss_init_var           (double mean, double var);
gauss_t gauss_init_std           (double mean, double stddev);
gauss_t gauss_add                (gauss_t a, gauss_t b);
gauss_t gauss_sub                (gauss_t a, gauss_t b);
gauss_t gauss_mul                (gauss_t a, gauss_t b);
gauss_t gauss_div                (gauss_t a, gauss_t b);
double  gauss_cdf_P              (gauss_t a, double x);
double  gauss_cdf_Q              (gauss_t a, double x);
double  gauss_log_product_normal (gauss_t a, gauss_t b);
double  gauss_log_ratio_normal   (gauss_t a, gauss_t b);
double  gauss_abs_diff           (gauss_t a, gauss_t b);


#endif