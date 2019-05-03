load('all_individuals_duplicates_removed');
POPULATION_SIZE = size(all_individuals,1);

%find maximum front
maxFront = 0;
for i=1:1:POPULATION_SIZE
   if (all_individuals(i,5) > maxFront) && (all_individuals(i,3) < 999999) && (~isnan(all_individuals(i,3)))
       maxFront = all_individuals(i,5);
   end
end

%find maximum complexity
maxComplexity = 0;
for i=1:1:POPULATION_SIZE
   if (all_individuals(i,4) > maxComplexity) && (all_individuals(i,3) < 999999) && (~isnan(all_individuals(i,3)))
       maxComplexity = all_individuals(i,4);
   end
end

%find maximum fitness
maxImplausibility = 0;
for i=1:1:POPULATION_SIZE
   if (all_individuals(i,3) > maxImplausibility) && (all_individuals(i,3) < 999999) && (~isnan(all_individuals(i,3)))
       maxImplausibility = all_individuals(i,3);
   end
end

load('all_individuals');
POPULATION_SIZE = size(all_individuals,1);
offlineConvergence = zeros(100, 1);
onlineConvergence = zeros(100,1);
cumulativeFilledArea = zeros(maxComplexity, round(maxImplausibility*10));%find the size of the area occupied for all cumulative generations.

for j=1:1:100 %j is the generation
    filledArea = zeros(maxComplexity, round(maxImplausibility*10)); %find the size of the area occupied this generation.
    for i=1:1:POPULATION_SIZE %i is used to iterate through the individuals
       if (all_individuals(i,1)+1 == j) && (all_individuals(i,3) < 999999) && (~isnan(all_individuals(i,3)))
           filledArea(all_individuals(i,4):1:maxComplexity, round(all_individuals(i,3)*10):1:round(maxImplausibility*10)) = 1;
           cumulativeFilledArea(all_individuals(i,4):1:maxComplexity, round(all_individuals(i,3)*10):1:round(maxImplausibility*10)) = 1;
       end
    end
    offlineConvergence(j) = sum(sum(cumulativeFilledArea));
    onlineConvergence(j) = sum(sum(filledArea));
end

%plot results.
figure(2);
clf;
plot(1:1:100, offlineConvergence);
title('offline convergence');
xlabel('(cumulative) generation');
ylabel('S');
grid on;

figure(3);
clf;
plot(1:1:100, onlineConvergence);
title('online convergence');
xlabel('generation');
ylabel('S');
grid on;

figure(4);
clf;
subplot(1,2,1);
imshow(cumulativeFilledArea);
subplot(1,2,2);
imshow(filledArea);
