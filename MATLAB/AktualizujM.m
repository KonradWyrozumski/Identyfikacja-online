%% ------------------- Opis funkcji ------------------- %%
% Funkcja zwraca zmienione warto�ci macierzy teta, fi, P
% oraz b��d et.
% Na wej�cie oczekuje podania wektor pr�bek wej��-wyj��
% oraz macierzy, kt�re maj� zosta� zaktualizowane.
%% ---------------------------------------------------- %%                    

function [teta, fi, P, et] = AktualizujM(Y, Yt, Ut, teta, fi, P, M)
    fi = [-Yt; fi(1:M-1); Ut; fi(M+1:length(fi)-1)];
    X = 1+fi'*P*fi;
    et = Y - fi'*teta;
    teta = teta + P*fi*et/X;
    P = P - P*fi*fi'*P/X;
end