/* GP.cpp */

#include "GP.h"

using namespace std;

int debugGP = 0;

//define a method to return a 1 if supplied with a function and 0 if not. 
int isFunction(string primitive){
	if((primitive != "+") && (primitive != "-") && (primitive != "*") && (primitive != "/") && (primitive != "^") && (primitive != "abs") && (primitive != "(1-x)")){
		return 0;
	}else{
		return 1;
	}
}

//define a method to generate a terminal up to and including the given primitive tier level. 
//Note: should initialise the random number generator seed once (or more) before running this function via srand(time(NULL)); 
string generatePrimitive(int tier){	//tier = 0 for function, 1-5 for terminal. 
	int randomConstantChance = 10;
	string tier0Functions[] = {"+", "-", "*", "/", "^", "abs", "(1-x)"};
	string tier1Terminals[] = {"mRoleLoadBeta1", "mRoleLoadBeta2", "mRoleLoadBeta3", "mRoleLoadBeta4", "(double)mMarritalStatus", "(double)mParenthoodStatus", "(double)mEmploymentStatus", "mLevelofInvolvementMarriage", "mLevelofInvolvementParenthood", "mLevelofInvolvementEmployment", "mpRolesEntity->getRoleExpectancyMarriage(sex,findAgeGroup())", "mpRolesEntity->getRoleExpectancyParenthood(sex,findAgeGroup())", "mpRolesEntity->getRoleExpectancyEmployment(sex,findAgeGroup())", "MAX_INCONGRUENCE", "mRoleSkill", "mRoleDrinkingOpportunityBeta1", "mRoleDrinkingOpportunityBeta2", "mRoleDrinkingOpportunityBeta3", "mRoleDrinkingOpportunityBeta4", "mBaselineOpportunity", "mDaysOfSocialisation", "mBetaSocialisationSpeed", "mRolesSocialisationBetaMoreRoles", "mRolesSocialisationBetaLessRoles", "mRoleStrainAffectingGatewayDispositionBeta", "mTendencyToDrinkInResponseToStrain"};
	string tier2Terminals[] = {"mRoleLoad", "mRoleIncongruence"};	
	string tier3Terminals[] = {"logOddsOppOut", "logOddsOppIn", "mRoleStrain", "dayBar", "mDispositionVectorOld[i]"};
	string tier4Terminals[] = {"mProbOppIn", "mProbOppOut", "modifier", "dispositionDiff"};
	string tier5Terminals[] = {"mDispositionVector[0]", "mDispositionVector[i]"};
	string tier6Terminals[] = {"probFirstDrink", "modifierByRoleStrain"};

	int numberOfFunctions = sizeof(tier0Functions)/sizeof(string);
	int numberOfTier1Terminals = sizeof(tier1Terminals)/sizeof(string);
	int numberOfTier2Terminals = sizeof(tier2Terminals)/sizeof(string);
	int numberOfTier3Terminals = sizeof(tier3Terminals)/sizeof(string);
	int numberOfTier4Terminals = sizeof(tier4Terminals)/sizeof(string);
	int numberOfTier5Terminals = sizeof(tier5Terminals)/sizeof(string);
	int numberOfTier6Terminals = sizeof(tier6Terminals)/sizeof(string);

	int cumulativeTier1Primitives = numberOfTier1Terminals;
	int cumulativeTier2Primitives = cumulativeTier1Primitives + numberOfTier2Terminals;
	int cumulativeTier3Primitives = cumulativeTier2Primitives + numberOfTier3Terminals;
	int cumulativeTier4Primitives = cumulativeTier3Primitives + numberOfTier4Terminals;
	int cumulativeTier5Primitives = cumulativeTier4Primitives + numberOfTier5Terminals;
	int cumulativeTier6Primitives = cumulativeTier5Primitives + numberOfTier6Terminals;
	int primitiveNumber;
	
	//cout << "tier1#: " << numberOfTier1Terminals << "  tier3#: " << numberOfTier3Terminals << endl;
	//srand(time(NULL));
	switch(tier){
		case 0:
			primitiveNumber = rand() % numberOfFunctions;
			return tier0Functions[primitiveNumber];
		case 1: 
			primitiveNumber = rand() % (cumulativeTier1Primitives + randomConstantChance) + 1; //generate random number
			if(primitiveNumber < cumulativeTier1Primitives){	//if less than the number of tier 1 primitives, return one. 
				return tier1Terminals[primitiveNumber];
			}else{
				if(!(rand() % 2)){
					return to_string(rand() % randomConstantChance + 1);	//else return a random number from 1-10. 
				}else{
					return to_string((rand() % randomConstantChance + 1)*0.1);	//or return a number from 0.1 - 1.
				}
			}
		case 2:
			primitiveNumber = rand() % (cumulativeTier2Primitives + randomConstantChance) + 1;
			if(primitiveNumber < cumulativeTier1Primitives){
				return tier1Terminals[primitiveNumber];
			}else if(primitiveNumber < cumulativeTier2Primitives){
				return tier2Terminals[primitiveNumber-cumulativeTier1Primitives];
			}else{
				if(!(rand() % 2)){
					return to_string(rand() % randomConstantChance + 1);	//else return a random number from 1-10. 
				}else{
					return to_string((rand() % randomConstantChance + 1)*0.1);	//or return a number from 0.1 - 1.
				}
			}
		case 3:
			primitiveNumber = rand() % (cumulativeTier3Primitives + randomConstantChance) + 1;
			if(primitiveNumber < cumulativeTier1Primitives){
				return tier1Terminals[primitiveNumber];
			}else if(primitiveNumber < cumulativeTier2Primitives){
				return tier2Terminals[primitiveNumber-cumulativeTier1Primitives];
			}else if(primitiveNumber < cumulativeTier3Primitives){
				return tier3Terminals[primitiveNumber-cumulativeTier2Primitives];
			}else{
				if(!(rand() % 2)){
					return to_string(rand() % randomConstantChance + 1);	//else return a random number from 1-10. 
				}else{
					return to_string((rand() % randomConstantChance + 1)*0.1);	//or return a number from 0.1 - 1.
				}
			}
		case 4:
			primitiveNumber = rand() % (cumulativeTier4Primitives + randomConstantChance) + 1;
			if(primitiveNumber < cumulativeTier1Primitives){
				return tier1Terminals[primitiveNumber];
			}else if(primitiveNumber < cumulativeTier2Primitives){
				return tier2Terminals[primitiveNumber-cumulativeTier1Primitives];
			}else if(primitiveNumber < cumulativeTier3Primitives){
				return tier3Terminals[primitiveNumber-cumulativeTier2Primitives];
			}else if(primitiveNumber < cumulativeTier4Primitives){
				return tier4Terminals[primitiveNumber-cumulativeTier3Primitives];
			}else{
				if(!(rand() % 2)){
					return to_string(rand() % randomConstantChance + 1);	//else return a random number from 1-10. 
				}else{
					return to_string((rand() % randomConstantChance + 1)*0.1);	//or return a number from 0.1 - 1.
				}
			}
		case 5:
			primitiveNumber = rand() % (cumulativeTier4Primitives + randomConstantChance) + 1;
			if(primitiveNumber < cumulativeTier1Primitives){
				return tier1Terminals[primitiveNumber];
			}else if(primitiveNumber < cumulativeTier2Primitives){
				return tier2Terminals[primitiveNumber-cumulativeTier1Primitives];
			}else if(primitiveNumber < cumulativeTier3Primitives){
				return tier3Terminals[primitiveNumber-cumulativeTier2Primitives];
			}else if(primitiveNumber < cumulativeTier4Primitives){
				return tier4Terminals[primitiveNumber-cumulativeTier3Primitives];
			}else{
				if(!(rand() % 2)){
					return to_string(rand() % randomConstantChance + 1);	//else return a random number from 1-10. 
				}else{
					return to_string((rand() % randomConstantChance + 1)*0.1);  //or return a number from 0.1 - 1.
				}
			}
		default: cout << "Error in generatePrimitive(), tier was " << tier << ", which is out of range." << endl;
	}
}

//define a method to return the node array numbers beneath a given node. 
int getSubNode(int node, int number){	//node = 1-63, number = 1 or 2.
	if((node >= 1) && (node <= 63)){
		return ((node*2) + (number-1));
	}else{
		cout << "Error in getSubNode(). Value " << node << " not in accepted range." << endl;
		return -1;
	}
}	

//define a method to copy and paste an individual to a new gen.
//Note: should initialise the random number generator seed once (or more) before running this function via srand(time(NULL));
int reproduce(Generation& parents, int pPosition, Generation& children, int cPosition){
	int i;
	for(i=0; i<63; i++){
		children.population[cPosition].setRoleLoadItem(i+1, parents.population[pPosition].getRoleLoadItem(i+1));
		children.population[cPosition].setRoleIncongruenceItem(i+1, parents.population[pPosition].getRoleIncongruenceItem(i+1));
		children.population[cPosition].setRoleStrainItem(i+1, parents.population[pPosition].getRoleStrainItem(i+1));
		children.population[cPosition].setFirstDrinkDispositionItem(i+1, parents.population[pPosition].getFirstDrinkDispositionItem(i+1));
		children.population[cPosition].setNextDrinkDispositionItem(i+1, parents.population[pPosition].getNextDrinkDispositionItem(i+1));
	}
}

