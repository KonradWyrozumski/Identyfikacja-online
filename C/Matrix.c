#include <stdio.h>
#include <stdlib.h>
#include "Matrix.h"
#include <cross_studio_io.h>

fxpnt_matrix_t* initMatrix(int rows, int colls) {
  fxpnt_matrix_t* matrix;
  int i, j;

  matrix = (fxpnt_matrix_t*)malloc(sizeof(fxpnt_matrix_t));  
  if(matrix == NULL) {
    return NULL;
  }

  matrix->colls = colls;
  matrix->rows = rows;
  matrix->matrix = (float**)malloc(matrix->rows * sizeof(float*));
  if(matrix == NULL) {
    free(matrix->matrix);
    return NULL;
  }

  for(i=0;i<matrix->rows;i++) {
    (matrix->matrix)[i] = (float*)malloc(matrix->colls * sizeof(float)); 
    if((matrix->matrix)[i] == NULL) {
      for(j=0;j<i;j--)
        free((matrix->matrix)[j]);
      free(matrix->matrix);
      free(matrix);
      return NULL;
    }
  }

  for(i=0;i<matrix->rows;i++)
    for(j=0;j<matrix->colls;j++) {     
      (matrix->matrix)[i][j] = 0;
    }

  return matrix;
}

fxpnt_matrix_t* initOnesMatrix(int rows, int colls){
  fxpnt_matrix_t* matrix;
  matrix = initMatrix(rows, colls);

  for(int i = 0; i < rows; i++){
    for(int j = 0; j < colls; j++)
      (matrix->matrix)[i][j] = 1;
  }
  return matrix;
}

fxpnt_matrix_t* initEyeMatrix(int rows) {
  fxpnt_matrix_t* matrix;
  matrix = initMatrix(rows, rows);
  int i;
  for(i = 0; i < rows; i++)
    matrix->matrix[i][i] = 1;
  return matrix;
}

fxpnt_matrix_t* copyMatrix(fxpnt_matrix_t** a_m){
	fxpnt_matrix_t* matrix;
	int i, j;

	matrix = initMatrix((*a_m)->rows, (*a_m)->colls);
	for(i=0;i<(*a_m)->rows;i++){
      for(j=0;j<(*a_m)->colls;j++)
        (matrix->matrix)[i][j] = ((*a_m)->matrix)[i][j];
	}
	return matrix;
}

void m_free(fxpnt_matrix_t** a_m){
	int i;
        if(*a_m == NULL)
          return;
        for(i=0;i<(*a_m)->rows;i++) {
		free(((*a_m)->matrix)[i]);
		((*a_m)->matrix)[i] = NULL;
	}

	free((*a_m)->matrix);
	(*a_m)->matrix = NULL;

	free(*a_m);
	*a_m = NULL;
}

void add_mm_a(fxpnt_matrix_t** a_m, fxpnt_matrix_t** b_m){
  int i,j;

  if((*a_m)->colls != (*b_m)->colls ||
    (*a_m)->rows  != (*b_m)->rows) {
    return;
  }
  for(i=0;i<(*a_m)->rows;i++) {
    for(j=0;j<(*a_m)->colls;j++) {
      ((*a_m)->matrix)[i][j] = ((*a_m)->matrix)[i][j] + ((*b_m)->matrix)[i][j];
    }
  }
}


void add_sm_a(float a, fxpnt_matrix_t **b_m)
{
  int i,j;

  for(i=0;i<(*b_m)->rows;i++) {
    for(j=0;j<(*b_m)->colls;j++) {
      ((*b_m)->matrix)[i][j] = ((*b_m)->matrix)[i][j] + a;
    }
  }
}


