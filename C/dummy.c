#include <stdio.h>
#include "iniparser/src/iniparser.h"

#define LINESIZE 1024

/* A dummy example adding 42 to n */
void PlusQuaranteDeux(dictionary* ini){
  /* Extract variables from dictionary */
  int n = iniparser_getint(ini, "inputs:n", -1);

  /* Algorithm */
  n += 42;

  /* Save results to dictionary */
  char val[LINESIZE];
  snprintf (val, LINESIZE, "%d", n);
  iniparser_set(ini,"outputs:r", val);
}