//declare a method to perform subtree crossover on a random subtree of a random syntax tree and transfer it to a new gen.
int crossover(Generation& parents, int p1Position, int p2Position, Generation& children, int cPosition){
	if(debugGP) cout << "running crossover for individuals " << p1Position << " and " << p2Position << " to next generation individual " << cPosition << endl;
	//srand(time(NULL));	
	int tree = rand() % 5 + 1;	//pick a random syntax tree to perform crossover on (1-5). 
	if(debugGP) cout << "tree selected: " << tree << endl;
	int p1cross, p2cross;	//pick a random terminal/node crossover points for parents. 
	int p1tier, p2tier;	//the crossover point tiers in the syntax tree. 
	int next = 0;
	int i;
	switch(tree){
		case 1:		//roleLoad
		case 2:		//roleIncongruence
		case 3:		//roleStrain
		case 4:		//firstDrinkDisposition
		case 5:		//nextDrinkDisposition
			while(!next){
				p1cross = rand() % 63 + 1; //generate a random number between 1 and 63 to choose the first crossover point.
//!!!ignore terminals of tree?	p1cross = rand() % 31 + 1; //generate a random number between 1 and 31 to choose the first crossover point.
				//If the chosen crossover point for the first parent is not empty, then exit the selection loop.
				if(debugGP) cout << "choosing p1cross as " << p1cross; 
				if((tree == 1) && (parents.population[p1Position].getRoleLoadItem(p1cross) != "_")){	
					next = 1;
					if(debugGP) cout << ", the slot is " << parents.population[p1Position].getRoleLoadItem(p1cross) << endl;
				}else if((tree == 2) && (parents.population[p1Position].getRoleIncongruenceItem(p1cross) != "_")){
					next = 1;	
					if(debugGP) cout << ", the slot is " << parents.population[p1Position].getRoleIncongruenceItem(p1cross) << endl;	
				}else if((tree == 3) && (parents.population[p1Position].getRoleStrainItem(p1cross) != "_")){
					next = 1;	
					if(debugGP) cout << ", the slot is " << parents.population[p1Position].getRoleStrainItem(p1cross) << endl;
				}else if((tree == 4) && (parents.population[p1Position].getFirstDrinkDispositionItem(p1cross) != "_")){
					next = 1;	
					if(debugGP) cout << ", the slot is " << parents.population[p1Position].getFirstDrinkDispositionItem(p1cross) << endl;
				}else if((tree == 5) && (parents.population[p1Position].getNextDrinkDispositionItem(p1cross) != "_")){
					next = 1;
					if(debugGP) cout << ", the slot is " << parents.population[p1Position].getNextDrinkDispositionItem(p1cross) << endl;
	
				}
				
			}
			if(debugGP) cout << "p1cross is " << p1cross << endl;
			next = 0;
			while(!next){
				//The second parent's crossover point must be on the same or greater tier than the first parent's crossover point tier to ensure that it ends in terminals and the array has enough space for it. 
//!!!Edit the code below to ensure p2tier is not equal to p1tier to ensure a subtree is swapped as opposed to a single node?
				if(p1cross == 1){				
					p1tier = 1; 	p2cross = rand() % 63 + 1;	//set to 1-63 (1st+ tier)
				}else if((p1cross >= 2) && (p1cross <= 3)){	
					p1tier = 2; 	p2cross = rand() % 62 + 2;	//set to 2-63 (2nd+ tier). 
				}else if ((p1cross >= 4) && (p1cross <= 7)){	
					p1tier = 3; 	p2cross = rand() % 60 + 4;	//set to 4-63 (3rd+ tier). 
				}else if ((p1cross >= 8) && (p1cross <= 15)){	
					p1tier = 4; 	p2cross = rand() % 56 + 8;	//set to 8-63 (4th+ tier).
				}else if ((p1cross >= 16) && (p1cross <= 31)){	
					p1tier = 5;	p2cross = rand() % 48 + 16;	//set to 16-63 (5th+ tier).
				}else if ((p1cross >= 32) && (p1cross <= 63)){	
					p1tier = 6;	p2cross = rand() % 32 + 32;	//set to 32-63 (6th tier).
				}else{	cout << "Error in crossover(), p1cross = " << p1cross << endl;	return -1;	}
				if(debugGP) cout << "p1 tier: " << p1tier << endl;
				if(debugGP) cout << "choosing p2cross as " << p2cross << endl;
				if(debugGP) cout << "tree = " << tree << " items in p2cross in trees 1-5: " << parents.population[p2Position].getRoleLoadItem(p2cross) << ", ";
				if(debugGP) cout << parents.population[p2Position].getRoleIncongruenceItem(p2cross) << ", " << parents.population[p2Position].getRoleStrainItem(p2cross) << ", ";
				if(debugGP) cout << parents.population[p2Position].getFirstDrinkDispositionItem(p2cross) << ", " << parents.population[p2Position].getNextDrinkDispositionItem(p2cross) << endl;
//if p2cross is less than or equal to p1cross, and the chosen crossover point for the second parent is not empty ("_"), then exit the loop
				if((tree == 1) && (parents.population[p2Position].getRoleLoadItem(p2cross) != "_")){	
					next = 1;	
					if(debugGP) cout << ", the slot is " << parents.population[p2Position].getRoleLoadItem(p1cross) << endl;
				}else if((tree == 2) && (parents.population[p2Position].getRoleIncongruenceItem(p2cross) != "_")){	
					next = 1;
					if(debugGP) cout << ", the slot is " << parents.population[p2Position].getRoleIncongruenceItem(p1cross) << endl;		
				}else if((tree == 3) && (parents.population[p2Position].getRoleStrainItem(p2cross) != "_")){	
					next = 1;	
					if(debugGP) cout << ", the slot is " << parents.population[p2Position].getRoleStrainItem(p1cross) << endl;
				}else if((tree == 4) && (parents.population[p2Position].getFirstDrinkDispositionItem(p2cross) != "_")){	
					next = 1;	
					if(debugGP) cout << ", the slot is " << parents.population[p2Position].getFirstDrinkDispositionItem(p1cross) << endl;
				}else if((tree == 5) && (parents.population[p2Position].getNextDrinkDispositionItem(p2cross) != "_")){	
					next = 1;	
					if(debugGP) cout << ", the slot is " << parents.population[p2Position].getNextDrinkDispositionItem(p1cross) << endl;
				}else{	if(debugGP) cout << "p2cross item was empty. p2cross = " << p2cross << endl; }
			}
			next = 0;
			if(p2cross == 1){				p2tier = 1; 	
			}else if((p2cross >= 2) && (p2cross <= 3)){	p2tier = 2;
			}else if ((p2cross >= 4) && (p2cross <= 7)){	p2tier = 3;
			}else if ((p2cross >= 8) && (p2cross <= 15)){	p2tier = 4;
			}else if ((p2cross >= 16) && (p2cross <= 31)){	p2tier = 5;
			}else if ((p2cross >= 32) && (p2cross <= 63)){	p2tier = 6;
			}else{	cout << "Error in crossover(), p2cross = " << p2cross << endl;	return -1;	}
			if(debugGP) cout << "p2 tier: " << p2tier << endl;
			//At this point, one of 4 trees has been chosen for crossover, a crossover point for the first parent has been chosen (and this point is not empty in the specified syntax tree) and a crossover point for the second parent has been chosen which has the same or less than the number leaves/terminals from the crossover point down of the first parent. Now the first parent has to be duplicated into the children generation, the syntax tree starting at the crossover point of the second parent needs to be copy and pasted into the first parent copy starting at the crossover point of the first parent, and any slots that are below a terminal need to be made empty (_). 
			reproduce(parents, p1Position, children, cPosition); //copy the first parent into the next generation.
			//Paste the second parent's syntax tree at crossover point 2 over the child's syntax tree at crossover point 1.
			switch(tree){
				case 1: //roleLoad
					if(debugGP) cout << "crossing over roleLoad" << endl;
					children.population[cPosition].setRoleLoadItem(p1cross, parents.population[p2Position].getRoleLoadItem(p2cross));
					if((p1tier <= 5) && (p1cross * 2 <= 63)){	//if there are 1 or more layers below:
						for(i=0; i<2; i++){	//set the 2 sub nodes in one layer down.
							if((p2tier <= 5) && (p2cross * 2 <= 63)){
//if there exists a corresponding slot in p2's subtree and it is not empty, set the child's tree to it, otherwise child's tree to "_". 
								children.population[cPosition].setRoleLoadItem(p1cross*2+i, parents.population[p2Position].getRoleLoadItem(p2cross*2+i));	
							}else{
								children.population[cPosition].setRoleLoadItem(p1cross*2+i,"_");
							}
						}
					}
					if((p1tier <= 4) && (p1cross * 4 <= 63)){	//if there are 2 or more layers below:
						for(i=0; i<4; i++){	//set the 4 sub nodes in two layers down.
							if((p2tier <= 4) && (p2cross * 4 <= 63)){
//if there exists a corresponding slot in p2's subtree and it is not empty, set the child's tree to it, otherwise child's tree to "_". 
								children.population[cPosition].setRoleLoadItem(p1cross*4+i, parents.population[p2Position].getRoleLoadItem(p2cross*4+i));	
							}else{
								children.population[cPosition].setRoleLoadItem(p1cross*4+i,"_");
							}
						}
					}
					if((p1tier <= 3) && (p1cross * 8 <= 63)){	//if there are 3 or more layers below:
						for(i=0; i<8; i++){	//set the 8 sub nodes in three layers down.
							if((p2tier <= 3) && (p2cross * 8 <= 63)){
//if there exists a corresponding slot in p2's subtree and it is not empty, set the child's tree to it, otherwise child's tree to "_". 
								children.population[cPosition].setRoleLoadItem(p1cross*8+i, parents.population[p2Position].getRoleLoadItem(p2cross*8+i));	
							}else{
								children.population[cPosition].setRoleLoadItem(p1cross*8+i,"_");
							}
						}
					}
					if((p1tier <= 2) && (p1cross * 16 <= 63)){	//if there are 4 or more layers below:
						for(i=0; i<16; i++){	//set the 16 sub nodes in four layers down.
							if((p2tier <= 2) && (p2cross * 16 <= 63)){
//if there exists a corresponding slot in p2's subtree and it is not empty, set the child's tree to it, otherwise child's tree to "_". 
								children.population[cPosition].setRoleLoadItem(p1cross*16+i, parents.population[p2Position].getRoleLoadItem(p2cross*16+i));	
							}else{
								children.population[cPosition].setRoleLoadItem(p1cross*16+i,"_");
							}
						}
					}
					if((p1tier <= 1) && (p1cross * 32 <= 63)){	//if there are 5 or more layers below:
						for(i=0; i<32; i++){	//set the 32 sub nodes in five layers down.
							if((p2tier <= 1) && (p2cross * 32 <= 63)){
//if there exists a corresponding slot in p2's subtree and it is not empty, set the child's tree to it, otherwise child's tree to "_". 
								children.population[cPosition].setRoleLoadItem(p1cross*32+i, parents.population[p2Position].getRoleLoadItem(p2cross*32+i));	
							}else{
								children.population[cPosition].setRoleLoadItem(p1cross*32+i,"_");
							}
						}
					}
					if(debugGP) cout << "p1 crossover point: " << p1cross << " (tier " << p1tier << "), : " <<  parents.population[p1Position].getRoleLoadItem(p1cross) << endl;
					if(debugGP) cout << "p2 crossover point: " << p2cross << " (tier " << p2tier << "), : " <<  parents.population[p2Position].getRoleLoadItem(p2cross) << endl;
					break;
				case 2: //roleIncongruence
					if(debugGP) cout << "crossing over roleIncongruence" << endl;
					children.population[cPosition].setRoleIncongruenceItem(p1cross, parents.population[p2Position].getRoleIncongruenceItem(p2cross));
					if((p1tier <= 5) && (p1cross * 2 <= 63)){	//if there are 1 or more layers below:
						for(i=0; i<2; i++){	//set the 2 sub nodes in one layer down.
							if((p2tier <= 5) && (p2cross * 2 <= 63)){
//if there exists a corresponding slot in p2's subtree and it is not empty, set the child's tree to it, otherwise child's tree to "_". 
								children.population[cPosition].setRoleIncongruenceItem(p1cross*2+i, parents.population[p2Position].getRoleIncongruenceItem(p2cross*2+i));	
							}else{
								children.population[cPosition].setRoleIncongruenceItem(p1cross*2+i,"_");
							}
						}
					}
					if((p1tier <= 4) && (p1cross * 4 <= 63)){	//if there are 2 or more layers below:
						for(i=0; i<4; i++){	//set the 4 sub nodes in two layers down.
							if((p2tier <= 4) && (p2cross * 4 <= 63)){
//if there exists a corresponding slot in p2's subtree and it is not empty, set the child's tree to it, otherwise child's tree to "_". 
								children.population[cPosition].setRoleIncongruenceItem(p1cross*4+i, parents.population[p2Position].getRoleIncongruenceItem(p2cross*4+i));	
							}else{
								children.population[cPosition].setRoleIncongruenceItem(p1cross*4+i,"_");
							}
						}
					}
					if((p1tier <= 3) && (p1cross * 8 <= 63)){	//if there are 3 or more layers below:
						for(i=0; i<8; i++){	//set the 8 sub nodes in three layers down.
							if((p2tier <= 3) && (p2cross * 8 <= 63)){
//if there exists a corresponding slot in p2's subtree and it is not empty, set the child's tree to it, otherwise child's tree to "_". 
								children.population[cPosition].setRoleIncongruenceItem(p1cross*8+i, parents.population[p2Position].getRoleIncongruenceItem(p2cross*8+i));	
							}else{
								children.population[cPosition].setRoleIncongruenceItem(p1cross*8+i,"_");
							}
						}
					}
					if((p1tier <= 2) && (p1cross * 16 <= 63)){	//if there are 4 or more layers below:
						for(i=0; i<16; i++){	//set the 16 sub nodes in four layers down.
							if((p2tier <= 2) && (p2cross * 16 <= 63)){
//if there exists a corresponding slot in p2's subtree and it is not empty, set the child's tree to it, otherwise child's tree to "_". 
								children.population[cPosition].setRoleIncongruenceItem(p1cross*16+i, parents.population[p2Position].getRoleIncongruenceItem(p2cross*16+i));	
							}else{
								children.population[cPosition].setRoleIncongruenceItem(p1cross*16+i,"_");
							}
						}
					}
					if((p1tier <= 1) && (p1cross * 32 <= 63)){	//if there are 5 or more layers below:
						for(i=0; i<32; i++){	//set the 32 sub nodes in five layers down.
							if((p2tier <= 1) && (p2cross * 32 <= 63)){
//if there exists a corresponding slot in p2's subtree and it is not empty, set the child's tree to it, otherwise child's tree to "_". 
								children.population[cPosition].setRoleIncongruenceItem(p1cross*32+i, parents.population[p2Position].getRoleIncongruenceItem(p2cross*32+i));	
							}else{
								children.population[cPosition].setRoleIncongruenceItem(p1cross*32+i,"_");
							}
						}
					}
					if(debugGP) cout << "p1 crossover point: " << p1cross << " (tier " << p1tier << "), : " <<  parents.population[p1Position].getRoleIncongruenceItem(p1cross) << endl;
					if(debugGP) cout << "p2 crossover point: " << p2cross << " (tier " << p2tier << "), : " <<  parents.population[p2Position].getRoleIncongruenceItem(p2cross) << endl;
					break;
				case 3: //roleStrain
					if(debugGP) cout << "crossing over roleStrain" << endl;
					children.population[cPosition].setRoleStrainItem(p1cross, parents.population[p2Position].getRoleStrainItem(p2cross));
					if((p1tier <= 5) && (p1cross * 2 <= 63)){	//if there are 1 or more layers below:
						for(i=0; i<2; i++){	//set the 2 sub nodes in one layer down.
							if((p2tier <= 5) && (p2cross * 2 <= 63)){
//if there exists a corresponding slot in p2's subtree and it is not empty, set the child's tree to it, otherwise child's tree to "_". 
								children.population[cPosition].setRoleStrainItem(p1cross*2+i, parents.population[p2Position].getRoleStrainItem(p2cross*2+i));	
							}else{
								children.population[cPosition].setRoleStrainItem(p1cross*2+i,"_");
							}
						}
					}
					if((p1tier <= 4) && (p1cross * 4 <= 63)){	//if there are 2 or more layers below:
						for(i=0; i<4; i++){	//set the 4 sub nodes in two layers down.
							if((p2tier <= 4) && (p2cross * 4 <= 63)){
//if there exists a corresponding slot in p2's subtree and it is not empty, set the child's tree to it, otherwise child's tree to "_". 
								children.population[cPosition].setRoleStrainItem(p1cross*4+i, parents.population[p2Position].getRoleStrainItem(p2cross*4+i));	
							}else{
								children.population[cPosition].setRoleStrainItem(p1cross*4+i,"_");
							}
						}
					}
					if((p1tier <= 3) && (p1cross * 8 <= 63)){	//if there are 3 or more layers below:
						for(i=0; i<8; i++){	//set the 8 sub nodes in three layers down.
							if((p2tier <= 3) && (p2cross * 8 <= 63)){
//if there exists a corresponding slot in p2's subtree and it is not empty, set the child's tree to it, otherwise child's tree to "_". 
								children.population[cPosition].setRoleStrainItem(p1cross*8+i, parents.population[p2Position].getRoleStrainItem(p2cross*8+i));	
							}else{
								children.population[cPosition].setRoleStrainItem(p1cross*8+i,"_");
							}
						}
					}
					if((p1tier <= 2) && (p1cross * 16 <= 63)){	//if there are 4 or more layers below:
						for(i=0; i<16; i++){	//set the 16 sub nodes in four layers down.
							if((p2tier <= 2) && (p2cross * 16 <= 63)){
//if there exists a corresponding slot in p2's subtree and it is not empty, set the child's tree to it, otherwise child's tree to "_". 
								children.population[cPosition].setRoleStrainItem(p1cross*16+i, parents.population[p2Position].getRoleStrainItem(p2cross*16+i));	
							}else{
								children.population[cPosition].setRoleStrainItem(p1cross*16+i,"_");
							}
						}
					}
					if((p1tier <= 1) && (p1cross * 32 <= 63)){	//if there are 5 or more layers below:
						for(i=0; i<32; i++){	//set the 32 sub nodes in five layers down.
							if((p2tier <= 1) && (p2cross * 32 <= 63)){
//if there exists a corresponding slot in p2's subtree and it is not empty, set the child's tree to it, otherwise child's tree to "_". 
								children.population[cPosition].setRoleStrainItem(p1cross*32+i, parents.population[p2Position].getRoleStrainItem(p2cross*32+i));	
							}else{
								children.population[cPosition].setRoleStrainItem(p1cross*32+i,"_");
							}
						}
					}
					if(debugGP) cout << "p1 crossover point: " << p1cross << " (tier " << p1tier << "), : " <<  parents.population[p1Position].getRoleStrainItem(p1cross) << endl;
					if(debugGP) cout << "p2 crossover point: " << p2cross << " (tier " << p2tier << "), : " <<  parents.population[p2Position].getRoleStrainItem(p2cross) << endl;
					break;
				case 4: //firstDrinkDisposition
					if(debugGP) cout << "crossing over firstDrinkDisposition" << endl;
					children.population[cPosition].setFirstDrinkDispositionItem(p1cross, parents.population[p2Position].getFirstDrinkDispositionItem(p2cross));
					if((p1tier <= 5) && (p1cross * 2 <= 63)){	//if there are 1 or more layers below:
						for(i=0; i<2; i++){	//set the 2 sub nodes in one layer down.
							if((p2tier <= 5) && (p2cross * 2 <= 63)){
//if there exists a corresponding slot in p2's subtree and it is not empty, set the child's tree to it, otherwise child's tree to "_". 
								children.population[cPosition].setFirstDrinkDispositionItem(p1cross*2+i, parents.population[p2Position].getFirstDrinkDispositionItem(p2cross*2+i));	
							}else{
								children.population[cPosition].setFirstDrinkDispositionItem(p1cross*2+i,"_");
							}
						}
					}
					if((p1tier <= 4) && (p1cross * 4 <= 63)){	//if there are 2 or more layers below:
						for(i=0; i<4; i++){	//set the 4 sub nodes in two layers down.
							if((p2tier <= 4) && (p2cross * 4 <= 63)){
//if there exists a corresponding slot in p2's subtree and it is not empty, set the child's tree to it, otherwise child's tree to "_". 
								children.population[cPosition].setFirstDrinkDispositionItem(p1cross*4+i, parents.population[p2Position].getFirstDrinkDispositionItem(p2cross*4+i));	
							}else{
								children.population[cPosition].setFirstDrinkDispositionItem(p1cross*4+i,"_");
							}
						}
					}
					if((p1tier <= 3) && (p1cross * 8 <= 63)){	//if there are 3 or more layers below:
						for(i=0; i<8; i++){	//set the 8 sub nodes in three layers down.
							if((p2tier <= 3) && (p2cross * 8 <= 63)){
//if there exists a corresponding slot in p2's subtree and it is not empty, set the child's tree to it, otherwise child's tree to "_". 
								children.population[cPosition].setFirstDrinkDispositionItem(p1cross*8+i, parents.population[p2Position].getFirstDrinkDispositionItem(p2cross*8+i));	
							}else{
								children.population[cPosition].setFirstDrinkDispositionItem(p1cross*8+i,"_");
							}
						}
					}
					if((p1tier <= 2) && (p1cross * 16 <= 63)){	//if there are 4 or more layers below:
						for(i=0; i<16; i++){	//set the 16 sub nodes in four layers down.
							if((p2tier <= 2) && (p2cross * 16 <= 63)){
//if there exists a corresponding slot in p2's subtree and it is not empty, set the child's tree to it, otherwise child's tree to "_". 
								children.population[cPosition].setFirstDrinkDispositionItem(p1cross*16+i, parents.population[p2Position].getFirstDrinkDispositionItem(p2cross*16+i));	
							}else{
								children.population[cPosition].setFirstDrinkDispositionItem(p1cross*16+i,"_");
							}
						}
					}
					if((p1tier <= 1) && (p1cross * 32 <= 63)){	//if there are 5 or more layers below:
						for(i=0; i<32; i++){	//set the 32 sub nodes in five layers down.
							if((p2tier <= 1) && (p2cross * 32 <= 63)){
//if there exists a corresponding slot in p2's subtree and it is not empty, set the child's tree to it, otherwise child's tree to "_". 
								children.population[cPosition].setFirstDrinkDispositionItem(p1cross*32+i, parents.population[p2Position].getFirstDrinkDispositionItem(p2cross*32+i));	
							}else{
								children.population[cPosition].setFirstDrinkDispositionItem(p1cross*32+i,"_");
							}
						}
					}
					if(debugGP) cout << "p1 crossover point: " << p1cross << " (tier " << p1tier << "), : " <<  parents.population[p1Position].getFirstDrinkDispositionItem(p1cross) << endl;
					if(debugGP) cout << "p2 crossover point: " << p2cross << " (tier " << p2tier << "), : " <<  parents.population[p2Position].getFirstDrinkDispositionItem(p2cross) << endl;
					break;
				case 5: //nextDrinkDisposition
					if(debugGP) cout << "crossing over nextDrinkDisposition" << endl;
					children.population[cPosition].setNextDrinkDispositionItem(p1cross, parents.population[p2Position].getNextDrinkDispositionItem(p2cross));
					if((p1tier <= 5) && (p1cross * 2 <= 63)){	//if there are 1 or more layers below:
						for(i=0; i<2; i++){	//set the 2 sub nodes in one layer down.
							if((p2tier <= 5) && (p2cross * 2 <= 63)){
//if there exists a corresponding slot in p2's subtree and it is not empty, set the child's tree to it, otherwise child's tree to "_". 
								children.population[cPosition].setNextDrinkDispositionItem(p1cross*2+i, parents.population[p2Position].getNextDrinkDispositionItem(p2cross*2+i));	
							}else{
								children.population[cPosition].setNextDrinkDispositionItem(p1cross*2+i,"_");
							}
						}
					}
					if((p1tier <= 4) && (p1cross * 4 <= 63)){	//if there are 2 or more layers below:
						for(i=0; i<4; i++){	//set the 4 sub nodes in two layers down.
							if((p2tier <= 4) && (p2cross * 4 <= 63)){
//if there exists a corresponding slot in p2's subtree and it is not empty, set the child's tree to it, otherwise child's tree to "_". 
								children.population[cPosition].setNextDrinkDispositionItem(p1cross*4+i, parents.population[p2Position].getNextDrinkDispositionItem(p2cross*4+i));	
							}else{
								children.population[cPosition].setNextDrinkDispositionItem(p1cross*4+i,"_");
							}
						}
					}
					if((p1tier <= 3) && (p1cross * 8 <= 63)){	//if there are 3 or more layers below:
						for(i=0; i<8; i++){	//set the 8 sub nodes in three layers down.
							if((p2tier <= 3) && (p2cross * 8 <= 63)){
//if there exists a corresponding slot in p2's subtree and it is not empty, set the child's tree to it, otherwise child's tree to "_". 
								children.population[cPosition].setNextDrinkDispositionItem(p1cross*8+i, parents.population[p2Position].getNextDrinkDispositionItem(p2cross*8+i));	
							}else{
								children.population[cPosition].setNextDrinkDispositionItem(p1cross*8+i,"_");
							}
						}
					}
					if((p1tier <= 2) && (p1cross * 16 <= 63)){	//if there are 4 or more layers below:
						for(i=0; i<16; i++){	//set the 16 sub nodes in four layers down.
							if((p2tier <= 2) && (p2cross * 16 <= 63)){
//if there exists a corresponding slot in p2's subtree and it is not empty, set the child's tree to it, otherwise child's tree to "_". 
								children.population[cPosition].setNextDrinkDispositionItem(p1cross*16+i, parents.population[p2Position].getNextDrinkDispositionItem(p2cross*16+i));	
							}else{
								children.population[cPosition].setNextDrinkDispositionItem(p1cross*16+i,"_");
							}
						}
					}
					if((p1tier <= 1) && (p1cross * 32 <= 63)){	//if there are 5 or more layers below:
						for(i=0; i<32; i++){	//set the 32 sub nodes in five layers down.
							if((p2tier <= 1) && (p2cross * 32 <= 63)){
//if there exists a corresponding slot in p2's subtree and it is not empty, set the child's tree to it, otherwise child's tree to "_". 
								children.population[cPosition].setNextDrinkDispositionItem(p1cross*32+i, parents.population[p2Position].getNextDrinkDispositionItem(p2cross*32+i));	
							}else{
								children.population[cPosition].setNextDrinkDispositionItem(p1cross*32+i,"_");
							}
						}
					}
					if(debugGP) cout << "p1 crossover point: " << p1cross << " (tier " << p1tier << "), : " <<  parents.population[p1Position].getNextDrinkDispositionItem(p1cross) << endl;
					if(debugGP) cout << "p2 crossover point: " << p2cross << " (tier " << p2tier << "), : " <<  parents.population[p2Position].getNextDrinkDispositionItem(p2cross) << endl;
					break;
				default:
					cout << "Error in crossover(), tree was " << tree << endl;
			}
			break;
		/*case 4:		//disposition
			while(!next){
				p1cross = rand() % 127 + 1; //generate a random number between 1 and 127 to choose the first crossover point.
				//If the chosen crossover point for the first parent is not empty, then exit the selection loop. 
				if(parents.population[p1Position].getDispositionMaleItem(p1cross) != "_"){	
					next = 1;	
				}
			}
			next = 0;
			while(!next){
				//The second parent's crossover point must be on the same or greater tier than the first parent's crossover point tier to ensure that it ends in terminals and the array has enough space for it. 
				if(p1cross == 1){				
					p1tier = 1; 	p2cross = rand() % 127 + 1;	//set to 1-127 (1st+ tier)
				}else if((p1cross >= 2) && (p1cross <= 3)){	
					p1tier = 2; 	p2cross = rand() % 126 + 2;	//set to 2-127 (2nd+ tier). 
				}else if ((p1cross >= 4) && (p1cross <= 7)){	
					p1tier = 3; 	p2cross = rand() % 124 + 4;	//set to 4-127 (3rd+ tier). 
				}else if ((p1cross >= 8) && (p1cross <= 15)){	
					p1tier = 4; 	p2cross = rand() % 120 + 8;	//set to 8-127 (4th+ tier).
				}else if ((p1cross >= 16) && (p1cross <= 31)){	
					p1tier = 5;	p2cross = rand() % 112 + 16;	//set to 16-127 (5th+ tier).
				}else if ((p1cross >= 32) && (p1cross <= 63)){	
					p1tier = 6;	p2cross = rand() % 96 + 32;	//set to 32-127 (6th+ tier).
				}else if ((p1cross >= 64) && (p1cross <= 127)){	
					p1tier = 7;	p2cross = rand() % 64 + 64;	//set to 64-127 (7th tier).
				}else{	cout << "Error in crossover(), p1cross = " << p1cross << endl;	return -1;	}
//if p2cross is less than or equal to p1cross, and the chosen crossover point for the second parent is not empty ("_"), then exit the loop
				if(parents.population[p2Position].getDispositionMaleItem(p2cross) != "_"){	
					next = 1;	
				}
			}
			next = 0;
			if(p2cross == 1){				p2tier = 1; 	
			}else if((p2cross >= 2) && (p2cross <= 3)){	p2tier = 2;
			}else if ((p2cross >= 4) && (p2cross <= 7)){	p2tier = 3;
			}else if ((p2cross >= 8) && (p2cross <= 15)){	p2tier = 4;
			}else if ((p2cross >= 16) && (p2cross <= 31)){	p2tier = 5;
			}else if ((p2cross >= 32) && (p2cross <= 63)){	p2tier = 6;
			}else if ((p2cross >= 64) && (p2cross <= 127)){	p2tier = 7;
			}else{	cout << "Error in crossover(), p2cross = " << p2cross << endl;	return -1;	}
			//cout << "p1 crossover point: " << p1cross << " (tier " << p1tier << "), : " <<  parents.population[p1Position].getDispositionMaleItem(p1cross) << endl;
			//cout << "p2 crossover point: " << p2cross << " (tier " << p2tier << "), : " <<  parents.population[p2Position].getDispositionMaleItem(p2cross) << endl;
			//At this point, a crossover point for the first parent has been chosen (and this point is not empty in the specified syntax tree) and a crossover point for the second parent has been chosen which has the same or less than the number leaves/terminals from the crossover point down of the first parent. Now the first parent has to be duplicated into the children generation, the syntax tree starting at the crossover point of the second parent needs to be copy and pasted into the first parent copy starting at the crossover point of the first parent, and any slots that are below a terminal need to be made empty. 
			reproduce(parents, p1Position, children, cPosition); //copy the first parent into the next generation.
			//Paste the second parent's syntax tree at crossover point 2 over the child's syntax tree at crossover point 1.
			//roleStrain
			children.population[cPosition].setDispositionItem(p1cross, parents.population[p2Position].getDispositionMaleItem(p2cross));
			if((p1tier <= 6) && (p1cross * 2 <= 127)){	//if there are 1 or more layers below:
				for(i=0; i<2; i++){	//set the 2 sub nodes in one layer down.
					if((p2tier <= 6) && (p2cross * 2 <= 127)){
//if there exists a corresponding slot in p2's subtree and it is not empty, set the child's tree to it, otherwise child's tree to "_". 
						children.population[cPosition].setDispositionItem(p1cross*2+i, parents.population[p2Position].getDispositionMaleItem(p2cross*2+i));	
					}else{
						children.population[cPosition].setDispositionItem(p1cross*2+i,"_");
					}
				}
			}
			if((p1tier <= 5) && (p1cross * 4 <= 127)){	//if there are 2 or more layers below:
				for(i=0; i<4; i++){	//set the 4 sub nodes in two layers down.
					if((p2tier <= 5) && (p2cross * 4 <= 127)){
//if there exists a corresponding slot in p2's subtree and it is not empty, set the child's tree to it, otherwise child's tree to "_". 
						children.population[cPosition].setDispositionItem(p1cross*4+i, parents.population[p2Position].getDispositionMaleItem(p2cross*4+i));	
					}else{
						children.population[cPosition].setDispositionItem(p1cross*4+i,"_");
					}
				}
			}
			if((p1tier <= 4) && (p1cross * 8 <= 127)){	//if there are 3 or more layers below:
				for(i=0; i<8; i++){	//set the 8 sub nodes in three layers down.
					if((p2tier <= 4) && (p2cross * 8 <= 127)){
//if there exists a corresponding slot in p2's subtree and it is not empty, set the child's tree to it, otherwise child's tree to "_". 
						children.population[cPosition].setDispositionItem(p1cross*8+i, parents.population[p2Position].getDispositionMaleItem(p2cross*8+i));	
					}else{
						children.population[cPosition].setDispositionItem(p1cross*8+i,"_");
					}
				}
			}
			if((p1tier <= 3) && (p1cross * 16 <= 127)){	//if there are 4 or more layers below:
				for(i=0; i<16; i++){	//set the 16 sub nodes in four layers down.
					if((p2tier <= 3) && (p2cross * 16 <= 127)){
//if there exists a corresponding slot in p2's subtree and it is not empty, set the child's tree to it, otherwise child's tree to "_". 
						children.population[cPosition].setDispositionItem(p1cross*16+i, parents.population[p2Position].getDispositionMaleItem(p2cross*16+i));	
					}else{
						children.population[cPosition].setDispositionItem(p1cross*16+i,"_");
					}
				}
			}
			if((p1tier <= 2) && (p1cross * 32 <= 127)){	//if there are 5 or more layers below:
				for(i=0; i<32; i++){	//set the 32 sub nodes in five layers down.
					if((p2tier <= 2) && (p2cross * 32 <= 127)){
//if there exists a corresponding slot in p2's subtree and it is not empty, set the child's tree to it, otherwise child's tree to "_". 
						children.population[cPosition].setDispositionItem(p1cross*32+i, parents.population[p2Position].getDispositionMaleItem(p2cross*32+i));	
					}else{
						children.population[cPosition].setDispositionItem(p1cross*32+i,"_");
					}
				}
			}
			if((p1tier <= 1) && (p1cross * 64 <= 127)){	//if there are 6 or more layers below:
				for(i=0; i<64; i++){	//set the 64 sub nodes in six layers down.
					if((p2tier <= 1) && (p2cross * 64 <= 127)){
//if there exists a corresponding slot in p2's subtree and it is not empty, set the child's tree to it, otherwise child's tree to "_". 
						children.population[cPosition].setDispositionItem(p1cross*64+i, parents.population[p2Position].getDispositionMaleItem(p2cross*64+i));	
					}else{
						children.population[cPosition].setDispositionItem(p1cross*64+i,"_");
					}
				}
			}
			break;
		case 6:		//dailyDisposition
			while(!next){
				p1cross = rand() % 15 + 1; //generate a random number between 1 and 15 to choose the first crossover point.
				//If the chosen crossover point for the first parent is not empty, then exit the selection loop. 
				if(parents.population[p1Position].getDailyDispositionMaleItem(p1cross) != "_"){	
					next = 1;	
				}
			}
			next = 0;
			while(!next){
				//The second parent's crossover point must be on the same or greater tier than the first parent's crossover point tier to ensure that it ends in terminals and the array has enough space for it. 
				if(p1cross == 1){				
					p1tier = 1; 	p2cross = rand() % 15 + 1;	//set to 1-15 (1st+ tier)
				}else if((p1cross >= 2) && (p1cross <= 3)){	
					p1tier = 2; 	p2cross = rand() % 14 + 2;	//set to 2-15 (2nd+ tier). 
				}else if ((p1cross >= 4) && (p1cross <= 7)){	
					p1tier = 3; 	p2cross = rand() % 12 + 4;	//set to 4-15 (3rd+ tier). 
				}else if ((p1cross >= 8) && (p1cross <= 15)){	
					p1tier = 4; 	p2cross = rand() % 8 + 8;	//set to 8-15 (4th tier).
				}else{	cout << "Error in crossover(), p1cross = " << p1cross << endl;	return -1;	}
//if p2cross is less than or equal to p1cross, and the chosen crossover point for the second parent is not empty ("_"), then exit the loop
				if(parents.population[p2Position].getDailyDispositionMaleItem(p2cross) != "_"){	
					next = 1;	
				}
			}
			next = 0;
			if(p2cross == 1){				p2tier = 1; 	
			}else if((p2cross >= 2) && (p2cross <= 3)){	p2tier = 2;
			}else if ((p2cross >= 4) && (p2cross <= 7)){	p2tier = 3;
			}else if ((p2cross >= 8) && (p2cross <= 15)){	p2tier = 4;
			}else{	cout << "Error in crossover(), p2cross = " << p2cross << endl;	return -1;	}
			//cout << "p1 crossover point: " << p1cross << " (tier " << p1tier << "), : " <<  parents.population[p1Position].getDailyDispositionMaleItem(p1cross) << endl;
			//cout << "p2 crossover point: " << p2cross << " (tier " << p2tier << "), : " <<  parents.population[p2Position].getDailyDispositionMaleItem(p2cross) << endl;
			//At this point, a crossover point for the first parent has been chosen (and this point is not empty in the specified syntax tree) and a crossover point for the second parent has been chosen which has the same or less than the number leaves/terminals from the crossover point down of the first parent. Now the first parent has to be duplicated into the children generation, the syntax tree starting at the crossover point of the second parent needs to be copy and pasted into the first parent copy starting at the crossover point of the first parent, and any slots that are below a terminal need to be made empty. 
			reproduce(parents, p1Position, children, cPosition); //copy the first parent into the next generation.
			//Paste the second parent's syntax tree at crossover point 2 over the child's syntax tree at crossover point 1.
			//DailyDisposition
			children.population[cPosition].setDailyDispositionItem(p1cross, parents.population[p2Position].getDailyDispositionMaleItem(p2cross));
			if((p1tier <= 3) && (p1cross * 2 <= 15)){	//if there are 1 or more layers below:
				for(i=0; i<2; i++){	//set the 2 sub nodes in one layer down.
					if((p2tier <= 3) && (p2cross * 2 <= 15)){
//if there exists a corresponding slot in p2's subtree and it is not empty, set the child's tree to it, otherwise child's tree to "_". 
						children.population[cPosition].setDailyDispositionItem(p1cross*2+i, parents.population[p2Position].getDailyDispositionMaleItem(p2cross*2+i));	
					}else{
						children.population[cPosition].setDailyDispositionItem(p1cross*2+i,"_");
					}
				}
			}
			if((p1tier <= 2) && (p1cross * 4 <= 15)){	//if there are 2 or more layers below:
				for(i=0; i<4; i++){	//set the 4 sub nodes in two layers down.
					if((p2tier <= 2) && (p2cross * 4 <= 15)){
//if there exists a corresponding slot in p2's subtree and it is not empty, set the child's tree to it, otherwise child's tree to "_". 
						children.population[cPosition].setDailyDispositionItem(p1cross*4+i, parents.population[p2Position].getDailyDispositionMaleItem(p2cross*4+i));	
					}else{
						children.population[cPosition].setDailyDispositionItem(p1cross*4+i,"_");
					}
				}
			}
			if((p1tier <= 1) && (p1cross * 8 <= 15)){	//if there are 3 or more layers below:
				for(i=0; i<8; i++){	//set the 8 sub nodes in three layers down.
					if((p2tier <= 1) && (p2cross * 8 <= 15)){
//if there exists a corresponding slot in p2's subtree and it is not empty, set the child's tree to it, otherwise child's tree to "_". 
						children.population[cPosition].setDailyDispositionItem(p1cross*8+i, parents.population[p2Position].getDailyDispositionMaleItem(p2cross*8+i));	
					}else{
						children.population[cPosition].setDailyDispositionItem(p1cross*8+i,"_");
					}
				}
			}
			break;*/
		default:
			cout << "Error in crossover(), tree was " << tree << endl;
			return -1;
	}
	if(debugGP) cout << "crossover finished" << endl;
	return 0;
}

