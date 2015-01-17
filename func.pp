#include "essentials.h"

#define PI 3.141592653589793238

//
// i think "func" must be defined with these four arguments for it to work in
// in the GSL ODE routines
//
int func(double t, const double y[], double f[], void *params)
{
    PARAMS* pp = (PARAMS*) params;
    
    // forced beta is just a shortcut
    double tw = t / 7.0;
    double forced_beta = pp->beta * forcing_w( tw, pp->phase1/7.0, pp->amp1, pp->box1 );    

    // basic SIR system with birth, death, and immune-loss
    f[0] = pp->b - ( forced_beta * y[0] * y[1] / pp->N ) - pp->mu * y[0] + pp->gamma * y[2];
    f[1] = forced_beta * y[0] * y[1] / pp->N  - ( pp->mu + pp->nu )* y[1];
    f[2] = pp->nu * y[1] - ( pp->mu + pp->gamma ) * y[2];

    // this is the CUMULATIVE incidence variable
    f[3] = forced_beta * y[0] * y[1] / pp->N; 
    
    //fprintf( stderr,"\t\tpp->amp1 = %1.3f \t P.amp1 = %1.3f\n", pp->amp1, P.amp1 ); fflush(stderr);
    
    return GSL_SUCCESS;
}

