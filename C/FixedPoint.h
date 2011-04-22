#include <stdint.h>

typedef int32_t fx_t;

#define FX_FRAC 31

#define fx_float(a) (a / (float)(1<<FX_FRAC))
#define fx_make(a)  ((fx_t)(a * (1<<FX_FRAC)))
#define fx_add(a,b) (a + b)
#define fx_sub(a,b) (a - b)
#define fx_mul(a,b) ((fx_t)(((int64_t)a * b) >> FX_FRAC))
#define fx_div(a,b) ((fx_t)(((int64_t)a << FX_FRAC) / b))