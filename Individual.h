/* Individual.h */

#ifndef INDIVIDUAL_H
#define INDIVIDUAL_H

#include <unistd.h>	/* chdir */
#include <iostream>	/* cout, cin */
#include <string>	/* string */

using namespace std;

class Individual{
   private:
	//EQUATION:mRoleLoad	
	string role_load[63];
	//mRoleLoad = mRoleLoadBeta1 * mMarritalStatus * mLevelofInvolvementMarriage + mRoleLoadBeta2 * mParenthoodStatus * mLevelofInvolvementParenthood + mRoleLoadBeta3 * mEmploymentStatus * mLevelofInvolvementEmployment + mRoleLoadBeta4 * (1 - mMarritalStatus) * mParenthoodStatus * mLevelofInvolvementParenthood;

  
	//EQUATION:mRoleIncongruence	
	string role_incongruence[63];
	//if (age <= MAX_AGE) { sum += fabs(static_cast<double>(mMarritalStatus) - mpRolesEntity->getRoleExpectancyMarriage(sex,findAgeGroup())); sum += fabs(static_cast<double>(mParenthoodStatus) - mpRolesEntity->getRoleExpectancyParenthood(sex,findAgeGroup())); sum += fabs(static_cast<double>(mEmploymentStatus) - mpRolesEntity->getRoleExpectancyEmployment(sex,findAgeGroup())); } mRoleIncongruence = sum/MAX_INCONGRUENCE; 
 
	/*
	//EQUATION:mOpportunity
	string opportunity[63];
	//logOddsOppOut = mBaselineOpportunity * (1 - mRoleDrinkingOpportunityBeta1 * mRoleLoad + mRoleDrinkingOpportunityBeta2 * mEmploymentStatus); logOddsOppIn = mBaselineOpportunity * (1 - mRoleDrinkingOpportunityBeta3 * mRoleLoad + mRoleDrinkingOpportunityBeta4 * (mMarritalStatus + mParenthoodStatus)); mProbOppIn = exp(logOddsOppIn) /(exp(logOddsOppIn) + exp(logOddsOppOut) + exp(mBaselineOpportunity)); mProbOppOut = exp(logOddsOppOut) /(exp(logOddsOppIn) + exp(logOddsOppOut) + exp(mBaselineOpportunity));
	 */

	//EQUATION:mRoleStrain	
	string role_strain[63];	 
	//mRoleStrain) = (1 - mRoleSkill * (mRoleLoad + mRoleIncongruence) / 2;

	/*
	//EQUATION:mDispositionVector
	string lifetime_disposition[63];
	//dayBar = (mDaysOfSocialisation - 365 / 2) / (365 / mBetaSocialisationSpeed); modifier = exp(dayBar)/(1+exp(dayBar)); dispositionDiff = mDispositionVectorOld[i] * (1 + mRolesSocialisationBetaMoreRoles) - mDispositionVectorOld[i];  mDispositionVector[i] = mDispositionVectorOld[i] + dispositionDiff * modifier; OR dispositionDiff = mDispositionVectorOld[i] * (1 + mRolesSocialisationBetaLessRoles) - mDispositionVectorOld[i]; mDispositionVector[i] = mDispositionVectorOld[i] + dispositionDiff * modifier;
	*/


	//EQUATION:probFirstDrink	
	string first_drink_disposition[63];	 
	//probFirstDrink = (mProbOppOut + mProbOppIn) * mDispositionVector[0] * (1 + mRoleStrainAffectingGatewayDispositionBeta * mTendencyToDrinkInResponseToStrain * mRoleStrain);  

	//EQUATION:probNextDrink	
	string next_drink_disposition[63];	 
	//modifierByRoleStrain = (mProbOppIn + mProbOppOut) * (1 + mRoleStrainAffectingNextDrinkDispositionBeta * mTendencyToDrinkInResponseToStrain * mRoleStrain); mpAgent->setDispositionByIndex(i, mDispositionVector[i] * modifierByRoleStrain);


	int complexity;	//number of slots in the arrays that are not empty.
	double fitness;	//result of fitness function evaluation.
	int np;	//the number of individuals that dominate this individual.
	int front;	//the Pareto front number of this individual. 
	int rank;	//the numeric ranking of this individual in the current generation based on Pareto front calculations. 
	double crowdingRank;  //a measure of how close to other solutions this individual is in the 2D space of fitness x complexity. 

   public:
	Individual();	//constructor (cannot be parameterised as arrays cannot be passed without being a pointer).
	
	string getRoleLoadItem(int item);
	string getRoleIncongruenceItem(int item);
	string getRoleStrainItem(int item);
	string getFirstDrinkDispositionItem(int item);
	string getNextDrinkDispositionItem(int item);
	int getComplexity();
	double getFitness();
	int getNP();
	int getFront();
	int getRank();
	double getCrowdingRank();

	void setRoleLoadItem(int item, string value);
	void setRoleIncongruenceItem(int item, string value);
	void setRoleStrainItem(int item, string value); 
	void setFirstDrinkDispositionItem(int item, string value);
	void setNextDrinkDispositionItem(int item, string value);
	void calculateComplexity();
	void setComplexity(int complexityIn); //used only for testing.
	void setFitness(double fitnessIn);
	void setNP(int NPIn);
	void setFront(int frontIn);
	void setRank(int rankIn); 
	void setCrowdingRank(double crowdingRankIn);
	
	int getDepth(int tree);
};	

#endif
