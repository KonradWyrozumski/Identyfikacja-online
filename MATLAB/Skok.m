%% ------------------- Opis funkcji ------------------- %%
% Funkcja przyjmuje 2 wektory:
% function [U Y] = generuj( M, L, N, delay )
% M - wektor wspolczynnikow w mianowniku M(q)
% L - wektor wspolczynnikow w liczniku L(q)
% N - liczba probek
% delay - opoznienie dla skoku
%
% Funkcja zwraca dwa wektory: U oraz Y,
% gdzie U to wektor wejsc (skok jednostkowy)
% natomiast Y wektor wyjœæ
%% --------------------------------------------------- %%

function [Y, U, T] = Skok ( M, L, N, delay, SPS, d)
if( delay > N )
    disp('Zbyt duze opoznienie!');
    return;
end

[~, m] = size( L );   % m - rozmiar wektora licznika
[~, n] = size( M );   % n - rozmiar wektora mianownika

U = [zeros(delay,1)' ones(N-delay, 1)'];    % utworz wektor skoku
Y = zeros(1,N);
T = [0: 1/SPS : N/SPS - 1/SPS];
for i = 1:N - d
   for j = 1 : n
       if(i > j)
           Y( i + d ) = Y( i + d ) - M( j ) * Y( i - j + d);
       end
   end
   for j = 1 : m
       if(i > j)
           Y( i + d ) = Y( i + d ) + L( j )*U( i - j + 1);
       end
   end
end
