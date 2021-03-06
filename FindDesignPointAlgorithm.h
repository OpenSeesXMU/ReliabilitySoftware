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
// $Source: /usr/local/cvs/OpenSees/SRC/reliability/analysis/designPoint/FindDesignPointAlgorithm.h,v $


//
// Written by Terje Haukaas (haukaas@ce.berkeley.edu) 
//

#ifndef FindDesignPointAlgorithm_h
#define FindDesignPointAlgorithm_h

#include "ReliabilityDomain.h"
#include "Vector.h"

class FindDesignPointAlgorithm
{

public:
	FindDesignPointAlgorithm();
	virtual ~FindDesignPointAlgorithm();

	virtual int findDesignPoint(ReliabilityDomain *theReliabilityDomain) =0;

	virtual Vector get_x() =0;
	virtual Vector get_u() =0;
	virtual Vector get_alpha() =0;
	virtual Vector get_gamma() =0;
	virtual int getNumberOfSteps() =0;
	virtual Vector getSecondLast_u() =0;
	virtual Vector getSecondLast_alpha() =0;
	virtual Vector getLastSearchDirection() =0;
	virtual double getFirstGFunValue() =0;
	virtual double getLastGFunValue() =0;
	virtual Vector getGradientInStandardNormalSpace() =0;
	virtual int getNumberOfEvaluations() = 0;
// Quan and Michele Jan 2006 
	virtual int setStartPt(Vector *) =0;
	
	
	ofstream * outputFile;
	

protected:


private:

};

#endif
