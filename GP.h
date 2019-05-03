/* GP.h */

#ifndef GP_H
#define GP_H

#include "FileEdit.h"
#include "Individual.h"
#include "Generation.h"

#include <stdio.h>	/* remove */
#include <stdlib.h>	/* rand, system, NULL, EXIT_FAILURE */
#include <unistd.h>	/* chdir */
#include <iostream>	/* cout, cin */
#include <fstream>	/* fin, fout */
#include <string>	/* string */
#include <ctime>	/* time */
#include <cmath>
#include <iomanip>	/* setprecision */

using namespace std;	

int isFunction(string primitive);	//declare a method to return a 1 if supplied with a function and 0 if not. 
string generatePrimitive(int tier);	//declare a method to generate a terminal up to and including the given primitive tier level.
int getSubNode(int node, int number);	//declare a method to return the node array numbers beneath a given node. 

int reproduce(Generation& parents, int pPosition, Generation& children, int cPosition); 	//declare a method to copy and paste an individual to a new gen.
int crossover(Generation& parents, int p1Position, int p2Position, Generation& children, int cPosition);	//declare a method to perform subtree crossover on a random subtree of a random syntax tree and transfer it to a new gen.
int subtreeMutation(Generation& parents, int pPosition, Generation& children, int cPosition);	//declare a method to perform subtree mutation on an individual by randomly selecting a node, mutating it and all nodes beneath it, and transfering the new individual to the next gen. 
int pointMutation(double mutationRate, Generation& parents, int pPosition, Generation& children, int cPosition);	//declare a method to perform point mutation on an individual randomly using a given mutation rate and transfer it to the next gen. 

//int testIndividualForCorrectness(Generation& gen, int position);	//declare a method to test an individual for correctness of all its syntax trees. Returns 0 if correct, -1 if incorrect. 

int applyIndividualToModel(Generation& gen, int position, string roles_path);	//declare a method to apply an individual's equations to the model.
int compileAndRunModel(string roles_path);	//declare a method to run the model.
int compileModel(string roles_path);	//declare a method to compile the model (but not run it).
int runModelJustCompileRT(string roles_path);	//declare a method to recompile just the RoleTheory.cpp file, then run the model. 
double calculateFitness(string roles_path);	//declare a method to calculate the fitness of an individual using the current model output. //INCOMPLETE, currently just returns a random double. 

double compileRTRunReturnFitness(string roles_path); //declare a method to compule the RolesTheory.cpp file, run the model, assess the fitness and return the fitness value.
int isIdentical(Generation& gen1, int individual1, Generation& gen2, int individual2);	//declare a method to test if two individuals are identical. 

#endif
