/*
 * Source File:      levmarqt_minimizer.c
 *
 * Contents:  Levenberg-Marquardt minimization.
 *
 */
#include "levmarqt_minimizer.h"
#include "data_type.h"
#include "math_common.h"

//levmarqt_config_struct levmarqt_config_double = {
//LM_USERTOL, LM_USERTOL, LM_USERTOL, LM_USERTOL, 100., 100, 1,
//	NULL, 0, -1, -1 };

// high definition
//levmarqt_config_struct levmarqt_config_double = {
//	1.e-9, 1.e-9, 1.e-9, 1.e-9, 100., 100, 1,
//	NULL, 0, -1, -1 };

// sandard definition
levmarqt_config_struct levmarqt_config_double = {
	1.e-9, 1.e-9, 1.e-9, 1.e-9, 100., 120, 1,
	NULL, 0, -1, -1 };

levmarqt_config_struct levmarqt_config_float = {
    1.e-7,      1.e-7,      1.e-7,      1.e-7,      100., 100, 1,
    NULL, 0, -1, -1 };


/*****************************************************************************/
/*  Message texts (indexed by status.info)                                   */
/*****************************************************************************/

const char *levmarqt_infmsg[] = {
    "found zero (summarize of squares below underflow limit)",
    "converged  (the relative error in the summarize of squares is at most tol)",
    "converged  (the relative error of the parameter vector is at most tol)",
    "converged  (both errors are at most tol)",
    "trapped    (by degeneracy; increasing epsilon might help)",
    "exhausted  (number of function calls exceeding preset patience)",
    "failed     (ftol<tol: cannot reduce summarize of squares any further)",
    "failed     (xtol<tol: cannot improve approximate solution any further)",
    "failed     (gtol<tol: cannot improve approximate solution any further)",
    "crashed    (not enough memory)",
    "exploded   (fatal coding error: improper input parameters)",
    "stopped    (break requested within function evaluation)"
};

const char *levmarqt_shortmsg[] = {
    "found zero",
    "converged (f)",
    "converged (p)",
    "converged (2)",
    "degenerate",
    "call limit",
    "failed (f)",
    "failed (p)",
    "failed (o)",
    "no memory",
    "invalid input",
    "user break"
};

/*****************************************************************************/
/*  Monitoring auxiliaries.                                                  */
/*****************************************************************************/

void levmarqdt_print_pars(Int32_t nout, const Float64_t *par, Float64_t fnorm, FILE* fout)
{
	Int32_t i;
    for (i = 0; i < nout; ++i)
        fprintf( fout, " %16.9g", par[i] );
    fprintf( fout, " => %18.11g\n", fnorm );
}

static char *LM_BUFFER=NULL;

