%% ------------------- Opis funkcji ------------------- %%
% Funkcja zwraca model o zadanym stopniu w postaci dw�ch
% wektor�w (Licznik oraz Mianownik) i warto�� kryterium
% AIC.
%% ---------------------------------------------------- %%
function [A, B, AIC] = ARMA(Y, U, M, n, d)
%% ------------------- Inicjalizacja macierzy ------------------- %%
[teta, P, fi] = InicjalizujM(Y, U, M, d);
sigma = 0;
%% -------------------------------------------------------------- %%

%% ------------------- Wyznacz wsp�czynniki ------------------- %%
for i = M + 2 + d:n
    [teta, fi, P, et] = AktualizujM(Y(i), Y(i-1), U(i-d), teta, fi, P, M);
    sigma = sigma + et^2;
end
%% ------------------------------------------------------------- %%
% Kryterium informacyjne AIC, przerobione, za�: log funkcja monotoniczna
AIC = sigma + 2*M;
% Wyci�gnij A, B z macierzy teta
A = teta(1 : M , 1);
B = teta(M + 1 : 2*M + 1, 1);