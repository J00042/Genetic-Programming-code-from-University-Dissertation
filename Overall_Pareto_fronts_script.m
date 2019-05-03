%file format: generation, individual, fitness/implausibility, complexity, Pareto front, crowding rank.

load('all_individuals.mat');
POPULATION_SIZE = size(all_individuals,1);

%2) Calculate the fitness and complexity for each individual. 
all_individuals(1:POPULATION_SIZE, 5) = -1;
all_individuals(1:POPULATION_SIZE, 6) = 0;
all_individuals(1:POPULATION_SIZE, 7) = 0;
%use column 7 for np, use column 5 for front. 

%sample the unique members:
unique_individuals = all_individuals(1,:);
j=1;
for p=1:1:POPULATION_SIZE
    uni = 1;
    for q=1:1:size(unique_individuals,1)
        if (all_individuals(p,3) == unique_individuals(q,3)) && (all_individuals(p,4) == unique_individuals(q,4))
            uni = 0; 
            break;
        end
    end
    if uni
        unique_individuals(j,:) = all_individuals(p,:);
        j = j + 1;
    end
end

%Comment these two lines if you do not want to remove duplicate
%individuals when calculating the Pareto fronts. 
%-----------------------------------------------------------------
all_individuals = unique_individuals;
POPULATION_SIZE = size(all_individuals,1);
%-----------------------------------------------------------------

% 3) Assign 2 values to each individual: The number of individuals that dominate said individual int np and int front. 
sizeOfFront1 = 0;
for p=1:1:POPULATION_SIZE	%each individual p in P[0]:
    disp(['p=', num2str(p)]);
    for q=1:1:POPULATION_SIZE	%each individual q in P[0]:
        if( ((all_individuals(p,3) > all_individuals(q,3)) && (all_individuals(p,4) >= all_individuals(q,4))) || ((all_individuals(p,3) >= all_individuals(q,3)) && (all_individuals(p,4) > all_individuals(q,4))) )
            all_individuals(p,7) = all_individuals(p,7)+1; %q dominates p, increase np by 1.
        else if( (isnan(all_individuals(p,3)) || (all_individuals(p,3) > 999999)) && ((~isnan(all_individuals(q,3))) && (all_individuals(q,3) <= 999999)) ) %if p fitness is nan and q fitness is not nan or 999999999, q dominates p. This should make all individuals that don't compile go to last front. 
                all_individuals(p,7) = all_individuals(p,7)+1;
            end
        end
    end
    if all_individuals(p,7) == 0
        all_individuals(p,5) = 1;
        sizeOfFront1 = sizeOfFront1 + 1;
    end
end
nextLowestNP=POPULATION_SIZE+1; 
individuals_unassigned_rank = POPULATION_SIZE - sizeOfFront1;
frontCount = 2;
while individuals_unassigned_rank > 0
    nextLowestNP=POPULATION_SIZE+1; 
    for p=1:1:POPULATION_SIZE %each individual p in P[0]:
        if ((all_individuals(p,5) == -1) && (all_individuals(p,7) <  nextLowestNP))
            nextLowestNP = all_individuals(p,7);   %basically find the lowest np value in the population for individuals that are unassigned a front. 
        end
    end
    for p=1:1:POPULATION_SIZE %each individual p in P[0]:
        if all_individuals(p,7) == nextLowestNP
            all_individuals(p,5) = frontCount;
            individuals_unassigned_rank = individuals_unassigned_rank - 1;
        end  %all individuals with this lowest np value will be assigned to the next front. 
    end
    frontCount = frontCount + 1;
    disp(['individuals unassigned rank =', num2str(individuals_unassigned_rank)]);
end
maxFront = frontCount - 1;
%all individuals should now be assigned a Pareto front.


%4) Assign a double crowdingRank to each individual by comparing its distance to all other individuals in the same front:
%   find the best and worst fitnesses and complexities in the population:
bestGenFitness = 999999999; worstGenFitness = 0; bestGenComplexity = 999999999; worstGenComplexity = 0;
for p=1:1:POPULATION_SIZE
    if all_individuals(p,3) > worstGenFitness
        worstGenFitness = all_individuals(p,3);
    end
    if all_individuals(p,3) < bestGenFitness
        bestGenFitness = all_individuals(p,3);
    end
    if all_individuals(p,4) > worstGenComplexity
        worstGenComplexity = all_individuals(p,4);
    end
    if all_individuals(p,4) < bestGenComplexity
        bestGenComplexity = all_individuals(p,4);
    end