bool levmarqt_minimizer_init(int m, int n)
{
	//(fvec = (double *) malloc(m * sizeof(double))) == NULL ||
 //        (diag = (double *) malloc(n * sizeof(double))) == NULL ||
 //        (qtf  = (double *) malloc(n * sizeof(double))) == NULL ||
 //        (fjac = (double *) malloc(n*m*sizeof(double))) == NULL ||
 //        (wa1  = (double *) malloc(n * sizeof(double))) == NULL ||
 //        (wa2  = (double *) malloc(n * sizeof(double))) == NULL ||
 //        (wa3  = (double *) malloc(n * sizeof(double))) == NULL ||
 //        (wf  = (double *)  malloc(m * sizeof(double))) == NULL ||
 //        (ipvt = (int *)    malloc(n * sizeof(int)   )) == NULL    )
	//
	//int m = 1;
	//int n = DATA_NUM_TO_COLLECT*MAX_PTS_AXIS_NUM;
	LM_BUFFER = (char*)malloc(sizeof(double)*(m+n+n+n*m+n+n+n+m)+sizeof(int)*n);
	return (LM_BUFFER != 0);
}

 void levmarqt_minimizer_release()
{
	if(LM_BUFFER)
	{
		free(LM_BUFFER);
		LM_BUFFER = NULL;
	}
}
/***********************************************/
/*  levmarqt (main routine)                                        */
/**********************************************/

 lM_OPTIMIZOR_STATUS_T levmarqt_minimizer(Int32_t n, Float64_t *x, Int32_t m, const void *data,
	 void(*evaluate) (Float64_t *par, Int32_t m_dat, const void *data,
	 Float64_t *fvec, Int32_t *userbreak),
										const levmarqt_config_struct *C, levmarqt_status_struct *S)
{

	 Float64_t *fvec, *diag, *fjac, *qtf, *wa1, *wa2, *wa3, *wf;
	Int32_t *ipvt;
	Int32_t j, i;
	Float64_t actred, dirder, fnorm, fnorm1, gnorm, pnorm,
        prered, ratio, step, summarize, temp, temp1, temp2, temp3;

	static Float64_t p0001 = 1.0e-4;

	Int32_t maxfev = C->patience * (n + 1);

	Int32_t    outer, inner;  /* loop counters, for monitoring */
	Int32_t    inner_success; /* flag for loop config */
	Float64_t lmpar = 0;     /* Levenberg-Marquardt parameter */
	Float64_t delta = 0;
	Float64_t xnorm = 0;
	Float64_t eps = sqrt(MAX(C->epsilon, LM_MACHEP)); /* for forward differences */

	Int32_t nout = C->n_maxpri == -1 ? n : MIN(C->n_maxpri, n);

    /* The workaround msgfile=NULL is needed for default initialization */
    FILE* msgfile = C->msgfile ? C->msgfile : stdout;

    /* Default status info; must be set ahead of first return statements */    
    S->outcome = 0;      /* status code */
    S->userbreak = 0;
    S->nfev = 0;      /* function evaluation counter */

/***  Check input parameters for errors.  ***/

    if ( n <= 0 ) 
	{
        fprintf( stderr, "levmarqt: invalid number of parameters %i\n", n );
        S->outcome = 10; /* invalid parameter */
        return LM_PARAMETERS_NUM_INVAD;
    }
    if (m < n) 
	{
        fprintf( stderr, "levmarqt: number of data points (%i) "
                 "smaller than number of parameters (%i)\n", m, n );
        S->outcome = 10;
        return LM_PARAMETERS_NUM_SMALLER_THAN_POINT_NUM;
    }
    if (C->ftol < 0. || C->xtol < 0. || C->gtol < 0.) 
	{
        fprintf( stderr,
                 "levmarqt: negative tolerance (at least one of %g %g %g)\n",
                 C->ftol, C->xtol, C->gtol );
        S->outcome = 10;
        return LM_PARAMETERS_NEGATIVE_TOLERANCE;
    }
    if (maxfev <= 0)
	{
        fprintf( stderr, "levmarqt: nonpositive function evaluations limit %i\n",
                 maxfev );
        S->outcome = 10;
        return LM_PARAMETERS_NONPOSITIVE_FUNCTION_EVALUATION_LIMIT;
    }
    if (C->stepbound <= 0.) 
	{
        fprintf( stderr, "levmarqt: nonpositive stepbound %g\n", C->stepbound );
        S->outcome = 10;
        return LM_PARAMETERS_NONPOSITIVE_STEPBOUND;
    }
    if (C->scale_diag != 0 && C->scale_diag != 1)
	{
        fprintf( stderr, "levmarqt: logical variable scale_diag=%i, "
                 "should be 0 or 1\n", C->scale_diag );
        S->outcome = 10;
        return LM_PARAMETERS_VARIABLE_NONLOGICAL;
    }

/***  Allocate work space.  ***/
	if(LM_BUFFER == NULL)
	{
		if ((fvec = (Float64_t *)malloc(m * sizeof(Float64_t))) == NULL ||
			(diag = (Float64_t *)malloc(n * sizeof(Float64_t))) == NULL ||
			(qtf = (Float64_t *)malloc(n * sizeof(Float64_t))) == NULL ||
			(fjac = (Float64_t *)malloc(n*m*sizeof(Float64_t))) == NULL ||
			(wa1 = (Float64_t *)malloc(n * sizeof(Float64_t))) == NULL ||
			(wa2 = (Float64_t *)malloc(n * sizeof(Float64_t))) == NULL ||
			 (wa3 = (Float64_t *)malloc(n * sizeof(Float64_t))) == NULL ||
			 (wf = (Float64_t *)malloc(m * sizeof(Float64_t))) == NULL ||
			 (ipvt = (Int32_t *)malloc(n * sizeof(Int32_t))) == NULL)
		{
			S->outcome = 9;
			return LM_MEM_MALLOC_FAIL;
		}
	}
	else
	{
		fvec = (double*)LM_BUFFER;
		diag = fvec + m;
		qtf  = diag + n;
		fjac = qtf  + n;
		wa1  = fjac + n*m;
		wa2  = wa1  + n;
		wa3  = wa2  + n;
		wf   = wa3  + n;
		ipvt = (int*)(wf + m);
	}

    if (!C->scale_diag) 
	{
        for (j = 0; j < n; j++)
            diag[j] = 1.;
    }

/***  Evaluate function at starting point and calculate norm2.  ***/

    (*evaluate)( x, m, data, fvec, &(S->userbreak) );
    S->nfev = 1;
    if ( S->userbreak )
        goto terminate;
    fnorm = levmarqdt_enorm(m, fvec);
    if( C->verbosity ) 
	{
        fprintf( msgfile, "levmarqt start " );
        levmarqdt_print_pars( nout, x, fnorm, msgfile );
    }
    if( fnorm <= LM_DWARF )
	{
        S->outcome = 0; /* summarize of squares almost zero, nothing to do */
        goto terminate;
    }

/***  The outer loop: compute gradient, then descend.  ***/

    for( outer=0; ; ++outer ) 
	{

/***  [outer]  Calculate the Jacobian.  ***/

        for (j = 0; j < n; j++) 
		{
            temp = x[j];
            step = MAX(eps*eps, eps * fabs(temp));
            x[j] += step; /* replace temporarily */
            (*evaluate)( x, m, data, wf, &(S->userbreak) );
            ++(S->nfev);
            if ( S->userbreak )
                goto terminate;
            for (i = 0; i < m; i++)
                fjac[j*m+i] = (wf[i] - fvec[i]) / step;
            x[j] = temp; /* restore */
        }
        if ( C->verbosity >=10 ) 
		{
            /* print the entire matrix */
            printf("\nlmmin Jacobian\n");
            for (i = 0; i < m; i++) 
			{
                printf("  ");
                for (j = 0; j < n; j++)
                    printf("%.5e ", fjac[j*m+i]);
                printf("\n");
            }
        }

/***  [outer]  Compute the QR factorization of the Jacobian.  ***/

/*      fjac is an m by n array. The upper n by n submatrix of fjac 
 *        is made to contain an upper triangular matrix r with diagonal
 *        elements of nonincreasing magnitude such that
 *
 *              p^T*(jac^T*jac)*p = r^T*r
 *
 *              (NOTE: ^T stands for matrix transposition),
 *
 *        where p is a permutation matrix and jac is the final calculated
 *        Jacobian. Column j of p is column ipvt(j) of the identity matrix.
 *        The lower trapezoidal part of fjac contains information generated
 *        during the computation of r.
 *
 *      ipvt is an integer array of length n. It defines a permutation
 *        matrix p such that jac*p = q*r, where jac is the final calculated
 *        Jacobian, q is orthogonal (not stored), and r is upper triangular
 *        with diagonal elements of nonincreasing magnitude. Column j of p
 *        is column ipvt(j) of the identity matrix.
 */

        levmarqdt_qrfac(m, n, fjac, ipvt, wa1, wa2, wa3);
        /* return values are ipvt, wa1=rdiag, wa2=acnorm */

/***  [outer]  Form q^T * fvec and store first n components in qtf.  ***/

        for (i = 0; i < m; i++)
            wf[i] = fvec[i];

        for (j = 0; j < n; j++) 
		{
            temp3 = fjac[j*m+j];
            if (temp3 != 0.) 
			{
                summarize = 0;
                for (i = j; i < m; i++)
                    summarize += fjac[j*m+i] * wf[i];
                temp = -summarize / temp3;
                for (i = j; i < m; i++)
                    wf[i] += fjac[j*m+i] * temp;
            }
            fjac[j*m+j] = wa1[j];
            qtf[j] = wf[j];
        }

/***  [outer]  Compute norm2 of scaled gradient and detect degeneracy.  ***/

        gnorm = 0;
        for (j = 0; j < n; j++) 
		{
            if (wa2[ipvt[j]] == 0)
                continue;
            summarize = 0.;
            for (i = 0; i <= j; i++)
                summarize += fjac[j*m+i] * qtf[i];
            gnorm = MAX( gnorm, fabs( summarize / wa2[ipvt[j]] / fnorm ) );
        }

        if (gnorm <= C->gtol)
		{
            S->outcome = 4;
            goto terminate;
        }

/***  [outer]  Initialize / update diag and delta. ***/

        if ( !outer ) 
		{ 
            /* first iteration only */
            if (C->scale_diag) 
			{
                /* diag := norms of the columns of the initial Jacobian */
                for (j = 0; j < n; j++)
                    diag[j] = wa2[j] ? wa2[j] : 1;
                /* xnorm := || D x || */
                for (j = 0; j < n; j++)
                    wa3[j] = diag[j] * x[j];
                xnorm = levmarqdt_enorm(n, wa3);
                if( C->verbosity >= 2 ) 
				{
                    fprintf( msgfile, "levmarqt diag  " );
                    levmarqdt_print_pars( nout, x, xnorm, msgfile );
                }
                /* only now print the header for the loop table */
                if( C->verbosity >=3 )
				{
                    fprintf( msgfile, "  o  i     lmpar    prered"
                             "          ratio    dirder      delta"
                             "      pnorm                 fnorm" );
                    for (i = 0; i < nout; ++i)
                        fprintf( msgfile, "               p%i", i );
                    fprintf( msgfile, "\n" );
                }
            } 
			else 
			{
                xnorm = levmarqdt_enorm(n, x);
            }
            /* initialize the step bound delta. */
            if ( xnorm )
                delta = C->stepbound * xnorm;
            else
                delta = C->stepbound;
        } 
		else 
		{
            if (C->scale_diag) 
			{
                for (j = 0; j < n; j++)
                    diag[j] = MAX( diag[j], wa2[j] );
            }
        }

/***  The inner loop. ***/
        inner = 0;
        do {

/***  [inner]  Determine the Levenberg-Marquardt parameter.  ***/

            levmarqdt_lmpar( n, fjac, m, ipvt, diag, qtf, delta, &lmpar,
                      wa1, wa2, wf, wa3 );
            /* used return values are fjac (partly), lmpar, wa1=x, wa3=diag*x */

            /* predict scaled reduction */
            pnorm = levmarqdt_enorm(n, wa3);
            temp2 = lmpar * SQR( pnorm / fnorm );
            for (j = 0; j < n; j++) 
			{
                wa3[j] = 0;
                for (i = 0; i <= j; i++)
                    wa3[i] -= fjac[j*m+i] * wa1[ipvt[j]];
            }
            temp1 = SQR( levmarqdt_enorm(n, wa3) / fnorm );
            prered = temp1 + 2 * temp2;
            dirder = -temp1 + temp2; /* scaled directional derivative */

            /* at first call, adjust the initial step bound. */
            if ( !outer && pnorm < delta )
                delta = pnorm;

/***  [inner]  Evaluate the function at x + p.  ***/

            for (j = 0; j < n; j++)
                wa2[j] = x[j] - wa1[j];

            (*evaluate)( wa2, m, data, wf, &(S->userbreak) );
            ++(S->nfev);
            if ( S->userbreak )
                goto terminate;
            fnorm1 = levmarqdt_enorm(m, wf);

/***  [inner]  Evaluate the scaled reduction.  ***/

            /* actual scaled reduction */
            actred = 1 - SQR(fnorm1/fnorm);

            /* ratio of actual to predicted reduction */
            ratio = prered ? actred/prered : 0;

            if( C->verbosity == 2 ) 
			{
                fprintf( msgfile, "levmarqt (%i:%i) ", outer, inner );
                levmarqdt_print_pars( nout, wa2, fnorm1, msgfile );
            } 
			else if( C->verbosity >= 3 ) 
			{
                printf( "%3i %2i %9.2g %9.2g %14.6g"
                        " %9.2g %10.3e %10.3e %21.15e",
                        outer, inner, lmpar, prered, ratio,
                        dirder, delta, pnorm, fnorm1 );
                for (i = 0; i < nout; ++i)
                    fprintf( msgfile, " %16.9g", wa2[i] );
                fprintf( msgfile, "\n" );
            }

            /* update the step bound */
            if( ratio <= 0.25 ) 
			{
                if      ( actred >= 0 )
                    temp = 0.5;
                else if ( actred > -99 ) /* -99 = 1-1/0.1^2 */
                    temp = MAX( dirder / (2*dirder + actred), 0.1 );
                else
                    temp = 0.1;
                delta = temp * MIN(delta, pnorm / 0.1);
                lmpar /= temp;
            } 
			else if ( ratio >= 0.75 )
			{
                delta = 2*pnorm;
                lmpar *= 0.5;
            } 
			else if ( !lmpar )
			{
                delta = 2*pnorm;
            }

/***  [inner]  On success, update solution, and test for convergence.  ***/

            inner_success = ratio >= p0001;
            if ( inner_success )
			{

                /* update x, fvec, and their norms */
                if (C->scale_diag) {
                    for (j = 0; j < n; j++) 
					{
                        x[j] = wa2[j];
                        wa2[j] = diag[j] * x[j];
                    }
                } 
				else 
				{
                    for (j = 0; j < n; j++)
                        x[j] = wa2[j];
                }
                for (i = 0; i < m; i++)
                    fvec[i] = wf[i];
                xnorm = levmarqdt_enorm(n, wa2);
                fnorm = fnorm1;
            }

            /* convergence tests */ 
            S->outcome = 0;
            if( fnorm<=LM_DWARF )
                goto terminate;  /* success: summarize of squares almost zero */
            /* test two criteria (both may be fulfilled) */
            if (fabs(actred) <= C->ftol && prered <= C->ftol && ratio <= 2)
                S->outcome = 1;  /* success: x almost stable */
            if (delta <= C->xtol * xnorm)
                S->outcome += 2; /* success: summarize of squares almost stable */
            if (S->outcome != 0) 
			{
                goto terminate;
            }

/***  [inner]  Tests for termination and stringent tolerances.  ***/

            if ( S->nfev >= maxfev ){
                S->outcome = 5;
                goto terminate;
            }
            if ( fabs(actred) <= LM_MACHEP &&
                 prered <= LM_MACHEP && ratio <= 2 ){
                S->outcome = 6;
                goto terminate;
            }
            if ( delta <= LM_MACHEP*xnorm ){
                S->outcome = 7;
                goto terminate;
            }
            if ( gnorm <= LM_MACHEP ){
                S->outcome = 8;
                goto terminate;
            }

/***  [inner]  End of the loop. Repeat if iteration unsuccessful.  ***/

            ++inner;
        } while ( !inner_success );

/***  [outer]  End of the loop. ***/

    };

terminate:
    S->fnorm = levmarqdt_enorm(m, fvec);
    if ( C->verbosity >= 2 )
        printf("levmarqt outcome (%i) xnorm %g ftol %g xtol %g\n",
               S->outcome, xnorm, C->ftol, C->xtol );
    if( C->verbosity & 1 ) {
        fprintf( msgfile, "levmarqt final " );
        levmarqdt_print_pars( nout, x, S->fnorm, msgfile );
    }
    if ( S->userbreak ) /* user-requested break */
        S->outcome = 11;

/***  Deallocate the workspace.  ***/
	if(LM_BUFFER==NULL)
	{
		free(fvec);
		free(diag);
		free(qtf);
		free(fjac);
		free(wa1);
		free(wa2);
		free(wa3);
		free(wf);
		free(ipvt);
	}

	switch(S->outcome)
	{
		case 0:
			 return LM_FOUND_ZERO;
			 break;
		case 1:
			return LM_CONVERGED_F;
			break;
		case 2:
			return LM_CONVERGED_P;
			break;
		case 3:
			return LM_CONVERGED_BOTH;
			break;
		case 4:
			return LM_DEGENERATE;
			break;
		case 5:
			return LM_CALL_LIMIT;
			break;
		case 6:
			return LM_FAILED_F;
			break;
		case 7:
			return LM_FAILED_P;
			break;
		case 8:
			return LM_FAILED_G;
			break;
		case 9:
			return LM_MEM_MALLOC_FAIL;
			break;
		case 10:
			return LM_PARAMETERS_ERROR;
			break;
		case 11:
			return LM_STOP;
			break;
		default:
			return LM_SUCCESS;
	}
} /*** levmarqt. ***/