//declare a method to perform subtree mutation on an individual by randomly selecting a node, mutating it and all nodes beneath it, and transfering the new individual to the next gen.
//Note: should initialise the random number generator seed once (or more) before running this function via srand(time(NULL));	
int subtreeMutation(Generation& parents, int pPosition, Generation& children, int cPosition){
	if(debugGP) cout << "performing subtree mutation, pPosition = " << pPosition << ", cPosition = " << cPosition << endl;
	//subtree mutation: select a node and substitute in a randomly generated subtree over it and the subtree below. 
	//srand(time(NULL));	
	int tree = rand() % 5 + 1;	//pick a random syntax tree to perform mutation on (1-5). 
	if(debugGP) cout << "tree selected: " << tree << endl;
	int pcross;	//pick a random terminal/node crossover points for the parent. 
	int ptier;	//the crossover point tier in the syntax tree. 
	int next = 0;
	int i;
	switch(tree){
		case 1:		//roleLoad
		case 2:		//roleIncongruence
		case 3:		//roleStrain
		case 4: 	//firstDrinkDisposition
		case 5:		//nextDrinkDisposition
			while(!next){
				pcross = rand() % 63 + 1; //generate a random number between 1 and 63 to choose the crossover point.
				if(debugGP) cout << "pcross selected as " << pcross;
				//If the chosen crossover point is not empty, then exit the selection loop. 
				if((tree == 1) && (parents.population[pPosition].getRoleLoadItem(pcross) != "_")){	
					next = 1;	
					if(debugGP) cout << ", slot is " << parents.population[pPosition].getRoleLoadItem(pcross) << endl;
				}else if((tree == 2) && (parents.population[pPosition].getRoleIncongruenceItem(pcross) != "_")){
					next = 1;
					if(debugGP) cout << ", slot is " << parents.population[pPosition].getRoleIncongruenceItem(pcross) << endl;		
				}else if((tree == 3) && (parents.population[pPosition].getRoleStrainItem(pcross) != "_")){
					next = 1;	
					if(debugGP) cout << ", slot is " << parents.population[pPosition].getRoleStrainItem(pcross) << endl;
				}else if((tree == 4) && (parents.population[pPosition].getFirstDrinkDispositionItem(pcross) != "_")){
					next = 1;	
					if(debugGP) cout << ", slot is " << parents.population[pPosition].getFirstDrinkDispositionItem(pcross) << endl;
				}else if((tree == 5) && (parents.population[pPosition].getNextDrinkDispositionItem(pcross) != "_")){
					next = 1;	
					if(debugGP) cout << ", slot is " << parents.population[pPosition].getNextDrinkDispositionItem(pcross) << endl;
				}
			}
			next = 0;
			if(pcross == 1){				ptier = 1; 
			}else if((pcross >= 2) && (pcross <= 3)){	ptier = 2;
			}else if ((pcross >= 4) && (pcross <= 7)){	ptier = 3;
			}else if ((pcross >= 8) && (pcross <= 15)){	ptier = 4;
			}else if ((pcross >= 16) && (pcross <= 31)){	ptier = 5;
			}else if ((pcross >= 32) && (pcross <= 63)){	ptier = 6;
			}else{	cout << "Error in subtreeMutate(), pcross = " << pcross << endl;	return -1;	}
			if(debugGP) cout << "ptier = " << ptier << endl;
			reproduce(parents, pPosition, children, cPosition); //copy the parent into the next generation.
			if(debugGP) cout << "reproduction of parent performed." << endl;
			//Paste a random syntax tree at the crossover point over the child's syntax tree.
			switch(tree){
				case 1: //roleLoad - tier 2 equation - use terminals from tier 1 only.
					if(debugGP) cout << "mutating role overload" << endl;
					if(isFunction(children.population[cPosition].getRoleLoadItem(pcross))){
						children.population[cPosition].setRoleLoadItem(pcross,generatePrimitive(0));
					}else{
						children.population[cPosition].setRoleLoadItem(pcross,generatePrimitive(1));
					}
					if((ptier <= 5) && (pcross * 2 <= 63)){	//if there are 1 or more layers below:
						for(i=0; i<2; i++){	//set the 2 sub nodes in one layer down.
							if(children.population[cPosition].getRoleLoadItem(pcross*2+i) != "_"){
//if the slot below is not empty, set the child's slot to a random primitive, otherwise child's tree to "_". 
								if(isFunction(children.population[cPosition].getRoleLoadItem(pcross*2+i))){
children.population[cPosition].setRoleLoadItem(pcross*2+i,generatePrimitive(0));
								}else{
children.population[cPosition].setRoleLoadItem(pcross*2+i,generatePrimitive(1));
								}
							}
						}
					}
					if((ptier <= 4) && (pcross * 4 <= 63)){	//if there are 2 or more layers below:
						for(i=0; i<4; i++){	//set the 4 sub nodes in two layers down.
							if(children.population[cPosition].getRoleLoadItem(pcross*4+i) != "_"){
//if the slot below is not empty, set the child's slot to a random primitive, otherwise child's tree to "_". 
								if(isFunction(children.population[cPosition].getRoleLoadItem(pcross*4+i))){
children.population[cPosition].setRoleLoadItem(pcross*4+i,generatePrimitive(0));
								}else{
children.population[cPosition].setRoleLoadItem(pcross*4+i,generatePrimitive(1));
								}
							}
						}
					}
					if((ptier <= 3) && (pcross * 8 <= 63)){	//if there are 3 or more layers below:
						for(i=0; i<8; i++){	//set the 8 sub nodes in three layers down.
							if(children.population[cPosition].getRoleLoadItem(pcross*8+i) != "_"){
//if the slot below is not empty, set the child's slot to a random primitive, otherwise child's tree to "_". 
								if(isFunction(children.population[cPosition].getRoleLoadItem(pcross*8+i))){
children.population[cPosition].setRoleLoadItem(pcross*8+i,generatePrimitive(0));
								}else{
children.population[cPosition].setRoleLoadItem(pcross*8+i,generatePrimitive(1));
								}
							}
						}
					}
					if((ptier <= 2) && (pcross * 16 <= 63)){	//if there are 4 or more layers below:
						for(i=0; i<16; i++){	//set the 16 sub nodes in four layers down.
							if(children.population[cPosition].getRoleLoadItem(pcross*16+i) != "_"){
//if the slot below is not empty, set the child's slot to a random primitive, otherwise child's tree to "_". 
								if(isFunction(children.population[cPosition].getRoleLoadItem(pcross*16+i))){
children.population[cPosition].setRoleLoadItem(pcross*16+i,generatePrimitive(0));
								}else{
children.population[cPosition].setRoleLoadItem(pcross*16+i,generatePrimitive(1));
								}
							}
						}
					}
					if((ptier <= 1) && (pcross * 32 <= 63)){	//if there are 5 or more layers below:
						for(i=0; i<32; i++){	//set the 32 sub nodes in five layers down.
							if(children.population[cPosition].getRoleLoadItem(pcross*32+i) != "_"){
//if the slot below is not empty, set the child's slot to a random primitive, otherwise child's tree to "_". 
								if(isFunction(children.population[cPosition].getRoleLoadItem(pcross*32+i))){
children.population[cPosition].setRoleLoadItem(pcross*32+i,generatePrimitive(0));
								}else{
children.population[cPosition].setRoleLoadItem(pcross*32+i,generatePrimitive(1));
								}
							}
						}
					}
					if(debugGP) cout << "p crossover point: " << pcross << " (tier " << ptier << "), : " <<  parents.population[pPosition].getRoleLoadItem(pcross) << endl;
					break;
				case 2: //roleIncongruence - tier 2 equation - use terminals from tier 1 only.
					if(debugGP) cout << "mutating roleIncongruence" << endl;
					if(isFunction(children.population[cPosition].getRoleIncongruenceItem(pcross))){
						children.population[cPosition].setRoleIncongruenceItem(pcross,generatePrimitive(0));
					}else{
						children.population[cPosition].setRoleIncongruenceItem(pcross,generatePrimitive(1));
					}
					if((ptier <= 5) && (pcross * 2 <= 63)){	//if there are 1 or more layers below:
						for(i=0; i<2; i++){	//set the 2 sub nodes in one layer down.
							if(children.population[cPosition].getRoleIncongruenceItem(pcross*2+i) != "_"){
//if the slot below is not empty, set the child's slot to a random primitive, otherwise child's tree to "_". 
								if(isFunction(children.population[cPosition].getRoleIncongruenceItem(pcross*2+i))){
children.population[cPosition].setRoleIncongruenceItem(pcross*2+i,generatePrimitive(0));
								}else{
children.population[cPosition].setRoleIncongruenceItem(pcross*2+i,generatePrimitive(1));
								}
							}
						}
					}
					if((ptier <= 4) && (pcross * 4 <= 63)){	//if there are 2 or more layers below:
						for(i=0; i<4; i++){	//set the 4 sub nodes in two layers down.
							if(children.population[cPosition].getRoleIncongruenceItem(pcross*4+i) != "_"){
//if the slot below is not empty, set the child's slot to a random primitive, otherwise child's tree to "_". 
								if(isFunction(children.population[cPosition].getRoleIncongruenceItem(pcross*4+i))){
children.population[cPosition].setRoleIncongruenceItem(pcross*4+i,generatePrimitive(0));
								}else{
children.population[cPosition].setRoleIncongruenceItem(pcross*4+i,generatePrimitive(1));
								}
							}
						}
					}
					if((ptier <= 3) && (pcross * 8 <= 63)){	//if there are 3 or more layers below:
						for(i=0; i<8; i++){	//set the 8 sub nodes in three layers down.
							if(children.population[cPosition].getRoleIncongruenceItem(pcross*8+i) != "_"){
//if the slot below is not empty, set the child's slot to a random primitive, otherwise child's tree to "_". 
								if(isFunction(children.population[cPosition].getRoleIncongruenceItem(pcross*8+i))){
children.population[cPosition].setRoleIncongruenceItem(pcross*8+i,generatePrimitive(0));
								}else{
children.population[cPosition].setRoleIncongruenceItem(pcross*8+i,generatePrimitive(1));
								}
							}
						}
					}
					if((ptier <= 2) && (pcross * 16 <= 63)){	//if there are 4 or more layers below:
						for(i=0; i<16; i++){	//set the 16 sub nodes in four layers down.
							if(children.population[cPosition].getRoleIncongruenceItem(pcross*16+i) != "_"){
//if the slot below is not empty, set the child's slot to a random primitive, otherwise child's tree to "_". 
								if(isFunction(children.population[cPosition].getRoleIncongruenceItem(pcross*16+i))){
children.population[cPosition].setRoleIncongruenceItem(pcross*16+i,generatePrimitive(0));
								}else{
children.population[cPosition].setRoleIncongruenceItem(pcross*16+i,generatePrimitive(1));
								}
							}
						}
					}
					if((ptier <= 1) && (pcross * 32 <= 63)){	//if there are 5 or more layers below:
						for(i=0; i<32; i++){	//set the 32 sub nodes in five layers down.
							if(children.population[cPosition].getRoleIncongruenceItem(pcross*32+i) != "_"){
//if the slot below is not empty, set the child's slot to a random primitive, otherwise child's tree to "_". 
								if(isFunction(children.population[cPosition].getRoleIncongruenceItem(pcross*32+i))){
children.population[cPosition].setRoleIncongruenceItem(pcross*32+i,generatePrimitive(0));
								}else{
children.population[cPosition].setRoleIncongruenceItem(pcross*32+i,generatePrimitive(1));
								}
							}
						}
					}
					if(debugGP) cout << "p crossover point: " << pcross << " (tier " << ptier << "), : " <<  parents.population[pPosition].getRoleIncongruenceItem(pcross) << endl;
					break;
				case 3: //roleStrain - tier 3 equation - use terminals from tier 1 & 2 only.
					if(debugGP) cout << "mutating roleStrain" << endl;
					if(isFunction(children.population[cPosition].getRoleStrainItem(pcross))){
						children.population[cPosition].setRoleStrainItem(pcross,generatePrimitive(0));
					}else{
						children.population[cPosition].setRoleStrainItem(pcross,generatePrimitive(2));
					}
					if((ptier <= 5) && (pcross * 2 <= 63)){	//if there are 1 or more layers below:
						for(i=0; i<2; i++){	//set the 2 sub nodes in one layer down.
							if(children.population[cPosition].getRoleStrainItem(pcross*2+i) != "_"){
//if the slot below is not empty, set the child's slot to a random primitive, otherwise child's tree to "_". 
								if(isFunction(children.population[cPosition].getRoleStrainItem(pcross*2+i))){
children.population[cPosition].setRoleStrainItem(pcross*2+i,generatePrimitive(0));
								}else{
children.population[cPosition].setRoleStrainItem(pcross*2+i,generatePrimitive(2));
								}
							}
						}
					}
					if((ptier <= 4) && (pcross * 4 <= 63)){	//if there are 2 or more layers below:
						for(i=0; i<4; i++){	//set the 4 sub nodes in two layers down.
							if(children.population[cPosition].getRoleStrainItem(pcross*4+i) != "_"){
//if the slot below is not empty, set the child's slot to a random primitive, otherwise child's tree to "_". 
								if(isFunction(children.population[cPosition].getRoleStrainItem(pcross*4+i))){
children.population[cPosition].setRoleStrainItem(pcross*4+i,generatePrimitive(0));
								}else{
children.population[cPosition].setRoleStrainItem(pcross*4+i,generatePrimitive(2));
								}
							}
						}
					}
					if((ptier <= 3) && (pcross * 8 <= 63)){	//if there are 3 or more layers below:
						for(i=0; i<8; i++){	//set the 8 sub nodes in three layers down.
							if(children.population[cPosition].getRoleStrainItem(pcross*8+i) != "_"){
//if the slot below is not empty, set the child's slot to a random primitive, otherwise child's tree to "_". 
								if(isFunction(children.population[cPosition].getRoleStrainItem(pcross*8+i))){
children.population[cPosition].setRoleStrainItem(pcross*8+i,generatePrimitive(0));
								}else{
children.population[cPosition].setRoleStrainItem(pcross*8+i,generatePrimitive(2));
								}
							}
						}
					}
					if((ptier <= 2) && (pcross * 16 <= 63)){	//if there are 4 or more layers below:
						for(i=0; i<16; i++){	//set the 16 sub nodes in four layers down.
							if(children.population[cPosition].getRoleStrainItem(pcross*16+i) != "_"){
//if the slot below is not empty, set the child's slot to a random primitive, otherwise child's tree to "_". 
								if(isFunction(children.population[cPosition].getRoleStrainItem(pcross*16+i))){
children.population[cPosition].setRoleStrainItem(pcross*16+i,generatePrimitive(0));
								}else{
children.population[cPosition].setRoleStrainItem(pcross*16+i,generatePrimitive(2));
								}
							}
						}
					}
					if((ptier <= 1) && (pcross * 32 <= 63)){	//if there are 5 or more layers below:
						for(i=0; i<32; i++){	//set the 32 sub nodes in five layers down.
							if(children.population[cPosition].getRoleStrainItem(pcross*32+i) != "_"){
//if the slot below is not empty, set the child's slot to a random primitive, otherwise child's tree to "_". 
								if(isFunction(children.population[cPosition].getRoleStrainItem(pcross*32+i))){
children.population[cPosition].setRoleStrainItem(pcross*32+i,generatePrimitive(0));
								}else{
children.population[cPosition].setRoleStrainItem(pcross*32+i,generatePrimitive(2));
								}
							}
						}
					}
					if(debugGP) cout << "p crossover point: " << pcross << " (tier " << ptier << "), : " <<  parents.population[pPosition].getRoleStrainItem(pcross) << endl;
					break;
				case 4: //FirstDrinkDisposition - tier 6 equation - use terminals from tiers 1, 2, 3, 4 & 5 only.
					if(debugGP) cout << "mutating first drink disposition" << endl;
					if(isFunction(children.population[cPosition].getFirstDrinkDispositionItem(pcross))){
						children.population[cPosition].setFirstDrinkDispositionItem(pcross,generatePrimitive(0));
					}else{
						children.population[cPosition].setFirstDrinkDispositionItem(pcross,generatePrimitive(5));
					}
					if((ptier <= 5) && (pcross * 2 <= 63)){	//if there are 1 or more layers below:
						for(i=0; i<2; i++){	//set the 2 sub nodes in one layer down.
							if(children.population[cPosition].getFirstDrinkDispositionItem(pcross*2+i) != "_"){
//if the slot below is not empty, set the child's slot to a random primitive, otherwise child's tree to "_". 
								if(isFunction(children.population[cPosition].getFirstDrinkDispositionItem(pcross*2+i))){
children.population[cPosition].setFirstDrinkDispositionItem(pcross*2+i,generatePrimitive(0));
								}else{
children.population[cPosition].setFirstDrinkDispositionItem(pcross*2+i,generatePrimitive(5));
								}
							}
						}
					}
					if((ptier <= 4) && (pcross * 4 <= 63)){	//if there are 2 or more layers below:
						for(i=0; i<4; i++){	//set the 4 sub nodes in two layers down.
							if(children.population[cPosition].getFirstDrinkDispositionItem(pcross*4+i) != "_"){
//if the slot below is not empty, set the child's slot to a random primitive, otherwise child's tree to "_". 
								if(isFunction(children.population[cPosition].getFirstDrinkDispositionItem(pcross*4+i))){
children.population[cPosition].setFirstDrinkDispositionItem(pcross*4+i,generatePrimitive(0));
								}else{
children.population[cPosition].setFirstDrinkDispositionItem(pcross*4+i,generatePrimitive(5));
								}
							}
						}
					}
					if((ptier <= 3) && (pcross * 8 <= 63)){	//if there are 3 or more layers below:
						for(i=0; i<8; i++){	//set the 8 sub nodes in three layers down.
							if(children.population[cPosition].getFirstDrinkDispositionItem(pcross*8+i) != "_"){
//if the slot below is not empty, set the child's slot to a random primitive, otherwise child's tree to "_". 
								if(isFunction(children.population[cPosition].getFirstDrinkDispositionItem(pcross*8+i))){
children.population[cPosition].setFirstDrinkDispositionItem(pcross*8+i,generatePrimitive(0));
								}else{
children.population[cPosition].setFirstDrinkDispositionItem(pcross*8+i,generatePrimitive(5));
								}
							}
						}
					}
					if((ptier <= 2) && (pcross * 16 <= 63)){	//if there are 4 or more layers below:
						for(i=0; i<16; i++){	//set the 16 sub nodes in four layers down.
							if(children.population[cPosition].getFirstDrinkDispositionItem(pcross*16+i) != "_"){
//if the slot below is not empty, set the child's slot to a random primitive, otherwise child's tree to "_". 
								if(isFunction(children.population[cPosition].getFirstDrinkDispositionItem(pcross*16+i))){
children.population[cPosition].setFirstDrinkDispositionItem(pcross*16+i,generatePrimitive(0));
								}else{
children.population[cPosition].setFirstDrinkDispositionItem(pcross*16+i,generatePrimitive(5));
								}
							}
						}
					}
					if((ptier <= 1) && (pcross * 32 <= 63)){	//if there are 5 or more layers below:
						for(i=0; i<32; i++){	//set the 32 sub nodes in five layers down.
							if(children.population[cPosition].getFirstDrinkDispositionItem(pcross*32+i) != "_"){
//if the slot below is not empty, set the child's slot to a random primitive, otherwise child's tree to "_". 
								if(isFunction(children.population[cPosition].getFirstDrinkDispositionItem(pcross*32+i))){
children.population[cPosition].setFirstDrinkDispositionItem(pcross*32+i,generatePrimitive(0));
								}else{
children.population[cPosition].setFirstDrinkDispositionItem(pcross*32+i,generatePrimitive(5));
								}
							}
						}
					}
					if(debugGP) cout << "p crossover point: " << pcross << " (tier " << ptier << "), : " <<  parents.population[pPosition].getFirstDrinkDispositionItem(pcross) << endl;
					break;
				case 5: //NextDrinkDisposition - tier 6 equation - use terminals from tiers 1, 2, 3, 4 & 5 only.
					if(debugGP) cout << "mutating next drink disposition" << endl;
					if(isFunction(children.population[cPosition].getNextDrinkDispositionItem(pcross))){
						children.population[cPosition].setNextDrinkDispositionItem(pcross,generatePrimitive(0));
					}else{
						children.population[cPosition].setNextDrinkDispositionItem(pcross,generatePrimitive(5));
					}
					if((ptier <= 5) && (pcross * 2 <= 63)){	//if there are 1 or more layers below:
						for(i=0; i<2; i++){	//set the 2 sub nodes in one layer down.
							if(children.population[cPosition].getNextDrinkDispositionItem(pcross*2+i) != "_"){
//if the slot below is not empty, set the child's slot to a random primitive, otherwise child's tree to "_". 
								if(isFunction(children.population[cPosition].getNextDrinkDispositionItem(pcross*2+i))){
children.population[cPosition].setNextDrinkDispositionItem(pcross*2+i,generatePrimitive(0));
								}else{
children.population[cPosition].setNextDrinkDispositionItem(pcross*2+i,generatePrimitive(5));
								}
							}
						}
					}
					if((ptier <= 4) && (pcross * 4 <= 63)){	//if there are 2 or more layers below:
						for(i=0; i<4; i++){	//set the 4 sub nodes in two layers down.
							if(children.population[cPosition].getNextDrinkDispositionItem(pcross*4+i) != "_"){
//if the slot below is not empty, set the child's slot to a random primitive, otherwise child's tree to "_". 
								if(isFunction(children.population[cPosition].getNextDrinkDispositionItem(pcross*4+i))){
children.population[cPosition].setNextDrinkDispositionItem(pcross*4+i,generatePrimitive(0));
								}else{
children.population[cPosition].setNextDrinkDispositionItem(pcross*4+i,generatePrimitive(5));
								}
							}
						}
					}
					if((ptier <= 3) && (pcross * 8 <= 63)){	//if there are 3 or more layers below:
						for(i=0; i<8; i++){	//set the 8 sub nodes in three layers down.
							if(children.population[cPosition].getNextDrinkDispositionItem(pcross*8+i) != "_"){
//if the slot below is not empty, set the child's slot to a random primitive, otherwise child's tree to "_". 
								if(isFunction(children.population[cPosition].getNextDrinkDispositionItem(pcross*8+i))){
children.population[cPosition].setNextDrinkDispositionItem(pcross*8+i,generatePrimitive(0));
								}else{
children.population[cPosition].setNextDrinkDispositionItem(pcross*8+i,generatePrimitive(5));
								}
							}
						}
					}
					if((ptier <= 2) && (pcross * 16 <= 63)){	//if there are 4 or more layers below:
						for(i=0; i<16; i++){	//set the 16 sub nodes in four layers down.
							if(children.population[cPosition].getNextDrinkDispositionItem(pcross*16+i) != "_"){
//if the slot below is not empty, set the child's slot to a random primitive, otherwise child's tree to "_". 
								if(isFunction(children.population[cPosition].getNextDrinkDispositionItem(pcross*16+i))){
children.population[cPosition].setNextDrinkDispositionItem(pcross*16+i,generatePrimitive(0));
								}else{
children.population[cPosition].setNextDrinkDispositionItem(pcross*16+i,generatePrimitive(5));
								}
							}
						}
					}
					if((ptier <= 1) && (pcross * 32 <= 63)){	//if there are 5 or more layers below:
						for(i=0; i<32; i++){	//set the 32 sub nodes in five layers down.
							if(children.population[cPosition].getNextDrinkDispositionItem(pcross*32+i) != "_"){
//if the slot below is not empty, set the child's slot to a random primitive, otherwise child's tree to "_". 
								if(isFunction(children.population[cPosition].getNextDrinkDispositionItem(pcross*32+i))){
children.population[cPosition].setNextDrinkDispositionItem(pcross*32+i,generatePrimitive(0));
								}else{
children.population[cPosition].setNextDrinkDispositionItem(pcross*32+i,generatePrimitive(5));
								}
							}
						}
					}
					if(debugGP) cout << "p crossover point: " << pcross << " (tier " << ptier << "), : " <<  parents.population[pPosition].getNextDrinkDispositionItem(pcross) << endl;
					break;
				default:
					cout << "Error 2 in subtreeMutate(), tree was " << tree << endl;
			}
			break;
		default:
			cout << "Error 1 in subtreeMutate(), tree was " << tree << endl;
			return -1;
	}
	if(debugGP) cout << "finished subtree mutation" << endl;
	return 0;
}	

