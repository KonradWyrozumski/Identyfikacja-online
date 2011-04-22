%% ------------------- Opis funkcji ------------------- %%
% Funkcja zwraca wektory wspó³czynników zidentyfikowanego
% obiektu, którego stopieñ licznika i mianownika wynosi M.
% Zwracany jest ten obiekt, którego wartoœæ wspó³czynnika
% AIC jest najmniejsza.
%% ---------------------------------------------------- %%                    

function [M_act, L_act] = IdentyfikujObiekt(U, Y, St, d)
AIC = inf;
for i=2:St
%% Wyznacz model ARMA dla obiektu o stopniu licznika i mianownika i
    [M, L, AIC_act] = ARMA(Y, U, i, 30, d);
%% Porównanie modelu z modelem o aktualnie najmniejszym kryterium b³êdu
    if(AIC_act < AIC)
        AIC = AIC_act;
        M_act = M;
        L_act = L;
%        St = i;
    end
end
M_act = M_act';
L_act = L_act';
end