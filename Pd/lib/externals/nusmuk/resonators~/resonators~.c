/*
   (c) 1988,1989,2007-2013 Adrian Freed

   Pure data port by Jamie Bullock <jamie@jamiebullock.com>

   basicresonators~
   Parallel bank of resonant filters

   Coefficients are linearly interpolated.

   Double precision is required because the filter topology
   (one with the minimum possible number of * +)
   ends up with coefficients near 1 for high Q filters


   Adrian Freed
*/

#include "m_pd.h"

#include <math.h>

#ifdef WIN_VERSION
#include <pmmintrin.h>
#endif

static t_class *resonators_class;
#define MAXRESONANCES 1024
#define OBJECT_NAME "resonators~"

typedef struct dresdesc
{
    t_float out1; // state
    t_float out2;
    t_float a1;
    t_float a1prime;
    t_float b1;
    t_float b2;
    t_float o_a1;
    t_float o_b1;
    t_float o_b2;
} 
dresdesc;

/* bank of filters */
typedef struct _resonators
{
    t_object  x_obj;

    short b_connected;

    dresdesc *dbase;
    int nres; 
    int nmax;	/* maximum number of filters */

    double samplerate;
    double sampleinterval;

    t_outlet *outlet1;  

    t_sample f;
} 
t_resonators;

// double precision interpolating (smooth) with input
void diresonators_perform64(t_resonators *x, 
        t_object *dsp64, 
        double **ins, 
        long numins, 
        double **outs, 
        long numouts, 
        long sampleframes, 
        long flags, 
        void *userparam)
{
    const double *in = *ins;
    t_resonators *op = x;
    double *out = *outs;
    long n = sampleframes;
    int nfilters = op->nres;
    register	double yn,yo;
    int i, j;
    double rate = 1.0/n;

#ifdef SQUASH_DENORMALS
    static int sq;
    if(!sq)
    {
        printf("squashing denormals\n");
        sq++;
    }
#if defined( __i386__ ) || defined( __x86_64__ )
    int oldMXCSR = _mm_getcsr(); // read the old MXCSR setting
    int newMXCSR = oldMXCSR | 0x8040; // set DAZ and FZ bits
    _mm_setcsr( newMXCSR );	 // write the new MXCSR setting to the MXCSR
#endif
#endif
    {
        dresdesc *f = op->dbase;
        for(j=0;j<n;++j)
        {
            out[j] = 0.0;
        }

        for(i=0;i< nfilters ;++i)
        {
            register double b1=f[i].o_b1, b2=f[i].o_b2, a1=f[i].o_a1;
            double a1inc = (f[i].a1-f[i].o_a1) *  rate;
            double b1inc = (f[i].b1-f[i].o_b1) *  rate;
            double b2inc = (f[i].b2-f[i].o_b2) *  rate;

            yo= f[i].out1;
            yn =f[i].out2;
            for(j=0;j<n;++j)
            {
                double x = yo;
                yo = b1*yo + b2*yn + a1*in[j];	

                out[j] += yo;

                yn = x;
                a1 += a1inc;
                b1 += b1inc;
                b2 += b2inc;
            }

            f[i].o_a1 = f[i].a1;
            f[i].o_b1 = f[i].b1;
            f[i].o_b2 = f[i].b2;
            f[i].out1= yo;
            f[i].out2 = yn;	
        }
    }
#ifdef SQUASH_DENORMALS
#if defined( __i386__ ) || defined( __x86_64__ )
    _mm_setcsr(oldMXCSR);
#endif
#endif
}

t_int *diresonators_perform(t_int *w)
{
    t_resonators *x = (t_resonators *)(w[1]);
    t_sample *fin = (t_sample *)(w[2]);
    t_sample *fout = (t_sample *)(w[3]);
    int n = (int)(w[4]);

    double in[n];
    double out[n];
    double *inp = NULL;
    double *outp = NULL;

    for(int j = 0; j < n; ++j)
    {
        in[j] = fin[j];
    }

    inp = in;
    outp = out;

    diresonators_perform64(x, NULL, &inp, 1, &outp, 1, n, 0, NULL);

    for(int j = 0; j < n; ++j)
    {
        fout[j] = out[j]; 
    }

    return (w+5);
}

// set the state variables of the filter to 0.0
void resonators_clear(t_resonators *x)
{
    dresdesc *df = x->dbase;
    int i;
    for(i=0;i<x->nres;++i)
    {
        df[i].out1 = df[i].out2 = 0.0;
    }
}

void resonators_dsp(t_resonators *x, t_signal **sp, short *connect)
{
    resonators_clear(x);

    dsp_add(diresonators_perform, 4, x, sp[0]->s_vec, sp[1]->s_vec,  sp[0]->s_n);
}

// This assumes we are single threaded, i.e. that we can never be interrupted by perform routine
void resonators_float(t_resonators *x, float ff)
{
    int i;
    dresdesc *dp = x->dbase;
    for(i=0;i<x->nres;++i)
    {
        dp[i].out2 += dp[i].a1prime*ff;		
    }
}


void resonators_bang(t_resonators *x)
{
    int i;
    t_atom filterstate[MAXRESONANCES*5+1];

    //	output filter state and coefficients to the second outlet
    // should we output the sample rate too or normalize the coefficients?
    SETFLOAT(&filterstate[0], x->samplerate);

    for(i=0;i<x->nres;++i)
    {
        SETFLOAT(&filterstate[1+i*5+0], x->dbase[i].out1);
        SETFLOAT(&filterstate[1+i*5+1], x->dbase[i].out2);
        SETFLOAT(&filterstate[1+i*5+2], x->dbase[i].a1);
        SETFLOAT(&filterstate[1+i*5+3], x->dbase[i].b1); // some people think this should be negative
        SETFLOAT(&filterstate[1+i*5+4], x->dbase[i].b2); // and this
    }
    outlet_list(x->outlet1, 0L, 1+i*5, filterstate);

}

