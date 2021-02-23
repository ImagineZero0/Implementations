function NSGA_II(pop,gen)

%% function nsga_2(pop,gen)
% is a multi-objective optimization function where the input arguments are 
% pop - Population size
% gen - Total number of generations
% 
% This functions is based on evolutionary algorithm for finding the optimal
% solution for multiple objective i.e. pareto front for the objectives. 

pop = round(pop);
gen = round(gen);
%% Initialising the problem with the required parameters
% Here we give the the required values to the variables to evaluate the
% pareto optimal solutions
%Here,
%*M = number of objectives,which are surface finish and power measure
%*V = number of decision variables, which are feed rate , spindle speed and
%depth of cut
%*LB = stores the minimum of all the decision variables as given in
%constraints i.e. Lower Bound
%*UB = stores the maximum of all the decision variables as given in
%constraints i.e. Upper Bound
M = 2;
V = 3;
LB(1) = 20;
LB(2) = 250;
LB(3) = 0.20;
UB(1) = 30;
UB(2) = 550;
UB(3) = 1.4;

%% Initialize the population
% Population is initialized with random values which are within the
% specified range.
% K is the total number of array elements.
K = M + V;
for i = 1 : pop
    % Initialize the decision variables based on the minimum and maximum
    % possible values.
    for j = 1 : V
        population(i,j) = LB(j) + (UB(j) - LB(j))*rand(1);
    end
    % For ease of computation and handling data the chromosome also has the
    % value of the objective function concatenated at the end. The elements
    % V + 1 to K has the objective function valued. 
    population(i,V + 1: K) = Multi_Objective(population(i,:), V);
end
%% Sort the initialized population
% Sort the population using non-domination-sort. 
population = Non_Domination_Sorting(population, M, V);

%% Start the evolution process
% here we select the best solutions and then perform reproduction to form
% new generation 

for i = 1 : gen
    % pool - size of the mating pool. 
    % tour - Tournament size.
    pool = round(pop/2);
    tour = 2;
    % A binary tournament selection is employed in NSGA-II.
    parent_chromosome = Tournament_Selection(population, pool, tour);
    
    % Perfrom SBX crossover and Polynomial Mutation operator
    % The distribution indices for crossover and mutation operators as mu = 20
    % and mum = 20 respectively.
    mu = 20;
    mum = 20;
    offspring_chromosome = ...
        Genetic_Operator(parent_chromosome, ...
        M, V, mu, mum, LB, UB);  
    [main_pop,~] = size(population);
    [offspring_pop,~] = size(offspring_chromosome);
    % intermediate_chromosome is a concatenation of current population and
    % the offspring population.
    intermediate_chromosome(1:main_pop,:) = population;
    intermediate_chromosome(main_pop + 1 : main_pop + offspring_pop,1 : M+V) = ...
        offspring_chromosome;

    % Non-domination-sort of intermediate population so that we could save
    % time on total computation and not do sort for the whole generation
 
    intermediate_chromosome = ...
        Non_Domination_Sorting(intermediate_chromosome, M, V);
    % Now we perform selection by selecting on the basis of rank and then
    % crowding distance
    population = replace_chromosome(intermediate_chromosome, M, V, pop);
end

% Visualizing the result
plot(population(:,V + 1),population(:,V + 2),'o');
    