//define a method to perform point mutation on an individual randomly using a given mutation rate and transfer it to the next gen.
//Note: mutationRate should be between 0 and 1, should run srand(time(NULL)) before this function. 
int pointMutation(double mutationRate, Generation& parents, int pPosition, Generation& children, int cPosition){
	if(debugGP) cout << "performing point mutation, mutationRate = " << mutationRate << ", pPosition = " << pPosition << ", cPosition = " << cPosition << endl;
	//srand(time(NULL));
	int i;
	reproduce(parents, pPosition, children, cPosition); //copy the parent into the next generation.
	if(debugGP) cout << "reproduced parent" << endl;
	for(i=1; i<=63; i++){	//RoleLoad is a tier 2 equation, hence only primitives from tier 1 should be used.
		if((children.population[cPosition].getRoleLoadItem(i) != "_") && ((((double)rand())/(RAND_MAX)) <= mutationRate)){
			if(debugGP) cout << "mutating RoleLoad." << endl;
			if(isFunction(children.population[cPosition].getRoleLoadItem(i))){
				children.population[cPosition].setRoleLoadItem(i,generatePrimitive(0));
			}else{
				children.population[cPosition].setRoleLoadItem(i,generatePrimitive(1));
			}
		} //RoleIncongruence is a tier 2 equation, hence only primitives from tier 1 should be used.
		if((children.population[cPosition].getRoleIncongruenceItem(i) != "_") && ((((double)rand())/(RAND_MAX)) <= mutationRate)){
			if(debugGP) cout << "mutating RoleIncongruence." << endl;
			if(isFunction(children.population[cPosition].getRoleIncongruenceItem(i))){
				children.population[cPosition].setRoleIncongruenceItem(i,generatePrimitive(0));
			}else{
				children.population[cPosition].setRoleIncongruenceItem(i,generatePrimitive(1));
			}
		} //RoleStrain is a tier 3 equation, hence only primitives from tiers 1 & 2 should be used.
		if((children.population[cPosition].getRoleStrainItem(i) != "_") && ((((double)rand())/(RAND_MAX)) <= mutationRate)){
			if(debugGP) cout << "mutating RoleStrain." << endl;
			if(isFunction(children.population[cPosition].getRoleStrainItem(i))){
				children.population[cPosition].setRoleStrainItem(i,generatePrimitive(0));
			}else{
				children.population[cPosition].setRoleStrainItem(i,generatePrimitive(2));
			}
		} //FirstDrinkDisposition is a tier 6 equation, hence only primitives from tiers 1, 2, 3, 4 & 5 should be used.
		if((children.population[cPosition].getFirstDrinkDispositionItem(i) != "_") && ((((double)rand())/(RAND_MAX)) <= mutationRate)){
			if(debugGP) cout << "mutating FirstDrinkDisposition." << endl;
			if(isFunction(children.population[cPosition].getFirstDrinkDispositionItem(i))){
				children.population[cPosition].setFirstDrinkDispositionItem(i,generatePrimitive(0));
			}else{
				children.population[cPosition].setFirstDrinkDispositionItem(i,generatePrimitive(5));
			}
		} //NextDrinkDisposition is a tier 6 equation, hence only primitives from tiers 1, 2, 3, 4 & 5 should be used.
		if((children.population[cPosition].getNextDrinkDispositionItem(i) != "_") && ((((double)rand())/(RAND_MAX)) <= mutationRate)){
			if(debugGP) cout << "mutating NextDrinkDisposition." << endl;
			if(isFunction(children.population[cPosition].getNextDrinkDispositionItem(i))){
				children.population[cPosition].setNextDrinkDispositionItem(i,generatePrimitive(0));
			}else{
				children.population[cPosition].setNextDrinkDispositionItem(i,generatePrimitive(5));
			}
		}
	}
	if(debugGP) cout << "Point mutation finished." << endl;
	return 0;
}	

