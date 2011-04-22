#include "arma.h"
#include "matrix.h"
#include "DAC.h"
#include "ADC.h"
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <cross_studio_io.h>
extern volatile int St;
void IdentyfikujObiekt(int St_m, int d)
{
  int AIC =  INT_MAX;             /* AIC = inf */
  int AIC_act;
  fxpnt_matrix_t *teta_act, *P_act, *fi_act;
  teta_act = NULL;
  P_act = NULL;
  fi_act = NULL;
  for (int i = 2; i < St_m; i++)
  {
    AIC_act = ARMA(i, 20, d);   /* Wyznacz wartoœci wspó³czynników dla obiektu rzêdu i */
    if(AIC_act < AIC)           /* Porównaj czy dla obiektu rzêdu i, kryterium AIC jest mniejsze ni¿ 
                                    najlepsze znalezione dotychczas */
    {
        AIC = AIC_act;
        St = i;
        m_free(&teta_act);
        m_free(&P_act);
        m_free(&fi_act);
        teta_act = copyMatrix(&teta);
        P_act = copyMatrix(&P);
        fi_act = copyMatrix(&fi);
    }
    m_free(&teta);
    m_free(&P);
    m_free(&fi);
  }
  teta = copyMatrix(&teta_act);
  P = copyMatrix(&P_act);
  fi = copyMatrix(&fi_act);
  
  m_free(&teta_act);
  m_free(&P_act);
  m_free(&fi_act);
}

int ARMA(int M, int n, int d)
{
  InicjalizujM(M, d);
  int sigma = 0;
  int et = 0;

  for(int i = M + 1 + d; i < n; i++){
      et = AktualizujM(Y->matrix[i][0], Y->matrix[i-1][0], U->matrix[i-d][0], M);
      sigma = sigma + et*et;
  }
  return (sigma + 2*M);
}

void InicjalizujM(int M, int d)
{
  int i, j;
  teta = initOnesMatrix(2*M+1, 1);
  for(i = 0; i < 2*M+1; i++)
    teta->matrix[i][0] = 0.1;

  P = initEyeMatrix(2*M + 1);
  for(i = 0; i < 2*M + 1; i++)
    P->matrix[i][i] = 1000;

  fi = initMatrix(2*M+1, 1);
  i = 0;
  for(j = M+d-1; j >=d; j--){
    fi->matrix[i++][0] = -Y->matrix[j][0];
  }
  for(j = M; j >=0; j--){
    fi->matrix[i++][0] = U->matrix[j][0];
  }
}

int AktualizujM(float Y, float Yt, float Ut, int M)
{
  fxpnt_matrix_t *Li, *Mi;
  float m, et = 0;

  /* Aktualizuj wektor fi */
  for(int i = M-1; i >= 1; i--)
    fi->matrix[i][0] = fi->matrix[i-1][0];
  fi->matrix[0][0] = -Yt;
  for(int i = fi->rows-1; i > M; i--)
    fi->matrix[i][0] = fi->matrix[i-1][0];
  fi->matrix[M][0] = Ut;
  
  Mi = copyMatrix(&fi);
  mul_mTm_a(&Mi, &P);           /* fi'*P */
  mul_mm_a(&Mi, &fi);           /* fi'*P*fi */
  m = 1 + Mi->matrix[0][0];     /* mianownik obu macierzy jest skalarem */
  m_free(&Mi);

  Li = copyMatrix(&fi);
  mul_mTm_a(&Li, &teta);        /* fi'*teta  - skalar*/
  et = Y - Li->matrix[0][0];
  m_free(&Li);

  Li = copyMatrix(&P);
  mul_mm_a(&Li, &fi);           /* P*fi */
  Mi = copyMatrix(&Li);
  mul_sm_a(et, &Li);            /* P*fi*et */
  m = 1/m;
  mul_sm_a(m, &Li);             /* P*fi*et/m */
  add_mm_a(&teta, &Li);         /* teta = teta + P*fi*et/m */
  m_free(&Li);

  mul_mmT_a(&Mi, &fi);          /* P*fi*fi' */
  mul_mm_a(&Mi, &P);            /* P*fi*fi'*P */
  mul_sm_a(m, &Mi);             /* P*fi*fi'*P/m */
  sub_mm_a(&P, &Mi);            /* P = P - P*fi*fi'*P/m */
  m_free(&Mi);

  return et;
}

void GenerujWejscia(void){  
  for(int i = 0; i < 40; i++)
     U->matrix[i][0] = GetRand(0,4095);
}

int GetRand(int min,int max){
  return(rand()%(max-min)+min);
}

int WyznaczOpoznienie(void){
  int skokY;
  for(int i = 1; i < 20; i++) {
    if(Y->matrix[i][0] != Y->matrix[i-1][0]){
      skokY = i;
      break;
    }
  }
  if(skokY > 0)
    return skokY - 1;
  else 
    return -1;
}

void TestujObiekt(){

  GenerujWejscia(); // generuje losowe wejscia 0-4095

  uint16_t myChar;
  for(int i = 0; i < 40; i++){
    DAC_TransmitByte(U->matrix[i][0]);
    DelayMs(50);
    Y->matrix[i][0] = (float)ADC_GetValue();
  }
}