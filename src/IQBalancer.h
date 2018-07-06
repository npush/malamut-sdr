#ifndef __IQBALANCER_H
#define __IQBALANCER_H

void IQBalancer_init( void );
void Process(COMPLEX_float* iq, int length);
void EstimateImbalance( void );
float Utility(float phase, float gain);
float GetRandomDirection( void );
void EstimatePower( void );

#define ACCURACY_PRIVILEGED 1
void Adjust(COMPLEX_float* buffer, int length, float phase, float gain);

#endif