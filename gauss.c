#include <math.h>
#include <gsl/gsl_randist.h>
#include <gsl/gsl_cdf.h>
#include "gauss.h"

#define SQRT_2_PI     2.506628274631      /* sqrt(2*PI) */
#define LOG_SQRT_2_PI 0.918938533204673   /* log(sqrt(2*PI)) */

gauss_t gauss_init_prec(double prec_mean, double prec){
	gauss_t r;
	r.prec_mean = prec_mean;
	r.prec      = prec;
	r.var       = 1.0 / prec;
	r.mean      = prec_mean / prec;
	r.stddev    = sqrt(r.var);
	return r;
}

gauss_t gauss_init_var(double mean, double var){
	gauss_t r;
	r.var       = var;
	r.mean      = mean;
	r.stddev    = sqrt(var);
	r.prec      = 1.0 / var;
	r.prec_mean = r.prec * r.mean;
	return r;
}

gauss_t gauss_init_std(double mean, double stddev){
	gauss_t r;
	r.var       = stddev * stddev;
	r.mean      = mean;
	r.stddev    = stddev;
	r.prec      = 1.0 / r.var;
	r.prec_mean = r.prec * r.mean;
	return r;
}

double gauss_log_product_normal(gauss_t a, gauss_t b){
	if(a.prec == 0 || b.prec == 0) return 0.0;
	
	double var_sum   = a.var + b.var;
	double mean_diff = a.mean - b.mean;
	
	return -LOG_SQRT_2_PI - (log(var_sum)/2.0) - ((mean_diff * mean_diff) / (2.0 * var_sum));
}

double gauss_log_ratio_normal(gauss_t a, gauss_t b){
	if(a.prec == 0 || b.prec == 0) return 0.0;
	
	double var_diff  = a.var - b.var;
	double mean_diff = a.mean - b.mean;

	return log(b.var) + LOG_SQRT_2_PI - (log(var_diff)/2.0) + ((mean_diff * mean_diff) / (2.0 * var_diff));
}


double gauss_cdf_P(gauss_t a, double x){
	return gsl_cdf_gaussian_P(x - a.mean, a.stddev);
}

double gauss_cdf_Q(gauss_t a, double x){
	return gsl_cdf_gaussian_Q(x - a.mean, a.stddev);
}

gauss_t gauss_add(gauss_t a, gauss_t b){
	gauss_t r;
	r.mean      = a.mean + b.mean;
	r.var       = a.var  + b.var;
	r.prec      = 1.0 / r.var;
	r.prec_mean = r.prec * r.mean;
	r.stddev    = sqrt(r.var);
	return r;
}





gauss_t gauss_sub(gauss_t a, gauss_t b){
	gauss_t temp = b;
	temp.mean = -temp.mean;
	return gauss_add(a, temp);
}

double gauss_abs_diff(gauss_t a, gauss_t b){
	double prec_mean_diff = fabs(a.prec_mean - b.prec_mean);
	double prec_diff = sqrt(fabs(a.prec - b.prec));
	return (prec_mean_diff > prec_diff) ? prec_mean_diff : prec_diff;
}

gauss_t gauss_div(gauss_t a, gauss_t b){
	double prec      = a.prec      - b.prec;
	double prec_mean = a.prec_mean - b.prec_mean;
	return gauss_init_prec(prec_mean, prec);
}

gauss_t gauss_mul(gauss_t a, gauss_t b){
	double prec      = a.prec      + b.prec;
	double prec_mean = a.prec_mean + b.prec_mean;
	return gauss_init_prec(prec_mean, prec);
}
