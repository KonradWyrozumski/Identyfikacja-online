#ifndef MATRIX_H
#define MATRIX_H

/* Macierz ma wymiar rows x cols, dostep poprzez:
	nazwa_macierzy [ row ][ col ]
*/

// Przechowuje macierz o rozmiarach rows x colls
typedef struct fxpnt_matrix {
  int  colls;
  int  rows;
  float** matrix;
} fxpnt_matrix_t;

// Inicjalizuj macierz o wymiarach rows x colls
fxpnt_matrix_t* initMatrix(int rows, int colls);

// Inicjalizuj macierz jedynkow¹ o wymiarach rows x colls
fxpnt_matrix_t* initOnesMatrix(int rows, int colls);

// Inicjalizuj macierz jedynkowa
fxpnt_matrix_t* initEyeMatrix(int rows);

// Utworz kopie macierzy
fxpnt_matrix_t* copyMatrix(fxpnt_matrix_t** a_m);

// Kasuj macierz
void m_free(fxpnt_matrix_t** a_m);

// Dodaj: a_m = a_m + b_m
void add_mm_a(fxpnt_matrix_t** a_m, fxpnt_matrix_t** b_m);

// Dodaj: b_m = a + b_m
void add_sm_a(float a, fxpnt_matrix_t **b_m);

// Odejmij: a_m = a_m - b_m
void sub_mm_a(fxpnt_matrix_t** a_m, fxpnt_matrix_t** b_m);

// Mnoz macierz: b_m = a * b_m
void mul_sm_a(float a, fxpnt_matrix_t** b_m);

// Mnoz macierz: a_m = a_m * b_m
void mul_mm_a(fxpnt_matrix_t** a_m, fxpnt_matrix_t** b_m);

// Mnoz macierz: a_m = (Transpose) a_m * b_m
void mul_mTm_a(fxpnt_matrix_t** a_m, fxpnt_matrix_t** b_m);

// Mnoz macierz: a_m = a_m * (Transpose) b_m
void mul_mmT_a(fxpnt_matrix_t** a_m, fxpnt_matrix_t** b_m);

// Znajdz maksymalny element
int findMax(fxpnt_matrix_t* m);

// Wypisz macierz na debug terminal (CrossWorks)
void showMatrix(fxpnt_matrix_t* m);
#endif