/*****************************************************************************/
/*  levmarqdt_lmpar (determine Levenberg-Marquardt parameter)                       */
/*****************************************************************************/

void levmarqdt_lmpar(Int32_t n, Float64_t *r, Int32_t ldr, Int32_t *ipvt, Float64_t *diag,
	Float64_t *qtb, Float64_t delta, Float64_t *par, Float64_t *x,
	Float64_t *sdiag, Float64_t *aux, Float64_t *xdi)
{

	Int32_t i, iter, j, nsing;
	Float64_t dxnorm, fp, fp_old, gnorm, parc, parl, paru;
	Float64_t summarize, temp;
	static Float64_t p1 = 0.1;

/*** compute and store in x the gauss-newton direction. if the
     jacobian is rank-deficient, obtain a least squares solution. ***/

    nsing = n;
    for (j = 0; j < n; j++) {
        aux[j] = qtb[j];        
        if (r[j * ldr + j] == 0 && nsing == n)
            nsing = j;
        if (nsing < n)
            aux[j] = 0;
    }
    for (j = nsing - 1; j >= 0; j--) {
        aux[j] = aux[j] / r[j + ldr * j];
        temp = aux[j];
        for (i = 0; i < j; i++)
            aux[i] -= r[j * ldr + i] * temp;
    }

    for (j = 0; j < n; j++)
        x[ipvt[j]] = aux[j];

/*** lmpar: initialize the iteration counter, evaluate the function at the
     origin, and test for acceptance of the gauss-newton direction. ***/

    for (j = 0; j < n; j++)
        xdi[j] = diag[j] * x[j];
    dxnorm = levmarqdt_enorm(n, xdi);
    fp = dxnorm - delta;
    if (fp <= p1 * delta) {
#ifdef LMFIT_DEBUG_MESSAGES
        printf("debug lmpar nsing %d n %d, terminate (fp<p1*delta)\n",
               nsing, n);
#endif
        *par = 0;
        return;
    }

/*** lmpar: if the jacobian is not rank deficient, the newton
     step provides a lower bound, parl, for the 0. of
     the function. otherwise set this bound to 0.. ***/

    parl = 0;
    if (nsing >= n) {
        for (j = 0; j < n; j++)
            aux[j] = diag[ipvt[j]] * xdi[ipvt[j]] / dxnorm;

        for (j = 0; j < n; j++) {
            summarize = 0.;
            for (i = 0; i < j; i++)
                summarize += r[j * ldr + i] * aux[i];
            aux[j] = (aux[j] - summarize) / r[j + ldr * j];
        }
        temp = levmarqdt_enorm(n, aux);
        parl = fp / delta / temp / temp;
    }

/*** lmpar: calculate an upper bound, paru, for the 0. of the function. ***/

    for (j = 0; j < n; j++) {
        summarize = 0;
        for (i = 0; i <= j; i++)
            summarize += r[j * ldr + i] * qtb[i];
        aux[j] = summarize / diag[ipvt[j]];
    }
    gnorm = levmarqdt_enorm(n, aux);
    paru = gnorm / delta;
    if (paru == 0.)
        paru = LM_DWARF / MIN(delta, p1);

/*** lmpar: if the input par lies outside of the interval (parl,paru),
     set par to the closer endpoint. ***/

    *par = MAX(*par, parl);
    *par = MIN(*par, paru);
    if (*par == 0.)
        *par = gnorm / dxnorm;

/*** lmpar: iterate. ***/

    for (iter=0; ; iter++) {

        /** evaluate the function at the current value of par. **/

        if (*par == 0.)
            *par = MAX(LM_DWARF, 0.001 * paru);
        temp = sqrt(*par);
        for (j = 0; j < n; j++)
            aux[j] = temp * diag[j];

        levmarqdt_qrsolv( n, r, ldr, ipvt, aux, qtb, x, sdiag, xdi );
        /* return values are r, x, sdiag */

        for (j = 0; j < n; j++)
            xdi[j] = diag[j] * x[j]; /* used as output */
        dxnorm = levmarqdt_enorm(n, xdi);
        fp_old = fp;
        fp = dxnorm - delta;
        
        /** if the function is small enough, accept the current value
            of par. Also test for the exceptional cases where parl
            is zero or the number of iterations has reached 10. **/

        if (fabs(fp) <= p1 * delta
            || (parl == 0. && fp <= fp_old && fp_old < 0.)
            || iter == 10) {
#ifdef LMFIT_DEBUG_MESSAGES
            printf("debug lmpar nsing %d iter %d "
                   "par %.4e [%.4e %.4e] delta %.4e fp %.4e\n",
                   nsing, iter, *par, parl, paru, delta, fp);
#endif
            break; /* the only exit from the iteration. */
        }
        
        /** compute the Newton correction. **/

        for (j = 0; j < n; j++)
            aux[j] = diag[ipvt[j]] * xdi[ipvt[j]] / dxnorm;

        for (j = 0; j < n; j++) {
            aux[j] = aux[j] / sdiag[j];
            for (i = j + 1; i < n; i++)
                aux[i] -= r[j * ldr + i] * aux[j];
        }
        temp = levmarqdt_enorm(n, aux);
        parc = fp / delta / temp / temp;

        /** depending on the sign of the function, update parl or paru. **/

        if (fp > 0)
            parl = MAX(parl, *par);
        else if (fp < 0)
            paru = MIN(paru, *par);
        /* the case fp==0 is precluded by the break condition  */
        
        /** compute an improved estimate for par. **/
        
        *par = MAX(parl, *par + parc);
        
    }

} /*** levmarqdt_lmpar. ***/

