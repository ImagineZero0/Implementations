%% This is the Objective function which needs to be optimised to find the pareto front or the optimal pareto solutions
% We make the function as follows and hence evaluate the pareto front
% We provide the requried variables so that they could be inputted as the
% same in question i.e. in vector form
function Fitness = Multi_Objective(Variables,~)
Fitness(1) = 2.31 + 0.66*Variables(1) - 0.12*Variables(2) - 0.15*Variables(3) - 0.11*Variables(1)*Variables(2) + 0.097*Variables(1)*Variables(3) + 0.15*Variables(2)*Variables(3) - 0.17*Variables(1)^2 - 0.47*Variables(2)^2 - 0.17*Variables(3)^2;
Fitness(2) = 1039.38 + 7.50*Variables(1) + 127.00*Variables(1)*Variables(2) + 35.50*Variables(3) - 1.25*Variables(1)*Variables(2) + 3.75*Variables(1)*Variables(3) + 1.25*Variables(2)*Variables(3);
end
% Here Fitnees(1) is the Surface Finish which needs to be optimised 
% Here Fitness(2) is the Power measure which needs to be optimised 
% Variables(1) denote the Feed Rate
% Variables(2) denote the Spindle Speed
% Variables(3) denote the Depth of Cut
% These are the variables on which the given objective functions would be
% optimised and hence we would find our pareto optimal solutions after many
% generations.