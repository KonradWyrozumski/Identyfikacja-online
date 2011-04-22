function [teta, P, fi] = InicjalizujM(Y, U, M, d)
teta = 0.1*ones(2*M + 1 ,1);
P = 1000*eye(2*M + 1, 2*M + 1);
fi = [-Y(M + d: -1 : 1 + d)' U(M + 1: -1 : 1, 1)']';
end