/* Generation.cpp */

#include "Generation.h"

using namespace std;

//Define constructor.
Generation::Generation(){	//unparametrised constructor.
}

//Define function to create a C++ equation from three primitives. 
string Generation::equationConstructor(string left, string middle, string right){
	//Check it is possible to create a correct equation. Sides must not be an operator, middle must be. 
	//If middle is a blank space, return a blank space. 
	if((left == "+") || (left == "-") || (left == "*") || (left == "/") || (left == "^") || (left == "abs") || (left == "(1-x)") || (left == "error") || (middle == "error") || (right == "+") || (right == "-") || (right == "*") || (right == "/") || (right == "^") || (right == "abs") || (right == "(1-x)") || (right == "error")){
		cout << "Error trying to create equation from: " << left << ", " << middle << ", " << right << "." << endl;	
		return "0";
	}else if(middle == "_"){
		//cout << "_" << endl;
		return "_";
	//If either of left or right is a blank space, return just middle (presumably a constant). (i.e. "_", "3", "_" returns "3").
	}else if((left == "_") || ((right == "_") && (middle != "abs"))){
		//cout << middle << endl;
		return middle;
	//If middle is not an operator, an equation cannot be made. 
	}else if((middle != "+") && (middle != "-") && (middle != "*") && (middle != "/") && (middle != "^") && (middle != "abs") && (middle != "(1-x)")){
		cout << "Error trying to create equation from: " << left << ", " << middle << ", " << right << "." << endl;	
		return "0";
	//If middle is a divide, power or absolute, return the function correctly (note abs() only uses left argument). 
	}else if(middle == "/"){
		//cout << (middle == "/") << endl;
		return ("safeDivide(" + left + "," + right + ")");	
	}else if(middle == "^"){
		//cout << (middle == "^") << endl;
		return ("pow(" + left + "," + right + ")");
	}else if(middle == "abs"){
		//cout << ("fabs(" + left + ")") << endl;
		return ("fabs(" + left + ")");
	}else if(middle == "(1-x)"){
		//cout << ("(1-" + left + ")") << endl;
		return ("(1-" + left + ")");
	//Else return the default equation format.
	}else{
		//cout << ("(" + left + " " + middle + " " + right + ")") << endl;
		return ("(" + left + " " + middle + " " + right + ")");
	}	
}

//define function to return an item from a syntax tree using number references.
string Generation::getArrayItem(int position, int arrayNumber, int item){ 
	switch(arrayNumber){
		case 1: return population[position].getRoleLoadItem(item); 		break;
		case 2: return population[position].getRoleIncongruenceItem(item); 	break;
		case 3: return population[position].getRoleStrainItem(item); 		break;
		case 4: return population[position].getFirstDrinkDispositionItem(item); break;
		case 5: return population[position].getNextDrinkDispositionItem(item);  break;
		default: 
			cout << "Error trying to return item " << item << "from array number " << arrayNumber << " position " << position << endl;	
			return "1";
	}
}

/*
//declare a function to return the equation from a syntax tree with 15 primitives. 
string Generation::getCode15Primitives(int position, int arrayNumber){ 
	//arrayNumber 7 = getDailyDispositionMaleItem(item), 8 = getDailyDispositionFemaleItem(item)
	if((arrayNumber != 7) && (arrayNumber != 8)){	
		cout << "error: getCode15Primitives(position, " << arrayNumber << "); must be 7 or 8" << endl;	
	}

	string eqf = equationConstructor(getArrayItem(position, arrayNumber, 14), getArrayItem(position, arrayNumber, 7), getArrayItem(position, arrayNumber, 15));
	string eqe = equationConstructor(getArrayItem(position, arrayNumber, 12), getArrayItem(position, arrayNumber, 6), getArrayItem(position, arrayNumber, 13));
	string eqd = equationConstructor(getArrayItem(position, arrayNumber, 10), getArrayItem(position, arrayNumber, 5), getArrayItem(position, arrayNumber, 11));
	string eqc = equationConstructor(getArrayItem(position, arrayNumber, 8), getArrayItem(position, arrayNumber, 4), getArrayItem(position, arrayNumber, 9));

	string eqb = equationConstructor(eqe, getArrayItem(position, arrayNumber, 3), eqf);
	string eqa = equationConstructor(eqc, getArrayItem(position, arrayNumber, 2), eqd);

	string eqfinal = equationConstructor(eqa, getArrayItem(position, arrayNumber, 1), eqb);
	return eqfinal;
}
*/

