%% ------------------- Opis funkcji ------------------- %%
% Funkcja zwraca wektory t, Y_dysk, U_dysk
% t - sprobkowany czas z symulacji
% Y_dysk, U_dysk - wektory spr�bkowane z czasem pr�bkowania zwi�zanym
%                     z SPS z g��wnej funkcji
%% ---------------------------------------------------- %%                    
function [t, Yob, Uob] = TestujObiekt()

%% ------------------- Ustawienia symulacji ------------------- %
global step_;
step_ = 1e-2;
simOpt = simset('AbsTol', 'auto', ...
                'Solver', 'ode3', ...
                'FixedStep', step_);
simTime = [0, 50];
model = 'obiekt';

% Initial Seed dla Random number
global iS SPS;
iS = abs(ceil((randn * 10)+10));

%% ------------------------------------------------------------ %
%% ------------------- Symulacja ------------------- %
sim(model, simTime, simOpt);
%% ------------------------------------------------- %

%% ------------------- Probkowanie sygna�u ci�g�ego ------------------- %
SPS_ = 1/step_ / SPS;   
Yob = downsample(Y_ciag, SPS_);
Uob = downsample(U_ciag, SPS_);
%% -------------------------------------------------------------------- %
end