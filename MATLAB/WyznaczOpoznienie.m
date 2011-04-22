%% ------------------- Opis funkcji ------------------- %%
% Funkcja zwraca opóŸnienie, bêd¹ce liczb¹ próbek po
% której od podania na obiekt sygna³u mo¿na oczekiwaæ 
% odpowiedzi - wymagane jest to do identyfikacji ARMA
%% ---------------------------------------------------- %%                    

function d = WyznaczOpoznienie(U, Y)

%% Wyznaczamy po jakim czasie nast¹pi³a zmiana na U
skokU = 0;
for i = 2:length(U)
    if(U(i) ~= U(i-1))
        skokU = i;
        break;
    end
end

%% Wyznaczamy po jakim czasie nast¹pi³a zmiana na Y
% Zak³adamy, ¿e zmiana polega jakimkolwiek skoku (?)
% Nale¿y to przedyskutowaæ z promotorem!!
skokY = 0;
for i = 2:length(Y)
    if(Y(i) ~= Y(i-1))
        skokY = i;
        break;
    end
end

%% OpóŸnienie w iloœciach próbek to ró¿nica miêdzy
% skokiem na Y a skokiem na U, odliczaj¹c jedn¹ próbkê
% na czas 0.0 [s]
d = skokY - skokU;

% Jeœli U(1) == 0 to musimy odj¹æ jeszcze jedn¹ próbkê,
% bo nie by³o stanu ustalonego
if(U(1) == 0)
    d = d - 1;
end
end