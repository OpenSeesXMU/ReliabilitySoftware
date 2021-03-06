/* ****************************************************************** **
**    OpenSees - Open System for Earthquake Engineering Simulation    **
**          Pacific Earthquake Engineering Research Center            **
**                                                                    **
**                                                                    **
** (C) Copyright 2001, The Regents of the University of California    **
** All Rights Reserved.                                               **
**                                                                    **
** Commercial use of this program without express permission of the   **
** University of California, Berkeley, is strictly prohibited.  See   **
** file 'COPYRIGHT'  in main directory for information on usage and   **
** redistribution,  and for a DISCLAIMER OF ALL WARRANTIES.           **
**                                                                    **
** Developed by:                                                      **
**   Frank McKenna (fmckenna@ce.berkeley.edu)                         **
**   Gregory L. Fenves (fenves@ce.berkeley.edu)                       **
**   Filip C. Filippou (filippou@ce.berkeley.edu)                     **
**                                                                    **
** Reliability module developed by:                                   **
**   Terje Haukaas (haukaas@ce.berkeley.edu)                          **
**   Armen Der Kiureghian (adk@ce.berkeley.edu)                       **
**                                                                    **
** ****************************************************************** */
                                                                        
// $Revision: 1.6 $
// $Date: 2003/10/27 23:45:41 $
// $Source: /usr/local/cvs/OpenSees/SRC/reliability/analysis/analysis/SORMAnalysis.cpp,v $


//
// Written by Terje Haukaas (haukaas@ce.berkeley.edu)
//

#include "SORMAnalysis.h"
#include "ReliabilityDomain.h"
#include "ReliabilityAnalysis.h"
#include "FindCurvatures.h"
#include "LimitStateFunction.h"
#include "NormalRV.h"
#include <math.h>
#include "Vector.h"

#include <fstream>
#include <iomanip>
#include <iostream>
using std::ifstream;
using std::ios;
using std::setw;
using std::setprecision;
using std::setiosflags;
using namespace std;

SORMAnalysis::SORMAnalysis(	ReliabilityDomain *passedReliabilityDomain,
							FindCurvatures *passedCurvaturesAlgorithm,
						    TCL_Char *passedFileName)
:ReliabilityAnalysis()
{
	theReliabilityDomain = passedReliabilityDomain;
	theCurvaturesAlgorithm = passedCurvaturesAlgorithm;
	fileName = new char[256];
	strcpy(fileName,passedFileName);
}


SORMAnalysis::~SORMAnalysis()
{
	if (fileName != 0)
		delete [] fileName;
}



int 
SORMAnalysis::analyze(void)
{
	// Alert the user that the SORM analysis has started
	cout << "SORM Analysis is running ... " << endl;


	// Declare variables used in this method
	Vector curvatures;
	int numberOfCurvatures;
	double beta;
	double pf1;
	double psi_beta;
	double product;
	int i;
	double pf2Breitung;
	double betaBreitung;
	LimitStateFunction *theLimitStateFunction;
	NormalRV *aStdNormRV = 0;
	aStdNormRV = new NormalRV(1,0.0,1.0,0.0);


	// Check if computer ran out of memory
	if (aStdNormRV==0) {
		cout << "SORMAnalysis::analyze() - out of memory while instantiating internal objects." << endl;
		return -1;
	}


	// Number of limit-state functions
	int numLsf = theReliabilityDomain->getNumberOfLimitStateFunctions();


	// Open output file
	ofstream outputFile( fileName, ios::out );


	// Loop over number of limit-state functions
	for (int lsf=1; lsf<=numLsf; lsf++ ) {


		// Inform the user which limit-state function is being evaluated
		cout << "Limit-state function number: " << lsf << endl;


		// Set tag of "active" limit-state function
		theReliabilityDomain->setTagOfActiveLimitStateFunction(lsf);


		// Get the limit-state function pointer
		theLimitStateFunction = 0;
		lsf = theReliabilityDomain->getTagOfActiveLimitStateFunction();
		theLimitStateFunction = theReliabilityDomain->getLimitStateFunctionPtr(lsf);
		if (theLimitStateFunction == 0) {
			cout << "SORMAnalysis::analyze() - could not find" << endl
				<< " limit-state function with tag #" << lsf << "." << endl;
			return -1;
		}


		// Compute curvature(s)
		if (theCurvaturesAlgorithm->computeCurvatures(theReliabilityDomain) < 0){
			cout << "SORMAnalysis::analyze() - failed while finding " << endl
				<< " curvatures for limit-state function number " << lsf << "." << endl;
			return -1;
		}


		// Get results
		curvatures = theCurvaturesAlgorithm->getCurvatures();
		numberOfCurvatures = curvatures.Size();
			

		// Get FORM results from the limit-state function
		beta = theLimitStateFunction->FORMReliabilityIndexBeta;
		pf1 = theLimitStateFunction->FORMProbabilityOfFailure_pf1;


		// Compute failure probability by "Breitung"
		double denominator = aStdNormRV->getCDFvalue(-beta);
		if (denominator == 0.0) {
			cout << "SORMAnalysis::analyze() - denominator zero " << endl
				<< " due to too large reliability index value." << endl;
			return -1;
		}
		psi_beta = aStdNormRV->getPDFvalue(beta)/denominator;
		product = 1.0;
		for (i=0; i<numberOfCurvatures; i++ ) {
			product = product / sqrt(1.0+psi_beta*curvatures(i));
		}
		pf2Breitung = pf1 * product;


		// Compute corresponding beta's
		betaBreitung = -aStdNormRV->getInverseCDFvalue(pf2Breitung);


		// Put results into reliability domain
		theLimitStateFunction->numberOfCurvatauresUsed = numberOfCurvatures;
		theLimitStateFunction->SORMUsingSearchPf2Breitung = pf2Breitung;
		theLimitStateFunction->SORMUsingSearchBetaBreitung = betaBreitung;


		// Print SORM results to the output file
		outputFile << "#######################################################################" << endl;
		outputFile << "#  SORM ANALYSIS RESULTS, LIMIT-STATE FUNCTION NUMBER "
			<<setiosflags(ios::left)<<setprecision(1)<<setw(4)<<lsf <<"            #" << endl;
		outputFile << "#  (Curvatures found from search algorithm.)                          #" << endl;
		outputFile << "#                                                                     #" << endl;
		outputFile << "#  Number of principal curvatures used: ............... " 
			<<setiosflags(ios::left)<<setprecision(5)<<setw(12)<<numberOfCurvatures
			<< "  #" << endl;
		outputFile << "#  Reliability index beta (impr. Breitung's formula):.. " 
			<<setiosflags(ios::left)<<setprecision(5)<<setw(12)<<betaBreitung 
			<< "  #" << endl;
		outputFile << "#  Corresponding estimated probability of failure pf2:.." 
			<<setiosflags(ios::left)<<setprecision(5)<<setw(12)<<pf2Breitung 
			<< "  #" << endl;
		outputFile << "#                                                                     #" << endl;
		outputFile << "#######################################################################" << endl << endl << endl;
	}


	// Inform user on screen
	cout << "SORM analysis completed. " << endl;

	// Clean up
	outputFile.close();
	delete aStdNormRV;

	return 0;
}