//define a method to apply an individual's equations to the model.
int applyIndividualToModel(Generation& gen, int position, string roles_path){
	if(debugGP) cout << "applying individual " << position << " to model in path " << roles_path << endl;
	FileEdit rolesTheoryFile(roles_path + "src/", "RolesTheory.cpp");
	//RoleOverload
	rolesTheoryFile.replaceLine((rolesTheoryFile.findText("//EQUATION:mRoleLoad")+1), gen.getRoleLoadCode(position));
	//RoleIncongruence
	rolesTheoryFile.replaceLine((rolesTheoryFile.findText("//EQUATION:mRoleIncongruence")+1), gen.getRoleIncongruenceCode(position));
	//RoleStrain
	rolesTheoryFile.replaceLine((rolesTheoryFile.findText("//EQUATION:mRoleStrain")+1), gen.getRoleStrainCode(position));
	//FirstDrinkDisposition
	rolesTheoryFile.replaceLine((rolesTheoryFile.findText("//EQUATION:probFirstDrink")+1), gen.getFirstDrinkDispositionCode(position));
	//NextDrinkDisposition
	rolesTheoryFile.replaceLine((rolesTheoryFile.findText("//EQUATION:NextDrinkDisposition")+1), gen.getNextDrinkDispositionCode(position));
	if(debugGP) cout << "individual applied." << endl;
	return 0;
}