//declare a function to return the equation from a syntax tree with 63 primitives. 
string Generation::getCode63Primitives(int position, int arrayNumber){ 
	//arrayNumber 1 = getRoleLoadCode(item), 2 = getRoleIncongruenceCode(item), 3 = getRoleStrainCode(item), 4 = getFirstDrinkDisposition(item), 5 = getNextDrinkDisposition(item)
	if((arrayNumber != 1) && (arrayNumber != 2)&& (arrayNumber != 3)&& (arrayNumber != 4) && (arrayNumber != 5)){	
		cout << "error: getCode63Primitives(position, " << arrayNumber << "); must be 1, 2, 3 or 6" << endl;	
	}
	string eqD = equationConstructor(getArrayItem(position, arrayNumber, 62), getArrayItem(position, arrayNumber, 31), getArrayItem(position, arrayNumber, 63));
	string eqC = equationConstructor(getArrayItem(position, arrayNumber, 60), getArrayItem(position, arrayNumber, 30), getArrayItem(position, arrayNumber, 61));
	string eqB = equationConstructor(getArrayItem(position, arrayNumber, 58), getArrayItem(position, arrayNumber, 29), getArrayItem(position, arrayNumber, 59));
	string eqA = equationConstructor(getArrayItem(position, arrayNumber, 56), getArrayItem(position, arrayNumber, 28), getArrayItem(position, arrayNumber, 57));
	string eqz = equationConstructor(getArrayItem(position, arrayNumber, 54), getArrayItem(position, arrayNumber, 27), getArrayItem(position, arrayNumber, 55));
	string eqy = equationConstructor(getArrayItem(position, arrayNumber, 52), getArrayItem(position, arrayNumber, 26), getArrayItem(position, arrayNumber, 53));
	string eqx = equationConstructor(getArrayItem(position, arrayNumber, 50), getArrayItem(position, arrayNumber, 25), getArrayItem(position, arrayNumber, 51));
	string eqw = equationConstructor(getArrayItem(position, arrayNumber, 48), getArrayItem(position, arrayNumber, 24), getArrayItem(position, arrayNumber, 49));
	string eqv = equationConstructor(getArrayItem(position, arrayNumber, 46), getArrayItem(position, arrayNumber, 23), getArrayItem(position, arrayNumber, 47));
	string equ = equationConstructor(getArrayItem(position, arrayNumber, 44), getArrayItem(position, arrayNumber, 22), getArrayItem(position, arrayNumber, 45));
	string eqt = equationConstructor(getArrayItem(position, arrayNumber, 42), getArrayItem(position, arrayNumber, 21), getArrayItem(position, arrayNumber, 43));
	string eqs = equationConstructor(getArrayItem(position, arrayNumber, 40), getArrayItem(position, arrayNumber, 20), getArrayItem(position, arrayNumber, 41));
	string eqr = equationConstructor(getArrayItem(position, arrayNumber, 38), getArrayItem(position, arrayNumber, 19), getArrayItem(position, arrayNumber, 39));
	string eqq = equationConstructor(getArrayItem(position, arrayNumber, 36), getArrayItem(position, arrayNumber, 18), getArrayItem(position, arrayNumber, 37));
	string eqp = equationConstructor(getArrayItem(position, arrayNumber, 34), getArrayItem(position, arrayNumber, 17), getArrayItem(position, arrayNumber, 35));
	string eqo = equationConstructor(getArrayItem(position, arrayNumber, 32), getArrayItem(position, arrayNumber, 16), getArrayItem(position, arrayNumber, 33));
	//cout << endl << eqD << "%" << eqC << "%" << eqB << "%" << eqA << "%" << eqz << "%" << eqy << "%" << eqx << "%" << eqw << "%" << eqv << "%" << equ << "%" << eqt << "%" << eqs << "%" << eqr << "%" << eqq << "%" << eqp << "%" << eqo << "%" << endl << endl;

	string eqn = equationConstructor(eqC, getArrayItem(position, arrayNumber, 15), eqD);
	string eqm = equationConstructor(eqA, getArrayItem(position, arrayNumber, 14), eqB);
	string eql = equationConstructor(eqy, getArrayItem(position, arrayNumber, 13), eqz);
	string eqk = equationConstructor(eqw, getArrayItem(position, arrayNumber, 12), eqx);
	string eqj = equationConstructor(equ, getArrayItem(position, arrayNumber, 11), eqv);
	string eqi = equationConstructor(eqs, getArrayItem(position, arrayNumber, 10), eqt);
	string eqh = equationConstructor(eqq, getArrayItem(position, arrayNumber, 9), eqr);
	string eqg = equationConstructor(eqo, getArrayItem(position, arrayNumber, 8), eqp);
	//cout << endl << eqn << "%" << eqm << "%" << eql << "%" << eqk << "%" << eqj << "%" << eqi << "%" << eqh << "%" << eqg << endl << endl;
	
	string eqf = equationConstructor(eqm, getArrayItem(position, arrayNumber, 7), eqn);
	string eqe = equationConstructor(eqk, getArrayItem(position, arrayNumber, 6), eql);
	string eqd = equationConstructor(eqi, getArrayItem(position, arrayNumber, 5), eqj);
	string eqc = equationConstructor(eqg, getArrayItem(position, arrayNumber, 4), eqh);

	string eqb = equationConstructor(eqe, getArrayItem(position, arrayNumber, 3), eqf);
	string eqa = equationConstructor(eqc, getArrayItem(position, arrayNumber, 2), eqd);

	string eqfinal = equationConstructor(eqa, getArrayItem(position, arrayNumber, 1), eqb);
	return eqfinal;
}

