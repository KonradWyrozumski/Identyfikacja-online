#ifndef PID_H
#define PID_H
#include "Matrix.h"
#include "stdlib.h"

/* Struktura przechowuj¹ca regulator i jego nastawy oraz ograniczenia */

typedef struct{
	float _Kp;	//|
	float _Ti;	//| Nastawy regulatora
	float _Td;	//|
	float _Tt;	// anti-windup
	float _H;	// ograniczenie gorne wyjscia
	float _L;	// ograniczenie dolne wyjscia
	float _buffU;	// bufor wart. sterowania
	float _buffE[2];// bufor wart. uchybu
	float _buffC[2];// bufor wart. calki
	float _T;	// czas probkowania
	
	int _i;		//zm. pomocnicza
}PIDfloat_t;

  volatile PIDfloat_t PID;
  volatile int SP;    // wartoœæ zadana
  volatile int PV;    // wartosc z obiektu
/* Funkcja symuluje skok na obiekcie na podstawie wartosci 
  macierzy teta. Zapisuje skok w wektorach U i Y o d³ugosci
  Nx1.
  Funkcja bierze pod uwage opoznienie d obiektu oraz opoznienie
  skoku delay
*/
void Skok(fxpnt_matrix_t** Y, fxpnt_matrix_t** U, int N, int d, int delay);
/* Funkcja dobiera nastawy do obiektu na podstawie odpowiedzi skokowej U i Y.
  Wartosci wspolczynnikow Kp, Ti, Td sa przekazywane przez referencje i zmieniane
  wewnatrz funkcji.
*/
void DobierzNastawy(fxpnt_matrix_t** Y, fxpnt_matrix_t** U, volatile PIDfloat_t *pid, int d, int SPS);
/* Funkcja inicjalizuj¹ca regulator PID.
  Kp, Ti, Td - nastawy regulatora

  Funkcja zwraca wartosc ktora jest przekazywana na 
  przetwornik DAC
*/
void initPID(float Kp,float Ti,float Td, float Tt,float T,float H,float L, volatile PIDfloat_t *PID);
float PIDOdpowiedz(float e, volatile PIDfloat_t *PID);
#endif