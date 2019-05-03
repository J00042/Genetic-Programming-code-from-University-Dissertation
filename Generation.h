/* Generation.h */

#ifndef GENERATION_H
#define GENERATION_H

#define POPULATION_SIZE 100 //Change to 100 or so later. 

#include "Individual.h"
#include "FileEdit.h"
#include <unistd.h>	/* chdir */
#include <iostream>	/* cout, cin */
#include <fstream>	/* fin, fout */
#include <string>	/* string */
#include <iomanip>	/* setprecision */

using namespace std;

class Generation{
   private:
	string equationConstructor(string left, string middle, string right);	//turn three primitives from the tree into an equation. 
	string getArrayItem(int position, int arrayNumber, int item);	//declare function to return an item from a syntax tree using number references. 
	//string getCode15Primitives(int position, int arrayNumber);	//declare a function to return the equation from a syntax tree with 15 primitives. 
	string getCode63Primitives(int position, int arrayNumber);	//declare a function to return the equation from a syntax tree with 63 primitives. 
	//string getCode127Primitives(int position, int arrayNumber);	//declare a function to return the equation from a syntax tree with 127 primitives. 

   public:
	Generation();	//constructor (cannot be parameterised as arrays cannot be passed without being a pointer).

	Individual population[POPULATION_SIZE];	//declare the population of individuals.
	
	void applyInitialModel(int position);	//set the initial model parameters to an individual.

	string getRoleLoadCode(int position);	//generate C++ code for the role load equation from a specific individual.
	string getRoleIncongruenceCode(int position);	//generate C++ code for the role incongruence equation from a specific individual.
	string getRoleStrainCode(int position);	//generate C++ code for the role strain equation from a specific individual.
	string getFirstDrinkDispositionCode(int position);	//generate C++ code for the first drink disposition equation from a specific individual.
	string getNextDrinkDispositionCode(int position);	//generate C++ code for the next drink disposition equation from a specific individual.
	void printGenerationToFile(string filename);	//create a text file and output the data of this generation to it. 
	double calculateAverageFitness();	//return the mean fitness of this generation, excluding nan and 999999999. 
	int individualsThatCompiled();	//return the number of individuals that compiled and did not have a fitness of 999999999 or nan this generation. 

};	

#endif
