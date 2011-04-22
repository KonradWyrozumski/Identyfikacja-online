%% ------------------- Opis funkcji ------------------- %%
% Funkcja zwraca model o zadanym stopniu w postaci dwóch
% wektorów (Licznik oraz Mianownik) i wartoœæ kryterium
% AIC.
%% ---------------------------------------------------- %%
function [A, B, AIC] = ARMA(Y, U, M, n, d)
%% ------------------- Inicjalizacja macierzy ------------------- %%
[teta, P, fi] = InicjalizujM(Y, U, M, d);
sigma = 0;
%% -------------------------------------------------------------- %%

%% ------------------- Wyznacz wspó³czynniki ------------------- %%
for i = M + 2 + d:n
    [teta, fi, P, et] = AktualizujM(Y(i), Y(i-1), U(i-d), teta, fi, P, M);
    sigma = sigma + et^2;
end
%% ------------------------------------------------------------- %%
% Kryterium informacyjne AIC, przerobione, za³: log funkcja monotoniczna
AIC = sigma + 2*M;
% Wyci¹gnij A, B z macierzy teta
A = teta(1 : M , 1);
B = teta(M + 1 : 2*M + 1, 1);