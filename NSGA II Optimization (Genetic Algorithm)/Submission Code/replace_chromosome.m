function f  = replace_chromosome(intermediate_chromosome, M, V,pop)

%% function f  = replace_chromosome(intermediate_chromosome,pro,pop)
% it replaces the chromosomes on the basis of rank and crowding

[N, ~] = size(intermediate_chromosome);

% Get the index for the population sort based on the rank
[~,index] = sort(intermediate_chromosome(:,M + V + 1));

% Now sort the individuals based on the index
for i = 1 : N
    sorted_chromosome(i,:) = intermediate_chromosome(index(i),:);
end
max_rank = max(intermediate_chromosome(:,M + V + 1));

previous_index = 0;
for i = 1 : max_rank 
    current_index = max(find(sorted_chromosome(:,M + V + 1) == i));
    % see if population is filled
    if current_index > pop
        remaining = pop - previous_index;
        temp_pop = ...
            sorted_chromosome(previous_index + 1 : current_index, :);
        % sort on basis of crowding distance in descending order to select
        % those candidates 
        [temp_sort,temp_sort_index] = ...
            sort(temp_pop(:, M + V + 2),'descend');
        % filling the population from these selected candidates on the
        % basis of crowding distance
        for j = 1 : remaining
            f(previous_index + j,:) = temp_pop(temp_sort_index(j),:);
        end
        return;
    elseif current_index < pop
        f(previous_index + 1 : current_index, :) = ...
            sorted_chromosome(previous_index + 1 : current_index, :);
    else
        f(previous_index + 1 : current_index, :) = ...
            sorted_chromosome(previous_index + 1 : current_index, :);
        return;
    end
    % hence getting the index for the last added individual
    previous_index = current_index;
end
