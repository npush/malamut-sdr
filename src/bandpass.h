#ifndef __BANDPASS_H_
#define __BANDPASS_H_

#define M_PI 3.1415926535897932384626433832795f
#define Pi  (3.1415926535897932384626433832795f)
#define TWOPI (2*3.1415926535897932384626433832795f)
               //6.283185307179586476925286766558

void Calculate_Window_coef( int window_type );
void Calculate_FIR_coef( int FreqLo, int FreqHi );
void Calculate_FIRam_coef( int FreqLo, int FreqHi );


#endif