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
                                                                        
// $Revision: 1.5 $
// $Date: 2003/03/04 00:39:04 $
// $Source: /usr/local/cvs/OpenSees/SRC/reliability/analysis/direction/HLRFSearchDirection.cpp,v $


//
// Written by Terje Haukaas (haukaas@ce.berkeley.edu) during Spring 2000
// Revised: haukaas 06/00 (core code)
//			haukaas 06/01 (made part of official OpenSees)
//

#include "HLRFSearchDirection.h"
#include "SearchDirection.h"
#include "Vector.h"
#include <iostream>
using namespace std;


HLRFSearchDirection::HLRFSearchDirection()
:SearchDirection(), searchDirection(1)
{
}

HLRFSearchDirection::~HLRFSearchDirection()
{
}




Vector
HLRFSearchDirection::getSearchDirection()
{
	return searchDirection;
}



int
HLRFSearchDirection::computeSearchDirection(
							int stepNumber,
							Vector u, 
							double gFunctionValue, 
							Vector gradientInStandardNormalSpace )
{

	// Compute the norm of the gradient
	double normOfGradient = gradientInStandardNormalSpace.Norm();


	// Check that the norm is not zero
	if (normOfGradient == 0.0) {
		cout << "HLRFSearchDirection::computeSearchDirection() - " << endl
			<< " the norm of the gradient is zero. " << endl;
		return -1;
	}

	
	// Compute the alpha-vector
	Vector alpha = gradientInStandardNormalSpace * ( (-1) / normOfGradient );


	// Compute the direction vector
	double alpha_times_u = alpha ^ u ;
	Vector direction = alpha * ( gFunctionValue / normOfGradient + alpha_times_u ) - u;

	searchDirection = direction;

	return 0;
}

