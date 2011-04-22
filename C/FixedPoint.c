#include "FixedPoint.h"

volatile unsigned char fxreg;
fxpnt64u_t _fxpnt_accu64;

/* Float to fixed point */
fxpnt_t fxf_to_fxpnt(float* f) {
  fxpnt_t a;

  a.full = (int)((*f)*(1UL<<31));
  

  fxreg |= ((*f)>=1.0)<<0x07;
  fxreg |= ((*f)<-1.0)<<0x06;
  fxreg |= FX_OHFLOW >> 3;
  fxreg |= FX_OLFLOW >> 3;

  a.full = FX_OHFLOW?INT_FAST32_MAX:a.full;
  FX_OHFLOW_CLEAR;
  FX_OLFLOW_CLEAR;

  return a;
}

/* Fixed point to float */
float fxfxpnt_to_f(fxpnt_t* a) {
  float f;

  f = (float)a->full / (1UL<<31);

  return f;
}

fxpnt_t fxadd(fxpnt_t* a, fxpnt_t* b) {
  fxpnt_t c;
  __asm__("ADDS %0, %1, %2             ":"=r" (c.full): "r" (a.full),      "r" (b.full):"cc");
  c.full = a->full + b->full;
  
  fxreg |= (!a->part.integer &&
            !b->part.integer &&
            c.part.integer)<<0x07;
  fxreg |= (a->part.integer &&
            b->part.integer &&
            !c.part.integer)<<0x06;  
  fxreg |= FX_OHFLOW >> 3;  
  fxreg |= FX_OLFLOW >> 3;


  c.full = FX_OHFLOW?INT_FAST32_MAX:c.full;
  c.full = FX_OLFLOW?INT_FAST32_MIN:c.full;
  FX_OHFLOW_CLEAR;
  FX_OLFLOW_CLEAR;

  return c;
}
/* Return: c = a - b */
fxpnt_t fxsub(fxpnt_t* a, fxpnt_t* b) {
  fxpnt_t c;

  c.full = a->full - b->full;

  
  fxreg |= (!a->part.integer &&
            b->part.integer &&
            c.part.integer)<<0x07;
  fxreg |= (a->part.integer &&
            !b->part.integer &&
            !c.part.integer)<<0x06;
  fxreg |= FX_OHFLOW >> 3;
  fxreg |= FX_OLFLOW >> 3;

  c.full = FX_OHFLOW?INT_FAST32_MAX:c.full;
  c.full = FX_OLFLOW?INT_FAST32_MIN:c.full;
  FX_OHFLOW_CLEAR;
  FX_OLFLOW_CLEAR;

  return c;
}

/* Return: c = a * b */
fxpnt_t fxmul(fxpnt_t* a, fxpnt_t* b) {
  fxpnt_t c;

  //_fxpnt_accu64.full = (int_fast64_t)a->full * b->full;
  //_fxpnt_accu64.full = (int_fast64_t)a->full*b->full;

  fxreg |= (_fxpnt_accu64.part.H == 0 && (a->full != 0 || b->full != 0))<<0x05;
  fxreg |= FX_UFLOW >> 3;

  c.full = _fxpnt_accu64.part.H;
  c.full = a->full * b->full;

  c.full = FX_UFLOW?0x01:_fxpnt_accu64.part.H;
  FX_UFLOW_CLEAR;
  
  return c;
}

/* Return: c = a / b */
fxpnt_t fxdiv(fxpnt_t* a, fxpnt_t* b) {
  fxpnt_t c;

  if(b->full == 0) {
    fxreg |= 0x02;
    c.full = INT_FAST32_MAX;
    return c;
  }

  _fxpnt_accu64.part.H  = a->full;
  _fxpnt_accu64.full    = _fxpnt_accu64.full / b->full;

  fxreg |= (_fxpnt_accu64.full == 0 && a->full != 0)<<0x05;
  fxreg |= FX_UFLOW >> 3;

  c.full = FX_UFLOW?0x01:_fxpnt_accu64.full;
  FX_UFLOW_CLEAR;

  fxreg |= (_fxpnt_accu64.full >= INT_FAST32_MAX)<<0x07;
  fxreg |= (_fxpnt_accu64.full < INT_FAST32_MIN)<<0x06;
  fxreg |= FX_OHFLOW >> 3;
  fxreg |= FX_OLFLOW >> 3;

  c.full = FX_OHFLOW?INT_FAST32_MAX:c.full;
  c.full = FX_OLFLOW?INT_FAST32_MIN:c.full;
  FX_OHFLOW_CLEAR;
  FX_OLFLOW_CLEAR;

  return c;
}
