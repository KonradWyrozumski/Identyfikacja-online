#include <stdio.h>
#include <stdlib.h>
#include "stm32f10x_conf.h"
#include "delay.h"
#include "matrix.h"

#ifndef ARMA_H
#define ARMA_H

fxpnt_matrix_t *Y, *U, *Ld, *Md, *teta, *P, *fi;
volatile int St;
  // Tablica odpowiedzi obiektu z chwili t:t-d (d - opoznienie)
fxpnt_matrix_t *odpowiedziObiektu;
  // Wartosc zadana zadana na obiekcie
float wartoscZadana;
  // Wyjscie z regulatora w chwili t
float wyjscieReg;
  // Wyjscie z regulatora w chwili t-1
float wyjscieRegPop;
  // Opoznienie obiektu
int d;
  // Nastawy regulatora
float Kp, Ti, Td;
/* Funkcja identyfikuje obiekt jako model o stopniu maksymalnym
  St_m oraz opoznieniu d. Obiekt jest przechowywany w globalnych
  tablicach Ld, Md (Licznik + Mianownik)
*/
void IdentyfikujObiekt(int St_m, int d);
/* Funkcja oblicza model obiektu o stopniu M, dla próbek n.
  Zwraca ona wartoœæ kryterium AIC
*/
int ARMA(int M, int n, int d);
/* Inicjalizuje macierze teta, fi, P o stopniu M i opoznieniu d.
  Zapisuje je jako globalne. (Obiekt nie zidentyfikowany jeszcze)
*/
void InicjalizujM(int M, int d);
/* Aktualizuje macierze teta, fi, P. 
  Y - wartosc wyjscia aktualnego
  Yt - wartosc wyjscia w chwili t-1
  Ut - wartosc z obiektu w chwili t-d
  M - stopien obiektu
*/
int AktualizujM(float Y, float Yt, float Ut, int M);
/* Funkcja generuje losowe wejscia jako szum bialy
  z przedzialu 0-100. Zapisuje je do tablicy globalnej
  U
*/
void GenerujWejscia(void);
/* Wyznacza i zwraca opoznienie obiektu w ilosci probek
  pomiedzy skokiem na obiekt, a odpowiedzia
*/
int WyznaczOpoznienie(void);
/* Wyznacza losowa liczne z przedzialu min:max */
int GetRand(int min,int max);
/* Funkcja testuje obiekt, wystawiaj¹c losowe
  wyjœcia z przedzia³u 0-4096, oraz zbieraj¹c
  próbki wejœæ do 2 wektorów 1x20
*/
void TestujObiekt();

#endif