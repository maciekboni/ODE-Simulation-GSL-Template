#include <math.h>
#include <stdio.h>
#include <ctype.h>
#include <assert.h>
#include <stdlib.h>

#include <gsl/gsl_errno.h>
#include <gsl/gsl_matrix.h>
#include <gsl/gsl_odeiv.h>

#include <gsl/gsl_sf_gamma.h>

#include <gsl/gsl_vector.h>
#include <gsl/gsl_multimin.h>


#include <iostream>
#include <string>
#include <sstream>
#include <fstream>

#include<vector>

#define NUMAC 1

// number of equations
#define DIM 4


//#define HAVE_INLINE 	// this will make GSL vector operations faster

#ifndef ESSENTIALS
#define ESSENTIALS

using namespace std;

struct parameters
{
    // these are all the parameters in the dynamical system
    double  nu;
    double  beta;
    double  d;
    double  eta;
    double  N;
    double  b;  		// birth
    double  mu; 		// death
    double  fraction_observed;
    double  gamma; 		// this is the immunity loss process R -> S
    double  amp1;
    double  box1;
    double  phase1;
};
typedef struct parameters PARAMS;


//
//
// this function contains the ode system
//
int func(double t, const double y[], double f[], void *params);


//void* jac;	// do this for C-compilation
//
// for C++ compilation we are replacing the void* declaration above with
// the inline dummy declaration below
inline int jac(double a1, const double* a2, double* a3, double* a4, void* a5)
{
    return 0;	
};



//
// forcing function - r is the boxiness function
//
inline double forcing_w( double t_in_weeks, double phase, double amp, double r )
{
    double tol = 1e-14;
    double year_in_weeks = (52.0 + (1.0/7.0));
    t_in_weeks -= phase;
    if( t_in_weeks < 0.0 ) t_in_weeks += year_in_weeks;
    while( t_in_weeks > year_in_weeks ) t_in_weeks -= year_in_weeks;
    assert( t_in_weeks >= 0.0 && t_in_weeks <= year_in_weeks );
    
    double t = t_in_weeks / (52.0 + (1.0/7.0));  // t goes from 0 to 1
    double t_orig = t;
    
    if( t < 0.5 ) t = 4.0*(t - 0.25);
    else t = 4.0*(t - 0.75);
    // now t goes from -1 to 1
    
    double t_rescaled;
    if( r > 0.0 )
    {
        t_rescaled = tanh(r*t) / tanh(r);
        t_rescaled = (t_rescaled +1.0)/4.0;
        if( t_orig > 0.5 ) t_rescaled += 0.5;
// 	if( t_rescaled < 0.0 || t_rescaled > 1.0 )
// 	{
// 	    fprintf(stderr,"\n\tt_orig=%1.5e \ttanh/tanh=%1.5e \tt_rescaled=%1.5e ", t_orig, tanh(r*t) / tanh(r), t_rescaled );
// 	    fflush(stderr);
// 	}
    }
    else
    {
	t_rescaled = t_orig;    
    }
       
    assert( t_rescaled >= -tol && t_rescaled <= 1.0+tol );
    double ff = 1.0 + amp*cos( 2.0*3.1415926*(t_rescaled) );
    
    return ff;
};



#endif // ESSENTIALS

