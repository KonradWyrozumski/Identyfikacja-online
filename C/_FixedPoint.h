#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#ifndef FIXED_H
#define FIXED_H

#define INT_FAST32_MAX 0x7fffffff
#define INT_FAST32_MIN (-INT_FAST32_MAX - 1)
#define INT_FAST64_MAX 0x7fffffffffffffffL
#define INT_FAST64_MIN (-INT_FAST64_MAX-1L)
typedef signed short int int_fast16_t;
typedef signed int int_fast32_t;
typedef signed long long int int_fast64_t;
typedef unsigned char uint8_t;

extern volatile unsigned char fxreg;

#define FX_OHFLOW (fxreg&0x80)
#define FX_OHFLOW_CLEAR (fxreg&=~0x80)
#define FX_OLFLOW (fxreg&0x40)
#define FX_OLFLOW_CLEAR (fxreg&=~0x40)
/* Underflow flag */
#define FX_UFLOW (fxreg&0x20)
#define FX_UFLOW_CLEAR (fxreg&=~0x20)
/* Global overflow flag */
#define FX_GOHFLOW (fxreg&0x10)
#define FX_GOHFLOW_CLEAR (fxreg&=~0x10)
#define FX_GOLFLOW (fxreg&0x08)
#define FX_GOLFLOW_CLEAR (fxreg&=~0x08)
/* Global underflow flag */
#define FX_GUFLOW (fxreg&0x04)
#define FX_GUFLOW_CLEAR (fxreg&=~0x04)
/* Divide by zero */
#define FX_DIVZERO (fxreg&0x02)
#define FX_DIVZERO_CLEAR (fxreg&=~0x02)

typedef union fxpntu{
  int full;
  struct fxpnts{
    int_fast32_t fraction : 31;
    int_fast32_t integer : 1;
  } part;
} fxpnt_t;

typedef union fxpnt64u {
  int_fast64_t full;
  struct fxpnt64s {
    int_fast64_t L : 31;
    int_fast64_t H : 33;
  } part;
} fxpnt64u_t;

/* Return fixed point from float */
fxpnt_t fxf_to_fxpnt(float* f);

/* Return float from fixed point */
float fxfxpnt_to_f(fxpnt_t* a);

/* Return: c = a + b */
fxpnt_t fxadd(fxpnt_t* a, fxpnt_t* b);

/* Return: c = a - b */
fxpnt_t fxsub(fxpnt_t* a, fxpnt_t* b);

/* Return: c = a * b */
fxpnt_t fxmul(fxpnt_t* a, fxpnt_t* b);

/* Return: c = a / b */
fxpnt_t fxdiv(fxpnt_t* a, fxpnt_t* b);

/*
#define FADD(a,b) ((a)+(b))
#define FSUB(a,b) ((a)-(b))
#define FMUL(a,b,q) (((a)*(b))>>(q))
#define FDIV(a,b,q) (((a)<<(q))/(b))

#define FADDI(a,b,q) ((a)+((b)<<(q)))
#define FSUBI(a,b,q) ((a)-((b)<<(q)))
#define FMULI(a,b) ((a)*(b))
#define FDIVI(a,b) ((a)/(b))

#define FCONV(a, q1, q2) (((q2)>(q1)) ? (a)<<((q2)-(q1)) : (a)>>((q1)-(q2)))

#define FADDG(a,b,q1,q2,q3) (FCONV(a,q1,q3)+FCONV(b,q2,q3))
#define FSUBG(a,b,q1,q2,q3) (FCONV(a,q1,q3)-FCONV(b,q2,q3))
#define FMULG(a,b,q1,q2,q3) FCONV((a)*(b), (q1)+(q2), q3)
#define FDIVG(a,b,q1,q2,q3) (FCONV(a, q1, (q2)+(q3))/(b))

#define TOFIX(d, q) ((int)( (d)*(double)(1<<(q)) ))
#define TOFLT(a, q) ( (double)(a) / (double)(1<<(q)) )

*/
#endif