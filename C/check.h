#ifndef _CHECK_H
#define _CHECK_H 1

#define active_check(cond, limit) { \
  int i;                            \
  for (i = (limit); !(cond); --i)   \
    if (i <= 0)                     \
      return -1;                    \
}

#endif