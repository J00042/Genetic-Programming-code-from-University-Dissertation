/* main.cpp */
/* to run this program, open linux terminal, use cd to navigate to the directory where this cpp file is stored, then enter:

g++ -o main.out main.cpp FileEdit.cpp Individual.cpp Generation.cpp GP.cpp
./main.out	

*/

#include "FileEdit.h"
#include "Individual.h"
#include "Generation.h"
#include "GP.h"

#include <fstream>
#include <string>
#include <stdio.h>
#include <stdlib.h>	/* system, NULL, EXIT_FAILURE */
#include <cmath>

#define GENERATIONS 100 //Change to 100 or so later. 

using namespace std;

int debug = 0;

int runGeneticProgram(int generations);	//declare a method to run a genetic program. 
int testPrimitiveGenerator();
int testFunction(int function);	//1 for reproduction, 2 for crossover, 3 for subtree mutation, 4 for point mutation.
int makeInitialModel(string roles_path);	//declare a method to create an individual contatining the initial model and apply it to the C++ model.
int testInitialModel(string roles_path);	//declare a method to create an individual containing the initial model, apply it to the C++ model and run the model.
int testFileEdit();	//declare a method to test the FileEdit.h class on the test.cpp file. 
int testCppEquation();	//declare a method to test the storage and C++ conversion of the original roles model. 
int testCSVReading();
int testFitnessCalculation();
int testIdenticalFunction();

int performGP_Pareto(int numberOfGenerations, double crossoverRate, double mutationRate, double pointMutationRate);
int performGP(int numberOfGenerations, double crossoverRate, double mutationRate, double pointMutationRate);
int performMultiCoreGP(int noOfCores, int numberOfGenerations, double crossoverRate, double mutationRate, double pointMutationRate);

Generation gen[GENERATIONS];	//create an array of generations of individuals.
Generation Q[GENERATIONS];

int main(){	
	performGP_Pareto(GENERATIONS, 0.8, 0.2, 0.1);
	//performGP(GENERATIONS, 0.7, 0.2, 0.1);
	//performMultiCoreGP(GENERATIONS, 2, 0.7, 0.2, 0.1);
	//testInitialModel("/home/jack/roles-gp/roles/");
	//testFunction(2);
	//testCSVReading();
	//compileAndRunModel("/home/jack/roles-gp/roles");
	//runModelJustCompileRT("/home/jack/roles-gp/roles");
	//testFitnessCalculation();
	//testPrimitiveGenerator();
	//testCppEquation();
	//testIdenticalFunction();

	return 0;
}

