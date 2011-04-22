%% ------------------- Opis funkcji ------------------- %%
% Funkcja zwraca wektory wsp�czynnik�w zidentyfikowanego
% obiektu, kt�rego stopie� licznika i mianownika wynosi M.
% Zwracany jest ten obiekt, kt�rego warto�� wsp�czynnika
% AIC jest najmniejsza.
%% ---------------------------------------------------- %%                    

function [M_act, L_act] = IdentyfikujObiekt(U, Y, St, d)
AIC = inf;
for i=2:St
%% Wyznacz model ARMA dla obiektu o stopniu licznika i mianownika i
    [M, L, AIC_act] = ARMA(Y, U, i, 30, d);
%% Por�wnanie modelu z modelem o aktualnie najmniejszym kryterium b��du
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