%% ------------------- Opis funkcji ------------------- %%
% Funkcja identyfikuje wspolczynniki regulatora PID,
% na wejscie wymaga podania wektorow wspolczynnikow 
% licznika i mianownika transmitancji, a takze szybkosci
% probkowania
%% ---------------------------------------------------- %%                    

function [Kp, Ti, Td] = DobierzNastawy(M, L, SPS, d)

[Y U T] = Skok(M, L, 500, 10, SPS, d);
Y_sp = Y;
U_sp = U;
%plot(T, Y), hold on;
% Wartosc ustabilizowanea w C trzeba by sprawdzac czy juz sie
% ustabilizowalo
% Y_stb = Y_sp(length(Y_sp));
% Y_0 = Y_sp(1);
%U_stb = U_sp(length(U_sp));
R_sp = U_sp;
R_stb = R_sp(length(R_sp));
R_0 = R_sp(1);
t_sp = T;

t_r_start = -1;
%t_y_start = -1;

% Kiedy byl skok - zmiana o 5%
for i=2:length(t_sp)
    if abs(R_sp(i)-R_0) > 0.05*R_stb && t_r_start == -1
        t_r_start = t_sp(i);
    end
    
%     if abs(Y_sp(i)-Y_0) > 0.05*Y_stb && t_r_start ~= -1
%         t_y_start = t_sp(i);    
%         break;
%     end
end

% Opoznienie
%Tdead = t_y_start - t_r_start;

%% Obiekt z cz�onem ca�kuj�cym
if(abs(Y_sp(length(Y_sp)) - Y_sp(ceil(0.9*length(Y_sp)))) > 0.05*Y_sp(length(Y_sp)))
    % oblicz nachylenie prostej
    k = (Y_sp(ceil(0.5*length(Y_sp))) - Y_sp(ceil(0.5*length(Y_sp)) - 1))/((T(ceil(0.5*length(Y_sp))) - T(ceil(0.5*length(Y_sp)) - 1)));
    a = Y_sp(ceil(0.5*length(Y_sp))) - k*T(ceil(0.5*length(Y_sp)));
    T0 = -a/k;
%     Kp = 1.2 / abs(b);
%     Ti = 2 * L;
%     Td = 0.5*L;
else
%% Obiekt bez cz�onu ca�kuj�cego    
    % Rozniczkuj sygnal wyj�ciowy by znale�� przegi�cie
    K =  diff(Y_sp);
    % Szukaj maksymalnej warto�ci pochodnej - tam jest przegi�cie
    [~, y] = max(diff(Y_sp));
    % Wspolczynnik stycznej k punkty wzi�te dla otoczenia przegi�cia
    k = (Y_sp(y+1) - Y_sp(y))/(T(y+1) - T(y));
    % a - punkt przeci�cia
    a = Y_sp(y) - k * T(y);
    T0 = -a/k;
end
%% Nastawy s�abe, du�e przeregulowanie - 20%
%    Kp = 1.2 / abs(a);
%    Ti = 2*T0;
%    Td = 0.5*T0;
%% Nastawy daj�ce minimaln� warto�� wska�nika jako�ci regulacji ISE
%    Kp = 1.4/abs(a);
%    Ti = 1.3*T0;
%    Td = 0.5*T0;
%% Nastawy daj�ce minimalne przeregulowanie - teoretycznie 0%
    Kp = 0.95/abs(a);
    Ti = 2.4*T0;
    Td = 0.4*T0;
end