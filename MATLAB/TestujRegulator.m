function [treg, Ywyj, Yreg] = TestujRegulator()
global step_time;
step_ = 1e-2;
step_time = 10;
model = 'obiekt_regulator';
simOpt = simset('AbsTol', 'auto', ...
                'Solver', 'ode3', ...
                'FixedStep', step_);
simTime = [0, 50];
sim(model, simTime, simOpt);

end