int performGP_Pareto(int numberOfGenerations, double crossoverRate, double mutationRate, double pointMutationRate){
	//set up variables.
	if(POPULATION_SIZE <= 1){
		cout << "Population size is " << POPULATION_SIZE << ", which is too small for GP." << endl;
			return -1;
	}
	cout << "Running a GP with " << numberOfGenerations << " generations, " << POPULATION_SIZE << " individuals per gen.\nCrossover rate: " << crossoverRate << ", Mutation rate: " << mutationRate << "." << endl;
	int i, j, k, p, q, r, rQ, t, tQ, individual1, individual2, individualChosen1, individualChosen2, next, bestIndividual, sizeOfFront1, nextLowestNP, individuals_unassigned_rank, frontCount, maxFront, bestComplexity, bestComplexityForBestFitness, nextBestComplexity, nextWorstComplexity, bestGenComplexity, worstGenComplexity;
	double random, bestFitness, bestFitnessForBestComplexity, distance, bestCrowdingRank, lastCrowdingRank, nextBestFitness, nextWorstFitness, nextBestFitnessDistance, nextBestComplexityDistance, nextWorstFitnessDistance, nextWorstComplexityDistance, bestGenFitness, worstGenFitness;
	string s;
	//srand(1);	//COMMENT THIS LINE AFTER TESTING.
	srand(time(NULL));	//deliver a seed to initialise the RNG.
	
	//apply the initial model to the model and attempt to compile it. 
	try{
		makeInitialModel("/home/jack/roles-gp/roles/");
		compileModel("/home/jack/roles-gp/roles/");
	}catch(...){
		cout << "model could not be compiled. Aborting." << endl;
		return -1;
	}
	
	//1) Create a generation object P (gen) containing the initial model, and heavily mutated versions of it (i.e. like creating a random initial population). 
	//Also create an empty generation object Q containing generations with empty slots for individuals.
	for(i=0; i<POPULATION_SIZE; i++){	//assign the initial model to each individual in generation 0 (the initial).
		gen[0].applyInitialModel(i);
	}
	for(i=1; i<POPULATION_SIZE; i++){	//heavily mutate all of gen 0 except the first individual. 
		pointMutation(0.2, gen[0], i, gen[0], i);
	}
	
	
	i=0;
	//2) Calculate the fitness and complexity for each individual. 
	for(j=0; j<POPULATION_SIZE; j++){	//for each individual in the intial generation:
		cout << "generation " << i << " individual " << (j+1) << ":" << endl;
		applyIndividualToModel(gen[i], j, "/home/jack/roles-gp/roles/");	//set the model to contain the equations given by an individual.
		try{					//attempt to:
			if(runModelJustCompileRT("/home/jack/roles-gp/roles") != 0){		//run the model, just recompiling the RoleTheory.cpp file.
				gen[i].population[j].setFitness(999999999); 					//if there is an exception, set the fitness to 999,999,999. 
				cout << "generation " << i << " individual " << (j+1) << " could not be run (didn't return 0). Fitness set to: " << gen[i].population[j].getFitness() << endl;
			
			}else{
				gen[i].population[j].setFitness(calculateFitness("/home/jack/roles-gp/roles/"));	//calculate and assign the fitness
				cout << "generation " << i << " individual " << (j+1) << " had fitness " << gen[i].population[j].getFitness() << endl;
			}
		}catch(...){
			gen[i].population[j].setFitness(999999999); 						//if there is an exception, set the fitness to 999,999,999. 
			cout << "generation " << i << " individual " << (j+1) << " could not be run (compile failed). Fitness set to: " << gen[i].population[j].getFitness() << endl << endl;
		}
		//gen[i].population[j].setFitness((rand()%10));	//COMMENT THIS LINE AFTER TESTING.
		
		gen[i].population[j].calculateComplexity();
		cout << endl;
	}
	
	
	for(i=0; i<numberOfGenerations; i++){	//for each generation:
	//for(i=0; i<1; i++){
		cout << endl << "Iterating over Generation " << i << endl << endl;
		
		//3) Assign 2 values to each individual: The number of individuals that dominate said individual int np and int front. 
		//Note, after the initial generation, the fitness and complexity for each individual in the current generation will already be known, no need to re-calculate. 
		sizeOfFront1 = 0;
		//gen[i].printGenerationToFile(("./generations/generation" + to_string(i)));
		for(p=0; p<POPULATION_SIZE; p++){	//each individual p in P[0]:
			if(debug) cout << "Calculating np for gen[" << i << "].population[" << p << "]. \tPOPULATION_SIZE = " << POPULATION_SIZE << endl;
			gen[i].population[p].setNP(0);
			gen[i].population[p].setFront(-1); //front = -1 means a front is not assigned. 
			for (q=0; q<POPULATION_SIZE; q++){	//each individual q in P[0]:
				if(debug)	cout << "p = " << p << ". \tq = " << q << ", np = " << gen[i].population[p].getNP() << endl;
				if(debug)	cout << "p.fitness = " << gen[i].population[p].getFitness() << ", p.complexity = " << gen[i].population[p].getComplexity() << ", q.fitness = " << gen[i].population[q].getFitness() << ", q.complexity = " << gen[i].population[q].getComplexity() << endl;
				if( ((gen[i].population[p].getFitness() > gen[i].population[q].getFitness()) && (gen[i].population[p].getComplexity() >= gen[i].population[q].getComplexity())) || ((gen[i].population[p].getFitness() >= gen[i].population[q].getFitness()) && (gen[i].population[p].getComplexity() > gen[i].population[q].getComplexity())) ){
					gen[i].population[p].setNP(gen[i].population[p].getNP() + 1);  //q dominates p, increase np by 1.
					if(debug) cout << "q dominates" << endl;
				}else if( (isnan(gen[i].population[p].getFitness()) || (gen[i].population[p].getFitness() > 999999)) && ((!isnan(gen[i].population[q].getFitness())) && (gen[i].population[q].getFitness() <= 999999)) ){ //if p fitness is nan and q fitness is not nan or 999999999, q dominates p. This should make all individuals that don't compile go to last front. 
					gen[i].population[p].setNP(gen[i].population[p].getNP() + 1);
					if(debug) cout << "q dominates as p fitness is nan or 999999999 but q's is not" << endl;
				}
			}
			if(gen[i].population[p].getNP() == 0){
				gen[i].population[p].setFront(1);
				sizeOfFront1++;
			}
			if(debug)	cout << "gen " << i << " individual " << p << " has NP: " << gen[i].population[p].getNP() << endl << endl;
		}
		nextLowestNP=POPULATION_SIZE+1; 
		individuals_unassigned_rank = POPULATION_SIZE - sizeOfFront1;
		frontCount = 2;
		if(debug) cout << "nextLowestNP = " << nextLowestNP << ", individuals_unassigned_rank = " << individuals_unassigned_rank << " out of " << POPULATION_SIZE << endl;
		//I'm going to do it differently from the paper here. Rather than create an arrays of fronts using array Sp, I'm going to iterate through all the individuals assigning them fronts based on their np value. If individuals have the same np, they should be in the same front. 
		while(individuals_unassigned_rank > 0){
			nextLowestNP=POPULATION_SIZE+1; 
			for(p=0; p<POPULATION_SIZE; p++){ //each individual p in P[0]:
				if ((gen[i].population[p].getFront() == -1) && (gen[i].population[p].getNP() <  nextLowestNP)){
					nextLowestNP = gen[i].population[p].getNP();   //basically find the lowest np value in the population for individuals that are unassigned a front. 
				}
			}
			for(p=0; p<POPULATION_SIZE; p++){ //each individual p in P[0]:
				if(gen[i].population[p].getNP() == nextLowestNP){ 
					gen[i].population[p].setFront(frontCount); 
					individuals_unassigned_rank--;
				}  //all individuals with this lowest np value will be assigned to the next front. 
			}
			if(debug) cout << "for frontCount " << frontCount << ", nextLowestNP set to " << nextLowestNP << ". Individuals with no rank changed to " << individuals_unassigned_rank << endl;
			frontCount++;
		}
		maxFront = frontCount -1;
		if(debug){for(p=0; p<POPULATION_SIZE; p++){	cout << "gen " << i << " individual " << p << " is part of Pareto front: " << gen[i].population[p].getFront() << ".\t np: " << gen[i].population[p].getNP() << "\tFitness: " << gen[i].population[p].getFitness() << ", \tcomplexity: " << gen[i].population[p].getComplexity() << endl; }}
		//all individuals should now be assigned a Pareto front.

		//*) Iterate through each front and test for identical individuals. Any individuals found to be identical to previously examined individuals will be placed in the second-to-last Pareto front. 
		frontCount = 1; p=0; q=0; //frontCout now used to count what front is being investigated.
		if(maxFront-2 >= 1){
			while(frontCount <= maxFront-2){ //examine every front except the last two. 
				for(p=1; p<POPULATION_SIZE; p++){ //p is the individual in question.
					for(q=0; q<p; q++){ //q is a previously examined individual. 
						if((gen[i].population[p].getFront()==frontCount) && (gen[i].population[q].getFront()==frontCount) && (isIdentical(gen[i], p, gen[i], q))){ //If the individual in question is found to be identical to a previously examined individual, move it to the second-to-last Pareto front. 
							gen[i].population[p].setFront(maxFront-1);
							if(debug) cout << "individual " << p << " found to be identical to individual " << q << ", putting individual " << p << " in front " << gen[i].population[p].getFront() << endl;
							break;
						}
					}
				}
				frontCount++;
			}
		}

		//4) Assign a double crowdingRank to each individual by comparing its distance to all other individuals in the same front:
		//find the best and worst fitnesses and complexities in the population:
		bestGenFitness = 999999999; worstGenFitness = 0; bestGenComplexity = 999999999; worstGenComplexity = 0;
		for(p=0; p<POPULATION_SIZE; p++){
			if(gen[i].population[p].getFitness() > worstGenFitness){
				worstGenFitness = gen[i].population[p].getFitness();
			}
			if(gen[i].population[p].getFitness() < bestGenFitness){
				bestGenFitness = gen[i].population[p].getFitness();
			}
			if(gen[i].population[p].getComplexity() > worstGenComplexity){
				worstGenComplexity = gen[i].population[p].getComplexity();
			}
			if(gen[i].population[p].getComplexity() < bestGenComplexity){
				bestGenComplexity = gen[i].population[p].getComplexity();
			}
		}
		//calculate the crowding rank for each individual in each front:
		frontCount = 1; //now used to count what front is being investigated.
		while(frontCount <= maxFront){
			bestFitness = 999999999;
			bestComplexity = 999999999; 
			bestFitnessForBestComplexity = 999999999;
			bestComplexityForBestFitness = 999999999;
			for(p=0; p<POPULATION_SIZE; p++){//each individual p in P[0]: find and remember the best fitness and complexity of the current front.
				if((gen[i].population[p].getFront() == frontCount) && (gen[i].population[p].getFitness() <= bestFitness)){
					if(gen[i].population[p].getFitness() < bestFitness){
						bestFitness = gen[i].population[p].getFitness();
						bestComplexityForBestFitness = 999999999;
					}
					if(gen[i].population[p].getComplexity() < bestComplexityForBestFitness){
						bestComplexityForBestFitness = gen[i].population[p].getComplexity();
					}
				}
				if((gen[i].population[p].getFront() == frontCount) && (gen[i].population[p].getComplexity() <= bestComplexity)){
					if(gen[i].population[p].getComplexity() < bestComplexity){
						bestComplexity = gen[i].population[p].getComplexity();
						bestFitnessForBestComplexity = 999999999;
					}
					if(gen[i].population[p].getFitness() < bestFitnessForBestComplexity){
						bestFitnessForBestComplexity = gen[i].population[p].getFitness();
					}
				}
			}
			if(debug) cout << "best fitness was " << bestFitness << " with complexity " << bestComplexityForBestFitness << endl << "best complexity was " << bestComplexity << " with fitness " << bestFitnessForBestComplexity << endl;
			for(p=0; p<POPULATION_SIZE; p++){//each individual p in P[0]:
				if(gen[i].population[p].getFront() == frontCount){
					//if the individual did not compile, set its crowding rank to 1. 
					if(isnan(gen[i].population[p].getFitness()) || (gen[i].population[p].getFitness() > 999999)){
						gen[i].population[p].setCrowdingRank(1); 
						continue;
					}
					//check for if p has the smallest fitness or complexity compared to all other individuals in the front and is on the Pareto boundary. If it does, set its crowdingRank to 999999999. 
					if(((gen[i].population[p].getFitness() == bestFitness) && (gen[i].population[p].getComplexity() == bestComplexityForBestFitness)) || ((gen[i].population[p].getComplexity() == bestComplexity) && (gen[i].population[p].getFitness() == bestFitnessForBestComplexity))){
						gen[i].population[p].setCrowdingRank(999999999); 
						if(debug) cout << "gen " << i << " individual " << p << " assigned crowding rank 999999999. Fitness: " << gen[i].population[p].getFitness() << ", complexity: " << gen[i].population[p].getComplexity() << ", best fitness " << bestFitness << ", best complexity " << bestComplexity << endl;
						continue;
					}
					//otherwise, set the crowdingRank to the sum of, the sum of the distances to each nearest neighbour, for each objective. 
					gen[i].population[p].setCrowdingRank(0); 
					nextBestComplexity = 0; nextBestFitness = 0; nextWorstComplexity = 0; nextWorstFitness = 0; 
					nextBestFitnessDistance = 999999999; nextBestComplexityDistance = 999999999; nextWorstComplexityDistance = 999999999; nextWorstFitnessDistance = 999999999;
					for(q=0; q<POPULATION_SIZE; q++){ //each individual q in P[0]:  //check the distance to each individual in the same front and remember the smallest one. 
						if((gen[i].population[q].getFront() == frontCount) && (p != q)){
							if((gen[i].population[p].getFitness() <= gen[i].population[q].getFitness()) && (gen[i].population[q].getFitness() - gen[i].population[p].getFitness() < nextWorstFitnessDistance)){
								nextWorstFitnessDistance = gen[i].population[q].getFitness() - gen[i].population[p].getFitness();
								nextWorstFitness = gen[i].population[q].getFitness();
							}
							if((gen[i].population[p].getFitness() >= gen[i].population[q].getFitness()) && (gen[i].population[p].getFitness() - gen[i].population[q].getFitness() < nextBestFitnessDistance)){
								nextBestFitnessDistance = gen[i].population[p].getFitness() - gen[i].population[q].getFitness();
								nextBestFitness = gen[i].population[q].getFitness();
							}
							if((gen[i].population[p].getComplexity() <= gen[i].population[q].getComplexity()) && (gen[i].population[q].getComplexity() - gen[i].population[p].getComplexity() < nextWorstComplexityDistance)){
								nextWorstComplexityDistance = gen[i].population[q].getComplexity() - gen[i].population[p].getComplexity();
								nextWorstComplexity = gen[i].population[q].getComplexity();
							}
							if((gen[i].population[p].getComplexity() >= gen[i].population[q].getComplexity()) && (gen[i].population[p].getComplexity() - gen[i].population[q].getComplexity() < nextBestComplexityDistance)){
								nextBestComplexityDistance = gen[i].population[p].getComplexity() - gen[i].population[q].getComplexity();
								nextBestComplexity = gen[i].population[q].getComplexity();
							}
						}
					}
					if((worstGenFitness == bestGenFitness) || (worstGenComplexity == bestGenComplexity)){
						distance = 0;
					}else{
						distance = fabs((((nextWorstFitness - nextBestFitness)/(worstGenFitness - bestGenFitness)) + ((nextWorstComplexity - nextBestComplexity) / (worstGenComplexity - bestGenComplexity))));
						//distance = ((nextWorstFitness - nextBestFitness)/(worstGenFitness - bestGenFitness));
					}
					if(debug) cout << "crowding rank for individual " << p << " is " << distance << endl;
					gen[i].population[p].setCrowdingRank(distance);
				}
			}
			frontCount++;
		}
		if(debug) cout << "maxFront = " << maxFront << ", part 4 finished on front count " << frontCount-1 << endl;
		if(debug){for(p=0; p<POPULATION_SIZE; p++){	cout << "gen " << i << " individual " << p << " has front = " << gen[i].population[p].getFront() << ", crowdingRank = " << gen[i].population[p].getCrowdingRank() << ", fitness = " << gen[i].population[p].getFitness() << ", complexity = " << gen[i].population[p].getComplexity() << endl; }}
		//all individuals should now be assigned a crowding rank.
		
		//Print generation P[i] to a file. 
		gen[i].printGenerationToFile(("./generations/generation" + to_string(i)));
		//If this generation is not the last, create a generation of children Q[i], otherwise exit this for loop. 
		if(i+1 >= numberOfGenerations){
			break;
		}
		
		//5) Fill up generation object Q[i] with individuals created using crossover, mutation and reproduction operations on the individuals in P[0] according to their front and crowdingRank. 
		//A lower front and higher crowdingRank is better. 
		//Also assess each created individual's fitness and complexity. 
		for(j=0; j<POPULATION_SIZE; j++){	//the individuals to breed to form Q are selected using tournament selection.
			//Tournament selection.
			next = 0;
			while(!next){
				individual1 = rand() % POPULATION_SIZE;	//Randomly select an individual for tournament selection.
				if(debug) cout << "individual1 = " << individual1 << endl;
				if ((gen[i].population[individual1].getFitness() < 3000) && (!isnan(gen[i].population[individual1].getFitness()))){ //don't pick individuals that did not compile or individuals with nan as the fitness.
					next = 1;
				}else{
					if(debug) cout << "individual rejected, fitness: " << gen[i].population[individual1].getFitness() << endl;
				}
			}
			next = 0;
			while(!next){
				individual2 = rand() % POPULATION_SIZE;	//Randomly select a second individual.
				if(debug) cout << "individual2 = " << individual2 << endl;
				if ((gen[i].population[individual2].getFitness() < 3000) && (!isnan(gen[i].population[individual2].getFitness()))){ //don't pick individuals that did not compile or individuals with nan as the fitness.
					next = 1;
				}else{
					if(debug) cout << "individual rejected, fitness: " << gen[i].population[individual2].getFitness() << endl;
				}
				if(debug) cout << "	trial individual2 = " << individual2 << endl;
			}
			if(gen[i].population[individual1].getFront() < gen[i].population[individual2].getFront()){	//If individual 1 has a lower front, choose individual 1.
				individualChosen1 = individual1;
				if(debug) cout << "individual 1 chosen. individual1 front: " << gen[i].population[individual1].getFront() << ", crowding: " << gen[i].population[individual1].getCrowdingRank() << ". individual 2 front: " << gen[i].population[individual2].getFront() << ", crowding: " << gen[i].population[individual2].getCrowdingRank() << endl;
			}else if((gen[i].population[individual1].getFront() == gen[i].population[individual2].getFront()) && (gen[i].population[individual1].getCrowdingRank() >= gen[i].population[individual2].getCrowdingRank())){ //Else if they have the same front but individual 1 has a higher or equal crowdingRank, choose individual 1. 
				individualChosen1 = individual1;
				if(debug) cout << "individual 1 chosen. individual1 front: " << gen[i].population[individual1].getFront() << ", crowding: " << gen[i].population[individual1].getCrowdingRank() << ". individual 2 front: " << gen[i].population[individual2].getFront() << ", crowding: " << gen[i].population[individual2].getCrowdingRank() << endl;
			}else{ //else choose individual 2. 
				individualChosen1 = individual2;
				if(debug) cout << "individual 2 chosen. individual1 front: " << gen[i].population[individual1].getFront() << ", crowding: " << gen[i].population[individual1].getCrowdingRank() << ". individual 2 front: " << gen[i].population[individual2].getFront() << ", crowding: " << gen[i].population[individual2].getCrowdingRank() << endl;
			}
			
			random = ( ((double)rand()) / (RAND_MAX) );	//Decide whether to use crossover, mutation, or reproduction.
			if(random <= crossoverRate){	//Choose crossover.
				//cout << "Crossover chosen." << endl;
				//Select a second parent.
				next = 0;
				while(!next){
					individual1 = rand() % POPULATION_SIZE;	//Randomly select an individual for tournament selection.
					if(debug) cout << "second individual1 = " << individual1 << endl;
					if ((gen[i].population[individual1].getFitness() < 3000) && (!isnan(gen[i].population[individual1].getFitness()))){ //don't pick individuals that did not compile or individuals with nan as the fitness.
						if(gen[i].population[individual1].getDepth(1) >= gen[i].population[individualChosen1].getDepth(1)){	//make sure all trees have a depth greater than or equal to all individualChosen1 tree depths, to avoid crossover infinite loop.
							if(gen[i].population[individual1].getDepth(2) >= gen[i].population[individualChosen1].getDepth(2)){
								if(gen[i].population[individual1].getDepth(3) >= gen[i].population[individualChosen1].getDepth(3)){
									if(gen[i].population[individual1].getDepth(4) >= gen[i].population[individualChosen1].getDepth(4)){
										if(gen[i].population[individual1].getDepth(5) >= gen[i].population[individualChosen1].getDepth(5)){
											if (debug) cout << "tree depths: " << gen[i].population[individual1].getDepth(1) << ", " << gen[i].population[individual1].getDepth(2);
											if (debug) cout << ", " << gen[i].population[individual1].getDepth(3) << ", " << gen[i].population[individual1].getDepth(4);
											if (debug) cout << ", " << gen[i].population[individual1].getDepth(5) << endl;
											next = 1;
										}
									}
								}
							}
						}
					}else{
						if(debug) cout << "second individual rejected, fitness: " << gen[i].population[individual1].getFitness() << endl;
					}
				}
				next = 0;
				while(!next){
					individual2 = rand() % POPULATION_SIZE;	//Randomly select a second, different individual.
					if(debug) cout << "second individual2 = " << individual2 << endl;
					if ((gen[i].population[individual2].getFitness() < 3000) && (!isnan(gen[i].population[individual2].getFitness()))){ //don't pick individuals that did not compile or individuals with nan as the fitness.
						if(gen[i].population[individual2].getDepth(1) >= gen[i].population[individualChosen1].getDepth(1)){	//make sure all trees have a depth greater than or equal to all individualChosen1 tree depths, to avoid crossover infinite loop error.
							if(gen[i].population[individual2].getDepth(2) >= gen[i].population[individualChosen1].getDepth(2)){
								if(gen[i].population[individual2].getDepth(3) >= gen[i].population[individualChosen1].getDepth(3)){
									if(gen[i].population[individual2].getDepth(4) >= gen[i].population[individualChosen1].getDepth(4)){
										if(gen[i].population[individual2].getDepth(5) >= gen[i].population[individualChosen1].getDepth(5)){
											if (debug) cout << "tree depths: " << gen[i].population[individual2].getDepth(1) << ", " << gen[i].population[individual2].getDepth(2);
											if (debug) cout << ", " << gen[i].population[individual2].getDepth(3) << ", " << gen[i].population[individual2].getDepth(4);
											if (debug) cout << ", " << gen[i].population[individual2].getDepth(5) << endl;
											next = 1;
										}
									}
								}
							}
						}
					}else{
						if(debug) cout << "second individual rejected, fitness: " << gen[i].population[individual2].getFitness() << endl;
					}
				}
				if(debug) cout << "	trial individual2 = " << individual2 << endl;
				if(gen[i].population[individual1].getFront() < gen[i].population[individual2].getFront()){	//If individual 1 has a lower front, choose individual 1.
					individualChosen2 = individual1;
					if(debug) cout << "individual 1 chosen. individual1 front: " << gen[i].population[individual1].getFront() << ", crowding: " << gen[i].population[individual1].getCrowdingRank() << ". individual 2 front: " << gen[i].population[individual2].getFront() << ", crowding: " << gen[i].population[individual2].getCrowdingRank() << endl;
				}else if((gen[i].population[individual1].getFront() == gen[i].population[individual2].getFront()) && (gen[i].population[individual1].getCrowdingRank() >= gen[i].population[individual2].getCrowdingRank())){ //Else if they have the same front but individual 1 has a higher or equal crowdingRank, choose individual 1. 
					individualChosen2 = individual1;
					if(debug) cout << "individual 1 chosen. individual1 front: " << gen[i].population[individual1].getFront() << ", crowding: " << gen[i].population[individual1].getCrowdingRank() << ". individual 2 front: " << gen[i].population[individual2].getFront() << ", crowding: " << gen[i].population[individual2].getCrowdingRank() << endl;
				}else{ //else choose individual 2. 
					individualChosen2 = individual2;
					if(debug) cout << "individual 2 chosen. individual1 front: " << gen[i].population[individual1].getFront() << ", crowding: " << gen[i].population[individual1].getCrowdingRank() << ". individual 2 front: " << gen[i].population[individual2].getFront() << ", crowding: " << gen[i].population[individual2].getCrowdingRank() << endl;
				}
				//Perform crossover.
				cout << "Generation Q(" << (i) << ") individual " << (j+1) << " is a child of generation " << i << " individuals " << (individualChosen1+1) << " and " << (individualChosen2+1) << endl;
				crossover(gen[i], individualChosen1, individualChosen2, Q[i], j);
				if(debug) cout << "crossover successful" << endl;
			}else if((random - crossoverRate) <= mutationRate){	//Choose mutation.
				//cout << "Mutation chosen." << endl;					
				if(!(rand() % 2)){	//50:50 chance of subtree mutation or point mutation.
					subtreeMutation(gen[i], individualChosen1, Q[i], j); //Perform subtree mutation.
					cout << "Generation Q(" << (i) << ") individual " << (j+1) << " is a subtree mutant of generation " << i << " individual " << (individualChosen1+1) << endl;
				}else{
					pointMutation(pointMutationRate, gen[i], individualChosen1, Q[i], j);	//Perform point mutation.
					cout << "Generation Q(" << (i) << ") individual " << (j+1) << " is a point mutant of generation " << i << " individual " << (individualChosen1+1) << endl;
				}
			}else{	//perform reproduction
				//cout << "Reproduction chosen." << endl;
				reproduce(gen[i], individualChosen1, Q[i], j);	//Perfrom reproduction.
				cout << "Generation Q(" << (i) << ") individual " << (j+1) << " is identical to generation " << i << " individual " << (individualChosen1+1) << endl;
			}
			
			//compute the fitness and complexity for each individual. 
			cout << "compiling Q(" << (i) << ")  individual " << (j+1) << ":" << endl;
			applyIndividualToModel(Q[i], j, "/home/jack/roles-gp/roles/");	//set the model to contain the equations given by an individual.
			try{	//attempt to:
				if(runModelJustCompileRT("/home/jack/roles-gp/roles") != 0){		//run the model, just recompiling the RoleTheory.cpp file.
					Q[i].population[j].setFitness(999999999); 					//if there is an exception, set the fitness to 999,999,999. 
					cout << "generation Q(" << i << ") individual " << (j+1) << " could not be run (didn't return 0). Fitness set to: " << Q[i].population[j].getFitness() << endl;
				}else{
					Q[i].population[j].setFitness(calculateFitness("/home/jack/roles-gp/roles/"));	//calculate and assign the fitness
					cout << "generation Q(" << i << ") individual " << (j+1) << " had fitness " << Q[i].population[j].getFitness() << endl;
				}
			}catch(...){
				Q[i].population[j].setFitness(999999999); 						//if there is an exception, set the fitness to 999,999,999. 
				cout << "generation Q(" << i << ") individual " << (j+1) << " could not be run (compile failed). Fitness set to: " << Q[i].population[j].getFitness() << endl << endl;
			}
			//Q[i].population[j].setFitness((rand()%10));	//COMMENT THIS LINE AFTER TESTING.
			Q[i].population[j].calculateComplexity();
			cout << endl;
		}
		
		//6) Redo steps 3 and 4 but for R instead of P, where R[0-99] = P (gen), R[100-199] = Q. For each individual in R: reset the np, sp, crowdingRank and front to -1, calculate the fitness and complexity, assign each individual a front number and assign each individual a crowding rank.
		//6.1) Recalculate the np's and fronts for gen[i] and Q[i], treating them as one double-sized Generation[i]. Access gen[i] for r 0-99 and Q[i] for r 100-199 for pop_size 100.
		//reset np, rank and crowdingRank for each individual in gen and Q. 
		for(r=0; r<POPULATION_SIZE; r++){
			gen[i].population[r].setNP(0);		gen[i].population[r].setFront(-1);	gen[i].population[r].setCrowdingRank(999999999);
			Q[i].population[r].setFront(-1);	Q[i].population[r].setFront(-1);	Q[i].population[r].setCrowdingRank(999999999);
		}
		sizeOfFront1 = 0;
		for(r=0; r<POPULATION_SIZE*2; r++){	//each individual r in R[i]:
			rQ = r - POPULATION_SIZE;
			if(r < POPULATION_SIZE){ //r=0-(P-1)
				gen[i].population[r].setNP(0);	
				gen[i].population[r].setFront(-1);	
				gen[i].population[r].setCrowdingRank(999999999);
			}else{ //r=P-(2P-1)
				Q[i].population[rQ].setNP(0);	
				Q[i].population[rQ].setFront(-1); 	
				Q[i].population[rQ].setCrowdingRank(999999999);
			}
			for (t=0; t<POPULATION_SIZE*2; t++){	//each individual t in R[i]:
				tQ = t - POPULATION_SIZE;
				
				if((r<POPULATION_SIZE)&&(t<POPULATION_SIZE)){	//r=0-(P-1); t=0-(P-1)
					if(debug) cout << "r = " << r << ", t = " << t << ", np = " << gen[i].population[r].getNP() << endl << "r.fitness = " << gen[i].population[r].getFitness() << ", r.complexity = " << gen[i].population[r].getComplexity() << ", t.fitness = " << gen[i].population[t].getFitness() << ", t.complexity = " << gen[i].population[t].getComplexity() << endl;
					if( ((gen[i].population[r].getFitness() > gen[i].population[t].getFitness()) && (gen[i].population[r].getComplexity() >= gen[i].population[t].getComplexity())) || ((gen[i].population[r].getFitness() >= gen[i].population[t].getFitness()) && (gen[i].population[r].getComplexity() > gen[i].population[t].getComplexity())) ){
						gen[i].population[r].setNP(gen[i].population[r].getNP() + 1);  //s dominates r, increase np by 1.
						if(debug) cout << "t dominates" << endl;
					}else if( (isnan(gen[i].population[r].getFitness()) || (gen[i].population[r].getFitness() > 999999)) && ((!isnan(gen[i].population[t].getFitness())) && (gen[i].population[t].getFitness() <= 999999)) ){ //if r fitness is nan and t fitness is not nan or 999999999, t dominates r. This should make all individuals that don't compile go to last front. 
						gen[i].population[r].setNP(gen[i].population[r].getNP() + 1);
						if(debug) cout << "t dominates as r's fitness is nan or 999999999 but t's is not" << endl;
					}
				}else if((r<POPULATION_SIZE)&&(t>=POPULATION_SIZE)){	//r=0-(P-1); t=P-(2P-1);
					if(debug) cout << "r = " << r << ", t = " << t << ", np = " << gen[i].population[r].getNP() << endl << "r.fitness = " << gen[i].population[r].getFitness() << ", r.complexity = " << gen[i].population[r].getComplexity() << ", t.fitness = " << Q[i].population[tQ].getFitness() << ", t.complexity = " << Q[i].population[tQ].getComplexity() << endl;
					if( ((gen[i].population[r].getFitness() > Q[i].population[tQ].getFitness()) && (gen[i].population[r].getComplexity() >= Q[i].population[tQ].getComplexity())) || ((gen[i].population[r].getFitness() >= Q[i].population[tQ].getFitness()) && (gen[i].population[r].getComplexity() > Q[i].population[tQ].getComplexity())) ){
						gen[i].population[r].setNP(gen[i].population[r].getNP() + 1);  //t dominates r, increase np by 1.
						if(debug) cout << "t dominates" << endl;
					}else if( (isnan(gen[i].population[r].getFitness()) || (gen[i].population[r].getFitness() > 999999)) && ((!isnan(Q[i].population[tQ].getFitness())) && (Q[i].population[tQ].getFitness() <= 999999)) ){ //if r fitness is nan and t fitness is not nan or 999999999, t dominates r. This should make all individuals that don't compile go to last front. 
						gen[i].population[r].setNP(gen[i].population[r].getNP() + 1);
						if(debug) cout << "t dominates as r's fitness is nan or 999999999 but t's is not" << endl;
					}
				}else if((r>=POPULATION_SIZE)&&(t<POPULATION_SIZE)){	//r=P-(2P-1)); t=0-(P-1)
					if(debug) cout << "r = " << r << ", t = " << t << ", np = " << Q[i].population[rQ].getNP() << endl << "r.fitness = " << Q[i].population[rQ].getFitness() << ", r.complexity = " << Q[i].population[rQ].getComplexity() << ", t.fitness = " << gen[i].population[t].getFitness() << ", t.complexity = " << gen[i].population[t].getComplexity() << endl;
					if( ((Q[i].population[rQ].getFitness() > gen[i].population[t].getFitness()) && (Q[i].population[rQ].getComplexity() >= gen[i].population[t].getComplexity())) || ((Q[i].population[rQ].getFitness() >= gen[i].population[t].getFitness()) && (Q[i].population[rQ].getComplexity() > gen[i].population[t].getComplexity())) ){
						Q[i].population[rQ].setNP(Q[i].population[rQ].getNP() + 1);  //t dominates r, increase np by 1.
						if(debug) cout << "t dominates" << endl;
					}else if( (isnan(Q[i].population[rQ].getFitness()) || (Q[i].population[rQ].getFitness() > 999999)) && ((!isnan(gen[i].population[t].getFitness())) && (gen[i].population[t].getFitness() <= 999999)) ){ //if r fitness is nan and t fitness is not nan or 999999999, t dominates r. This should make all individuals that don't compile go to last front. 
						Q[i].population[rQ].setNP(Q[i].population[rQ].getNP() + 1);
						if(debug) cout << "t dominates as r's fitness is nan or 999999999 but t's is not" << endl;
					}
				}else if((r>=POPULATION_SIZE)&&(t>=POPULATION_SIZE)){	//r=P-(2P-1); t=P-(2P-1)
					if(debug) cout << "r = " << r << ", t = " << t << ", np = " << Q[i].population[rQ].getNP() << endl << "r.fitness = " << Q[i].population[rQ].getFitness() << ", r.complexity = " << Q[i].population[rQ].getComplexity() << ", t.fitness = " << Q[i].population[tQ].getFitness() << ", t.complexity = " << Q[i].population[tQ].getComplexity() << endl;
					if( ((Q[i].population[rQ].getFitness() > Q[i].population[tQ].getFitness()) && (Q[i].population[rQ].getComplexity() >= Q[i].population[tQ].getComplexity())) || ((Q[i].population[rQ].getFitness() >= Q[i].population[tQ].getFitness()) && (Q[i].population[rQ].getComplexity() > Q[i].population[tQ].getComplexity())) ){
						Q[i].population[rQ].setNP(Q[i].population[rQ].getNP() + 1);  //t dominates r, increase np by 1.
						if(debug) cout << "t dominates" << endl;
					}else if( (isnan(Q[i].population[rQ].getFitness()) || (Q[i].population[rQ].getFitness() > 999999)) && ((!isnan(Q[i].population[tQ].getFitness())) && (Q[i].population[tQ].getFitness() <= 999999)) ){ //if r fitness is nan and t fitness is not nan or 999999999, t dominates r. This should make all individuals that don't compile go to last front. 
						Q[i].population[rQ].setNP(Q[i].population[rQ].getNP() + 1);
						if(debug) cout << "t dominates as r's fitness is nan or 999999999 but t's is not" << endl;
					}
				}else{cout << "error after if((r<POPULATION_SIZE)&&(t<POPULATION_SIZE)){\tr=" << r << ", t=" << t << endl;}
			}
			if(r<POPULATION_SIZE){
				if(gen[i].population[r].getNP() == 0){
					gen[i].population[r].setFront(1);
					sizeOfFront1++;
				}
				if(debug)	cout << "R[" << i << "] individual " << r << " has NP: " << gen[i].population[r].getNP() << endl << endl;
			}else{
				if(Q[i].population[rQ].getNP() == 0){
					Q[i].population[rQ].setFront(1);
					sizeOfFront1++;
				}
				if(debug)	cout << "R[" << i << "] individual " << r << " has NP: " << Q[i].population[rQ].getNP() << endl << endl;
			}
		}
		nextLowestNP=POPULATION_SIZE+1; 
		individuals_unassigned_rank = (POPULATION_SIZE*2) - sizeOfFront1;
		frontCount = 2;
		if(debug) cout << "nextLowestNP = " << nextLowestNP << ", individuals_unassigned_rank = " << individuals_unassigned_rank << " out of " << POPULATION_SIZE*2 << endl;
		//I'm going to do it differently from the paper here. Rather than create an arrays of fronts using array Sp, I'm going to iterate through all the individuals assigning them fronts based on their np value. If individuals have the same np, they should be in the same front. 
		while(individuals_unassigned_rank > 0){
			nextLowestNP=(POPULATION_SIZE*2)+1; 
			if(debug) cout << "individuals_unassigned_rank = " << individuals_unassigned_rank << ", nextLowestNP = " << nextLowestNP << endl;
			for(r=0; r<POPULATION_SIZE*2; r++){ //each individual r in R[0]:
				rQ = r - POPULATION_SIZE;
				if ((r<POPULATION_SIZE) && (gen[i].population[r].getFront() == -1) && (gen[i].population[r].getNP() <  nextLowestNP)){
					nextLowestNP = gen[i].population[r].getNP();   //basically find the lowest np value in the population for individuals that are unassigned a front. 
					if(debug) cout << "\tnextLowestNP set to " << nextLowestNP << endl;
				}
				if((r>=POPULATION_SIZE) && (Q[i].population[rQ].getFront() == -1) && (Q[i].population[rQ].getNP() <  nextLowestNP)){
					nextLowestNP = Q[i].population[rQ].getNP();   //basically find the lowest np value in the population for individuals that are unassigned a front. 
					if(debug) cout << "\tnextLowestNP set to " << nextLowestNP << endl;
				}
			}
			for(r=0; r<POPULATION_SIZE*2; r++){ //each individual r in R[0]:
				rQ = r - POPULATION_SIZE;
				if((r<POPULATION_SIZE) && (gen[i].population[r].getNP() == nextLowestNP)){ 
					gen[i].population[r].setFront(frontCount); 
					individuals_unassigned_rank--;
					if(debug) cout << "\tindividuals unassigned a rank set to " << individuals_unassigned_rank << endl;
				}  //all individuals with this lowest np value will be assigned to the next front. 
				if((r>=POPULATION_SIZE) && (Q[i].population[rQ].getNP() == nextLowestNP)){ 
					Q[i].population[rQ].setFront(frontCount); 
					individuals_unassigned_rank--;
					if(debug) cout << "\tindividuals unassigned a rank set to " << individuals_unassigned_rank << endl;
				}  //all individuals with this lowest np value will be assigned to the next front. 
			}
			if(debug) cout << "for frontCount " << frontCount << ", nextLowestNP set to " << nextLowestNP << ". Individuals with no rank changed to " << individuals_unassigned_rank << endl;
			frontCount++;
		}
		maxFront = frontCount -1;
		if(debug){for(r=0; r<POPULATION_SIZE; r++){	cout << "R(" << i << ") individual " << r << " is part of Pareto front: " << gen[i].population[r].getFront() << ".\t np: " << gen[i].population[r].getNP() << "\tFitness: " << gen[i].population[r].getFitness() << ", \tcomplexity: " << gen[i].population[r].getComplexity() << endl; }}
		if(debug){for(rQ=0; rQ<POPULATION_SIZE; rQ++){	cout << "R(" << i << ") individual " << rQ + POPULATION_SIZE << " is part of Pareto front: " << Q[i].population[rQ].getFront() << ".\t np: " << Q[i].population[rQ].getNP() << "\tFitness: " << Q[i].population[rQ].getFitness() << ", \tcomplexity: " << Q[i].population[rQ].getComplexity() << endl; }}
		//all individuals should now be assigned a Pareto front.
		
		//*) Iterate through each front and test for identical individuals. Any individuals found to be identical to previously examined individuals will be placed in the second-to-last Pareto front. 
		frontCount = 1; r=0; t=0, rQ=0, tQ=0; //frontCout now used to count what front is being investigated.
		if(maxFront-2 >= 1){
			while(frontCount <= maxFront-2){ //examine every front except the last two. 
				for(r=1; r<POPULATION_SIZE; r++){ //r is the individual in question.
					for(t=0; t<r; t++){ //t and tQ are previously examined individuals. 
						tQ=t;
						if((gen[i].population[r].getFront()==frontCount) && (gen[i].population[t].getFront()==frontCount) && (isIdentical(gen[i], r, gen[i], t))){ //If the individual in question is found to be identical to a previously examined individual, move it to the second-to-last Pareto front. 
							gen[i].population[r].setFront(maxFront-1);
							if(debug) cout << "R individual " << r << " found to be identical to R individual " << t << ", putting individual " << r << " in front " << gen[i].population[r].getFront() << endl;
							break;
						}
						if((gen[i].population[r].getFront()==frontCount) && (Q[i].population[tQ].getFront()==frontCount) && (isIdentical(gen[i], r, Q[i], tQ))){ //If the individual in question is found to be identical to a previously examined individual, move it to the second-to-last Pareto front. 
							gen[i].population[r].setFront(maxFront-1);
							if(debug) cout << "R individual " << r << " found to be identical to R individual " << t+POPULATION_SIZE << ", putting individual " << r << " in front " << gen[i].population[r].getFront() << endl;
							break;
						}
					}
				}
				for(rQ=1; rQ<POPULATION_SIZE; rQ++){ //rQ is the individual in question.
					for(t=0; t<rQ; t++){ //t and tQ are previously examined individuals. 
						tQ=t;
						if((Q[i].population[rQ].getFront()==frontCount) && (gen[i].population[t].getFront()==frontCount) && (isIdentical(Q[i], rQ, gen[i], t))){ //If the individual in question is found to be identical to a previously examined individual, move it to the second-to-last Pareto front. 
							Q[i].population[rQ].setFront(maxFront-1);
							if(debug) cout << "R individual " << rQ+POPULATION_SIZE << " found to be identical to R individual " << t << ", putting individual " << rQ+POPULATION_SIZE << " in front " << Q[i].population[rQ].getFront() << endl;
							break;
						}
						if((Q[i].population[rQ].getFront()==frontCount) && (Q[i].population[tQ].getFront()==frontCount) && (isIdentical(Q[i], rQ, Q[i], tQ))){ //If the individual in question is found to be identical to a previously examined individual, move it to the second-to-last Pareto front. 
							Q[i].population[rQ].setFront(maxFront-1);
							if(debug) cout << "R individual " << rQ+POPULATION_SIZE << " found to be identical to R individual " << t+POPULATION_SIZE << ", putting individual " << rQ+POPULATION_SIZE << " in front " << Q[i].population[rQ].getFront() << endl;
							break;
						}
					}
				}
				frontCount++;
			}
		}
		
		//6.2) Assign a double crowdingRank to each individual in R by comparing its distance to all other individuals in the same front, 
		//find the best and worst fitnesses and complexities in the population:
		bestGenFitness = 999999999; worstGenFitness = 0; bestGenComplexity = 999999999; worstGenComplexity = 0;
		for(r=0; r<POPULATION_SIZE*2; r++){
			rQ = r - POPULATION_SIZE;
			if(r<POPULATION_SIZE){
				if(gen[i].population[r].getFitness() > worstGenFitness){
					worstGenFitness = gen[i].population[r].getFitness();
				}
				if(gen[i].population[r].getFitness() < bestGenFitness){
					bestGenFitness = gen[i].population[r].getFitness();
				}
				if(gen[i].population[r].getComplexity() > worstGenComplexity){
					worstGenComplexity = gen[i].population[r].getComplexity();
				}
				if(gen[i].population[r].getComplexity() < bestGenComplexity){
					bestGenComplexity = gen[i].population[r].getComplexity();
				}
			}else{
				if(Q[i].population[rQ].getFitness() > worstGenFitness){
					worstGenFitness = Q[i].population[rQ].getFitness();
				}
				if(Q[i].population[rQ].getFitness() < bestGenFitness){
					bestGenFitness = Q[i].population[rQ].getFitness();
				}
				if(Q[i].population[rQ].getComplexity() > worstGenComplexity){
					worstGenComplexity = Q[i].population[rQ].getComplexity();
				}
				if(Q[i].population[rQ].getComplexity() < bestGenComplexity){
					bestGenComplexity = Q[i].population[rQ].getComplexity();
				}
			}
		}
		frontCount = 1; //now used to count what front is being investigated.
		while(frontCount <= maxFront){
			bestFitness = 999999999;
			bestComplexity = 999999999; 
			bestComplexityForBestFitness = 999999999;
			bestFitnessForBestComplexity = 999999999;
			for(r=0; r<(POPULATION_SIZE*2); r++){//each individual r in R[0]: find and remember the best fitness and complexity of the current front.
				rQ = r - POPULATION_SIZE;
				if(r<POPULATION_SIZE){
					if((gen[i].population[r].getFront() == frontCount) && (gen[i].population[r].getFitness() <= bestFitness)){
						if(gen[i].population[r].getFitness() < bestFitness){
							bestFitness = gen[i].population[r].getFitness();
							bestComplexityForBestFitness = 999999999;
						}
						//if(debug) cout << "\tbestFitness set to " << gen[i].population[r].getFitness() << endl;
						if(gen[i].population[r].getComplexity() < bestComplexityForBestFitness){
							bestComplexityForBestFitness = gen[i].population[r].getComplexity();
						}
					}
					if((gen[i].population[r].getFront() == frontCount) && (gen[i].population[r].getComplexity() <= bestComplexity)){
						if(gen[i].population[r].getComplexity() < bestComplexity){
							bestComplexity = gen[i].population[r].getComplexity();
							bestFitnessForBestComplexity = 999999999;
						}
						//if(debug) cout << "\tbestFitness set to " << gen[i].population[r].getFitness() << endl;
						if(gen[i].population[r].getFitness() < bestFitnessForBestComplexity){
							bestFitnessForBestComplexity = gen[i].population[r].getFitness();
						}
					}
				}else{
					if((Q[i].population[rQ].getFront() == frontCount) && (Q[i].population[rQ].getFitness() <= bestFitness)){
						if(Q[i].population[rQ].getFitness() < bestFitness){
							bestFitness = Q[i].population[rQ].getFitness();
							bestComplexityForBestFitness = 999999999;
						}
						//if(debug) cout << "\tbestFitness set to " << Q[i].population[rQ].getFitness() << endl;
						if(Q[i].population[rQ].getComplexity() < bestComplexityForBestFitness){
							bestComplexityForBestFitness = Q[i].population[rQ].getComplexity();
						}
					}
					if((Q[i].population[rQ].getFront() == frontCount) && (Q[i].population[rQ].getComplexity() <= bestComplexity)){
						if(Q[i].population[rQ].getComplexity() < bestComplexity){
							bestComplexity = Q[i].population[rQ].getComplexity();
							bestFitnessForBestComplexity = 999999999;
						}
						//if(debug) cout << "\tbestFitness set to " << Q[i].population[rQ].getFitness() << endl;
						if(Q[i].population[rQ].getFitness() < bestFitnessForBestComplexity){
							bestFitnessForBestComplexity = Q[i].population[rQ].getFitness();
						}
					}
				}
			}
			if(debug) cout << "best fitness for front " << frontCount << " was " << bestFitness << ". Best complexity for front " << frontCount << " was " << bestComplexity << endl;
			if(debug) cout << "best fitness was " << bestFitness << " with complexity " << bestComplexityForBestFitness << endl << "best complexity was " << bestComplexity << " with fitness " << bestFitnessForBestComplexity << endl;
			for(r=0; r<POPULATION_SIZE*2; r++){//each individual r in R[0]:
				rQ = r - POPULATION_SIZE;
				if(debug) cout << "\tr = " << r << ", rQ = " << rQ << ", population_size = " << POPULATION_SIZE << ", frontCount = " << frontCount << endl;
				if((r<POPULATION_SIZE) && (gen[i].population[r].getFront() == frontCount)){
					//if the individual did not compile, set its crowding rank to 1. 
					if(isnan(gen[i].population[r].getFitness()) || (gen[i].population[r].getFitness() > 999999)){
						gen[i].population[r].setCrowdingRank(1); 
						if(debug) cout << "individual " << r << " did not compile, crowdingRank set to " << gen[i].population[r].getCrowdingRank() << endl;
						continue;
					}
					//check for if r has the smallest fitness or complexity compared to all other individuals in the front. If it does, set its crowdingRank to 999999999. 
					if(((gen[i].population[r].getFitness() == bestFitness) && (gen[i].population[r].getComplexity() == bestComplexityForBestFitness)) || ((gen[i].population[r].getComplexity() == bestComplexity) && (gen[i].population[r].getFitness() == bestFitnessForBestComplexity))){
						gen[i].population[r].setCrowdingRank(999999999); 
						if(debug) cout << "individual " << r << " has the smallest fitness or complexity, crowdingRank set to " << gen[i].population[r].getCrowdingRank() << endl;
						continue;
					}
					//otherwise, set the crowdingRank to the sum of, the sum of the distances to each nearest neighbour, for each objective. 
					gen[i].population[r].setCrowdingRank(0); 
					nextBestComplexity = 0; nextBestFitness = 0; nextWorstComplexity = 0; nextWorstFitness = 0; 
					nextBestFitnessDistance = 999999999; nextBestComplexityDistance = 999999999; nextWorstComplexityDistance = 999999999; nextWorstFitnessDistance = 999999999;
					for(t=0; t<POPULATION_SIZE*2; t++){ //each individual t in R[0]:  //check the distance to each individual in the same front and remember the smallest one. 
						tQ = t - POPULATION_SIZE;
						if(t<POPULATION_SIZE){
							if((gen[i].population[r].getFront() == frontCount) && (r != t)){
								if((gen[i].population[r].getFitness() <= gen[i].population[t].getFitness()) && (gen[i].population[t].getFitness() - gen[i].population[r].getFitness() < nextWorstFitnessDistance)){
									nextWorstFitnessDistance = gen[i].population[t].getFitness() - gen[i].population[r].getFitness();
									nextWorstFitness = gen[i].population[t].getFitness();
								}
								if((gen[i].population[r].getFitness() >= gen[i].population[t].getFitness()) && (gen[i].population[r].getFitness() - gen[i].population[t].getFitness() < nextBestFitnessDistance)){
									nextBestFitnessDistance = gen[i].population[r].getFitness() - gen[i].population[t].getFitness();
									nextBestFitness = gen[i].population[t].getFitness();
								}
								if((gen[i].population[r].getComplexity() <= gen[i].population[t].getComplexity()) && (gen[i].population[t].getComplexity() - gen[i].population[r].getComplexity() < nextWorstComplexityDistance)){
									nextWorstComplexityDistance = gen[i].population[t].getComplexity() - gen[i].population[r].getComplexity();
									nextWorstComplexity = gen[i].population[t].getComplexity();
								}
								if((gen[i].population[r].getComplexity() >= gen[i].population[t].getComplexity()) && (gen[i].population[r].getComplexity() - gen[i].population[t].getComplexity() < nextBestComplexityDistance)){
									nextBestComplexityDistance = gen[i].population[r].getComplexity() - gen[i].population[t].getComplexity();
									nextBestComplexity = gen[i].population[t].getComplexity();
								}
							}
						}else{
							if((Q[i].population[tQ].getFront() == frontCount) && (r != t)){
								if((gen[i].population[r].getFitness() <= Q[i].population[tQ].getFitness()) && (Q[i].population[tQ].getFitness() - gen[i].population[r].getFitness() < nextWorstFitnessDistance)){
									nextWorstFitnessDistance = Q[i].population[tQ].getFitness() - gen[i].population[r].getFitness();
									nextWorstFitness = Q[i].population[tQ].getFitness();
								}
								if((gen[i].population[r].getFitness() >= Q[i].population[tQ].getFitness()) && (gen[i].population[r].getFitness() - Q[i].population[tQ].getFitness() < nextBestFitnessDistance)){
									nextBestFitnessDistance = gen[i].population[r].getFitness() - Q[i].population[tQ].getFitness();
									nextBestFitness = Q[i].population[tQ].getFitness();
								}
								if((gen[i].population[r].getComplexity() <= Q[i].population[tQ].getComplexity()) && (Q[i].population[tQ].getComplexity() - gen[i].population[r].getComplexity() < nextWorstComplexityDistance)){
									nextWorstComplexityDistance = Q[i].population[tQ].getComplexity() - gen[i].population[r].getComplexity();
									nextWorstComplexity = Q[i].population[tQ].getComplexity();
								}
								if((gen[i].population[r].getComplexity() >= Q[i].population[tQ].getComplexity()) && (gen[i].population[r].getComplexity() - Q[i].population[tQ].getComplexity() < nextBestComplexityDistance)){
									nextBestComplexityDistance = gen[i].population[r].getComplexity() - Q[i].population[tQ].getComplexity();
									nextBestComplexity = Q[i].population[tQ].getComplexity();
								}
							}
						}
					}
					if((worstGenFitness == bestGenFitness) || (worstGenComplexity == bestGenComplexity)){
						distance = 0;
					}else{
						distance = fabs((((nextWorstFitness - nextBestFitness)/(worstGenFitness - bestGenFitness)) + ((nextWorstComplexity - nextBestComplexity) / (worstGenComplexity - bestGenComplexity))));
						//distance = ((nextWorstFitness - nextBestFitness)/(worstGenFitness - bestGenFitness));
					}
					if(debug) cout << "crowding rank for individual " << r << " is " << distance << endl;
					gen[i].population[r].setCrowdingRank(distance);
				}else if((r>=POPULATION_SIZE) && (Q[i].population[rQ].getFront() == frontCount)){
					//if the individual did not compile, set its crowding rank to 1. 
					if(isnan(Q[i].population[rQ].getFitness()) || (Q[i].population[rQ].getFitness() > 999999)){
						Q[i].population[rQ].setCrowdingRank(1); 
						if(debug) cout << "individual " << r << " did not compile, crowdingRank set to " << Q[i].population[rQ].getCrowdingRank() << endl;
						continue;
					}
					//check for if r has the smallest fitness or complexity compared to all other individuals in the front. If it does, set its crowdingRank to 999999999. 
					if(((Q[i].population[rQ].getFitness() == bestFitness) && (Q[i].population[rQ].getComplexity() == bestComplexityForBestFitness)) || ((Q[i].population[rQ].getComplexity() == bestComplexity) && (Q[i].population[rQ].getFitness() == bestFitnessForBestComplexity))){
						Q[i].population[rQ].setCrowdingRank(999999999); 
						if(debug) cout << "individual " << r << " has the smallest fitness or complexity, crowdingRank set to " << Q[i].population[rQ].getCrowdingRank() << endl;
						continue;
					}
					//otherwise, set the crowdingRank to the sum of, the sum of the distances to each nearest neighbour, for each objective. 
					gen[i].population[r].setCrowdingRank(0); 
					nextBestComplexity = 0; nextBestFitness = 0; nextWorstComplexity = 0; nextWorstFitness = 0; 
					nextBestFitnessDistance = 999999999; nextBestComplexityDistance = 999999999; nextWorstComplexityDistance = 999999999; nextWorstFitnessDistance = 999999999;
					for(t=0; t<POPULATION_SIZE*2; t++){ //each individual t in R[0]:  //check the distance to each individual in the same front and remember the smallest one. 
						tQ = t - POPULATION_SIZE;
						if(t<POPULATION_SIZE){
							if((Q[i].population[rQ].getFront() == frontCount) && (r != t)){
								if((Q[i].population[rQ].getFitness() <= gen[i].population[t].getFitness()) && (gen[i].population[t].getFitness() - Q[i].population[rQ].getFitness() < nextWorstFitnessDistance)){
									nextWorstFitnessDistance = gen[i].population[t].getFitness() - Q[i].population[rQ].getFitness();
									nextWorstFitness = gen[i].population[t].getFitness();
								}
								if((Q[i].population[rQ].getFitness() >= gen[i].population[t].getFitness()) && (Q[i].population[rQ].getFitness() - gen[i].population[t].getFitness() < nextBestFitnessDistance)){
									nextBestFitnessDistance = Q[i].population[rQ].getFitness() - gen[i].population[t].getFitness();
									nextBestFitness = gen[i].population[t].getFitness();
								}
								if((Q[i].population[rQ].getComplexity() <= gen[i].population[t].getComplexity()) && (gen[i].population[t].getComplexity() - Q[i].population[rQ].getComplexity() < nextWorstComplexityDistance)){
									nextWorstComplexityDistance = gen[i].population[t].getComplexity() - Q[i].population[rQ].getComplexity();
									nextWorstComplexity = gen[i].population[t].getComplexity();
								}
								if((Q[i].population[rQ].getComplexity() >= gen[i].population[t].getComplexity()) && (Q[i].population[rQ].getComplexity() - gen[i].population[t].getComplexity() < nextBestComplexityDistance)){
									nextBestComplexityDistance = Q[i].population[rQ].getComplexity() - gen[i].population[t].getComplexity();
									nextBestComplexity = gen[i].population[t].getComplexity();
								}
							}
						}else{
							if((Q[i].population[tQ].getFront() == frontCount) && (r != t)){
								if((Q[i].population[rQ].getFitness() <= Q[i].population[tQ].getFitness()) && (Q[i].population[tQ].getFitness() - Q[i].population[rQ].getFitness() < nextWorstFitnessDistance)){
									nextWorstFitnessDistance = Q[i].population[tQ].getFitness() - Q[i].population[rQ].getFitness();
									nextWorstFitness = Q[i].population[tQ].getFitness();
								}
								if((Q[i].population[rQ].getFitness() >= Q[i].population[tQ].getFitness()) && (Q[i].population[rQ].getFitness() - Q[i].population[tQ].getFitness() < nextBestFitnessDistance)){
									nextBestFitnessDistance = Q[i].population[rQ].getFitness() - Q[i].population[tQ].getFitness();
									nextBestFitness = Q[i].population[tQ].getFitness();
								}
								if((Q[i].population[rQ].getComplexity() <= Q[i].population[tQ].getComplexity()) && (Q[i].population[tQ].getComplexity() - Q[i].population[rQ].getComplexity() < nextWorstComplexityDistance)){
									nextWorstComplexityDistance = Q[i].population[tQ].getComplexity() - Q[i].population[rQ].getComplexity();
									nextWorstComplexity = Q[i].population[tQ].getComplexity();
								}
								if((Q[i].population[rQ].getComplexity() >= Q[i].population[tQ].getComplexity()) && (Q[i].population[rQ].getComplexity() - Q[i].population[tQ].getComplexity() < nextBestComplexityDistance)){
									nextBestComplexityDistance = Q[i].population[rQ].getComplexity() - Q[i].population[tQ].getComplexity();
									nextBestComplexity = Q[i].population[tQ].getComplexity();
								}
							}
						}
					}
					if((worstGenFitness == bestGenFitness) || (worstGenComplexity == bestGenComplexity)){
						distance = 0;
					}else{
						distance = fabs((((nextWorstFitness - nextBestFitness)/(worstGenFitness - bestGenFitness)) + ((nextWorstComplexity - nextBestComplexity) / (worstGenComplexity - bestGenComplexity))));
						//distance = ((nextWorstFitness - nextBestFitness)/(worstGenFitness - bestGenFitness));
					}
					if(debug) cout << "crowding rank for individual " << r << " is " << distance << endl;
					Q[i].population[rQ].setCrowdingRank(distance);
				}
			}
			frontCount++;
		}
		if(debug) cout << "maxFront = " << maxFront << ", part 4 finished on front count " << frontCount-1 << endl;
		if(debug){for(r=0; r<POPULATION_SIZE; r++){	cout << "gen " << i << " individual " << r << " has front = " << gen[i].population[r].getFront() << ", crowdingRank = " << gen[i].population[r].getCrowdingRank() << ", fitness = " << gen[i].population[r].getFitness() << ", complexity = " << gen[i].population[r].getComplexity() << endl; }}
		if(debug){for(rQ=0; rQ<POPULATION_SIZE; rQ++){	cout << "gen " << i << " individual " << rQ + POPULATION_SIZE << " has front = " << Q[i].population[rQ].getFront() << ", crowdingRank = " << Q[i].population[rQ].getCrowdingRank() << ", fitness = " << Q[i].population[rQ].getFitness() << ", complexity = " << Q[i].population[rQ].getComplexity() << endl; }}
		//all individuals should now be assigned a crowding rank.
		
		//7) Create the next generation P[1] by taking all the individuals from front 1, 2, 3, etc. in order of decreasing crowdingRank 
		//until all 100 slots in generation P[1] are filled.
		frontCount = 1;
		j = 0;
		while(j<POPULATION_SIZE){ //while gen[i+1] is not full:		use j to know the spaces left in gen[i+1], use t to count how many individuals there are in the current front.
			//iterate over R and find the number of individuals with a front matching frontCount.
			t = 0;
			for(r=0; r<POPULATION_SIZE*2; r++){
				rQ = r - POPULATION_SIZE;
				if(r<POPULATION_SIZE){
					if(gen[i].population[r].getFront() == frontCount){
						t++;
					}
				}else{
					if(Q[i].population[rQ].getFront() == frontCount){
						t++;
					}
				}
			}
			if(debug) cout << "number of individuals with front " << frontCount << " found to be " << t << ". There are " << POPULATION_SIZE - j << " slots remaining in gen[" << i+1 << "]." << endl;
			//if this number of individuals is less than j, add every individual with this front to gen[i+1]
			if(t <= POPULATION_SIZE - j){
				if(debug) cout << "adding all individuals with front " << frontCount << " to gen[" << i+1 << "]:" << endl;
				for(r=0; r<POPULATION_SIZE*2; r++){
					rQ = r - POPULATION_SIZE;
					if(r<POPULATION_SIZE){
						if(gen[i].population[r].getFront() == frontCount){
							//reproduce(gen[i], r, gen[i+1], j);
							gen[i+1].population[j] = gen[i].population[r];
							j++;
							if (debug) cout << "\tR individual " << r << " added to slot " << j << " of gen[" << i+1 << "], front was " << gen[i].population[r].getFront() << endl;
						}
					}else{
						if(Q[i].population[rQ].getFront() == frontCount){
							//reproduce(Q[i], rQ, gen[i+1], j);
							gen[i+1].population[j] = Q[i].population[rQ];
							j++;
							if (debug) cout << "\tR individual " << r << " added to slot " << j << " of gen[" << i+1 << "], front was " << Q[i].population[rQ].getFront() << endl;
						}
					}
				}
			}
			//else, go through the individuals and if there is room in gen[i+1] (j>=0) then add the individuals to gen[i+1] in order of crowding rank.
			else{
				if(debug) cout << "number of individuals with front " << frontCount << " exceeds remaining space in gen[" << i+1 << "] (" << POPULATION_SIZE - j << ")." << endl;
				lastCrowdingRank = 9999999999;
				bestCrowdingRank = 0;
				while((t>0) && (j<POPULATION_SIZE)){
					//find the best crowding rank for that front in R. 
					for(r=0; r<POPULATION_SIZE*2; r++){
						rQ = r - POPULATION_SIZE;
						if(r<POPULATION_SIZE){
							if((gen[i].population[r].getFront() == frontCount) && (gen[i].population[r].getCrowdingRank() > bestCrowdingRank) && (gen[i].population[r].getCrowdingRank() < lastCrowdingRank)){
								bestCrowdingRank = gen[i].population[r].getCrowdingRank();
							}
						}else{
							if((Q[i].population[rQ].getFront() == frontCount) && (Q[i].population[rQ].getCrowdingRank() > bestCrowdingRank) && (Q[i].population[rQ].getCrowdingRank() < lastCrowdingRank)){
								bestCrowdingRank = Q[i].population[rQ].getCrowdingRank();
							}
						}
					}
					if(debug) cout << "bestCrowdingRank found was " << bestCrowdingRank << ", lastCrowdingRank was " << lastCrowdingRank << endl;
					//for each individual found below, if there is room, add them to gen[i+1].
					for(r=0; r<POPULATION_SIZE*2; r++){
						rQ = r - POPULATION_SIZE;
						if(r<POPULATION_SIZE){
							if((gen[i].population[r].getFront() == frontCount) && (gen[i].population[r].getCrowdingRank() == bestCrowdingRank) && (j<POPULATION_SIZE)){
								//reproduce(gen[i], r, gen[i+1], j);
								gen[i+1].population[j] = gen[i].population[r];
								if (debug) cout << "\tR individual " << r << " added to slot " << j << " of gen[" << i+1 << "], front was " << gen[i].population[r].getFront() << ", crowdingRank was " << gen[i].population[r].getCrowdingRank() << ". Slots remaining: " << POPULATION_SIZE - j -1 << endl;
								j++;
								t--;
							}
						}else{
							if((Q[i].population[rQ].getFront() == frontCount) && (Q[i].population[rQ].getCrowdingRank() == bestCrowdingRank) && (j<POPULATION_SIZE)){
								//reproduce(Q[i], rQ, gen[i+1], j);
								gen[i+1].population[j] = Q[i].population[rQ];
								if (debug) cout << "\tR individual " << r << " added to slot " << j << " of gen[" << i+1 << "], front was " << Q[i].population[rQ].getFront() << ", crowdingRank was " << Q[i].population[rQ].getCrowdingRank() << ". Slots remaining: " << POPULATION_SIZE - j -1 << endl;
								j++;
								t--;
							}
						}
						if(j<0){
							break;
						}
					}
					lastCrowdingRank = bestCrowdingRank;
					bestCrowdingRank = 0;
				}
			}
			lastCrowdingRank = bestCrowdingRank;
			bestCrowdingRank = 0;
			frontCount++;
		}			
	}
	cout << "\nEnd of GP run." << endl;
	return 0;
}
	

