function [Y, U, T] = GeneratorWeWy ( M, L, N, SPS, d)
[~, m] = size( L );   % m - rozmiar wektora licznika
[~, n] = size( M );   % n - rozmiar wektora mianownika

U = ceil((rand(N,1)*100));    % utworz wektor skoku
Y = zeros(N,1);
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

for i = 1:N
   fprintf('  Y->matrix[%d][0] = %d;\n', i-1, Y(i));
end
fprintf('\n\n');
for i = 1:N
   fprintf('  U->matrix[%d][0] = %d;\n', i-1, U(i));
end