// This assumes we are single threaded, i.e. that we can never be interrupted by perform routine
void resonators_list(t_resonators *x, t_symbol *s, short argc, t_atom *argv)
{
    int i;
    int nres;
    double srbar = x->sampleinterval;
    dresdesc *dp = x->dbase;

    if (argc%3!=0) 
    {
        pd_error((t_object *)x, "multiple of 3 floats required (frequency amplitude decayRate)");
        return;
    }

    for(i=0; (i*3)<argc; ++i)
    {
        if (i >= MAXRESONANCES)
        {
            pd_error((t_object *)x, "list has more than %d resonances; dropping extras", MAXRESONANCES);
            break;
        }
        else
        {
            // Here are the filter design equations		
            double f = atom_getfloatarg(i*3,argc,argv);
            double g = 	atom_getfloatarg(i*3+1,argc,argv);
            double rate = atom_getfloatarg(i*3+2,argc,argv);
            double r;
            r =  exp(-rate*srbar);

            if((f<=0.0) || (f>=(0.995*x->samplerate*0.5)) || (r<=0.0) || (r>1.0))
            {
                // post("Warning parameters out of range");
                dp[i].b1 = 0.0;
                dp[i].b1 = 0.0;
                dp[i].a1prime = 0.0;
            }
            else
            {
                double ts;
                f *= 2.0*3.14159265358979323*srbar;
                ts = g;
                ts *= sin(f);
                dp[i].a1 = ts *  (1.0-r);   //this is one of the relavent L norms
                dp[i].b2 =  -r*r;
                dp[i].b1 = r*cos(f)*2.0;

                //this is the other norm that establishes the impulse response of the right amplitude (scaled
                // so that it can be summed into the state variable outside the perform routine (for efficiency)
                // This was chosen to correspond with IRCAM Resan representation
                dp[i].a1prime = ts/dp[i].b2;					            
            }
        }
    }

    /* Now we know how many "good" resonances (freq > 0) were in the list */
    nres = i;

    for(i=0; i < nres; ++i)
    {
        if(i >= x->nres) 	/* If there are now more resonances than there were: */ 
        {
            // Set old a1 to zero so that the input to the new resonators will ramp up over the first signal vector.
            dp[i].o_a1 = 0.0;
            dp[i].o_b1 = dp[i].b1;
            dp[i].o_b2 = dp[i].b2;

            // Clear out state variables for these totally new resonances
            dp[i].out1 = dp[i].out2 = 0.0f;
        }
    }
    x->nres = nres;	
    //		post("nres %d x->nres %d", nres, x->nres);
}

void *resonators_new(t_symbol *s, short argc, t_atom *argv)
{
    t_resonators *x = (t_resonators *)pd_new(resonators_class);
    x->samplerate =  sys_getsr();

    if(x->samplerate <= 0.0)
    {
        x->samplerate = 44100.0;
    }

    x->sampleinterval = 1.0/x->samplerate;
    x->dbase = (dresdesc *)getbytes(MAXRESONANCES * sizeof(dresdesc));

    if(x->dbase==0)
    {			
        pd_error((t_object *)x, "not enough memory. ");
        return 0;
    }

    x->nres = MAXRESONANCES;
    resonators_clear(x); // clears state
    x->nres = 0;
    resonators_list(x,s,argc,argv);
    dresdesc *df = x->dbase;

    for(int i = 0; i < x->nres; ++i)
    {
        df[i].o_a1 = df[i].a1;
        df[i].o_b1 = df[i].b1;
        df[i].o_b2 = df[i].b2;
    }

    outlet_new(&x->x_obj, &s_signal);
    x->outlet1 = outlet_new(&x->x_obj, &s_list);

    return (void *)x;
}

void resonators_free(t_resonators *x) 
{
    freebytes(x->dbase, MAXRESONANCES * sizeof(dresdesc));
}


void resonators_tilde_setup(void) {
    resonators_class = class_new(gensym("resonators~"),
            (t_newmethod)resonators_new,
            (t_method)resonators_free,
            sizeof(t_resonators),
            CLASS_DEFAULT,
            A_GIMME, 0);

    post("%s: Copyright (c) 1986, 1987-2013 Adrian Freed. All Rights Reserved", OBJECT_NAME);
    post("%s: Pure Data port by Jamie Bullock <jamie@jamiebullock.com>", OBJECT_NAME);
    post("%s: Maximum number of resonances: %d", OBJECT_NAME, MAXRESONANCES);

    class_addmethod(resonators_class, (t_method)resonators_dsp, gensym("dsp"), A_CANT, 0);
    class_addmethod(resonators_class, (t_method)resonators_list, gensym("list"), A_GIMME, 0);
    class_addmethod(resonators_class, (t_method)resonators_clear, gensym("clear"), 0);
    class_addmethod(resonators_class, (t_method)resonators_bang, gensym("bang"), 0);

    /* Need to add the "float" method after CLASS_MAINSIGNALIN() or it gets overwritten */
    CLASS_MAINSIGNALIN(resonators_class, t_resonators, f);
    class_addmethod(resonators_class, (t_method)resonators_float, gensym("float"), A_FLOAT, 0);
}