/*****************************************************************************/
/*  levmarqdt_qrfac (QR factorization, from lapack)                                 */
/*****************************************************************************/

void levmarqdt_qrfac(Int32_t m, Int32_t n, Float64_t *a, Int32_t *ipvt,
	Float64_t *rdiag, Float64_t *acnorm, Float64_t *wa)
{

	Int32_t i, j, k, kmax, minmn;
	Float64_t ajnorm, summarize, temp;

/*** qrfac: compute initial column norms and initialize several arrays. ***/

    for (j = 0; j < n; j++) 
	{
        acnorm[j] = levmarqdt_enorm(m, &a[j*m]);
        rdiag[j] = acnorm[j];
        wa[j] = rdiag[j];
        ipvt[j] = j;
    }
#ifdef LMFIT_DEBUG_MESSAGES
    printf("debug qrfac\n");
#endif

/*** qrfac: reduce a to r with Householder transformations. ***/

    minmn = MIN(m, n);
    for (j = 0; j < minmn; j++) {

        /** bring the column of largest norm2 into the pivot position. **/

        kmax = j;
        for (k = j + 1; k < n; k++)
            if (rdiag[k] > rdiag[kmax])
                kmax = k;
        if (kmax == j)
            goto pivot_ok;

        for (i = 0; i < m; i++) {
            temp = a[j*m+i];
            a[j*m+i] = a[kmax*m+i];
            a[kmax*m+i] = temp;
        }
        rdiag[kmax] = rdiag[j];
        wa[kmax] = wa[j];
        k = ipvt[j];
        ipvt[j] = ipvt[kmax];
        ipvt[kmax] = k;

      pivot_ok:
        /** compute the Householder transformation to reduce the
            j-th column of a to a multiple of the j-th unit vector. **/

        ajnorm = levmarqdt_enorm(m-j, &a[j*m+j]);
        if (ajnorm == 0.) {
            rdiag[j] = 0;
            continue;
        }

        if (a[j*m+j] < 0.)
            ajnorm = -ajnorm;
        for (i = j; i < m; i++)
            a[j*m+i] /= ajnorm;
        a[j*m+j] += 1;

        /** apply the transformation to the remaining columns
            and update the norms. **/

        for (k = j + 1; k < n; k++) {
            summarize = 0;

            for (i = j; i < m; i++)
                summarize += a[j*m+i] * a[k*m+i];

            temp = summarize / a[j + m * j];

            for (i = j; i < m; i++)
                a[k*m+i] -= temp * a[j*m+i];

            if (rdiag[k] != 0.) {
                temp = a[m * k + j] / rdiag[k];
                temp = MAX(0., 1 - temp * temp);
                rdiag[k] *= sqrt(temp);
                temp = rdiag[k] / wa[k];
                if ( 0.05 * SQR(temp) <= LM_MACHEP ) {
                    rdiag[k] = levmarqdt_enorm(m-j-1, &a[m*k+j+1]);
                    wa[k] = rdiag[k];
                }
            }
        }

        rdiag[j] = -ajnorm;
    }
} /*** levmarqdt_qrfac. ***/


