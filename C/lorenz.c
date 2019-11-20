/* Integrates Lorenz 63 and 95 differential equations using 4th order Runge-Kutta
   Eigen manages arrays
   Data are read and written in CSV format.
*/
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <string.h>
#include <Eigen/Dense>
#include "iniparser/src/iniparser.h"

#define LINESIZE 1024
using namespace Eigen;
using namespace std;
const IOFormat CSVFormat(StreamPrecision, DontAlignCols, ", ", "\n");

/* lorenz 63 differential equation */
void f63(ArrayXd& y, const ArrayXd& x, double sigma, double rho, double beta){
  y(0) = sigma*(x(1)-x(0));
  y(1) = x(0)*(rho-x(2))-x(1);
  y(2) = x(0)*x(1)-beta*x(2);
}

/* lorenz 95 differential equation */
void f95(ArrayXd& y, const ArrayXd& x, double F){
  int N = x.size();
  for (int i=0; i<N; i++){
    y(i) = (x((i+1)%N) - x((i-2+N)%N))*x((i-1+N)%N) - x(i%N) + F;
  }
}

/* integration of lorenz63 trajectory */
void lorenz63(dictionary* ini){

  /* reading parameters */
  int N = iniparser_getint(ini, "inputs:n",-1);
  int Ndt = iniparser_getint(ini, "inputs:ndt",-1);
  double dt = iniparser_getdouble(ini, "inputs:dt",-1.0);
  double sigma = iniparser_getdouble(ini, "inputs:sigma",-1.0);
  double rho = iniparser_getdouble(ini, "inputs:rho",-1.0);
  double beta = iniparser_getdouble(ini, "inputs:beta",-1.0);
  int Nc = iniparser_getint(ini, "inputs:nc",-1);
  const char* str_dir = iniparser_getstring(ini, "files:dir", NULL);
  const char* str_xts = iniparser_getstring(ini, "outputs:xts", NULL);
  const char* str_x0 = iniparser_getstring(ini, "inputs:x0", NULL);

  /* initializing trajectory */
  char line[LINESIZE];
  snprintf(line,LINESIZE,"../%s/%s",str_dir,str_x0);
  FILE* ifile = fopen(line, "r");
  fgets(line,LINESIZE,ifile);
  char* tok;
  ArrayXXd X(Nc,N);
  tok = strtok(line,",");
  for(int i=0; i<N; i++){
    X(0,i) = atof(tok);
    tok = strtok(NULL,",");
  }
  fclose(ifile);

  /* 4th order Runge-Kutta */
  ArrayXd x(N);
  ArrayXd k1(N);
  ArrayXd k2(N);
  ArrayXd k3(N);
  ArrayXd k4(N);
  for(int j=0; j<Nc-1; j++){
    x = X.row(j);
    for(int i=0; i<Ndt; i++){
      f63(k1,x,sigma,rho,beta);
      f63(k2,x+(0.5*dt)*k1,sigma,rho,beta);
      f63(k3,x+(0.5*dt)*k2,sigma,rho,beta);
      f63(k4,x+dt*k3,sigma,rho,beta);
      x += (dt/6)*(k1+2*k2+2*k3+k4);
    }
    X.row(j+1) = x;
  }

  /* Writing trajectory in CSV file */
  snprintf (line, LINESIZE, "../%s/%s", str_dir, str_xts);
  ofstream ofile(line);
  ofile << X.format(CSVFormat);
  ofile.close();
}

/* integration of lorenz95 trajectory */
void lorenz95(dictionary* ini){

  /* reading parameters*/
  int N = iniparser_getint(ini, "inputs:n",-1);
  int Ndt = iniparser_getint(ini, "inputs:ndt",-1);
  double dt = iniparser_getdouble(ini, "inputs:dt",-1.0);
  double F = iniparser_getdouble(ini, "inputs:f",-1.0);
  int Nc = iniparser_getint(ini, "inputs:nc",-1);
  const char* str_dir = iniparser_getstring(ini, "files:dir", NULL);
  const char* str_xts = iniparser_getstring(ini, "outputs:xts", NULL);
  const char* str_x0 = iniparser_getstring(ini, "inputs:x0", NULL);

  /* initializing trajectory */
  char line[LINESIZE];
  snprintf(line,LINESIZE,"../%s/%s",str_dir,str_x0);
  FILE* ifile = fopen(line, "r");
  fgets(line,LINESIZE,ifile);
  char* tok;
  ArrayXXd X(Nc,N);
  tok = strtok(line,",");
  for(int i=0; i<N; i++){
    X(0,i) = atof(tok);
    tok = strtok(NULL,",");
  }
  fclose(ifile);

  /* 4th order Runge-Kutta */
  ArrayXd x(N);
  ArrayXd k1(N);
  ArrayXd k2(N);
  ArrayXd k3(N);
  ArrayXd k4(N);
  for(int j=0; j<Nc-1; j++){
    x = X.row(j);
    for(int i=0; i<Ndt; i++){
      f95(k1,x,F);
      f95(k2,x+(0.5*dt)*k1,F);
      f95(k3,x+(0.5*dt)*k2,F);
      f95(k4,x+dt*k3,F);
      x += (dt/6)*(k1+2*k2+2*k3+k4);
    }
    X.row(j+1) = x;
  }

  /* Writing trajectory in CSV file */
  snprintf (line, LINESIZE, "../%s/%s", str_dir, str_xts);
  ofstream ofile(line);
  ofile << X.format(CSVFormat);
  ofile.close();
}