/*
//declare a function to return the equation from a syntax tree with 127 primitives. 
string Generation::getCode127Primitives(int position, int arrayNumber){ 
	//arrayNumber 4 = getDispositionMaleItem(item), 5 = getDispositionFemaleItem(item)
	if((arrayNumber != 4) && (arrayNumber != 5)){	
		cout << "error: getCode63Primitives(position, " << arrayNumber << "); must be 4 or 5" << endl;	
	}
	string eqkappa = equationConstructor(getArrayItem(position, arrayNumber, 126), getArrayItem(position, arrayNumber, 63), getArrayItem(position, arrayNumber, 127)); 	//cout << "eqkappa = " << eqkappa << endl;
	string eqiota = equationConstructor(getArrayItem(position, arrayNumber, 124), getArrayItem(position, arrayNumber, 62), getArrayItem(position, arrayNumber, 125));	//cout << "eqiota = " << eqiota << endl;
	string eqtheta = equationConstructor(getArrayItem(position, arrayNumber, 122), getArrayItem(position, arrayNumber, 61), getArrayItem(position, arrayNumber, 123));	//cout << "eqtheta = " << eqtheta << endl;
	string eqeta = equationConstructor(getArrayItem(position, arrayNumber, 120), getArrayItem(position, arrayNumber, 60), getArrayItem(position, arrayNumber, 121));	//cout << "eqeta = " << eqeta << endl;
	string eqzeta = equationConstructor(getArrayItem(position, arrayNumber, 118), getArrayItem(position, arrayNumber, 59), getArrayItem(position, arrayNumber, 119));	//cout << "eqzeta = " << eqzeta << endl;
	string eqepsilon = equationConstructor(getArrayItem(position, arrayNumber, 116), getArrayItem(position, arrayNumber, 58), getArrayItem(position, arrayNumber, 117));	//cout << "eqepsilon = " << eqepsilon << endl;
	string eqdelta = equationConstructor(getArrayItem(position, arrayNumber, 114), getArrayItem(position, arrayNumber, 57), getArrayItem(position, arrayNumber, 115));	//cout << "eqdelta = " << eqdelta << endl;
	string eqgamma = equationConstructor(getArrayItem(position, arrayNumber, 112), getArrayItem(position, arrayNumber, 56), getArrayItem(position, arrayNumber, 113));	//cout << "eqgamma = " << eqgamma << endl;
	string eqbeta = equationConstructor(getArrayItem(position, arrayNumber, 110), getArrayItem(position, arrayNumber, 55), getArrayItem(position, arrayNumber, 111));	//cout << "eqbeta = " << eqbeta << endl;
	string eqalpha = equationConstructor(getArrayItem(position, arrayNumber, 108), getArrayItem(position, arrayNumber, 54), getArrayItem(position, arrayNumber, 109));	//cout << "eqalpha = " << eqalpha << endl;
	string eqZ = equationConstructor(getArrayItem(position, arrayNumber, 106), getArrayItem(position, arrayNumber, 53), getArrayItem(position, arrayNumber, 107));	//cout << "eqZ = " << eqZ << endl;
	string eqY = equationConstructor(getArrayItem(position, arrayNumber, 104), getArrayItem(position, arrayNumber, 52), getArrayItem(position, arrayNumber, 105));	//cout << "eqY = " << eqY << endl;
	string eqX = equationConstructor(getArrayItem(position, arrayNumber, 102), getArrayItem(position, arrayNumber, 51), getArrayItem(position, arrayNumber, 103));	//cout << "eqX = " << eqX << endl;
	string eqW = equationConstructor(getArrayItem(position, arrayNumber, 100), getArrayItem(position, arrayNumber, 50), getArrayItem(position, arrayNumber, 101));	//cout << "eqW = " << eqW << endl;
	string eqV = equationConstructor(getArrayItem(position, arrayNumber, 98), getArrayItem(position, arrayNumber, 49), getArrayItem(position, arrayNumber, 99));	//cout << "eqV = " << eqV << endl;
	string eqU = equationConstructor(getArrayItem(position, arrayNumber, 96), getArrayItem(position, arrayNumber, 48), getArrayItem(position, arrayNumber, 97));	//cout << "eqU = " << eqU << endl;
	string eqT = equationConstructor(getArrayItem(position, arrayNumber, 94), getArrayItem(position, arrayNumber, 47), getArrayItem(position, arrayNumber, 95));	//cout << "eqT = " << eqT << endl;
	string eqS = equationConstructor(getArrayItem(position, arrayNumber, 92), getArrayItem(position, arrayNumber, 46), getArrayItem(position, arrayNumber, 93));	//cout << "eqS = " << eqS << endl;
	string eqR = equationConstructor(getArrayItem(position, arrayNumber, 90), getArrayItem(position, arrayNumber, 45), getArrayItem(position, arrayNumber, 91));	//cout << "eqR = " << eqR << endl;
	string eqQ = equationConstructor(getArrayItem(position, arrayNumber, 88), getArrayItem(position, arrayNumber, 44), getArrayItem(position, arrayNumber, 89));	//cout << "eqQ = " << eqQ << endl;
	string eqP = equationConstructor(getArrayItem(position, arrayNumber, 86), getArrayItem(position, arrayNumber, 43), getArrayItem(position, arrayNumber, 87));	//cout << "eqP = " << eqP << endl;
	string eqO = equationConstructor(getArrayItem(position, arrayNumber, 84), getArrayItem(position, arrayNumber, 42), getArrayItem(position, arrayNumber, 85));	//cout << "eqO = " << eqO << endl;
	string eqN = equationConstructor(getArrayItem(position, arrayNumber, 82), getArrayItem(position, arrayNumber, 41), getArrayItem(position, arrayNumber, 83));	//cout << "eqN = " << eqN << endl;
	string eqM = equationConstructor(getArrayItem(position, arrayNumber, 80), getArrayItem(position, arrayNumber, 40), getArrayItem(position, arrayNumber, 81));	//cout << "eqM = " << eqM << endl;
	string eqL = equationConstructor(getArrayItem(position, arrayNumber, 78), getArrayItem(position, arrayNumber, 39), getArrayItem(position, arrayNumber, 79));	//cout << "eqL = " << eqL << endl;
	string eqK = equationConstructor(getArrayItem(position, arrayNumber, 76), getArrayItem(position, arrayNumber, 38), getArrayItem(position, arrayNumber, 77));	//cout << "eqK = " << eqK << endl;
	string eqJ = equationConstructor(getArrayItem(position, arrayNumber, 74), getArrayItem(position, arrayNumber, 37), getArrayItem(position, arrayNumber, 75));	//cout << "eqJ = " << eqJ << endl;
	string eqI = equationConstructor(getArrayItem(position, arrayNumber, 72), getArrayItem(position, arrayNumber, 36), getArrayItem(position, arrayNumber, 73));	//cout << "eqI = " << eqI << endl;
	string eqH = equationConstructor(getArrayItem(position, arrayNumber, 70), getArrayItem(position, arrayNumber, 35), getArrayItem(position, arrayNumber, 71));	//cout << "eqH = " << eqH << endl;
	string eqG = equationConstructor(getArrayItem(position, arrayNumber, 68), getArrayItem(position, arrayNumber, 34), getArrayItem(position, arrayNumber, 69));	//cout << "eqG = " << eqG << endl;
	string eqF = equationConstructor(getArrayItem(position, arrayNumber, 66), getArrayItem(position, arrayNumber, 33), getArrayItem(position, arrayNumber, 67));	//cout << "eqF = " << eqF << endl;
	string eqE = equationConstructor(getArrayItem(position, arrayNumber, 64), getArrayItem(position, arrayNumber, 32), getArrayItem(position, arrayNumber, 65));	//cout << "eqE = " << eqE << endl;
	
	string eqD = equationConstructor(eqiota, getArrayItem(position, arrayNumber, 31), eqkappa); //cout << "\neqD = " << eqD << endl;
	string eqC = equationConstructor(eqeta, getArrayItem(position, arrayNumber, 30), eqtheta); //cout << "eqC = " << eqC << endl;
	string eqB = equationConstructor(eqepsilon, getArrayItem(position, arrayNumber, 29), eqzeta); //cout << "eqB = " << eqB << endl;
	string eqA = equationConstructor(eqgamma, getArrayItem(position, arrayNumber, 28), eqdelta); //cout << "eqA = " << eqA << endl;
	string eqz = equationConstructor(eqalpha, getArrayItem(position, arrayNumber, 27), eqbeta); //cout << "eqz = " << eqz << endl;
	string eqy = equationConstructor(eqY, getArrayItem(position, arrayNumber, 26), eqZ); //cout << "eqy = " << eqy << endl;
	string eqx = equationConstructor(eqW, getArrayItem(position, arrayNumber, 25), eqX); //cout << "eqx = " << eqx << endl;
	string eqw = equationConstructor(eqU, getArrayItem(position, arrayNumber, 24), eqV); //cout << "eqw = " << eqw << endl;
	string eqv = equationConstructor(eqS, getArrayItem(position, arrayNumber, 23), eqT); //cout << "eqv = " << eqv << endl;
	string equ = equationConstructor(eqQ, getArrayItem(position, arrayNumber, 22), eqR); //cout << "equ = " << equ << endl;
	string eqt = equationConstructor(eqO, getArrayItem(position, arrayNumber, 21), eqP); //cout << "eqt = " << eqt << endl;
	string eqs = equationConstructor(eqM, getArrayItem(position, arrayNumber, 20), eqN); //cout << "eqs = " << eqs << endl;
	string eqr = equationConstructor(eqK, getArrayItem(position, arrayNumber, 19), eqL); //cout << "eqr = " << eqr << endl;
	string eqq = equationConstructor(eqI, getArrayItem(position, arrayNumber, 18), eqJ); //cout << "eqq = " << eqq << endl;
	string eqp = equationConstructor(eqG, getArrayItem(position, arrayNumber, 17), eqH); //cout << "eqp = " << eqp << endl;
	string eqo = equationConstructor(eqE, getArrayItem(position, arrayNumber, 16), eqF); //cout << "eqo = " << eqo << endl;

	string eqn = equationConstructor(eqC, getArrayItem(position, arrayNumber, 15), eqD); //cout << "\neqn = " << eqn << endl;
	string eqm = equationConstructor(eqA, getArrayItem(position, arrayNumber, 14), eqB); //cout << "eqm = " << eqm << endl;
	string eql = equationConstructor(eqy, getArrayItem(position, arrayNumber, 13), eqz); //cout << "eql = " << eql << endl;
	string eqk = equationConstructor(eqw, getArrayItem(position, arrayNumber, 12), eqx); //cout << "eqk = " << eqk << endl;
	string eqj = equationConstructor(equ, getArrayItem(position, arrayNumber, 11), eqv); //cout << "eqj = " << eqj << endl;
	string eqi = equationConstructor(eqs, getArrayItem(position, arrayNumber, 10), eqt); //cout << "eqi = " << eqi << endl;
	string eqh = equationConstructor(eqq, getArrayItem(position, arrayNumber, 9), eqr); //cout << "eqh = " << eqh << endl;
	string eqg = equationConstructor(eqo, getArrayItem(position, arrayNumber, 8), eqp); //cout << "eqg = " << eqg << endl;
	
	string eqf = equationConstructor(eqm, getArrayItem(position, arrayNumber, 7), eqn); //cout << "\neqf = " << eqD << endl;
	string eqe = equationConstructor(eqk, getArrayItem(position, arrayNumber, 6), eql); //cout << "eqe = " << eqD << endl;
	string eqd = equationConstructor(eqi, getArrayItem(position, arrayNumber, 5), eqj); //cout << "eqd = " << eqD << endl;
	string eqc = equationConstructor(eqg, getArrayItem(position, arrayNumber, 4), eqh); //cout << "eqc = " << eqD << endl;

	string eqb = equationConstructor(eqe, getArrayItem(position, arrayNumber, 3), eqf); //cout << "\neqb = " << eqD << endl;
	string eqa = equationConstructor(eqc, getArrayItem(position, arrayNumber, 2), eqd); //cout << "eqa = " << eqD << endl;

	string eqfinal = equationConstructor(eqa, getArrayItem(position, arrayNumber, 1), eqb); //cout << "\neqfinal = " << eqD << endl;
	return eqfinal;
}
*/

