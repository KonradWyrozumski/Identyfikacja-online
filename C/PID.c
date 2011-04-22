#include "arma.h"
#include "PID.h"
#include <cross_studio_io.h>
#include <stdio.h>
#include <math.h>

extern volatile int St;
extern volatile float PrevError, IntTerm;
extern volatile float PrevError_C, IntTerm_C;
void Skok(fxpnt_matrix_t **Y, fxpnt_matrix_t **U, int N, int d, int delay){
  int i,j;
  m_free(Y);
  m_free(U);
  *U = initMatrix(N, 1);
  *Y = initMatrix(N, 1);
  for(i = 0; i < delay; i++)
    (*U)->matrix[i][0] = 0;
  for(i = delay; i < N; i++)
    (*U)->matrix[i][0] = 1;
  for(i = 0; i < N-d; i++){
    for(j = 0; j < St; j++){
      if(i > j){
        (*Y)->matrix[i+d][0] = (*Y)->matrix[i+d][0] - teta->matrix[j][0]*(*Y)->matrix[i-j+d-1][0];
        }
    }
    for(j = 0; j <= teta->rows - St; j++){
      if(i > j){
        (*Y)->matrix[i+d][0] = (*Y)->matrix[i+d][0] + teta->matrix[j+St][0]*(*U)->matrix[i-j][0];
        }
    }
  }
}

void DobierzNastawy(fxpnt_matrix_t** Y, fxpnt_matrix_t** U, volatile PIDfloat_t *pid, int d, int SPS){
  int delay = 10;
  int delay_ob = 0;
  float k, a, T0;
  int i;
  Skok(Y, U, 500, delay_ob, delay);
  int length = (*Y)->rows;
  /* obiekt z cz³onem ca³kuj¹cym */
  if(fabs((*Y)->matrix[length-1][0] - (*Y)->matrix[(int)ceil(0.9*length)][0]) > 0.05*(*Y)->matrix[length-1][0])
  {
    k = ((*Y)->matrix[(int)ceil(0.5*length)][0] - (*Y)->matrix[(int)ceil(0.5*length)-1][0])/(1.0/SPS);
    a = (*Y)->matrix[(int)ceil(0.5*length)][0] - k * (int)ceil(0.5*length)* 1.0/SPS;
    T0 = -a/k;
  }
  else
  {
    int y;
    fxpnt_matrix_t *K;
    K = initMatrix((*Y)->rows-1, 1);
    for(i = 0; i < (*Y)->rows - 1; i++)
      K->matrix[i][0] = (*Y)->matrix[i+1][0]-(*Y)->matrix[i][0];
    y = findMax(K);
    m_free(&K);
    k = ((*Y)->matrix[y+1][0]-(*Y)->matrix[y][0])/SPS;
    a = (*Y)->matrix[y][0] - (float)(k*SPS*y);
    T0 = -a/k;
  }
  pid->_Kp = fabs(0.95/a);
  pid->_Ti = (2.4*abs(T0));
  pid->_Td = (0.4*abs(T0));
}

void initPID(float Kp,float Ti,float Td, float Tt,float T,float H,float L, volatile PIDfloat_t *pid){
  pid->_Kp=Kp;
  pid->_Ti=Ti;
  pid->_Td=Td;
  pid->_Tt=Tt;
  pid->_T=T;
  pid->_H=H;
  pid->_L=L;
  pid->_buffU=0;
  pid->_buffE[0]=0;
  pid->_buffE[1]=0;
  pid->_buffC[0]=0;
  pid->_buffC[1]=0;
  pid->_i=0;
}

float PIDOdpowiedz(float e, volatile PIDfloat_t *pid){
  float u=0,tmp=0;

  if(pid->_buffU > pid->_H)
    tmp=pid->_H - pid->_buffU;
  if(pid->_buffU < pid->_L)
    tmp=pid->_L - pid->_buffU;
  if(pid->_i)//simpson
    u=( (pid->_buffE[0]+4*pid->_buffE[1]+e)/pid->_Ti+tmp)*pid->_T/3 + pid->_buffC[0];
  else//trapez
    u=( (pid->_buffE[1]+e)/pid->_Ti+tmp)*pid->_T/2 + pid->_buffC[1];

  pid->_buffC[0]=pid->_buffC[1];
  pid->_buffC[1]=u;
  u=u+(e-pid->_buffE[0])/(2*pid->_T)*pid->_Td;
  pid->_buffE[0]=pid->_buffE[1];
  pid->_buffE[1]=e;
  u=(u+e)*pid->_Kp;
  pid->_buffU=u;
  if(u > pid->_H)
    u=pid->_H;
  if(u < pid->_L)
    u=pid->_L;
  pid->_i=(!pid->_i);
  return u;
}