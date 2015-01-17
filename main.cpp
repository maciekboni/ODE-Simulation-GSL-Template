#include "essentials.h"
#include "predict.h"

void ParseArgs(int argc, char **argv);

double G_CLO_BETA = 0.6;
double G_CLO_TF = 300.0;
double G_CLO_RHO = .0001;


int main(int argc, char* argv[])
{
    ParseArgs(argc,argv);
    
    int ac,k;
    const gsl_odeiv_step_type * T 	= gsl_odeiv_step_rkf45;
    gsl_odeiv_step * s 			= gsl_odeiv_step_alloc (T, DIM);
    gsl_odeiv_control * c 		= gsl_odeiv_control_y_new (1e-6, 0.0);
    gsl_odeiv_evolve * e 		= gsl_odeiv_evolve_alloc (DIM);

    // start time and end time
    double t0=0.0; double t1=1.0;
    
    // allocate space for array of state variables
    // double* y0 = malloc( DIM * sizeof(double) );
    double* y0 = new double[DIM];

    // allocate space for incidence variable which you will also need
    double* incidence = new double[NUMAC];
   
    // allocate space for parameter structure
    PARAMS* pp=NULL; 
    pp = new PARAMS;
 
    assert( pp );
    assert( NUMAC==1 );

    // initialize parameters
    /*for(ac=0; ac<NUMAC; ac++)
	for(k=0; k<NUMAC; k++)
	    pp->z[ac*NUMAC+k] = pContactMatrix[ac*NUMAC+k];    
    for(ac=0; ac<NUMAC; ac++) pp->e[ac] = 0.2;*/
    pp->nu = 0.2; pp->beta = G_CLO_BETA; pp->d = 0.0; pp->eta = 0.1; 
    pp->N = 8000000.0; pp->mu = 1.0 / (365.0*70.0); pp->b = 400; pp->gamma = 1.0 / 1000.0;
    pp->fraction_observed = G_CLO_RHO;
    pp->box1   = 0.0;
    pp->amp1   = 0.0;
    pp->phase1 = 0.0;
    
    // set initial conditions for ODE system
    assert( DIM==4 );
    y0[0] = 4000000.0;
    y0[1] = 5000.0;
    y0[2] = 4000000.0;
    y0[3] = 0.0;


    while( t0 < G_CLO_TF )
    {
	printf("%1.1f \t %1.4f \t %1.4f \t %1.4f \n", t0, y0[0], y0[1], y0[2] );
	       
        predict( t0, t1, y0, pp, incidence, s, c, e );	
	
	t0+=1.0; t1+=1.0;
    }
    
    
    
    // free memory
    gsl_odeiv_evolve_free (e);
    gsl_odeiv_control_free (c);
    gsl_odeiv_step_free (s);
    delete[] y0;
    delete[] incidence;
    delete pp;
    return 0;
}



// parses command line arguments
void ParseArgs(int argc, char **argv)
{
    int i, start;
    start=1;

    /*if( argc<start )
    { 
        PrintUsageModes(); 
        exit(-1);
    }
        
    if( argv[1][0]=='n' && argv[1][1]=='o' && argv[1][2]=='n' && argv[1][3]=='e' && argv[1][4]==0 )
    {
        //fprintf(stderr, "\n\n\tnot printing to Outfile\n\n");
    }
    else 
    {
        Outfile = fopen( argv[1], "w" );
    }
    
    prm_intro_day = atof( argv[2] );*/
    
    string str;
    for(i=start; i<argc; i++)
    {
        str = argv[i];
             if( str == "-beta" )		G_CLO_BETA  		= atof( argv[++i] );
	else if( str == "-tf" ) 		G_CLO_TF  		= atof( argv[++i] );
	else if( str == "-rho" ) 		G_CLO_RHO  		= atof( argv[++i] );
 	else
        {
            fprintf(stderr, "\n\tUnknown option [%s] on command line.\n\n", argv[i]);
            exit(-1);
        }
    }

    return;
}