//Define a function to write the initial model to an individual. 
void Generation::applyInitialModel(int position){
	int i;	
	
	string initialRoleLoadModel[63] = {
	"+", "+", "+", "*", "*", "*", "*", "*", "*", "*",
	"*", "*", "*", "*", "*", "1", "mRoleLoadBeta1", "*", "*", "1",
	"mRoleLoadBeta2", "*", "*", "1", "mRoleLoadBeta3", "*", "*", "*", "(1-x)", "*", 
	"*", "_", "_", "_", "_", "(double)mMarritalStatus", "1", "mLevelofInvolvementMarriage", "1", "_",
	"_", "_", "_", "(double)mParenthoodStatus", "1", "mLevelofInvolvementParenthood", "1", "_", "_", "_",
	"_", "(double)mEmploymentStatus", "1", "mLevelofInvolvementEmployment", "1", "mRoleLoadBeta4", "1", "(double)mMarritalStatus", "1", "(double)mParenthoodStatus",
	"1", "mLevelofInvolvementParenthood", "1"};
	/*string initialRoleLoadModel[63] = {
	"+", "+", "+", "*", "*", "*", "*", "mRoleLoadBeta1", "*", "mRoleLoadBeta2",
	"*", "mRoleLoadBeta3", "*", "*", "*", "_", "_", "(double)mMarritalStatus", "mLevelofInvolvementMarriage", "_",
	"_", "(double)mParenthoodStatus", "mLevelofInvolvementParenthood", "_", "_", "(double)mEmploymentStatus", "mLevelofInvolvementEmployment", "mRoleLoadBeta4", "(1-x)", "(double)mParenthoodStatus", 
	"mLevelofInvolvementParenthood", "_", "_", "_", "_", "_", "_", "_", "_", "_", 
	"_", "_", "_", "_", "_", "_", "_","_", "_", "_",
	"_", "_", "_", "_", "_", "_", "_", "(double)mMarritalStatus", "1", "_",
	"_", "_", "_"};*/
	
	string initialRoleIncongruenceModel[63] = {
	"/", "+", "*", "abs", "+", "1", "MAX_INCONGRUENCE", "-","1", "abs",
	"abs", "_", "_", "_", "_", "(double)mMarritalStatus", "mpRolesEntity->getRoleExpectancyMarriage(sex,findAgeGroup())", "_", "_", "-",
	"1", "-", "1", "_", "_", "_", "_","_", "_", "_",
	"_", "_", "_", "_", "_", "_", "_","_", "_", "(double)mParenthoodStatus",
	"mpRolesEntity->getRoleExpectancyParenthood(sex,findAgeGroup())", "_", "_", "(double)mEmploymentStatus", "mpRolesEntity->getRoleExpectancyEmployment(sex,findAgeGroup())", "_", "1","_", "_", "_",
	"_", "_", "_", "_", "_", "_", "_","_", "_", "_",
	"_", "_", "_"};

	string initialRoleStrainModel[63] = {
	"/", "*", "*", "(1-x)", "+", "1", "2", "*", "1", "*",
	"*", "_", "_", "_", "_", "1", "mRoleSkill", "_", "_", "1",
	"mRoleLoad", "1", "mRoleIncongruence", "_", "_", "_", "_", "_", "_", "_",
	"_", "_", "_", "_", "_", "_", "_", "_", "_", "_",
	"_", "_", "_", "_", "_", "_", "_", "_", "_", "_",
	"_", "_", "_", "_", "_", "_", "_", "_", "_", "_",
	"_", "_", "_"};
	/*string initialRoleStrainModel[63] = {
	"/", "*", "2", "(1-x)", "+", "_", "_", "mRoleSkill", "1", "mRoleLoad",
	"mRoleIncongruence", "_", "_", "_", "_", "_", "_", "_", "_", "_",
	"_", "_", "_", "_", "_", "_", "_", "_", "_", "_",
	"_", "_", "_", "_", "_", "_", "_", "_", "_", "_",
	"_", "_", "_", "_", "_", "_", "_", "_", "_", "_",
	"_", "_", "_", "_", "_", "_", "_", "_", "_", "_",
	"_", "_", "_"};*/
	
	string initialFirstDrinkDispositionModel[63] = {
	"*", "*", "+", "+", "*", "1", "*", "*", "*", "mDispositionVector[0]",
	"1", "_", "_", "*", "*", "mProbOppOut", "1", "mProbOppIn", "1", "_",
	"_", "_", "_", "_", "_", "_", "_", "mRoleStrainAffectingGatewayDispositionBeta", "1", "*",
	"*", "_", "_", "_", "_", "_", "_", "_", "_", "_",
	"_", "_", "_", "_", "_", "_", "_", "_", "_", "_",
	"_", "_", "_", "_", "_", "_", "_", "_", "_", "mTendencyToDrinkInResponseToStrain",
	"1", "mRoleStrain", "1"};
	/*string initialFirstDrinkDispositionModel[63] = {
	"*", "*", "+", "+", "mDispositionVector[0]", "1", "*", "mProbOppOut", "mProbOppIn", "_",
	"_", "_", "_", "mRoleStrainAffectingGatewayDispositionBeta", "*", "_", "_", "_", "_", "_",
	"_", "_", "_", "_", "_", "_", "_", "_", "_", "mTendencyToDrinkInResponseToStrain",
	"mRoleStrain", "_", "_", "_", "_", "_", "_", "_", "_", "_",
	"_", "_", "_", "_", "_", "_", "_", "_", "_", "_",
	"_", "_", "_", "_", "_", "_", "_", "_", "_", "_",
	"_", "_", "_"};*/

	string initialNextDrinkDispositionModel[63] = {
	"*", "*", "+", "+", "*", "1", "*", "*", "*", "mDispositionVector[i]",
	"1", "_", "_", "*", "*", "mProbOppIn", "1", "mProbOppOut", "1", "_",
	"_", "_", "_", "_", "_", "_", "_", "mRoleStrainAffectingNextDrinkDispositionBeta", "1", "*",
	"*", "_", "_", "_", "_", "_", "_", "_", "_", "_",
	"_", "_", "_", "_", "_", "_", "_", "_", "_", "_",
	"_", "_", "_", "_", "_", "_", "_", "_", "_", "mTendencyToDrinkInResponseToStrain",
	"1", "mRoleStrain", "1"};
	/*string initialNextDrinkDispositionModel[63] = {
	"*", "*", "+", "+", "mDispositionVector[i]", "1", "*", "mProbOppIn", "mProbOppOut", "_",
	"_", "_", "_", "mRoleStrainAffectingNextDispositionBeta", "*", "_", "_", "_", "_", "_",
	"_", "_", "_", "_", "_", "_", "_", "_", "_", "mTendencyToDrinkInResponseToStrain",
	"mRoleStrain", "_", "_", "_", "_", "_", "_", "_", "_", "_",
	"_", "_", "_", "_", "_", "_", "_", "_", "_", "_",
	"_", "_", "_", "_", "_", "_", "_", "_", "_", "_",
	"_", "_", "_"};	*/

	for(i=1; i<=63; i++){
		population[position].setRoleLoadItem(i, initialRoleLoadModel[i-1]);
		population[position].setRoleIncongruenceItem(i, initialRoleIncongruenceModel[i-1]);
		population[position].setRoleStrainItem(i, initialRoleStrainModel[i-1]);
		population[position].setFirstDrinkDispositionItem(i, initialFirstDrinkDispositionModel[i-1]);
		population[position].setNextDrinkDispositionItem(i, initialNextDrinkDispositionModel[i-1]);
	}
	population[position].calculateComplexity();
}

