%% ------------------- Opis funkcji ------------------- %%
% Funkcja s³u¿y do rysowania i graficznego rysowania odpowiedzi
% uk³adu zidentyfikowanego i oryginalnego
%% --------------------------------------------------- %%

function RysujOdpowiedzi()
global L M Ld Md SPS Ywyj Yreg;
figure('Name', 'Porównanie odpowiedzi skokowych');
subplot(2,1,1); step(tf(L, M)); grid on; title('Obiekt oryginalny');
subplot(2,1,2); step(d2c(tf(Ld, [1 Md], 1/SPS))); grid on; title('Obiekt zidentyfikowany');
figure('Name', 'Odpowiedzi uk³adu z regulatorem');
subplot(2,1,1); plot(Yreg); grid on; title('Wyjœcie z regulatora');
subplot(2,1,2); plot(Ywyj); grid on; title('Wyjœcie z obiektu');
end