int performMultiCoreGP(int noOfCores, int numberOfGenerations, double crossoverRate, double mutationRate, double pointMutationRate){ //NOT FINISHED!
	if(POPULATION_SIZE <= 1){
		cout << "Population size is " << POPULATION_SIZE << ", which is too small for GP." << endl;
		return -1;
	}
	if((noOfCores < 1) || (noOfCores > 10)){
		cout << "Number of cores is " << noOfCores << ", which should be between 1 and 10." << endl;
		return -1;
	}
	cout << "Running a GP with " << numberOfGenerations << " generations, " << POPULATION_SIZE << " individuals per gen.\nParallel computation with " << noOfCores << " cores.\nCrossover rate: " << crossoverRate << ", Mutation rate: " << mutationRate << "." << endl;
	int i, j, k, core, coreRun, systemOutput, numberOfRequiredIterations, individual1, individual2, individualChosen1, individualChosen2, next, bestIndividual;
	double random, bestFitness;
	string s;
	srand(time(NULL));	//deliver a seed to initialise the RNG.
	try{
		makeInitialModel("/home/jack/roles-gp/roles/");
		compileModel("/home/jack/roles-gp/roles/");
	}catch(...){
		cout << "initial model could not be compiled. Aborting." << endl;
		return -1;
	}
	for(i=0; i<POPULATION_SIZE; i++){	//assign the initial model to each individual in generation 0 (the initial).
		gen[0].applyInitialModel(i);
	}
	for(i=1; i<POPULATION_SIZE; i++){	//heavily mutate all of gen 0 except the first individual. 
		pointMutation(0.2, gen[0], i, gen[0], i);
	}
	for(i=0; i<numberOfGenerations; i++){	//for each generation:
		cout << endl;
		//FOR LOOP BELOW REQUIRES PARALLEL OPERATION. DUPLICATE ROLES FOLDERS, RUN, COLLECT FITNESS FROM EACH. 
		//coreRun is the current iteration of running a number of cores in parallel to get through all individuals. 
		j = 0;
		numberOfRequiredIterations = (int)ceil((double)POPULATION_SIZE/(double)noOfCores); //e.g. for 10 individuals 3 cores = (10/3)=(3.33...)->(4) iterations to get through every individual.
		//DUPLICATE THE ROLES MODEL FOLDER AS ROLES1, ROLES2, ETC. NUMBER OF FOLDERS = NO. OF CORES
		//cout << "Checking if processor is available...\t";
		//if (system(NULL)) puts ("Ok");
		//else exit (EXIT_FAILURE);
		chdir("/home/jack/roles-gp/");
		for(core=1; core<=noOfCores; core++){
			systemOutput = system(("cp -a roles roles" + to_string(core)).c_str());
			//cout << "File roles" << core << " created status: " << systemOutput << endl;
		}
		for(coreRun=1; coreRun<=numberOfRequiredIterations; coreRun++){	//for each individual in a generation:
			cout << "generation " << i << " individuals " << (j+1) << " to " << (j+noOfCores) << ":" << endl;
			//COMPILE, RUN, AND OBTAIN THE FITNESS OF EACH ROLES MODEL IN PARALLEL.
			//APPLY THE INITIAL MODEL TO EACH ROLES FOLDER COPY.
			for(core=1; core<=noOfCores; core++){
				j = (core-1) + ((coreRun-1)*noOfCores);	//j corresponds to the individual in the array being applied. 
				if(j <= POPULATION_SIZE){
					applyIndividualToModel(gen[i], j, "/home/jack/roles-gp/roles" + to_string(core) + "/");	//set the model to contain the equations given by an individual.
					cout << "applied individual " << j << " to folder roles" << core << " during core iteration " << coreRun << "." << endl;
				}
			}
			//COMPILE AND RUN EACH ROLES FOLDER MODEL IN PARALLEL.
			chdir("/home/jack/roles-gp/");
			s = s + "cd roles" + to_string(1).c_str() + "/ && make compile_roleTheory_run_model";
			for(core=2; core<=noOfCores; core++){
				s = s + " & cd roles" + to_string(core).c_str() + "/ && make compile_roleTheory_run_model";
			}
			system(s.c_str() + noOfCores);
			
			return 0;
			try{					//attempt to:
				if(runModelJustCompileRT("/home/jack/roles-gp/roles") != 0){		//run the model, just recompiling the RoleTheory.cpp file.
					gen[i].population[j].setFitness(999999999); //if there is an exception, set the fitness to 999,999,999. 
					cout << "generation " << i << " individual " << (j+1) << " could not be run (didn't return 0). Fitness set to: " << gen[i].population[j].getFitness() << endl;
				
				}else{
					gen[i].population[j].setFitness(calculateFitness("/home/jack/roles-gp/roles/"));	//calculate and assign the fitness
					// gen[i].population[j].setFitness((rand()%10));
					cout << "generation " << i << " individual " << (j+1) << " had fitness " << gen[i].population[j].getFitness() << endl;
				}
			}catch(...){
				gen[i].population[j].setFitness(999999999); //if there is an exception, set the fitness to 999,999,999. 
				cout << "generation " << i << " individual " << (j+1) << " could not be run (compile failed). Fitness set to: " << gen[i].population[j].getFitness() << endl << endl;
			}
			//cout << gen[i].getRoleLoadCode(j) << endl;
			//cout << gen[i].getRoleIncongruenceCode(j) << endl;
			//cout << gen[i].getRoleStrainCode(j) << endl;
			//cout << gen[i].getFirstDrinkDispositionCode(j) << endl;
			//cout << gen[i].getNextDrinkDispositionCode(j) << endl;
			cout << endl;
		}
		for(j=0; j<POPULATION_SIZE; j++){
			cout << "Individual " << j+1;
			cout << ", Fitness: " << to_string(gen[i].population[j].getFitness());
			cout << ", \tComplexity: " << to_string(gen[i].population[j].getComplexity()) << endl;
		}
		cout << "The average fitness of generation " << i << " was " << gen[i].calculateAverageFitness() << endl;
		gen[i].printGenerationToFile(("./generations/generation" + to_string(i)));
		/*if(i==0){
			cout << "\ncontinue GP with this initial generation? (y/n) : ";
			cin >> s;
			if((s != "Y") && (s != "y") && (s != "YES") && (s != "yes") && (s != "Yes")){
				cout << "GP aborted." << endl;
				return 0;
			}
			cout << endl;
		}*/
		if((i+1) != numberOfGenerations){	//create the next generation, if there is one.
			bestIndividual = 0;			//the best individual is reproduced into the next generation as individual 1.
			bestFitness = gen[i].population[0].getFitness();
			for(j=1; j<POPULATION_SIZE; j++){	
				if(gen[i].population[j].getFitness() < bestFitness){
					bestFitness = gen[i].population[j].getFitness();
					bestIndividual = j;
				}
			}
			reproduce(gen[i], bestIndividual, gen[i+1], 0);	//Perfrom reproduction.
			cout << "Generation " << (i+1) << " individual 1 is identical to generation " << i << " individual " << (bestIndividual+1) << ", a copy of the best individual with fitness " << gen[i].population[bestIndividual].getFitness() << "." << endl;
			for(j=1; j<POPULATION_SIZE; j++){	//the remaining individuals are created using tournament selection.
				//Tournament selection.
				next = 0;
				while(!next){
					individual1 = rand() % POPULATION_SIZE;	//Randomly select an individual for tournament selection.
					if(debug) cout << "individual1 = " << individual1 << endl;
					if ((gen[i].population[individual1].getFitness() < 3000) && (!isnan(gen[i].population[individual1].getFitness()))){ //don't pick individuals that did not compile or individuals with nan as the fitness.
						next = 1;
					}else{
						if(debug) cout << "individual rejected, fitness: " << gen[i].population[individual1].getFitness() << endl;
					}

				}
				next = 0;
				while(!next){
					individual2 = rand() % POPULATION_SIZE;	//Randomly select a second, different individual.
					if(debug) cout << "individual2 = " << individual2 << endl;
					if ((gen[i].population[individual2].getFitness() < 3000) && (!isnan(gen[i].population[individual2].getFitness()))){ //don't pick individuals that did not compile or individuals with nan as the fitness.
						next = 1;
					}else{
						if(debug) cout << "individual rejected, fitness: " << gen[i].population[individual2].getFitness() << endl;
					}
					if(debug) cout << "	trial individual2 = " << individual2 << endl;
				}
				if(gen[i].population[individual1].getFitness() <= gen[i].population[individual2].getFitness()){	//Remember the best individual by comparing fitness.
					individualChosen1 = individual1;
					if(debug) cout << "individual 1 chosen." << endl;
				}else{
					individualChosen1 = individual2;
					if(debug) cout << "individual 2 chosen." << endl;
				}
				
				random = ( ((double)rand()) / (RAND_MAX) );	//Decide whether to use crossover, mutation, or reproduction.
				if(random <= crossoverRate){	//Choose crossover.
					//cout << "Crossover chosen." << endl;
					//Select a second parent.
					next = 0;
					while(!next){
						individual1 = rand() % POPULATION_SIZE;	//Randomly select an individual for tournament selection.
						if(debug) cout << "second individual1 = " << individual1 << endl;
						if ((gen[i].population[individual1].getFitness() < 3000) && (!isnan(gen[i].population[individual1].getFitness()))){ //don't pick individuals that did not compile or individuals with nan as the fitness.
							next = 1;
						}else{
							if(debug) cout << "second individual rejected, fitness: " << gen[i].population[individual1].getFitness() << endl;
						}

					}
					next = 0;
					while(!next){
						individual2 = rand() % POPULATION_SIZE;	//Randomly select a second, different individual.
						if(debug) cout << "second individual2 = " << individual2 << endl;
						if ((gen[i].population[individual2].getFitness() < 3000) && (!isnan(gen[i].population[individual2].getFitness()))){ //don't pick individuals that did not compile or individuals with nan as the fitness.
							next = 1;
						}else{
							if(debug) cout << "second individual rejected, fitness: " << gen[i].population[individual2].getFitness() << endl;

						}
					}
					if(debug) cout << "	trial individual2 = " << individual2 << endl;
					if(gen[i].population[individual1].getFitness() <= gen[i].population[individual2].getFitness()){	//Remember the best individual by comparing fitness.
						individualChosen2 = individual1;
						if(debug) cout << "second individual 1 chosen." << endl;
					}else{
						individualChosen2 = individual2;
						if(debug) cout << "second individual 2 chosen." << endl;
					}
					//Perform crossover.
					cout << "Generation " << (i+1) << " individual " << (j+1) << " is a child of generation " << i << " individuals " << (individualChosen1+1) << " and " << (individualChosen2+1) << endl;
					crossover(gen[i], individualChosen1, individualChosen2, gen[i+1], j);
				}else if((random - crossoverRate) <= mutationRate){	//Choose mutation.
					//cout << "Mutation chosen." << endl;					
					if(!(rand() % 2)){	//50:50 chance of subtree mutation or point mutation.
						subtreeMutation(gen[i], individualChosen1, gen[i+1], j); //Perform subtree mutation.
						cout << "Generation " << (i+1) << " individual " << (j+1) << " is a subtree mutant of generation " << i << " individual " << (individualChosen1+1) << endl;
					}else{
						pointMutation(pointMutationRate, gen[i], individualChosen1, gen[i+1], j);	//Perform point mutation.
						cout << "Generation " << (i+1) << " individual " << (j+1) << " is a point mutant of generation " << i << " individual " << (individualChosen1+1) << endl;
					}
				}else{	//perform reproduction
					//cout << "Reproduction chosen." << endl;
					reproduce(gen[i], individualChosen1, gen[i+1], j);	//Perfrom reproduction.
					cout << "Generation " << (i+1) << " individual " << (j+1) << " is identical to generation " << i << " individual " << (individualChosen1+1) << endl;
				}
			}
		}
	}
	cout << "\nEnd of GP run." << endl;
	return 0;
}


