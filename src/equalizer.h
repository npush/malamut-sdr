#ifndef __EQUALIZER_H_
#define __EQUALIZER_H_

void Init_Equal( signed char *gainDB );
void Equal( float *inputF32, float *outputF32 );
#endif