//define a method to run the model.
int compileAndRunModel(string roles_path){
	if(debugGP) cout << "compiling and running the model at " << roles_path << endl;
	int i;
	cout << "Checking if processor is available...\t";
	if (system(NULL)) puts ("Ok");
	else exit (EXIT_FAILURE);

	cout << "Navigating to model directory...";
	i=chdir(roles_path.c_str());	//change this to path to 'roles' folder
	cout << "\tThe value returned was: " << i << endl; //0 if worked, -1 if failed
	if(i != 0)	return i;

	cout << "compiling roles model...";
	i=system("make compile");
	cout << "\tThe value returned was: " << i << endl;
	if(i != 0)	return i;

	cout << "running model...";
	i=system("mpirun -n 1 ./bin/main.exe ./props/config.props ./props/model.props");
	cout << "The value returned was: " << i << endl;
	if(i != 0)	return i;
	
	return 0;
}

int compileModel(string roles_path){
	if(debugGP) cout << "compiling the model at " << roles_path << endl;
	int i;
	cout << "Checking if processor is available...\t";
	if (system(NULL)) puts ("Ok");
	else exit (EXIT_FAILURE);

	cout << "Navigating to model directory...";
	i=chdir(roles_path.c_str());	//change this to path to 'roles' folder
	cout << "\tThe value returned was: " << i << endl; //0 if worked, -1 if failed
	if(i != 0)	return i;

	cout << "compiling roles model...";
	i=system("make compile");
	cout << "The value returned was: " << i << endl;
	if(i != 0)	return i;
	
	return 0;
}