end
%calculate the crowding rank for each individual in each front:
frontCount = 1; %now used to count what front is being investigated.
while frontCount <= maxFront
    bestFitness = 999999999;
    bestComplexity = 999999999; 
    bestFitnessForBestComplexity = 999999999;
    bestComplexityForBestFitness = 999999999;
    for p=1:1:POPULATION_SIZE %each individual p in P[0]: find and remember the best fitness and complexity of the current front.
        if (all_individuals(p,5) == frontCount) && (all_individuals(p,3) <= bestFitness)
            if all_individuals(p,3) < bestFitness
                bestFitness = all_individuals(p,3);
                bestComplexityForBestFitness = 999999999;
            end
            if all_individuals(p,4) < bestComplexityForBestFitness
                bestComplexityForBestFitness = all_individuals(p,4);
            end
        end
        if (all_individuals(p,5) == frontCount) && (all_individuals(p,4) <= bestComplexity)
            if all_individuals(p,4) < bestComplexity
                bestComplexity = all_individuals(p,4);
                bestFitnessForBestComplexity = 999999999;
            end
            if all_individuals(p,3) < bestFitnessForBestComplexity
                bestFitnessForBestComplexity = all_individuals(p,4);
            end
        end
    end
    for p=1:1:POPULATION_SIZE %each individual p in P[0]:
        if all_individuals(p,5) == frontCount
            %if the individual did not compile, set its crowding rank to 1. 
            if (isnan(all_individuals(p,3))) || (all_individuals(p,3) > 999999)
                all_individuals(p,6) = 1;
                continue;
            end
            %check for if p has the smallest fitness or complexity compared to all other individuals in the front and is on the Pareto boundary. If it does, set its crowdingRank to 999999999. 
            if(((all_individuals(p,3) == bestFitness) && (all_individuals(p,4) == bestComplexityForBestFitness)) || ((all_individuals(p,4) == bestComplexity) && (all_individuals(p,4) == bestFitnessForBestComplexity)))
                all_individuals(p,6) = 999999999; 
                continue;
            end
            %otherwise, set the crowdingRank to the sum of, the sum of the distances to each nearest neighbour, for each objective. 
            all_individuals(p,6) = 0; 
            nextBestComplexity = 0; nextBestFitness = 0; nextWorstComplexity = 0; nextWorstFitness = 0; 
            nextBestFitnessDistance = 999999999; nextBestComplexityDistance = 999999999; nextWorstComplexityDistance = 999999999; nextWorstFitnessDistance = 999999999;
            for q=1:1:POPULATION_SIZE %each individual q in P[0]:  %check the distance to each individual in the same front and remember the smallest one. 
                if (all_individuals(p,5) == frontCount) && (p ~= q)
                    if (all_individuals(p,3) <= all_individuals(q,3)) && (all_individuals(q,3) - all_individuals(p,3) < nextWorstFitnessDistance)
                        nextWorstFitnessDistance = all_individuals(q,3) - all_individuals(p,3);
                        nextWorstFitness = all_individuals(q,3);
                    end
                    if (all_individuals(p,3) >= all_individuals(q,3)) && (all_individuals(p,3) - all_individuals(q,3) < nextBestFitnessDistance)
                        nextBestFitnessDistance = all_individuals(p,3) - all_individuals(q,3);
                        nextBestFitness = all_individuals(q,3);
                    end
                    if (all_individuals(p,4) <= all_individuals(q,4)) && (all_individuals(q,4) - all_individuals(p,4) < nextWorstComplexityDistance)
                        nextWorstComplexityDistance = all_individuals(q,4) - all_individuals(p,4);
                        nextWorstComplexity = all_individuals(q,4);
                    end
                    if (all_individuals(p,4) >= all_individuals(q,4)) && (all_individuals(p,4) - all_individuals(q,4) < nextBestComplexityDistance)
                        nextBestComplexityDistance = all_individuals(p,4) - all_individuals(q,4);
                        nextBestComplexity = all_individuals(q,4);
                    end
                end
            end
            if (worstGenFitness == bestGenFitness) || (worstGenComplexity == bestGenComplexity)
                distance = 0;
            else
                distance = abs((((nextWorstFitness - nextBestFitness)/(worstGenFitness - bestGenFitness)) + ((nextWorstComplexity - nextBestComplexity) / (worstGenComplexity - bestGenComplexity))));
                %distance = ((nextWorstFitness - nextBestFitness)/(worstGenFitness - bestGenFitness));
            end
            all_individuals(p,6) = distance;
        end
    end
    frontCount = frontCount + 1;
end

disp(['number of fronts = ', num2str(maxFront)]);

figure(1);
clf;
frontCount = 1;
colour = ['r', 'g', 'c', 'b', 'm', 'k', 'r', 'g', 'c', 'b', 'm'];
for frontCount=1:1:1 %fronts to plot
    i = 0;
    for p=1:1:POPULATION_SIZE
        if all_individuals(p,5) == frontCount
            i = i + 1;
        end
    end
    points = zeros(i,2);
    j=1;
    for p=1:1:POPULATION_SIZE
        if all_individuals(p,5) == frontCount
            points(j,1) = all_individuals(p,3);
            points(j,2) = all_individuals(p,4);
            j = j + 1;
        end
    end
    hold on;
    points = sortrows(points);
    plot(points(:,1), points(:,2), ['x-',colour(frontCount)]);
end
xlabel('implausibility');
ylabel('complexity');
grid on;
title('Pareto front 1 of the total population')
legend('Pareto front 1', '2', '3', '4', '5', '6', '7', '8', '9', '10');
%}