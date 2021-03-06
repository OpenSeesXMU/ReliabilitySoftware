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
                                                                        
// $Revision: 1.1 $
// $Date: 2003/03/04 00:44:51 $
// $Source: /usr/local/cvs/OpenSees/SRC/reliability/domain/spectrum/PointsSpectrum.cpp,v $


//
// Written by Terje Haukaas (haukaas@ce.berkeley.edu)
//

#include "PointsSpectrum.h"
#include "Spectrum.h"
#include "Vector.h"
#include "classTags.h"
#include <iostream>
using namespace std;

PointsSpectrum::PointsSpectrum(int tag, Vector freq, Vector ampl)
:Spectrum(tag,SPECTRUM_points)
{
	// Check that the frequency and the amplitude vectors have the same size
	int numPoints = freq.Size();
	if (numPoints != ampl.Size()) {
		cout << "Number of points to PointsSpectrum is not consistent!" << endl;
	}

	// Check that the frequencies are consequtive
	for (int i=1; i<freq.Size(); i++) {
		if (freq(i-1)>freq(i)) {
			cout << "ERROR: The given Spectrum frequencies are not consequtive!" << endl;
		}
	}

	frequencies = new Vector(freq);
	amplitudes = new Vector(ampl);

}

PointsSpectrum::~PointsSpectrum()
{
	if (frequencies != 0) 
		delete frequencies;
	if (amplitudes != 0)
		delete frequencies;
}

void
PointsSpectrum::Print(OPS_Stream &s, int flag)  
{
}


double
PointsSpectrum::getMinFrequency()
{
	return (*frequencies)(0);
}


double
PointsSpectrum::getMaxFrequency()
{
	return (*frequencies)(frequencies->Size()-1);
}


double
PointsSpectrum::getAmplitude(double frequency)
{
	double result;

	if (frequency < (*frequencies)(0)  ||  frequency > (*frequencies)(frequencies->Size()-1) ) {
		result = 0.0;
	}
	else {
		double dy, dx, a, b;
		for (int i=1; i<frequencies->Size(); i++) {
			if (frequency > (*frequencies)(i-1) && frequency < (*frequencies)(i)) {
				dy = (*amplitudes)(i)  -  (*amplitudes)(i-1);
				dx = (*frequencies)(i)  -  (*frequencies)(i-1);
				a = dy/dx;
				b = (*amplitudes)(i-1);
				result = a * (frequency-(*frequencies)(i-1)) + b;
			}
		}
	}

	return result;
}
