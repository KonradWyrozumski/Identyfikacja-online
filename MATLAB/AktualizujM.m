%% ------------------- Opis funkcji ------------------- %%
% Funkcja zwraca zmienione wartoœci macierzy teta, fi, P
% oraz b³¹d et.
% Na wejœcie oczekuje podania wektor próbek wejœæ-wyjœæ
% oraz macierzy, które maj¹ zostaæ zaktualizowane.
%% ---------------------------------------------------- %%                    

function [teta, fi, P, et] = AktualizujM(Y, Yt, Ut, teta, fi, P, M)
    fi = [-Yt; fi(1:M-1); Ut; fi(M+1:length(fi)-1)];
    X = 1+fi'*P*fi;
    et = Y - fi'*teta;
    teta = teta + P*fi*et/X;
    P = P - P*fi*fi'*P/X;
end