int runModelJustCompileRT(string roles_path){
	if(debugGP) cout << "compiling RolesTheory.cpp and running the model at " << roles_path << endl;
	int i;
	cout << "Checking if processor is available...\t";
	if (system(NULL)) puts ("Ok");
	else exit (EXIT_FAILURE);

	cout << "Navigating to model directory...";
	i=chdir(roles_path.c_str());	//change this to path to 'roles' folder
	cout << "\tThe value returned was: " << i << endl; //0 if worked, -1 if failed
	if(i != 0)	return i;

	cout << "compiling new roleTheory.cpp file...";
	i=system("make compile_roleTheory");
	cout << "The value returned was: " << i << endl;
	if(i != 0)	return i;

	cout << "running model...";
	i=system("mpirun -n 1 ./bin/main.exe ./props/config.props ./props/model.props");
	cout << "\tThe value returned was: " << i << endl;
	if(i != 0)	return i;
	
	return 0;
}

//define a method to calculate the fitness of an individual using the current model output.
double calculateFitness(string roles_path){	//INCOMPLETE - COMPLEXITY NOT TAKEN INTO ACCOUNT
	if(debugGP) cout << "calculating fitness for roles model at path " << roles_path << endl;
	int i, j, k, m, dataUsed=0;
	FileEdit outputData((roles_path + "outputs/"), "annual_data.csv");
	FileEdit targetData("/home/jack/roles-gp/programs/", "target graphs.csv");
	string outputYearString, targetYearString;
	double outputYear, targetYear, sum=0, fitness;
	double PREV_SEX1Ptar, PREV_SEX2Ptar, FREQ_SEX1Ptar, FREQ_SEX2Ptar, QUANT_SEX1Ptar, QUANT_SEX2Ptar;
	double PREV_SEX1Pout, PREV_SEX2Pout, FREQ_SEX1Pout, FREQ_SEX2Pout, QUANT_SEX1Pout, QUANT_SEX2Pout;
	double Maleout, Femaleout, XIIMonthDrinkersMaleout, XIIMonthDrinkersFemaleout, FreqMaleout, FreqFemaleout, QuantMaleout, QuantFemaleout;
	double PREV_SEX1SE, PREV_SEX2SE, FREQ_SEX1SE, FREQ_SEX2SE, QUANT_SEX1SE, QUANT_SEX2SE;
	double PREV_SEX1range = 1, PREV_SEX2range = 1, FREQ_SEX1range = 6, FREQ_SEX2range = 6, QUANT_SEX1range = 30, QUANT_SEX2range = 30;
	double percentage = 0.1;
	double residual[(21*6)];
	//double differences[21];  //see note below

//Note: The number of items in the residual array is the number of years the target data has data for. This number will need to be changed if the target data is updated (i.e. it should be the number of rows that contain data in the target data spreadsheet minus the row for headers). It will also need to be changed in the for loop below.

	for(i=2; i<=17; i++){ //read rows 2 to 11 of outputs. //NOTE: Change this i<= to the number of the last row with data!
		//Read the output year row i and find the corresponding simulated target year row j (if it exists). Exclude 1979. 
		outputYearString = outputData.readCSV(i, 1);	//read in the year in the simulation (ticks in days)
		outputYear = (stod(outputYearString)/365) + 1979;	//convert the simulation tick into a year. e.g. (365/365)+1979 = 1980.
		//cout << "outputYear: " << outputYear << endl;
		targetYear = 0;
		for(j=2; j<32; j++){	//work out what row in the target data corresponds to the year in the output data. 
			//cout << "targetDataYearString: " << targetData.readCSV(j, 1) << endl;
			if(stod(targetData.readCSV(j, 1)) == outputYear){ //compare the targetData year to the outputData year. 
				targetYear = stod(targetData.readCSV(j, 1));
				break; //j is assigned to the correct row, exit the for loop. 
			}
		}
		//If data for the actual year does not exist, continue, else increment the number of data used. 
		if((targetYear == 0) || (targetYear == 1979)){
			//cout << "data not found for year " << outputYear << endl;
			continue; //restart for loop with next i. 
		}
		//cout << "targetYear: " << targetYear << ". i (outputDataRow): " << i << ". j (targetDataRow): " << j << endl;

		//read in target data for year targetYear (from row j)
		try{	PREV_SEX1Ptar = stod(targetData.readCSV(j, 2));	} catch(...){ PREV_SEX1Ptar = -1;	} //=-1 if error.
 		try{	PREV_SEX2Ptar = stod(targetData.readCSV(j, 4));	} catch(...){ PREV_SEX2Ptar = -1; 	} //=-1 if error.
 		try{	FREQ_SEX1Ptar = stod(targetData.readCSV(j, 14));} catch(...){ FREQ_SEX1Ptar = -1; 	} //=-1 if error.
 		try{	FREQ_SEX2Ptar = stod(targetData.readCSV(j, 16));} catch(...){ FREQ_SEX2Ptar = -1; 	} //=-1 if error.
 		try{	QUANT_SEX1Ptar = stod(targetData.readCSV(j, 26));}catch(...){ QUANT_SEX1Ptar = -1; 	} //=-1 if error.
 		try{	QUANT_SEX2Ptar = stod(targetData.readCSV(j, 28));}catch(...){ QUANT_SEX1Ptar = -1; 	} //=-1 if error.

		try{	PREV_SEX1SE = stod(targetData.readCSV(j, 3));	} catch(...){ PREV_SEX1SE = -1; 	} //=-1 if error.
 		try{	PREV_SEX2SE = stod(targetData.readCSV(j, 5));	} catch(...){ PREV_SEX2SE = -1; 	} //=-1 if error.
 		try{	FREQ_SEX1SE = stod(targetData.readCSV(j, 15));	} catch(...){ FREQ_SEX1SE = -1; 	} //=-1 if error.
 		try{	FREQ_SEX2SE = stod(targetData.readCSV(j, 17));	} catch(...){ FREQ_SEX2SE = -1; 	} //=-1 if error.
 		try{	QUANT_SEX1SE = stod(targetData.readCSV(j, 27));	} catch(...){ QUANT_SEX1SE = -1; 	} //=-1 if error.
 		try{	QUANT_SEX2SE = stod(targetData.readCSV(j, 29));	} catch(...){ QUANT_SEX2SE = -1; 	} //=-1 if error.

		//read in output data for year targetYear (from row i)
		try{	Maleout = stod(outputData.readCSV(i, 3));	} catch(...){ Maleout = -1; 	} //=-1 if error.
		try{	Femaleout = stod(outputData.readCSV(i, 4));	} catch(...){ Femaleout = -1; 	} //=-1 if error.
		try{	XIIMonthDrinkersMaleout = stod(outputData.readCSV(i, 10));} catch(...){	XIIMonthDrinkersMaleout = -1; 	} //=-1 if error.
		try{	XIIMonthDrinkersFemaleout = stod(outputData.readCSV(i, 11));} catch(...){ XIIMonthDrinkersFemaleout = -1; } //=-1 if error.
		try{	QuantMaleout = stod(outputData.readCSV(i, 16));	} catch(...){ QuantMaleout = -1; 	} //=-1 if error.
		try{	QuantFemaleout = stod(outputData.readCSV(i, 17));}catch(...){ QuantFemaleout = -1; 	} //=-1 if error.
		try{	FreqMaleout = stod(outputData.readCSV(i, 22));	} catch(...){ FreqMaleout = -1; 	} //=-1 if error.
		try{	FreqFemaleout = stod(outputData.readCSV(i, 23));} catch(...){ FreqFemaleout = -1; 	} //=-1 if error.

		//calculate the metrics used in the target data for the output data.
		if((XIIMonthDrinkersMaleout != -1) && (Maleout != -1)){ 
			PREV_SEX1Pout = XIIMonthDrinkersMaleout/Maleout; }else{ PREV_SEX1Pout = -1; }
		if((XIIMonthDrinkersFemaleout != -1) && (Femaleout != -1)){ 
			PREV_SEX2Pout = XIIMonthDrinkersFemaleout/Femaleout; }else{ PREV_SEX2Pout = -1; }
		if((FreqMaleout!= -1) && (XIIMonthDrinkersMaleout != -1)){ 
			FREQ_SEX1Pout = FreqMaleout/XIIMonthDrinkersMaleout; }else{ FREQ_SEX1Pout = -1; }
		if((FreqFemaleout != -1) && (XIIMonthDrinkersFemaleout != -1)){ 
			FREQ_SEX2Pout = FreqFemaleout/XIIMonthDrinkersFemaleout; }else{ FREQ_SEX2Pout = -1; }
		if((QuantMaleout != -1) && (XIIMonthDrinkersMaleout != -1)){ 
			QUANT_SEX1Pout = QuantMaleout*14/XIIMonthDrinkersMaleout; }else{ QUANT_SEX1Pout = -1; }
		if((QuantFemaleout != -1) && (XIIMonthDrinkersFemaleout != -1)){ 
			QUANT_SEX2Pout = QuantFemaleout*14/XIIMonthDrinkersFemaleout; }else{ QUANT_SEX2Pout = -1; }

		//cout << targetYear << " PREV_SEX1Pout: " << PREV_SEX1Pout << endl;
		//cout << targetYear << " PREV_SEX2Pout: " << PREV_SEX2Pout << endl;
		//cout << targetYear << " FREQ_SEX1Pout: " << FREQ_SEX1Pout << endl;
		//cout << targetYear << " FREQ_SEX2Pout: " << FREQ_SEX2Pout << endl;
		//cout << targetYear << " QUANT_SEX1Pout: " << QUANT_SEX1Pout << endl;
		//cout << targetYear << " QUANT_SEX2Pout: " << QUANT_SEX2Pout << endl;
		
		//cout << targetYear << " PREV_SEX1Ptar: " << PREV_SEX1Ptar << endl;
		//cout << targetYear << " PREV_SEX2Ptar: " << PREV_SEX2Ptar << endl;
		//cout << targetYear << " FREQ_SEX1Ptar: " << FREQ_SEX1Ptar << endl;
		//cout << targetYear << " FREQ_SEX2Ptar: " << FREQ_SEX2Ptar << endl;
		//cout << targetYear << " QUANT_SEX1Pout: " << QUANT_SEX1Pout << endl;
		//cout << targetYear << " QUANT_SEX2Pout: " << QUANT_SEX2Pout << endl;

		//cout << targetYear << " PREV_SEX1SE: " << PREV_SEX1SE << endl;
		//cout << targetYear << " PREV_SEX2SE: " << PREV_SEX2SE << endl;
		//cout << targetYear << " FREQ_SEX1SE: " << FREQ_SEX1SE << endl;
		//cout << targetYear << " FREQ_SEX2SE: " << FREQ_SEX2SE << endl;
		//cout << targetYear << " QUANT_SEX1SE: " << QUANT_SEX1SE << endl;
		//cout << targetYear << " QUANT_SEX2SE: " << QUANT_SEX2SE << endl;

		//calculate the residuals using sum((abs(y* - y))/(sqrt((sm)^2 + (dm)^2)))
		if((PREV_SEX1Pout != -1) && (PREV_SEX1Ptar != -1) && (PREV_SEX1SE != -1) && (PREV_SEX1range != -1)){
			residual[dataUsed] = abs(PREV_SEX1Pout - PREV_SEX1Ptar) / sqrt( pow(PREV_SEX1SE,2) + pow(percentage * PREV_SEX1range,2) );
			//cout << targetYear << " residual 0: " << residual[dataUsed] << endl;			
			dataUsed++;	
		}
		if((PREV_SEX2Pout != -1) && (PREV_SEX2Ptar != -1) && (PREV_SEX2SE != -1) && (PREV_SEX2range != -1)){
			residual[dataUsed] = abs(PREV_SEX2Pout - PREV_SEX2Ptar) / sqrt( pow(PREV_SEX2SE,2) + pow(percentage * PREV_SEX2range,2) );
			//cout << targetYear << " residual 1: " << residual[dataUsed] << endl;			
			dataUsed++;
		}
		if((FREQ_SEX1Pout != -1) && (FREQ_SEX1Ptar != -1) && (FREQ_SEX1SE != -1) && (FREQ_SEX1range != -1)){
			residual[dataUsed] = abs(FREQ_SEX1Pout - FREQ_SEX1Ptar) / sqrt( pow(FREQ_SEX1SE,2) + pow(percentage * FREQ_SEX1range,2) );
			//cout << targetYear << " residual 2: " << residual[dataUsed] << endl;			
			dataUsed++;
		}
		if((FREQ_SEX2Pout != -1) && (FREQ_SEX2Ptar != -1) && (FREQ_SEX2SE != -1) && (FREQ_SEX2range != -1)){
			residual[dataUsed] = abs(FREQ_SEX2Pout - FREQ_SEX2Ptar) / sqrt( pow(FREQ_SEX2SE,2) + pow(percentage * FREQ_SEX2range,2) );
			//cout << targetYear << " residual 3: " << residual[dataUsed] << endl;			
			dataUsed++;
		}
		if((QUANT_SEX1Pout != -1) && (QUANT_SEX1Ptar != -1) && (QUANT_SEX1SE != -1) && (QUANT_SEX1range != -1)){
			residual[dataUsed] = abs(QUANT_SEX1Pout - QUANT_SEX1Ptar) / sqrt( pow(QUANT_SEX1SE,2) + pow(percentage * QUANT_SEX1range,2) );
			//cout << targetYear << " residual 4: " << residual[dataUsed] << endl;			
			dataUsed++;
		}
		if((QUANT_SEX2Pout != -1) && (QUANT_SEX2Ptar != -1) && (QUANT_SEX2SE != -1) && (QUANT_SEX2range != -1)){
			residual[dataUsed] = abs(QUANT_SEX2Pout - QUANT_SEX2Ptar) / sqrt( pow(QUANT_SEX2SE,2) + pow(percentage * QUANT_SEX2range,2) );
			//cout << targetYear << " residual 5: " << residual[dataUsed] << endl;			
			dataUsed++;
		}
	}
	if(dataUsed == 0){ //If there was an error, return 999999 for fitness.
		return 999999;
	}
	for(i=0; i<dataUsed; i++){
		if(residual[i] != -1){
			sum = sum + residual[i];
		}
	}
	fitness = sum/dataUsed;
	return fitness;
}

