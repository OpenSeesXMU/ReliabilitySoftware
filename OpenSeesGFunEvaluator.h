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
                                                                        
// $Revision: 1.7 $
// $Date: 2003/10/27 23:45:43 $
// $Source: /usr/local/cvs/OpenSees/SRC/reliability/analysis/gFunction/OpenSeesGFunEvaluator.h,v $


//
// Written by Terje Haukaas (haukaas@ce.berkeley.edu)
//

#ifndef OpenSeesGFunEvaluator_h
#define OpenSeesGFunEvaluator_h

#include "GFunEvaluator.h"
#include "Vector.h"
#include "ReliabilityDomain.h"
#include <tcl.h>

#include <fstream>
using std::ofstream;


class OpenSeesGFunEvaluator : public GFunEvaluator
{

public:


	OpenSeesGFunEvaluator(Tcl_Interp *passedTclInterp,
						ReliabilityDomain *passedReliabilityDomain,
						TCL_Char *fileName, double dt);
	OpenSeesGFunEvaluator(Tcl_Interp *passedTclInterp,
		ReliabilityDomain *passedReliabilityDomain,
		 int nsteps, double dt, TCL_Char *fileName);
	OpenSeesGFunEvaluator(Tcl_Interp *passedTclInterp,
						ReliabilityDomain *passedReliabilityDomain,
						int nsteps, double dt);
	~OpenSeesGFunEvaluator();

	int		runGFunAnalysis(Vector x);
	int		tokenizeSpecials(TCL_Char *theExpression);

	void    setNsteps(int nsteps);
	double  getDt();

	double getG2(double g, double littleDeltaT);

protected:

private:
	int createRecorders();
	int removeRecorders();
	char *rec_node_occurrence(char tempchar[100], bool createRecorders, int &line, int &column);
	char *rec_element_occurrence(char tempchar[100], bool createRecorders, int &line, int &column);
	char *fileName;
	int nsteps;
	double dt;

};

#endif