//Define a function to return code for the RoleLoad equation from an individual.
string Generation::getRoleLoadCode(int position){
	string eqfinal = getCode63Primitives(position, 1);
	return ("mRoleLoad = " + eqfinal + ";");
}

//Define a function to return code for the RoleIncongruence equation from an individual.
string Generation::getRoleIncongruenceCode(int position){
	string eqfinal = getCode63Primitives(position, 2);
	return ("mRoleIncongruence = " + eqfinal + ";");
}

//Define a function to return code for the RoleStrain equation from an individual. 
string Generation::getRoleStrainCode(int position){
	string eqfinal = getCode63Primitives(position, 3);
	return ("mRoleStrain = " + eqfinal + ";");
}

//Define a function to return code for the FirstDrinkDisposition equation from an individual. 
string Generation::getFirstDrinkDispositionCode(int position){
	string eqfinal = getCode63Primitives(position, 4);
	return ("probFirstDrink = " + eqfinal + ";");
}

//Define a function to return code for the NextDrinkDisposition equation from an individual. 
string Generation::getNextDrinkDispositionCode(int position){
	string eqfinal = getCode63Primitives(position, 5);
	return ("mpAgent->setDispositionByIndex(i, " + eqfinal + ");");
}

//Define a function to write the contents of a generation to a .txt file. 
void Generation::printGenerationToFile(string filename){
	int i, j;
	ofstream outputFile;
	ifstream inputFile;
	double averageComplexity = 0, averageFrontFitness = 0;
	int compiled=0, highestFront=0;
	outputFile.open((filename + ".txt"));
	outputFile << "Average fitness: " << calculateAverageFitness() << ", number of individuals that compiled: " << individualsThatCompiled() << endl;
	for(i=0; i<POPULATION_SIZE; i++){
		population[i].calculateComplexity();
		outputFile << "\n\nIndividual " << i+1;
		outputFile << ", Fitness: ";
		outputFile << to_string(population[i].getFitness());
		outputFile << ", Complexity: " << population[i].getComplexity();
		outputFile << ", Pareto Front: " << population[i].getFront();
		outputFile << ", Crowding Rank: " << population[i].getCrowdingRank() << endl;
		//cout << "Individual " << i+1;
		//cout << ", Fitness: " << to_string(population[i].getFitness());
		//cout << ", \tComplexity: " << population[i].getComplexity() << endl;
		outputFile << "\t" << getRoleLoadCode(i) << endl;
		outputFile << "\t" << getRoleIncongruenceCode(i) << endl;
		outputFile << "\t" << getRoleStrainCode(i) << endl;
		outputFile << "\t" << getFirstDrinkDispositionCode(i) << endl;
		outputFile << "\t" << getNextDrinkDispositionCode(i) << endl;
		outputFile << "\tRoleLoad tree: ";
		for(j=1; j<=63; j++){
			outputFile << population[i].getRoleLoadItem(j) << ", ";
		}
		outputFile << "\n\tRoleIncongruence tree: ";
		for(j=1; j<=63; j++){
			outputFile << population[i].getRoleIncongruenceItem(j) << ", ";
		}
		outputFile << "\n\tRoleStrain tree: ";
		for(j=1; j<=63; j++){
			outputFile << population[i].getRoleStrainItem(j) << ", ";
		}
		outputFile << "\n\tFirstDrinkDisposition tree: ";
		for(j=1; j<=63; j++){
			outputFile << population[i].getFirstDrinkDispositionItem(j) << ", ";
		}
		outputFile << "\n\tNextDrinkDisposition tree: ";
		for(j=1; j<=63; j++){
			outputFile << population[i].getNextDrinkDispositionItem(j) << ", ";
		}
	}
	cout << "created file " << filename << ".txt" << endl;
	outputFile.close();
	
	//csv to contain id, fitness, complexity, Pareto front, and crowdingRank.
	outputFile.open((filename + "_table.csv"));
	outputFile << "Individual, Fitness, Complexity, Pareto Front, Crowding Rank" << endl; //, Average fitness:," << calculateAverageFitness() << ",number of individuals that compiled:," << individualsThatCompiled() << endl;
	for(i=0; i<POPULATION_SIZE; i++){
		outputFile << i+1 << "," << to_string(population[i].getFitness()) << "," << population[i].getComplexity() << "," << population[i].getFront() << "," << population[i].getCrowdingRank() << endl;
	}
	cout << "created file " << filename << "_table.csv" << endl;
	outputFile.close();
	
	//csv for front 1 only.
	outputFile.open((filename + "_front_1.csv"));
	outputFile << "Individual, Fitness, Complexity, Crowding Rank" << endl; //, Average fitness:," << calculateAverageFitness() << ",number of individuals that compiled:," << individualsThatCompiled() << endl;
	for(i=0; i<POPULATION_SIZE; i++){
		if(population[i].getFront() == 1){
			outputFile << i+1 << "," << to_string(population[i].getFitness()) << "," << population[i].getComplexity() << "," << population[i].getCrowdingRank() << endl;
		}
	}
	cout << "created file " << filename << "_front_1.csv" << endl;
	outputFile.close();
	
	//csv for summary of entire run.
	inputFile.open(("./generations/summary_of_run.csv"));	//check if the file exists by opening it as an input
	if(!inputFile.is_open()){ //if the file does not exist:
		inputFile.close();
		outputFile.open(("./generations/summary_of_run.csv"));
		outputFile << "Generation, Pareto Front, Average Fitness, Average Complexity" << endl;
		outputFile.close();
		
	}else{	//if the file does exist:
		inputFile.close();
	}
	outputFile.open(("./generations/summary_of_run.csv"), ios_base::app);
	//calculate the generation's average complexity and write this and the average fitness to the file. 
	for(i=0; i<POPULATION_SIZE; i++){
		if((!isnan(population[i].getFitness())) && (population[i].getFitness() < 999999)){
			averageComplexity += population[i].getComplexity();
			compiled++;
		}
	}
	averageComplexity = averageComplexity / compiled;
	outputFile << filename << ",all," << calculateAverageFitness() << "," << averageComplexity << endl;
	//cout << filename << ",all," << calculateAverageFitness() << "," << averageComplexity << endl;
	//find the highest front in the generation. 
	for(i=0; i<POPULATION_SIZE; i++){
		if(population[i].getFront() > highestFront){
			highestFront = population[i].getFront();
		}
	}
	//print the average fitness and complexity for each front to the file.
	for(j=1; j<=highestFront; j++){
		averageComplexity = 0; averageFrontFitness = 0; compiled = 0; 
		for(i=0; i<POPULATION_SIZE; i++){
			if(population[i].getFront() == j){
				if((!isnan(population[i].getFitness())) && (population[i].getFitness() < 999999)){
					averageFrontFitness += population[i].getFitness();
					averageComplexity += population[i].getComplexity();
					compiled++;
				}
			}
		}
		averageFrontFitness = averageFrontFitness/compiled;
		averageComplexity = averageComplexity/compiled;
		outputFile << filename << "," << j << "," << averageFrontFitness << "," << averageComplexity << endl;
	//cout << filename << "," << j << "," << averageFrontFitness << "," << averageComplexity << endl;
	}
	outputFile.close();	
	cout << "appended file " << "./generations/summary_of_run.csv" << endl;
}

double Generation::calculateAverageFitness(){
	int i, p=0;
	double averageFitness = 0;
	for(i=0; i<POPULATION_SIZE; i++){
		if((!isnan(population[i].getFitness())) && (population[i].getFitness() < 9999999)){
			averageFitness += population[i].getFitness();
			p++;
		}
	}
	if(p!=0){
		return (averageFitness/p);
	}else{
		return 999999999;
	}
}

int Generation::individualsThatCompiled(){
	int i = 0;
	for(i=0; i<POPULATION_SIZE; i++){
		if((isnan(population[i].getFitness())) || (population[i].getFitness() < 9999999)){
			i++;
		}
	}
	return i;
}


