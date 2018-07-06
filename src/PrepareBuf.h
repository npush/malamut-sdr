#ifndef __PREPAREBUF_H_
#define __PREPAREBUF_H_

void Get_New_Pointer( void );
void Cpy_New_data(int txm);
void Cpy_Old_data( void );
void Prepare_buff_i2s_format( COMPLEX_long *buf, int size , int type);
  
#endif