int performGP(int numberOfGenerations, double crossoverRate, double mutationRate, double pointMutationRate){
	/* //File output location test.
	Generation gen1;	//contains 100 individuals numbered 0 to 99. 
	gen1.applyInitialModel(0);
	gen1.population[0].setFitness(1.11294);
	gen1.printGenerationToFile("generationOutTest1");
	cout << "done" << endl;*/

	if(POPULATION_SIZE <= 1){
		cout << "Population size is " << POPULATION_SIZE << ", which is too small for GP." << endl;
			return -1;
	}
	cout << "Running a GP with " << numberOfGenerations << " generations, " << POPULATION_SIZE << " individuals per gen.\nCrossover rate: " << crossoverRate << ", Mutation rate: " << mutationRate << "." << endl;
	int i, j, k, individual1, individual2, individualChosen1, individualChosen2, next, bestIndividual;
	double random, bestFitness;
	string s;
	srand(time(NULL));	//deliver a seed to initialise the RNG.
	
	try{
		makeInitialModel("/home/jack/roles-gp/roles/");
		compileModel("/home/jack/roles-gp/roles/");
	}catch(...){
		cout << "model could not be compiled. Aborting." << endl;
		return -1;
	}
	//gen1.printGenerationToFile("generationOutTest2");
	for(i=0; i<POPULATION_SIZE; i++){	//assign the initial model to each individual in generation 0 (the initial).
		gen[0].applyInitialModel(i);
	}
	for(i=1; i<POPULATION_SIZE; i++){	//heavily mutate all of gen 0 except the first individual. 
		pointMutation(0.2, gen[0], i, gen[0], i);
	}
	for(i=0; i<numberOfGenerations; i++){	//for each generation:
		cout << endl;
		for(j=0; j<POPULATION_SIZE; j++){	//for each individual in a generation:
			cout << "generation " << i << " individual " << (j+1) << ":" << endl;
			applyIndividualToModel(gen[i], j, "/home/jack/roles-gp/roles/");	//set the model to contain the equations given by an individual.
			try{					//attempt to:
				if(runModelJustCompileRT("/home/jack/roles-gp/roles") != 0){		//run the model, just recompiling the RoleTheory.cpp file.
					gen[i].population[j].setFitness(999999999); //if there is an exception, set the fitness to 999,999,999. 
					cout << "generation " << i << " individual " << (j+1) << " could not be run (didn't return 0). Fitness set to: " << gen[i].population[j].getFitness() << endl;
				
				}else{
					gen[i].population[j].setFitness(calculateFitness("/home/jack/roles-gp/roles/"));	//calculate and assign the fitness
					// gen[i].population[j].setFitness((rand()%10));
					cout << "generation " << i << " individual " << (j+1) << " had fitness " << gen[i].population[j].getFitness() << endl;
				}
			}catch(...){
				gen[i].population[j].setFitness(999999999); //if there is an exception, set the fitness to 999,999,999. 
				cout << "generation " << i << " individual " << (j+1) << " could not be run (compile failed). Fitness set to: " << gen[i].population[j].getFitness() << endl << endl;
			}
			//gen[i].population[j].setFitness((rand()%10));	//COMMENT THIS LINE AFTER TESTING.
			//cout << gen[i].getRoleLoadCode(j) << endl;
			//cout << gen[i].getRoleIncongruenceCode(j) << endl;
			//cout << gen[i].getRoleStrainCode(j) << endl;
			//cout << gen[i].getFirstDrinkDispositionCode(j) << endl;
			//cout << gen[i].getNextDrinkDispositionCode(j) << endl;
			cout << endl;
			//if(remove("/home/jack/roles-gp/roles/outputs/annual_data.csv")==0){cout << "file removed" << endl; 
			//}else{ cout << "file not removed" << endl;}
		}
		for(j=0; j<POPULATION_SIZE; j++){
			cout << "Individual " << j+1;
			cout << ", Fitness: " << to_string(gen[i].population[j].getFitness());
			cout << ", \tComplexity: " << to_string(gen[i].population[j].getComplexity()) << endl;
		}
		cout << "The average fitness of generation " << i << " was " << gen[i].calculateAverageFitness() << endl;
		//gen1.printGenerationToFile("./generations/generationOutTest3");
		//gen[i].printGenerationToFile("./generations/generationOutTest4");
		gen[i].printGenerationToFile(("./generations/generation" + to_string(i)));
		/*if(i==0){
			cout << "\ncontinue GP with this initial generation? (y/n) : ";
			cin >> s;
			if((s != "Y") && (s != "y") && (s != "YES") && (s != "yes") && (s != "Yes")){
				cout << "GP aborted." << endl;
				return 0;
			}
			cout << endl;
		}*/
		if((i+1) != numberOfGenerations){	//create the next generation, if there is one.
			bestIndividual = 0;			//the best individual is reproduced into the next generation as individual 1.
			bestFitness = gen[i].population[0].getFitness();
			for(j=1; j<POPULATION_SIZE; j++){	
				if(gen[i].population[j].getFitness() < bestFitness){
					bestFitness = gen[i].population[j].getFitness();
					bestIndividual = j;
				}
			}
			reproduce(gen[i], bestIndividual, gen[i+1], 0);	//Perfrom reproduction.
			cout << "Generation " << (i+1) << " individual 1 is identical to generation " << i << " individual " << (bestIndividual+1) << ", a copy of the best individual with fitness " << gen[i].population[bestIndividual].getFitness() << "." << endl;
			for(j=1; j<POPULATION_SIZE; j++){	//the remaining individuals are created using tournament selection.
				//Tournament selection.
				next = 0;
				while(!next){
					individual1 = rand() % POPULATION_SIZE;	//Randomly select an individual for tournament selection.
					cout << "individual1 = " << individual1 << endl;
					if ((gen[i].population[individual1].getFitness() < 3000) && (!isnan(gen[i].population[individual1].getFitness()))){ //don't pick individuals that did not compile or individuals with nan as the fitness.
						next = 1;
					}else{
						cout << "individual rejected, fitness: " << gen[i].population[individual1].getFitness() << endl;
					}

				}
				next = 0;
				while(!next){
					individual2 = rand() % POPULATION_SIZE;	//Randomly select a second, different individual.
					cout << "individual2 = " << individual2 << endl;
					if ((gen[i].population[individual2].getFitness() < 3000) && (!isnan(gen[i].population[individual2].getFitness()))){ //don't pick individuals that did not compile or individuals with nan as the fitness.
						next = 1;
					}else{
						cout << "individual rejected, fitness: " << gen[i].population[individual2].getFitness() << endl;
					}
					cout << "	trial individual2 = " << individual2 << endl;
				}
				if(gen[i].population[individual1].getFitness() <= gen[i].population[individual2].getFitness()){	//Remember the best individual by comparing fitness.
					individualChosen1 = individual1;
					cout << "individual 1 chosen." << endl;
				}else{
					individualChosen1 = individual2;
					cout << "individual 2 chosen." << endl;
				}
				
				random = ( ((double)rand()) / (RAND_MAX) );	//Decide whether to use crossover, mutation, or reproduction.
				if(random <= crossoverRate){	//Choose crossover.
					//cout << "Crossover chosen." << endl;
					//Select a second parent.
					next = 0;
					while(!next){
						individual1 = rand() % POPULATION_SIZE;	//Randomly select an individual for tournament selection.
						cout << "second individual1 = " << individual1 << endl;
						if ((gen[i].population[individual1].getFitness() < 3000) && (!isnan(gen[i].population[individual1].getFitness()))){ //don't pick individuals that did not compile or individuals with nan as the fitness.
							if(gen[i].population[individual1].getDepth(1) >= gen[i].population[individualChosen1].getDepth(1)){	//make sure all trees have a depth greater than or equal to all individualChosen1 tree depths, to avoid crossover infinite loop.
								if(gen[i].population[individual1].getDepth(2) >= gen[i].population[individualChosen1].getDepth(2)){
									if(gen[i].population[individual1].getDepth(3) >= gen[i].population[individualChosen1].getDepth(3)){
										if(gen[i].population[individual1].getDepth(4) >= gen[i].population[individualChosen1].getDepth(4)){
											if(gen[i].population[individual1].getDepth(5) >= gen[i].population[individualChosen1].getDepth(5)){
												if (debug) cout << "tree depths: " << gen[i].population[individual1].getDepth(1) << ", " << gen[i].population[individual1].getDepth(2);
												if (debug) cout << ", " << gen[i].population[individual1].getDepth(3) << ", " << gen[i].population[individual1].getDepth(4);
												if (debug) cout << ", " << gen[i].population[individual1].getDepth(5) << endl;
												next = 1;
											}
										}
									}
								}
							}
						}else{
							cout << "second individual rejected, fitness: " << gen[i].population[individual1].getFitness() << endl;
						}

					}
					next = 0;
					while(!next){
						individual2 = rand() % POPULATION_SIZE;	//Randomly select a second, different individual.
						cout << "second individual2 = " << individual2 << endl;
						if ((gen[i].population[individual2].getFitness() < 3000) && (!isnan(gen[i].population[individual2].getFitness()))){ //don't pick individuals that did not compile or individuals with nan as the fitness.
							if(gen[i].population[individual2].getDepth(1) >= gen[i].population[individualChosen1].getDepth(1)){	//make sure all trees have a depth greater than or equal to all individualChosen1 tree depths, to avoid crossover infinite loop.
								if(gen[i].population[individual2].getDepth(2) >= gen[i].population[individualChosen1].getDepth(2)){
									if(gen[i].population[individual2].getDepth(3) >= gen[i].population[individualChosen1].getDepth(3)){
										if(gen[i].population[individual2].getDepth(4) >= gen[i].population[individualChosen1].getDepth(4)){
											if(gen[i].population[individual2].getDepth(5) >= gen[i].population[individualChosen1].getDepth(5)){
												if (debug) cout << "tree depths: " << gen[i].population[individual2].getDepth(1) << ", " << gen[i].population[individual2].getDepth(2);
												if (debug) cout << ", " << gen[i].population[individual2].getDepth(3) << ", " << gen[i].population[individual2].getDepth(4);
												if (debug) cout << ", " << gen[i].population[individual2].getDepth(5) << endl;
												next = 1;
											}
										}
									}
								}
							}
						}else{
							cout << "second individual rejected, fitness: " << gen[i].population[individual2].getFitness() << endl;

						}
					}
					cout << "	trial individual2 = " << individual2 << endl;
					if(gen[i].population[individual1].getFitness() <= gen[i].population[individual2].getFitness()){	//Remember the best individual by comparing fitness.
						individualChosen2 = individual1;
						cout << "second individual 1 chosen." << endl;
					}else{
						individualChosen2 = individual2;
						cout << "second individual 2 chosen." << endl;
					}
					//Perform crossover.
					cout << "Generation " << (i+1) << " individual " << (j+1) << " is a child of generation " << i << " individuals " << (individualChosen1+1) << " and " << (individualChosen2+1) << endl;
					crossover(gen[i], individualChosen1, individualChosen2, gen[i+1], j);
					cout << "crossover successful" << endl;
				}else if((random - crossoverRate) <= mutationRate){	//Choose mutation.
					//cout << "Mutation chosen." << endl;					
					if(!(rand() % 2)){	//50:50 chance of subtree mutation or point mutation.
						subtreeMutation(gen[i], individualChosen1, gen[i+1], j); //Perform subtree mutation.
						cout << "Generation " << (i+1) << " individual " << (j+1) << " is a subtree mutant of generation " << i << " individual " << (individualChosen1+1) << endl;
					}else{
						pointMutation(pointMutationRate, gen[i], individualChosen1, gen[i+1], j);	//Perform point mutation.
						cout << "Generation " << (i+1) << " individual " << (j+1) << " is a point mutant of generation " << i << " individual " << (individualChosen1+1) << endl;
					}
				}else{	//perform reproduction
					//cout << "Reproduction chosen." << endl;
					reproduce(gen[i], individualChosen1, gen[i+1], j);	//Perfrom reproduction.
					cout << "Generation " << (i+1) << " individual " << (j+1) << " is identical to generation " << i << " individual " << (individualChosen1+1) << endl;
				}
			}
		}
	}
	cout << "\nEnd of GP run." << endl;
	return 0;
}

