
#ifndef __LEVMARQT_MINIMIZER_H__
#define __LEVMARQT_MINIMIZER_H__
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <float.h>
#include "data_type.h"

//#include "..\common\reuse.h"
//#include "..\common\bev_data_type.h"
/*****************************************************************************/
/*  Numeric constants                                                        */
/*****************************************************************************/

/* machine-dependent constants from float64_t.h */
//#define LM_MACHEP     DBL_EPSILON   /* resolution of arithmetic */
//#define LM_DWARF      DBL_MIN       /* smallest nonzero number */
//#define LM_SQRT_DWARF sqrt(DBL_MIN) /* square should not underflow */
//#define LM_SQRT_GIANT sqrt(DBL_MAX) /* square should not overflow */
//#define LM_USERTOL    30*LM_MACHEP  /* users are recommended to require this */


#define LM_MACHEP     (1.2e-16)
#define	LM_DWARF      (1.0e-38)
#define	LM_SQRT_DWARF (3.834e-20)
#define	LM_SQRT_GIANT (1.304e+19)
#define	LM_USERTOL   (1.e-12)

/* If the above values do not work, the following seem good for an x86:
 LM_MACHEP     .555e-16
 LM_DWARF      9.9e-324 
 LM_SQRT_DWARF 1.e-160   
 LM_SQRT_GIANT 1.e150 
 LM_USERTOL   1.e-14
   The following values should work on any machine:
 LM_MACHEP     1.2e-16
 LM_DWARF      1.0e-38
 LM_SQRT_DWARF 3.834e-20
 LM_SQRT_GIANT 1.304e19
 LM_USERTOL   1.e-12
*/


/* Collection of input parameters for config. */
typedef struct {

    Float64_t ftol;      /* Relative error desired in the summarize of squares.
                         Termination occurs when both the actual and
                         predicted relative reductions in the summarize of squares
                         are at most ftol. */
	Float64_t xtol;      /* Relative error between last two approximations.
                         Termination occurs when the relative error between
                         two consecutive iterates is at most xtol. */
	Float64_t gtol;      /* Orthogonality desired between fvec and its derivs.
                         Termination occurs when the cosine of the angle
                         between fvec and any column of the Jacobian is at
                         most gtol in absolute value. */
	Float64_t epsilon;   /* Step used to calculate the Jacobian, should be
                         slightly larger than the relative error in the
                         user-supplied functions. */
	Float64_t stepbound; /* Used in determining the initial step bound. This
                         bound is set to the product of stepbound and the
                         Euclidean norm2 of diag*x if nonzero, or else to
                         stepbound itself. In most cases stepbound should lie
                         in the interval (0.1,100.0). Generally, the value
                         100.0 is recommended. */
    Int32_t patience;     /* Used to set the maximum number of function evaluations
                         to patience*(number_of_parameters+1). */
	Int32_t scale_diag;   /* If 1, the variables will be rescaled internally.
                         Recommended value is 1. */
    FILE* msgfile;    /* Progress messages will be written to this file. */

	Int32_t verbosity;    /* OR'ed: 1: print some messages; 2: print Jacobian. */
	Int32_t n_maxpri;     /* -1, or max number of parameters to print. */
	Int32_t m_maxpri;     /* -1, or max number of residuals to print. */

} levmarqt_config_struct ;


/* Collection of output parameters for status info. */
typedef struct {
	Float64_t fnorm;     /* norm2 of the residue vector fvec. */
	Int32_t nfev;         /* actual number of iterations. */
	Int32_t outcome;      /* Status indicator. Nonnegative values are used as index
                         for the message text levmarqt_infmsg, set in levmarqt.c. */
	Int32_t userbreak;    /* Set when function evaluation requests termination. */
} levmarqt_status_struct;

