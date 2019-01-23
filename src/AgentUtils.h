
// returns char * and not float
// #define ROUND_FLOAT(VAL, PREC) printf("%."#PREC"f", VAL);
// #include <math.h>


#ifndef AGENT_UTILS_H
#define AGENT_UTILS_H

#define ROUND_FLOAT(val, prec) round(prec*(val))/(1.0*prec);

#endif