int testPrimitiveGenerator(){
	srand(time(NULL));
	cout << generatePrimitive(0) << " " << generatePrimitive(0) << " " << generatePrimitive(0) << endl;
	cout << generatePrimitive(1) << " " << generatePrimitive(1) << " " << generatePrimitive(1) << endl;
	cout << generatePrimitive(2) << " " << generatePrimitive(2) << " " << generatePrimitive(2) << endl;
	cout << generatePrimitive(3) << " " << generatePrimitive(3) << " " << generatePrimitive(3) << endl;
	cout << generatePrimitive(4) << " " << generatePrimitive(4) << " " << generatePrimitive(4) << endl;
	cout << generatePrimitive(5) << " " << generatePrimitive(5) << " " << generatePrimitive(5) << endl;
}

int testFunction(int function){ //1 for reproduction, 2 for crossovver, 3 for subtree mutation, 4 for point mutation.
	srand(time(NULL));
	Generation gen[2];
	gen[0].applyInitialModel(0);	
	gen[0].applyInitialModel(1);
	switch(function){
		case 1:
			reproduce(gen[0], 0, gen[1], 0);		break;
		case 2:
			crossover(gen[0], 0, 1, gen[1], 0);		break;
		case 3:
			subtreeMutation(gen[0], 0, gen[1], 0);		break;
		case 4:
			pointMutation(0.2, gen[0], 0, gen[1], 0);	break;
		default:
			reproduce(gen[0], 0, gen[1], 1);
	}
	cout << endl << gen[0].getRoleLoadCode(0);
	cout << endl << gen[1].getRoleLoadCode(0) << endl << endl;
	cout << endl << gen[0].getRoleIncongruenceCode(0);
	cout << endl << gen[1].getRoleIncongruenceCode(0) << endl << endl;
	cout << endl << gen[0].getRoleStrainCode(0);
	cout << endl << gen[1].getRoleStrainCode(0) << endl << endl;
	cout << endl << gen[0].getFirstDrinkDispositionCode(0);
	cout << endl << gen[1].getFirstDrinkDispositionCode(0) << endl << endl;
	cout << endl << gen[0].getNextDrinkDispositionCode(0);
	cout << endl << gen[1].getNextDrinkDispositionCode(0) << endl << endl;
	gen[0].printGenerationToFile("generation0");
	gen[1].printGenerationToFile("generation1");
}

