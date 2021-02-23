function f = Tournament_Selection(chromosome, pool_size, tour_size)
% in this process we select the mating population on the basis of
% non-domination rank and crowding factor
[pop, variables] = size(chromosome);
% The peunltimate element contains the information about rank.
rank = variables - 1;
% The last element contains information about crowding distance.
distance = variables;

% Until the mating pool is filled, perform tournament selection
for i = 1 : pool_size
    for j = 1 : tour_size
        candidate(j) = round(pop*rand(1));
        if candidate(j) == 0
            candidate(j) = 1;
        end
        if j > 1
            % if same candidate chosen then choosing another one 
            while ~isempty(find(candidate(1 : j - 1) == candidate(j)))
                candidate(j) = round(pop*rand(1));
                if candidate(j) == 0
                    candidate(j) = 1;
                end
            end
        end
    end
    % candidate's information
    for j = 1 : tour_size
        c_obj_rank(j) = chromosome(candidate(j),rank);
        c_obj_distance(j) = chromosome(candidate(j),distance);
    end
    min_candidate = ...
        find(c_obj_rank == min(c_obj_rank));
    % if minimum canditate not single then checking on crowding factor
    % basis
    if length(min_candidate) ~= 1
        max_candidate = ...
        find(c_obj_distance(min_candidate) == max(c_obj_distance(min_candidate)));
        % select a particular solution from these candidates
        if length(max_candidate) ~= 1
            max_candidate = max_candidate(1);
        end
        %Adding individual
        f(i,:) = chromosome(candidate(min_candidate(max_candidate)),:);
    else
        f(i,:) = chromosome(candidate(min_candidate(1)),:);
    end
end
