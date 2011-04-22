% Ustawienia MATLABa
clear all;
close all;
warning off MATLAB:divideByZero;
warning off MATLAB:singularMatrix;
format short; % e
%clc;

%% ------------------- Wstêpne ustawienia ------------------- %
global L M T SPS iS step_ step_time Ld Md Ywyj Yreg;
% SPS - Samples Per Second 
%       czêstotliwoœæ próbkowania
SPS = 10;
%% ---------------------------------------------------------- %

%% ------------------- Obiekt ------------------- %
% Obiekt symulowany posiada transmitancjê typu:
%          -(sT)  L(s) 
% G(s) = e^   *  -----
%                 M(s)
% L - wektor wspó³czynników licznika
% M - wektor wspó³czynników mianownika
% T - opóŸnienie
L = [3];
M = [2, 1, 1];
T = 3 * 1/SPS;    % opóŸnienie o n próbek
%% ---------------------------------------------- %

%% ------------------- Symulacja ------------------- %
% Transmitancja obiektu ci¹g³ego
obiekt_ciag = tf(L, M);

% Transmitancja obiektu dyskretnego
obiekt_dysk = c2d(obiekt_ciag, 1/SPS);

% Symulujemy obiekt, by otrzymaæ wartoœci wektora dyskregnego
% odpowiadaj¹cego wejœciom i wyjœciom uk³adu
% (uwzglêdnione opóŸnienie)
[to, Yo, Uo] = TestujObiekt();

%% ------------------------------------------------- %

%% ------------------- Dalsza obródka wektorów wej-wyj ------------------- %
% Wyznaczenie opóŸnienia obiektu, jako iloœci próbek
% pomiêdzy wejœciem na obiekt, a jego reakcj¹
d = WyznaczOpoznienie(Uo, Yo);

% Wyznaczenie wspó³czynników obiektu metod¹ identyfikacji
% ARMA
% M - stopieñ licznika = stopieñ mianownika
St = 10;
[Md, Ld] = IdentyfikujObiekt(Uo, Yo, St, d);
%% -----------------------------------------------------------------------

%% ------------------- Dobór nastaw do regulatora PID ------------------- %
[Kp, Ti, Td] = DobierzNastawy(Md, Ld, SPS, d);
%% ---------------------------------------------------------------------- %

%% ------------- Wykonaj skok na obiekcie z regulatorem ------------------%
[treg, Ywyj, Yreg] = TestujRegulator();
%% ---------------------------------------------------------------------- %

%% ------------- Rysuj odpowiedzi uk³adu ------------------%
% RysujOdpowiedzi();
%% ---------------------------------------------------------------------- %