int makeInitialModel(string roles_path){
	srand(time(NULL));
	Generation gen[2];
	gen[0].applyInitialModel(0);
	applyIndividualToModel(gen[0], 0, (roles_path));

	return 0;
}


int testInitialModel(string roles_path){
	srand(time(NULL));
	Generation gen[2];
	gen[0].applyInitialModel(0);
	applyIndividualToModel(gen[0], 0, (roles_path));
	compileAndRunModel("/home/jack/roles-gp/roles");

	return 0;
}

int testCppEquation(){	//define a method to test the FileEdit.h class on the test.cpp file.
	Generation gen1;	//contains 100 individuals numbered 0 to 99. 
	gen1.applyInitialModel(0);
	
	cout << gen1.getRoleLoadCode(0) << endl << endl;
	cout << gen1.getRoleIncongruenceCode(0) << endl << endl;
	cout << gen1.getRoleStrainCode(0) << endl << endl;
	cout << gen1.getFirstDrinkDispositionCode(0) << endl << endl;
	cout << gen1.getNextDrinkDispositionCode(0) << endl << endl;
	gen1.population[0].setFitness(1.11294);

	gen1.printGenerationToFile("generationOutTest");
	cout << "done" << endl;
	
	return 0;
}

int testFileEdit(){	//define a method to test the storage and C++ conversion of the original roles model. 
	FileEdit testFile("/home/jack/roles-gp/programs/", "test.cpp");	

	int lineNumber = 0;
	//testFile.setFile("/home/jack/roles-gp/programs/", "test.cpp");
	lineNumber = testFile.findText("4) find this");
	cout << lineNumber << endl;
	testFile.replaceLine(lineNumber + 1, "Replaced.");
	return 0;
}