double compileRTRunReturnFitness(string roles_path){
	double i;
	//cout << "Checking if processor is available...\t";
	//if (system(NULL)) puts ("Ok");
	//else exit (EXIT_FAILURE);

	//cout << "Navigating to model directory...";
	i=chdir(roles_path.c_str());	//change this to path to 'roles' folder
	//cout << "\tThe value returned was: " << i << endl; //0 if worked, -1 if failed
	if(i != 0)	return 999999999;

	//cout << "compiling new roleTheory.cpp file...";
	i=system("make compile_roleTheory");
	//cout << "The value returned was: " << i << endl;
	if(i != 0)	return 999999999;

	//cout << "running model...";
	i=system("mpirun -n 1 ./bin/main.exe ./props/config.props ./props/model.props");
	//cout << "\tThe value returned was: " << i << endl;
	if(i != 0)	return 999999999;

	i = calculateFitness(roles_path);
	
	return i;

}

int isIdentical(Generation& gen1, int individual1, Generation& gen2, int individual2){
	int i=1, identical=1;
	for(i=1; i<=63; i++){
		if(gen1.population[individual1].getRoleLoadItem(i) != gen2.population[individual2].getRoleLoadItem(i)){	identical = 0; break;	}
		if(gen1.population[individual1].getRoleIncongruenceItem(i) != gen2.population[individual2].getRoleIncongruenceItem(i)){	identical = 0; break;	}
		if(gen1.population[individual1].getRoleStrainItem(i) != gen2.population[individual2].getRoleStrainItem(i)){	identical = 0; break;	}
		if(gen1.population[individual1].getFirstDrinkDispositionItem(i) != gen2.population[individual2].getFirstDrinkDispositionItem(i)){	identical = 0; break;	}
		if(gen1.population[individual1].getNextDrinkDispositionItem(i) != gen2.population[individual2].getNextDrinkDispositionItem(i)){	identical = 0; break;	}
	}
	return identical;
}