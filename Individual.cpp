/* Individual.cpp */

#include "Individual.h"

using namespace std;

Individual::Individual(){	//unparametrised constructor. 
	int i;
	complexity = -1;
	fitness = -1;
	for(i=0; i<63; i++){
		role_load[i] = "_";
		role_incongruence[i] = "_";
		role_strain[i] = "_";
		first_drink_disposition[i] = "_";
		next_drink_disposition[i] = "_";
	}
	np = -1;
	front = -1;
	rank = -1;
}
//in the methods below, item is range [1 - 64].
string Individual::getRoleLoadItem(int item){			return role_load[item-1];		}
string Individual::getRoleIncongruenceItem(int item){		return role_incongruence[item-1];	}
string Individual::getRoleStrainItem(int item){			return role_strain[item-1];		}
string Individual::getFirstDrinkDispositionItem(int item){	return first_drink_disposition[item-1];	}
string Individual::getNextDrinkDispositionItem(int item){	return next_drink_disposition[item-1];	}
int Individual::getComplexity(){	return complexity;	}
double Individual::getFitness(){	return fitness;		}
int Individual::getNP(){	return np;	}
int Individual::getFront(){	return front;	}
int Individual::getRank(){	return rank;	}
double Individual::getCrowdingRank(){	return crowdingRank;	}


void Individual::setRoleLoadItem(int item, string value){		role_load[item-1] = value;		}
void Individual::setRoleIncongruenceItem(int item, string value){	role_incongruence[item-1] = value;	}
void Individual::setRoleStrainItem(int item, string value){		role_strain[item-1] = value;		}
void Individual::setFirstDrinkDispositionItem(int item, string value){	first_drink_disposition[item-1] = value;}
void Individual::setNextDrinkDispositionItem(int item, string value){	next_drink_disposition[item-1] = value; }

void Individual::calculateComplexity(){
	int i;
	complexity=0;
	for(i=0; i<63; i++){
		if(role_load[i] != "_"){		complexity++;	}
		if(role_incongruence[i] != "_"){	complexity++;	}
		if(role_strain[i] != "_"){		complexity++;	}
		if(first_drink_disposition[i] != "_"){	complexity++;	}
		if(next_drink_disposition[i] != "_"){	complexity++;	}
	}
}

void Individual::setComplexity(int complexityIn){	complexity = complexityIn;	}
void Individual::setFitness(double fitnessIn){	fitness = fitnessIn;	}
void Individual::setNP(int NPIn){	np = NPIn;	}
void Individual::setFront(int frontIn){	front = frontIn;	}
void Individual::setRank(int rankIn){	rank = rankIn;	}
void Individual::setCrowdingRank(double crowdingRankIn){	crowdingRank = crowdingRankIn;	}
	
int Individual::getDepth(int tree){	//define a method to find the number of the last array slot that is full, then return the syntax tree depth. 
	int i, last=0;
	switch(tree){
		case 1: //RoleLoad
			for(i=1; i<64; i++){
				if(getRoleLoadItem(i) != "_"){
					last = i;
				}
			}
			break;
		case 2: //RoleIncongruence
			for(i=1; i<64; i++){
				if(getRoleIncongruenceItem(i) != "_"){
					last = i;
				}
			}
			break;
		case 3: //RoleStrain
			for(i=1; i<64; i++){
				if(getRoleStrainItem(i) != "_"){
					last = i;
				}
			}
			break;
		case 4:
			for(i=1; i<64; i++){
				if(getFirstDrinkDispositionItem(i) != "_"){
					last = i;
				}
			}
			break;
		case 5:
			for(i=1; i<64; i++){
				if(getNextDrinkDispositionItem(i) != "_"){
					last = i;
				}
			}
			break;
	}
	if(last == 1)								{	return 1;	}
	else if((last >= 2) && (last <= 3))		{	return 2;	}
	else if((last >= 4) && (last <= 7))		{	return 3;	}
	else if((last >= 8) && (last <= 15))	{	return 4;	}
	else if((last >= 16) && (last <= 31))	{	return 5;	}
	else if((last >= 32) && (last <= 64))	{	return 6;	}
	else {cout << "error in getDepth(), last was: " << last << endl;	return -1;	}
}