% Ustawienia MATLABa
clear all;
close all;
warning off MATLAB:divideByZero;
warning off MATLAB:singularMatrix;
format short; % e
%clc;

%% ------------------- Wst�pne ustawienia ------------------- %
global L M T SPS iS step_ step_time Ld Md Ywyj Yreg;
% SPS - Samples Per Second 
%       cz�stotliwo�� pr�bkowania
SPS = 10;
%% ---------------------------------------------------------- %

%% ------------------- Obiekt ------------------- %
% Obiekt symulowany posiada transmitancj� typu:
%          -(sT)  L(s) 
% G(s) = e^   *  -----
%                 M(s)
% L - wektor wsp�czynnik�w licznika
% M - wektor wsp�czynnik�w mianownika
% T - op�nienie
L = [3];
M = [2, 1, 1];
T = 3 * 1/SPS;    % op�nienie o n pr�bek
%% ---------------------------------------------- %

%% ------------------- Symulacja ------------------- %
% Transmitancja obiektu ci�g�ego
obiekt_ciag = tf(L, M);

% Transmitancja obiektu dyskretnego
obiekt_dysk = c2d(obiekt_ciag, 1/SPS);

% Symulujemy obiekt, by otrzyma� warto�ci wektora dyskregnego
% odpowiadaj�cego wej�ciom i wyj�ciom uk�adu
% (uwzgl�dnione op�nienie)
[to, Yo, Uo] = TestujObiekt();

%% ------------------------------------------------- %

%% ------------------- Dalsza obr�dka wektor�w wej-wyj ------------------- %
% Wyznaczenie op�nienia obiektu, jako ilo�ci pr�bek
% pomi�dzy wej�ciem na obiekt, a jego reakcj�
d = WyznaczOpoznienie(Uo, Yo);

% Wyznaczenie wsp�czynnik�w obiektu metod� identyfikacji
% ARMA
% M - stopie� licznika = stopie� mianownika
St = 10;
[Md, Ld] = IdentyfikujObiekt(Uo, Yo, St, d);
%% -----------------------------------------------------------------------

%% ------------------- Dob�r nastaw do regulatora PID ------------------- %
[Kp, Ti, Td] = DobierzNastawy(Md, Ld, SPS, d);
%% ---------------------------------------------------------------------- %

%% ------------- Wykonaj skok na obiekcie z regulatorem ------------------%
[treg, Ywyj, Yreg] = TestujRegulator();
%% ---------------------------------------------------------------------- %

%% ------------- Rysuj odpowiedzi uk�adu ------------------%
% RysujOdpowiedzi();
%% ---------------------------------------------------------------------- %