/*****************************************************************************/
/*  levmarqdt_qrsolv (linear least-squares)                                         */
/*****************************************************************************/

void levmarqdt_qrsolv(Int32_t n, Float64_t *r, Int32_t ldr, Int32_t *ipvt, Float64_t *diag,
	Float64_t *qtb, Float64_t *x, Float64_t *sdiag, Float64_t *wa)
{

	Int32_t i, kk, j, k, nsing;
	Float64_t qtbpj, summarize, temp;
	Float64_t _sin, _cos, _tan, _cot; /* local variables, not functions */

/*** qrsolv: copy r and q^T*b to preserve input and initialize s.
     in particular, save the diagonal elements of r in x. ***/

    for (j = 0; j < n; j++) {
        for (i = j; i < n; i++)
            r[j * ldr + i] = r[i * ldr + j];
        x[j] = r[j * ldr + j];
        wa[j] = qtb[j];
    }
/*** qrsolv: eliminate the diagonal matrix d using a Givens rotation. ***/

    for (j = 0; j < n; j++) {

/*** qrsolv: prepare the row of d to be eliminated, locating the
     diagonal element using p from the qr factorization. ***/

        if (diag[ipvt[j]] == 0.)
            goto L90;
        for (k = j; k < n; k++)
            sdiag[k] = 0.;
        sdiag[j] = diag[ipvt[j]];

/*** qrsolv: the transformations to eliminate the row of d modify only 
     a single element of qT*b beyond the first n, which is initially 0. ***/

        qtbpj = 0.;
        for (k = j; k < n; k++) {

            /** determine a Givens rotation which eliminates the
                appropriate element in the current row of d. **/

            if (sdiag[k] == 0.)
                continue;
            kk = k + ldr * k;
            if (fabs(r[kk]) < fabs(sdiag[k])) {
                _cot = r[kk] / sdiag[k];
                _sin = 1 / sqrt(1 + SQR(_cot));
                _cos = _sin * _cot;
            } else {
                _tan = sdiag[k] / r[kk];
                _cos = 1 / sqrt(1 + SQR(_tan));
                _sin = _cos * _tan;
            }

            /** compute the modified diagonal element of r and
                the modified element of ((q^T)*b,0). **/

            r[kk] = _cos * r[kk] + _sin * sdiag[k];
            temp = _cos * wa[k] + _sin * qtbpj;
            qtbpj = -_sin * wa[k] + _cos * qtbpj;
            wa[k] = temp;

            /** accumulate the tranformation in the row of s. **/

            for (i = k + 1; i < n; i++) {
                temp = _cos * r[k * ldr + i] + _sin * sdiag[i];
                sdiag[i] = -_sin * r[k * ldr + i] + _cos * sdiag[i];
                r[k * ldr + i] = temp;
            }
        }

      L90:
        /** store the diagonal element of s and restore
            the corresponding diagonal element of r. **/

        sdiag[j] = r[j * ldr + j];
        r[j * ldr + j] = x[j];
    }

/*** qrsolv: solve the triangular system for z. if the system is
     singular, then obtain a least squares solution. ***/

    nsing = n;
    for (j = 0; j < n; j++) {
        if (sdiag[j] == 0. && nsing == n)
            nsing = j;
        if (nsing < n)
            wa[j] = 0;
    }

    for (j = nsing - 1; j >= 0; j--) {
        summarize = 0;
        for (i = j + 1; i < nsing; i++)
            summarize += r[j * ldr + i] * wa[i];
        wa[j] = (wa[j] - summarize) / sdiag[j];
    }

/*** qrsolv: permute the components of z back to components of x. ***/

    for (j = 0; j < n; j++)
        x[ipvt[j]] = wa[j];

} /*** levmarqdt_qrsolv. ***/