void sub_mm_a(fxpnt_matrix_t** a_m, fxpnt_matrix_t** b_m){
  int i,j;

  if((*a_m)->colls != (*b_m)->colls ||
    (*a_m)->rows  != (*b_m)->rows) {
    return;
  }
  for(i=0;i<(*a_m)->rows;i++) {
    for(j=0;j<(*a_m)->colls;j++) {
      ((*a_m)->matrix)[i][j] = ((*a_m)->matrix)[i][j] - ((*b_m)->matrix)[i][j];
    }
  }
}

void mul_sm_a(float a, fxpnt_matrix_t** b_m){
  int i,j;

  for(i=0;i<(*b_m)->rows;i++) {
    for(j=0;j<(*b_m)->colls;j++) {
      ((*b_m)->matrix)[i][j] = a * ((*b_m)->matrix)[i][j];
    }
  }	
}
void mul_mm_a(fxpnt_matrix_t** a_m, fxpnt_matrix_t** b_m){
  fxpnt_matrix_t* matrix;
  int i,j,k;
  float m, s;
  if((*a_m)->colls != (*b_m)->rows) {
    return;
  }

  matrix = initMatrix((*a_m)->rows, (*b_m)->colls);
  if(matrix == NULL) {
    return;
  }

  for(i=0;i<(*a_m)->rows;i++) {
    for(j=0;j<(*b_m)->colls;j++) {
	  s = 0;
      for(k=0;k<(*a_m)->colls;k++) {
        m = ((*a_m)->matrix)[i][k] * ((*b_m)->matrix)[k][j];
        s = s + m;
      }
      (matrix->matrix)[i][j] = s;
    }
  }

  m_free(a_m);
  *a_m = matrix;
}

void mul_mTm_a(fxpnt_matrix_t** a_m, fxpnt_matrix_t** b_m){
  fxpnt_matrix_t* matrix;
  int i,j,k;
  float fx_m, fx_s;

  if((*a_m)->rows != (*b_m)->rows) {
    return;
  }

  matrix = initMatrix((*a_m)->colls, (*b_m)->colls);
  if(matrix == NULL) {
    return;
  }

  for(i=0;i<(*a_m)->colls;i++) {
    for(j=0;j<(*b_m)->colls;j++) {
      fx_s = 0;
      for(k=0;k<(*a_m)->rows;k++) {
        fx_m = ((*a_m)->matrix)[k][i] * ((*b_m)->matrix)[k][j];
        fx_s = fx_s + fx_m;
      }
      (matrix->matrix)[i][j] = fx_s;
    }
  }

  m_free(a_m);
  *a_m = matrix;
}
void mul_mmT_a(fxpnt_matrix_t** a_m, fxpnt_matrix_t** b_m) {
fxpnt_matrix_t* matrix;
  int i,j,k;
  float fx_m, fx_s;

  if((*a_m)->colls != (*b_m)->colls) {
    return;
  }

  matrix = initMatrix((*a_m)->rows, (*b_m)->rows);
  if(matrix == NULL) {
    return;
  }

  for(i=0;i<(*a_m)->rows;i++) {
    for(j=0;j<(*b_m)->rows;j++) {
      fx_s = 0;
      for(k=0;k<(*a_m)->colls;k++) {
        fx_m = ((*a_m)->matrix)[i][k] * ((*b_m)->matrix)[j][k];
        fx_s = fx_s + fx_m;
      }
      (matrix->matrix)[i][j] = fx_s;
    }
  }

  m_free(a_m);
  *a_m = matrix;
}

int findMax(fxpnt_matrix_t* m){
	int i, j;
	float max = 0.0;
	for(i = 0; i < m->rows; i++){
		if(m->matrix[i][0] > max){
			max = m->matrix[i][0];
			j = i;
		}
	}
	return j;
}

void showMatrix(fxpnt_matrix_t* m){
  int i, j;
  for(i = 0; i < m->rows; i++){
    for(j = 0; j < m->colls; j++){
      debug_printf("%f \t", (m->matrix)[i][j]);
    }
    debug_printf("\n");
  }
  debug_printf("\n");
}