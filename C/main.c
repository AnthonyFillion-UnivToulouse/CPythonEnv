/* Main C file
   read config file and run aglorithms
*/
#include <stdio.h>
#include "iniparser/src/iniparser.h"
#include "lorenz.h"
#include "dummy.h"

int main(int argc, char* argv[]){

  /* Read config file */
  if (argc != 2){
    fprintf(stderr, "Wrong number of arguments in %s\n", argv[0]);
    return -1;
  }
  dictionary* ini;
  ini = iniparser_load(argv[1]);
  if (ini==NULL) {
    fprintf(stderr, "cannot parse config file: %s\n", argv[1]);
    return -1 ;
  }

  /* Run algorithms */
  const char* str_algo = iniparser_getstring(ini, "inputs:algo", NULL);
    if (strcmp(str_algo,"PlusQuaranteDeux") == 0){
    PlusQuaranteDeux(ini);
  }else if (strcmp(str_algo,"lorenz63") == 0){
    lorenz63(ini);
  }else if (strcmp(str_algo,"lorenz95") == 0){
    lorenz95(ini);
  }else{
    fprintf(stderr, "parameter algo not registered\n");
  }

  /* Save dictionnary eventually modified */
  FILE* f = fopen(argv[1], "w");
  iniparser_dump_ini(ini,f);
  iniparser_freedict(ini);
  fclose(f);

  return 0;
}