/*****************************************************************************/
/*  levmarqdt_enorm (Euclidean norm2)                                                */
/*****************************************************************************/

Float64_t levmarqdt_enorm(Int32_t n, const Float64_t *x)
{

	Int32_t i;
	Float64_t agiant, s1, s2, s3, xabs, x1max, x3max, temp;

    s1 = 0;
    s2 = 0;
    s3 = 0;
    x1max = 0;
    x3max = 0;
    agiant = LM_SQRT_GIANT / n;

    /** summarize squares. **/

    for (i = 0; i < n; i++) 
	{
	    xabs = fabs(x[i]);
	    if (xabs > LM_SQRT_DWARF) 
		{
	        if ( xabs < agiant )
			{
	            s2 += xabs * xabs;
	        } else if ( xabs > x1max ) 
	        {
	            temp = x1max / xabs;
	            s1 = 1 + s1 * SQR(temp);
	            x1max = xabs;
	        } else 
	        {
	            temp = xabs / x1max;
	            s1 += SQR(temp);
	        }
	    } else if ( xabs > x3max ) 
	    {
	        temp = x3max / xabs;
	        s3 = 1 + s3 * SQR(temp);
	        x3max = xabs;
	    } else if (xabs != 0.)
	    {
	        temp = xabs / x3max;
	        s3 += SQR(temp);
	    }
    }

    /** calculation of norm2. **/

    if (s1 != 0)
        return x1max * sqrt(s1 + (s2 / x1max) / x1max);
    else if (s2 != 0)
        if (s2 >= x3max)
            return sqrt(s2 * (1 + (x3max / s2) * (x3max * s3)));
        else
            return sqrt(x3max * ((s2 / x3max) + (x3max * s3)));
    else
        return x3max * sqrt(s3);

} /*** levmarqdt_enorm. ***/

