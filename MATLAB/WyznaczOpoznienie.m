%% ------------------- Opis funkcji ------------------- %%
% Funkcja zwraca op�nienie, b�d�ce liczb� pr�bek po
% kt�rej od podania na obiekt sygna�u mo�na oczekiwa� 
% odpowiedzi - wymagane jest to do identyfikacji ARMA
%% ---------------------------------------------------- %%                    

function d = WyznaczOpoznienie(U, Y)

%% Wyznaczamy po jakim czasie nast�pi�a zmiana na U
skokU = 0;
for i = 2:length(U)
    if(U(i) ~= U(i-1))
        skokU = i;
        break;
    end
end

%% Wyznaczamy po jakim czasie nast�pi�a zmiana na Y
% Zak�adamy, �e zmiana polega jakimkolwiek skoku (?)
% Nale�y to przedyskutowa� z promotorem!!
skokY = 0;
for i = 2:length(Y)
    if(Y(i) ~= Y(i-1))
        skokY = i;
        break;
    end
end

%% Op�nienie w ilo�ciach pr�bek to r�nica mi�dzy
% skokiem na Y a skokiem na U, odliczaj�c jedn� pr�bk�
% na czas 0.0 [s]
d = skokY - skokU;

% Je�li U(1) == 0 to musimy odj�� jeszcze jedn� pr�bk�,
% bo nie by�o stanu ustalonego
if(U(1) == 0)
    d = d - 1;
end
end