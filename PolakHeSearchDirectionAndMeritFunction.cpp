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
                                                                        
// $Revision: 1.2 $
// $Date: 2003/10/27 23:45:42 $
// $Source: /usr/local/cvs/OpenSees/SRC/reliability/analysis/direction/PolakHeSearchDirectionAndMeritFunction.cpp,v $


//
// Written by Terje Haukaas (haukaas@ce.berkeley.edu) 
//

#include "PolakHeSearchDirectionAndMeritFunction.h"
#include "SearchDirection.h"
#include "MeritFunctionCheck.h"
#include "Vector.h"
#include <iostream>
using namespace std;


PolakHeSearchDirectionAndMeritFunction::PolakHeSearchDirectionAndMeritFunction(double pgamma, double pdelta)
:SearchDirection(), MeritFunctionCheck()
{
	gamma = pgamma;
	delta = pdelta;
	alpha = 0.0;
}

PolakHeSearchDirectionAndMeritFunction::~PolakHeSearchDirectionAndMeritFunction()
{
}




Vector
PolakHeSearchDirectionAndMeritFunction::getSearchDirection()
{
	return searchDirection;
}


int
PolakHeSearchDirectionAndMeritFunction::setAlpha(double palpha)
{
	alpha = palpha;
	
	return 0;
}



int
PolakHeSearchDirectionAndMeritFunction::computeSearchDirection(
							int stepNumber, 
							Vector u, 
							double gFunctionValue, 
							Vector gradientInStandardNormalSpace)
{
	// Advise the user if the start value of the limit-state function 
	// is out of the 'ideal range' for the Polak-He algorithm
	if (stepNumber == 1) {
		if (gFunctionValue > 15.0 || gFunctionValue < 2.0) {
			cout << "WARNING: The start value of the limit-state function is outside " << endl
				<< " the ideal range for fastest convergence of the Polak-He algorithm. " << endl;
		}
	}

	// Compute elements of the A matrix
	double oneOverDelta = 1.0/delta;
	double a11 = oneOverDelta * (u ^ u);
	double a22 = oneOverDelta * (gradientInStandardNormalSpace ^ gradientInStandardNormalSpace);
	double a12 = oneOverDelta * (u ^ gradientInStandardNormalSpace); 


	// Compute the elements of the b vector:
	double b1;
	if (gFunctionValue <= 0.0) {
		b1 = 0.0;
	}
	else {
		b1 = gFunctionValue;
	}
	double b2 = b1 - gFunctionValue;
	b1 = gamma*b1;


	// Compute factors of second order equation
	double a = 0.5*a11 + 0.5*a22 - a12;
	double b = b1 - b2 + a12 - a22;
	double c = 0.5*a22 + b2;


	// Find coordinates of minimum point [z1 z2]
	double z1, z2;
	if (a<0.0) {
		cout << "ERROR: PolakHeSearchDirectionAndMeritFunction::computeSearchDirection() " << endl
			<< " the quadratic term is negative! " << endl;
		return -1;
	}
	else if (a < 1.0e-9) {

		// Here the quadratic term is zero, so we use one of the endpoints
		double Fend1 = c;     // x=0.0
		double Fend2 = a+b+c; // x=1.0

		if (Fend1 < Fend2) {
			z1 = 0.0;
			z2 = 1.0;
			thetaFunction = -Fend1;
		}
		else {
			z1 = 1.0;
			z2 = 0.0;
			thetaFunction = -Fend2;
		}
	}
	else {

		// The 'normal' case
		double x = -b/(2.0*a);
		z1 = x;
		z2 = 1.0 - x;
		thetaFunction = -(a*x*x + b*x + c);



		// Check if the minimum point is outside the 'triangle'
		if (z1 < 0.0 || z1 > 1.0 || z2 < 0.0 || z2 > 1.0 ) {

			// Use one of the endpoints
			double Fend1 = c;     // x=0.0
			double Fend2 = a+b+c; // x=1.0

			if (Fend1 < Fend2) {
				z1 = 0.0;
				z2 = 1.0;
				thetaFunction = -Fend1;
			}
			else {
				z1 = 1.0;
				z2 = 0.0;
				thetaFunction = -Fend2;
			}
		}
	}


	// With the coordinates of the minimum point the direction now reads
	searchDirection = -z1*u - z2*gradientInStandardNormalSpace;
	
	return 0;
}




double 
PolakHeSearchDirectionAndMeritFunction::getMeritFunctionValue(Vector u, double g, Vector grad_G)
{
	cout << "WARNING: PolakHeSearchDirectionAndMeritFunction::getMeritFunctionValue() --" << endl
		<< " no explicit merit function value is computed." << endl;
	return 0.0;
}



int 
PolakHeSearchDirectionAndMeritFunction::updateMeritParameters(Vector u, double g, Vector grad_G)
{
	cout << "WARNING: PolakHeSearchDirectionAndMeritFunction::updateMeritParameters() --" << endl
		<< " no explicit merit function value is computed." << endl;

	return 0;
}




int
PolakHeSearchDirectionAndMeritFunction::check(Vector u_old, 
											  double g_old, 
											  Vector grad_G_old, 
											  double stepSize,
											  Vector stepDirection,
											  double g_new)
{
	// New point in standard normal space
	Vector u_new = u_old + stepSize*stepDirection;

	// Check that the factor alpha is set (since this is a dual purpose class...)
	if (alpha == 0.0) {
		cout << "ERROR: PolakHeSearchDirectionAndMeritFunction::check()" << endl
			<< " the alpha factor is not set! " << endl;
	}


	// Determine the 'g plus' value
	double g_old_plus;
	if (g_old <= 0.0 ) {
		g_old_plus = 0.0;
	}
	else {
		g_old_plus = g_old;
	}
	

	// Evaluate the 'merit' function
	double term1 = 0.5*(u_new^u_new) - 0.5*(u_old^u_old) - gamma * g_old_plus;
	double term2 = g_new - g_old_plus;
	double F;
	if (term1 > term2) {
		F = term1;
	}
	else {
		F = term2;
	}


	// Do the check
	if (  F  <=  (alpha*stepSize*thetaFunction)  ) {
		return 0;  // ok
	}
	else {
		return -1; // not ok
	}
	
}
