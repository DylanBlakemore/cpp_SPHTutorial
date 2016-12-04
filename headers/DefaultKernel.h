/*
 * DefaultKernel.h
 *
 *  Created on: 04 Dec 2016
 *      Author: dylan
 */

#ifndef DEFAULTKERNEL_H_
#define DEFAULTKERNEL_H_

#include "Kernel.h"

class DefaultKernel:Kernel {
public:
	void computeWeight(Particle* p1, Particle* p2, float h, float* weight);
	void computeGradient(Particle* p1, Particle* p2, float h, float* weight);
	void computeLaplacian(Particle* p1, Particle* p2, float h, float* weight);
};



#endif /* DEFAULTKERNEL_H_ */