typedef enum
{
	LM_SUCCESS, // optimizer success
	LM_PARAMETERS_NUM_INVAD ,
	LM_PARAMETERS_NUM_SMALLER_THAN_POINT_NUM,
	LM_PARAMETERS_NEGATIVE_TOLERANCE,
	LM_PARAMETERS_NONPOSITIVE_FUNCTION_EVALUATION_LIMIT,
	LM_PARAMETERS_NONPOSITIVE_STEPBOUND,
	LM_PARAMETERS_VARIABLE_NONLOGICAL,
	LM_MEM_MALLOC_FAIL,
	LM_FOUND_ZERO,
	LM_CONVERGED_F,         // success: x almost stable
	LM_CONVERGED_P,         // success: summarize of squares almost stable
	LM_CONVERGED_BOTH,      // user-requested break 
	LM_DEGENERATE,          // LM algorithm degenerate to gradient desendent algorithm
	LM_CALL_LIMIT,          // max iteration time is arrived at, usually not converged 
	LM_FAILED_F,
	LM_FAILED_P,
	LM_FAILED_G,
	LM_PARAMETERS_ERROR,
	LM_STOP
}lM_OPTIMIZOR_STATUS_T;


/* Preset (and recommended) config parameter settings. */
extern levmarqt_config_struct levmarqt_config_double;
extern levmarqt_config_struct levmarqt_config_float;

/* Preset message texts. */
extern const char *levmarqt_infmsg[];
extern const char *levmarqt_shortmsg[];

bool levmarqt_minimizer_init(int m, int n);
void levmarqt_minimizer_release();

/* Levenberg-Marquardt */
lM_OPTIMIZOR_STATUS_T levmarqt_minimizer(Int32_t n_par, Float64_t *par, Int32_t m_dat, const void *data,
	void(*evaluate) (Float64_t *par, Int32_t m_dat, const void *data,
	Float64_t *fvec, Int32_t *userbreak),
											const levmarqt_config_struct *config, 
											levmarqt_status_struct *status);
/*
 *   This routine contains the core algorithm of our library.
 *
 *   It minimizes the summarize of the squares of m nonlinear functions
 *   in n variables by a modified Levenberg-Marquardt algorithm.
 *   The function evaluation is done by the user-provided routine 'evaluate'.
 *   The Jacobian is then calculated by a forward-difference approximation.
 *
 *   Parameters:
 *
 *      n is the number of variables (INPUT, positive integer).
 *
 *      x is the solution vector (INPUT/OUTPUT, array of length n).
 *        On input it must be set to an estimated solution.
 *        On output it yields the final estimate of the solution.
 *
 *      m is the number of functions to be minimized (INPUT, positive integer).
 *        It must fulfill m>=n.
 *
 *      data is a pointer that is ignored by levmarqt; it is however forwarded
 *        to the user-supplied functions evaluate and printout.
 *        In a typical application, it contains experimental data to be fitted.
 *
 *      evaluate is a user-supplied function that calculates the m functions.
 *        Parameters:
 *          n, x, m, data as above.
 *          fvec is an array of length m; on OUTPUT, it must contain the
 *            m function values for the parameter vector x.
 *          userbreak is an integer pointer. When *userbreak is set to a 
 *            nonzero value, levmarqt will terminate.
 *
 *      config contains INPUT variables that config the fit algorithm,
 *        as declared and explained in lmstruct.h
 *
 *      status contains OUTPUT variables that inform about the fit result,
 *        as declared and explained in lmstruct.h
 */

/* Refined calculation of Eucledian norm2. */
Float64_t levmarqdt_enorm(Int32_t, const Float64_t *);

/* function declarations (implemented below). */
void levmarqdt_lmpar(Int32_t n, Float64_t *r, Int32_t ldr, Int32_t *ipvt, Float64_t *diag,
	Float64_t *qtb, Float64_t delta, Float64_t *par, Float64_t *x,
	Float64_t *sdiag, Float64_t *aux, Float64_t *xdi);
void levmarqdt_qrfac(Int32_t m, Int32_t n, Float64_t *a, Int32_t *ipvt,
	Float64_t *rdiag, Float64_t *acnorm, Float64_t *wa);
void levmarqdt_qrsolv(Int32_t n, Float64_t *r, Int32_t ldr, Int32_t *ipvt, Float64_t *diag,
	Float64_t *qtb, Float64_t *x, Float64_t *sdiag, Float64_t *wa);


#undef SQR
#define SQR(x)   (x)*(x)


#endif 