int testCSVReading(){
	FileEdit csvFile("/home/jack/roles-gp/programs/", "target graphs.csv");
	cout << "row 2 item 1: '" << csvFile.readCSV(2, 1) << "'" << endl;
	cout << "row 2 item 2: '" << csvFile.readCSV(2, 2) << "'" << endl;
	cout << "row 2 item 4: '" << csvFile.readCSV(2, 4) << "'" << endl;
	cout << "row 2 item 25: '" << csvFile.readCSV(2, 25) << "'" << endl;
	cout << "row 2 item 26: '" << csvFile.readCSV(2, 26) << "'" << endl;
	cout << "row 2 item 38: '" << csvFile.readCSV(2, 38) << "'" << endl;
	cout << "row 3 item 1: '" << csvFile.readCSV(3, 1) << "'" << endl;
	cout << "row 3 item 38: '" << csvFile.readCSV(3, 38) << "'" << endl;
	cout << "row 4 item 1: '" << csvFile.readCSV(4, 1) << "'" << endl;
	cout << "row 4 item 38: '" << csvFile.readCSV(4, 38) << "'" << endl;
	return 0;	
}

int testFitnessCalculation(){
	double fitness = calculateFitness("/home/jack/roles-gp/roles/");
	cout << "Fitness: " << fitness << endl;
	return 0;
}

int testIdenticalFunction(){
	Generation gen1;	//contains 100 individuals numbered 0 to 99. 
	gen1.applyInitialModel(0);
	gen1.applyInitialModel(1);
	gen1.applyInitialModel(2);
	gen1.population[2].setRoleLoadItem(1, "-");
	cout << "should return 1 (are identical): " << isIdentical(gen1, 0, gen1, 1) << endl;
	cout << "should return 0 (are not identical): " << isIdentical(gen1, 0, gen1, 2) << endl;
}
